#include "simplegui/segmented_control.h"
#include "detail/common.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QStringList>
#include <QVariantAnimation>
#include <QWidget>

#include <algorithm>

namespace simplegui {
namespace {

class SegmentWidget : public QWidget {
public:
    QStringList items;
    int selected = -1;
    double highlight = 0.0;  // animated position of the selection, in segments
    QColor accent = QColor(0x00, 0xe5, 0xff);
    std::function<void(int)> user_selected;

    SegmentWidget() : anim_(new QVariantAnimation(this)) {
        setFocusPolicy(Qt::StrongFocus);
        setCursor(Qt::PointingHandCursor);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        anim_->setDuration(200);
        anim_->setEasingCurve(QEasingCurve::OutCubic);
        QObject::connect(anim_, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
            highlight = v.toDouble();
            update();
        });
    }

    void select(int index, bool animate) {
        selected = items.isEmpty() ? -1 : std::clamp(index, 0, static_cast<int>(items.size()) - 1);
        anim_->stop();
        if (!animate || !isVisible() || selected < 0) {
            highlight = std::max(0, selected);
            update();
            return;
        }
        anim_->setStartValue(highlight);
        anim_->setEndValue(static_cast<double>(selected));
        anim_->start();
    }

    QSize sizeHint() const override {
        int widest = 40;
        for (const auto& s : items) widest = std::max(widest, fontMetrics().horizontalAdvance(s) + 28);
        return {std::max(1, static_cast<int>(items.size())) * widest + 6, fontMetrics().height() + 16};
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        const QRectF r = QRectF(rect()).adjusted(1, 1, -1, -1);
        p.setPen(QPen(QColor(0x1b, 0x3a, 0x4b), 1));
        p.setBrush(QColor(0x0a, 0x0f, 0x1e));
        p.drawRoundedRect(r, 8, 8);
        const int n = static_cast<int>(items.size());
        if (n == 0) return;
        const double seg = (r.width() - 4) / n;

        if (selected >= 0) {
            const QRectF hl(r.left() + 2 + highlight * seg, r.top() + 2, seg, r.height() - 4);
            QColor fill = accent;
            fill.setAlphaF(isEnabled() ? 0.22 : 0.08);
            p.setBrush(fill);
            p.setPen(QPen(accent, 1));
            p.drawRoundedRect(hl, 6, 6);
        }
        for (int i = 0; i < n; ++i) {
            const QRectF cell(r.left() + 2 + i * seg, r.top(), seg, r.height());
            p.setPen(i == selected ? (isEnabled() ? QColor(Qt::white) : QColor(0x6c, 0x8a, 0x99)) : QColor(0x8b, 0xa3, 0xb3));
            p.drawText(cell, Qt::AlignCenter, fontMetrics().elidedText(items.at(i), Qt::ElideRight, static_cast<int>(seg) - 8));
        }
    }

    void mousePressEvent(QMouseEvent* e) override {
        const int n = static_cast<int>(items.size());
        if (n == 0 || e->button() != Qt::LeftButton) return;
        const int index = std::clamp(static_cast<int>(e->position().x() / (width() / static_cast<double>(n))), 0, n - 1);
        choose(index);
    }

    void keyPressEvent(QKeyEvent* e) override {
        if (e->key() == Qt::Key_Left) {
            choose(selected - 1);
        } else if (e->key() == Qt::Key_Right) {
            choose(selected + 1);
        } else {
            QWidget::keyPressEvent(e);
        }
    }

private:
    void choose(int index) {
        if (items.isEmpty()) return;
        index = std::clamp(index, 0, static_cast<int>(items.size()) - 1);
        if (index == selected) return;
        select(index, true);
        if (user_selected) user_selected(selected);
    }
    QVariantAnimation* anim_;
};

}  // namespace

struct SegmentedControl::Impl {
    QPointer<SegmentWidget> widget = new SegmentWidget();
    detail::Event<int, const std::string&> changed = detail::make_event<int, const std::string&>(widget);
    ~Impl() { detail::delete_if_orphan(widget); }
};

SegmentedControl::SegmentedControl(const std::vector<std::string>& items, int selected_index)
    : pimpl(std::make_shared<Impl>()) {
    set_items(items);
    pimpl->widget->select(selected_index, false);
    std::weak_ptr<Impl> weak = pimpl;
    pimpl->widget->user_selected = [weak](int index) {
        auto d = weak.lock();
        if (!d || !d->widget) return;
        detail::fire(d->changed, index, detail::ss(d->widget->items.value(index)));
    };
}

SegmentedControl::~SegmentedControl() = default;

void SegmentedControl::set_items(const std::vector<std::string>& items) {
    auto* w = pimpl->widget.data();
    if (!w) return;
    w->items.clear();
    for (const auto& s : items) w->items << detail::qs(s);
    w->select(0, false);
    w->updateGeometry();
}

std::vector<std::string> SegmentedControl::items() const {
    std::vector<std::string> out;
    if (pimpl->widget) {
        for (const auto& s : pimpl->widget->items) out.push_back(detail::ss(s));
    }
    return out;
}

int SegmentedControl::count() const { return pimpl->widget ? static_cast<int>(pimpl->widget->items.size()) : 0; }

void SegmentedControl::set_selected_index(int index) {
    if (!pimpl->widget || index < 0 || index >= pimpl->widget->items.size()) return;  // unknown index: ignore
    pimpl->widget->select(index, true);
}

int SegmentedControl::selected_index() const { return pimpl->widget ? pimpl->widget->selected : -1; }

std::string SegmentedControl::selected_text() const {
    if (!pimpl->widget || pimpl->widget->selected < 0) return {};
    return detail::ss(pimpl->widget->items.value(pimpl->widget->selected));
}

void SegmentedControl::set_accent_color(const std::string& color) {
    if (!pimpl->widget) return;
    pimpl->widget->accent = detail::parse_color(color, pimpl->widget->accent);
    pimpl->widget->update();
}

EventConnection SegmentedControl::on_change(std::function<void(int, const std::string&)> handler) {
    return detail::add_handler(pimpl->changed, std::move(handler));
}

QWidget* SegmentedControl::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
