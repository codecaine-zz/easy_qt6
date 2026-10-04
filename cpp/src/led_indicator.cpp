#include "simplegui/led_indicator.h"
#include "detail/common.h"

#include <QEvent>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include <algorithm>

namespace simplegui {
namespace {

class LedWidget : public detail::PaintedWidget {
public:
    QColor color = QColor(0x22, 0xc5, 0x5e);
    bool on = true;
    bool blink_phase = true;  // toggled by the blink timer
    int diameter = 16;
    QString label;
    QTimer* blink;

    LedWidget() : blink(new QTimer(this)) {
        QObject::connect(blink, &QTimer::timeout, this, [this]() {
            blink_phase = !blink_phase;
            update();
        });
        refresh_size();
    }

    void refresh_size() {
        const int text_w = label.isEmpty() ? 0 : fontMetrics().horizontalAdvance(label) + 12;
        setFixedSize(diameter + 8 + text_w, std::max(diameter + 8, fontMetrics().height() + 4));
        update();
    }

protected:
    // Style sheets may change the font after construction; re-measure then.
    void changeEvent(QEvent* e) override {
        if (e->type() == QEvent::FontChange || e->type() == QEvent::StyleChange) refresh_size();
        QWidget::changeEvent(e);
    }

    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        const bool lit = on && (!blink->isActive() || blink_phase);
        const QPointF c(4 + diameter / 2.0, height() / 2.0);
        const double r = diameter / 2.0;

        if (lit) {  // halo
            QRadialGradient halo(c, r + 4);
            QColor h = color;
            h.setAlphaF(0.45);
            halo.setColorAt(0.5, h);
            h.setAlphaF(0.0);
            halo.setColorAt(1.0, h);
            p.setPen(Qt::NoPen);
            p.setBrush(halo);
            p.drawEllipse(c, r + 4, r + 4);
        }
        QRadialGradient body(c - QPointF(r * 0.3, r * 0.3), r * 1.3);
        const QColor base = lit ? color : color.darker(400);
        body.setColorAt(0.0, lit ? base.lighter(160) : base.lighter(130));
        body.setColorAt(1.0, base);
        p.setBrush(body);
        p.setPen(QPen(QColor(0, 0, 0, 120), 1));
        p.drawEllipse(c, r, r);

        if (!label.isEmpty()) {
            p.setPen(palette().color(QPalette::WindowText));
            p.drawText(QRectF(diameter + 12, 0, width() - diameter - 12, height()), Qt::AlignLeft | Qt::AlignVCenter, label);
        }
    }
};

}  // namespace

struct LedIndicator::Impl {
    QPointer<LedWidget> widget = new LedWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

LedIndicator::LedIndicator(const std::string& color, bool on) : pimpl(std::make_shared<Impl>()) {
    set_color(color);
    set_on(on);
}

LedIndicator::~LedIndicator() = default;

void LedIndicator::set_on(bool on) {
    if (!pimpl->widget) return;
    pimpl->widget->on = on;
    pimpl->widget->update();
}

bool LedIndicator::is_on() const { return pimpl->widget && pimpl->widget->on; }

void LedIndicator::set_color(const std::string& color) {
    if (!pimpl->widget) return;
    pimpl->widget->color = detail::parse_color(color, pimpl->widget->color);
    pimpl->widget->update();
}

void LedIndicator::set_blinking(bool blinking, int interval_ms) {
    auto* w = pimpl->widget.data();
    if (!w) return;
    if (blinking) {
        w->blink->start(std::max(50, interval_ms));
    } else {
        w->blink->stop();
        w->blink_phase = true;
    }
    w->update();
}

bool LedIndicator::is_blinking() const { return pimpl->widget && pimpl->widget->blink->isActive(); }

void LedIndicator::set_diameter(int pixels) {
    if (!pimpl->widget) return;
    pimpl->widget->diameter = std::clamp(pixels, 4, 256);
    pimpl->widget->refresh_size();
}

void LedIndicator::set_label(const std::string& text) {
    if (!pimpl->widget) return;
    pimpl->widget->label = detail::qs(text);
    pimpl->widget->refresh_size();
}

QWidget* LedIndicator::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
