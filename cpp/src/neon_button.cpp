#include "simplegui/neon_button.h"
#include "detail/common.h"

#include <QAbstractButton>
#include <QEnterEvent>
#include <QPainter>
#include <QVariantAnimation>

#include <algorithm>

namespace simplegui {
namespace {

class NeonWidget : public QAbstractButton {
public:
    QColor color = QColor(0x00, 0xe5, 0xff);
    double glow = 0.0;  // 0 = idle, 1 = fully lit (animated on hover)

    NeonWidget() : anim_(new QVariantAnimation(this)) {
        setProperty("sg_transparent", true);
        setCursor(Qt::PointingHandCursor);
        setFocusPolicy(Qt::StrongFocus);
        setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        anim_->setDuration(180);
        QObject::connect(anim_, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
            glow = v.toDouble();
            update();
        });
    }

    QSize sizeHint() const override {
        QFont f = font();
        f.setBold(true);
        const QFontMetrics fm(f);
        return {std::max(90, fm.horizontalAdvance(text()) + 40), std::max(36, fm.height() + 18)};
    }

protected:
    void enterEvent(QEnterEvent* e) override {
        fade_to(1.0);
        QAbstractButton::enterEvent(e);
    }
    void leaveEvent(QEvent* e) override {
        fade_to(0.0);
        QAbstractButton::leaveEvent(e);
    }

    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        const QRectF r = QRectF(rect()).adjusted(5, 5, -5, -5);
        const bool enabled = isEnabled();
        QColor c = enabled ? color : QColor(0x2b, 0x4a, 0x55);
        const double lit = enabled ? std::max(glow, hasFocus() ? 0.5 : 0.0) : 0.0;

        // Outer glow: several soft strokes, stronger when hovered.
        for (int i = 3; i >= 1; --i) {
            QColor g = c;
            g.setAlphaF((0.06 + 0.10 * lit) * (4 - i) / 3.0);
            p.setPen(QPen(g, i * 3.0));
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(r, 6, 6);
        }
        QColor fill = c;
        fill.setAlphaF(isDown() ? 0.35 : 0.06 + 0.14 * lit);
        p.setBrush(fill);
        p.setPen(QPen(c, 1.5));
        p.drawRoundedRect(r, 6, 6);

        QFont f = font();
        f.setBold(true);
        f.setLetterSpacing(QFont::AbsoluteSpacing, 1.5);
        p.setFont(f);
        p.setPen(lit > 0.5 ? QColor(Qt::white) : c);
        p.drawText(r, Qt::AlignCenter, text());
    }

private:
    void fade_to(double v) {
        anim_->stop();
        anim_->setStartValue(glow);
        anim_->setEndValue(v);
        anim_->start();
    }
    QVariantAnimation* anim_;
};

}  // namespace

struct NeonButton::Impl {
    QPointer<NeonWidget> widget = new NeonWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

NeonButton::NeonButton(const std::string& text, const std::string& color) : pimpl(std::make_shared<Impl>()) {
    set_text(text);
    set_color(color);
}

NeonButton::~NeonButton() = default;

void NeonButton::set_text(const std::string& text) {
    if (!pimpl->widget) return;
    pimpl->widget->setText(detail::qs(text));
    pimpl->widget->updateGeometry();
}

std::string NeonButton::get_text() const {
    return pimpl->widget ? detail::ss(pimpl->widget->text()) : std::string();
}

void NeonButton::set_color(const std::string& color) {
    if (!pimpl->widget) return;
    pimpl->widget->color = detail::parse_color(color, pimpl->widget->color);
    pimpl->widget->update();
}

void NeonButton::click() {
    if (pimpl->widget) pimpl->widget->click();
}

EventConnection NeonButton::on_click(std::function<void()> handler) {
    if (!pimpl->widget || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->widget.data(), &QAbstractButton::clicked,
                                         [handler = std::move(handler)]() { handler(); }));
}

QWidget* NeonButton::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
