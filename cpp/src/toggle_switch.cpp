#include "simplegui/toggle_switch.h"
#include "detail/common.h"

#include <QAbstractButton>
#include <QPainter>
#include <QVariantAnimation>

namespace simplegui {
namespace {

constexpr int kTrackW = 44;
constexpr int kTrackH = 24;

class SwitchWidget : public QAbstractButton {
public:
    QColor on_color = QColor(0x00, 0xe5, 0xff);
    double knob = 0.0;  // 0 = left (off), 1 = right (on); animated

    SwitchWidget() : anim_(new QVariantAnimation(this)) {
        setCheckable(true);
        setCursor(Qt::PointingHandCursor);
        setFocusPolicy(Qt::StrongFocus);
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        anim_->setDuration(160);
        anim_->setEasingCurve(QEasingCurve::OutCubic);
        QObject::connect(anim_, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
            knob = v.toDouble();
            update();
        });
        QObject::connect(this, &QAbstractButton::toggled, this, [this](bool on) { animate_to(on); });
    }

    void animate_to(bool on) {
        anim_->stop();
        if (!isVisible()) {  // no animation before the window is shown
            knob = on ? 1.0 : 0.0;
            update();
            return;
        }
        anim_->setStartValue(knob);
        anim_->setEndValue(on ? 1.0 : 0.0);
        anim_->start();
    }

    QSize sizeHint() const override {
        const int text_w = text().isEmpty() ? 0 : fontMetrics().horizontalAdvance(text()) + 10;
        return {kTrackW + text_w + 4, std::max(kTrackH + 4, fontMetrics().height() + 4)};
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        const QRectF track(2, (height() - kTrackH) / 2.0, kTrackW, kTrackH);

        // Track: blend from grey to the "on" color as the knob moves.
        const QColor off(0x33, 0x41, 0x55);
        QColor c;
        c.setRgbF(off.redF() + (on_color.redF() - off.redF()) * knob,
                  off.greenF() + (on_color.greenF() - off.greenF()) * knob,
                  off.blueF() + (on_color.blueF() - off.blueF()) * knob);
        if (!isEnabled()) c.setAlphaF(0.4);
        if (knob > 0.01) {  // soft neon glow
            QColor glow = on_color;
            glow.setAlphaF(0.25 * knob);
            p.setPen(QPen(glow, 4));
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(track.adjusted(-1, -1, 1, 1), kTrackH / 2.0, kTrackH / 2.0);
        }
        p.setPen(Qt::NoPen);
        p.setBrush(c);
        p.drawRoundedRect(track, kTrackH / 2.0, kTrackH / 2.0);

        const double d = kTrackH - 6;
        const double x = track.left() + 3 + knob * (kTrackW - 6 - d);
        p.setBrush(QColor(0xf8, 0xfa, 0xfc));
        p.drawEllipse(QRectF(x, track.top() + 3, d, d));

        if (hasFocus()) {
            p.setPen(QPen(on_color, 1, Qt::DotLine));
            p.setBrush(Qt::NoBrush);
            p.drawRoundedRect(track.adjusted(-2, -2, 2, 2), kTrackH / 2.0 + 2, kTrackH / 2.0 + 2);
        }
        if (!text().isEmpty()) {
            p.setPen(palette().color(isEnabled() ? QPalette::Active : QPalette::Disabled, QPalette::WindowText));
            p.drawText(QRectF(track.right() + 10, 0, width() - track.right() - 10, height()),
                       Qt::AlignLeft | Qt::AlignVCenter, text());
        }
    }

private:
    QVariantAnimation* anim_;
};

}  // namespace

struct ToggleSwitch::Impl {
    QPointer<SwitchWidget> widget = new SwitchWidget();
    detail::Event<bool> toggled = detail::make_event<bool>(widget);
    ~Impl() { detail::delete_if_orphan(widget); }
};

ToggleSwitch::ToggleSwitch(bool on, const std::string& label) : pimpl(std::make_shared<Impl>()) {
    pimpl->widget->setText(detail::qs(label));
    pimpl->widget->setChecked(on);
    pimpl->widget->knob = on ? 1.0 : 0.0;
    std::weak_ptr<Impl> weak = pimpl;
    // `clicked` only fires for user actions (mouse/keyboard) and toggle(), not set_on().
    QObject::connect(pimpl->widget.data(), &QAbstractButton::clicked, [weak](bool checked) {
        if (auto d = weak.lock()) detail::fire(d->toggled, checked);
    });
}

ToggleSwitch::~ToggleSwitch() = default;

void ToggleSwitch::set_on(bool on) {
    if (pimpl->widget) pimpl->widget->setChecked(on);
}

bool ToggleSwitch::is_on() const { return pimpl->widget && pimpl->widget->isChecked(); }

void ToggleSwitch::toggle() {
    if (pimpl->widget) pimpl->widget->click();
}

void ToggleSwitch::set_text(const std::string& label) {
    if (!pimpl->widget) return;
    pimpl->widget->setText(detail::qs(label));
    pimpl->widget->updateGeometry();
}

std::string ToggleSwitch::get_text() const {
    return pimpl->widget ? detail::ss(pimpl->widget->text()) : std::string();
}

void ToggleSwitch::set_on_color(const std::string& color) {
    if (!pimpl->widget) return;
    pimpl->widget->on_color = detail::parse_color(color, pimpl->widget->on_color);
    pimpl->widget->update();
}

EventConnection ToggleSwitch::on_toggle(std::function<void(bool)> handler) {
    return detail::add_handler(pimpl->toggled, std::move(handler));
}

QWidget* ToggleSwitch::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
