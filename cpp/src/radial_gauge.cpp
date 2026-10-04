#include "simplegui/radial_gauge.h"
#include "detail/common.h"

#include <QPainter>
#include <QPainterPath>
#include <QVariantAnimation>
#include <QWidget>

#include <algorithm>
#include <cmath>

namespace simplegui {
namespace {

constexpr double kStartAngle = 225.0;  // degrees, Qt convention (0 = 3 o'clock, counter-clockwise)
constexpr double kSweep = 270.0;
constexpr double kPi = 3.14159265358979323846;

class GaugeWidget : public QWidget {
public:
    QString title;
    QString units;
    double min = 0.0;
    double max = 100.0;
    double target = 0.0;
    double shown = 0.0;   // animated value actually drawn
    double warning = 1e300;
    double danger = 1e300;
    int decimals = 0;
    bool animated = true;
    QColor color = QColor(0x00, 0xe5, 0xff);

    GaugeWidget() : anim_(new QVariantAnimation(this)) {
        setMinimumSize(160, 160);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        anim_->setDuration(450);
        anim_->setEasingCurve(QEasingCurve::OutCubic);
        QObject::connect(anim_, &QVariantAnimation::valueChanged, this, [this](const QVariant& v) {
            shown = v.toDouble();
            update();
        });
    }

    void go_to(double v) {
        target = std::clamp(v, min, max);
        anim_->stop();
        if (!animated || !isVisible()) {
            shown = target;
            update();
            return;
        }
        anim_->setStartValue(shown);
        anim_->setEndValue(target);
        anim_->start();
    }

    QColor color_for(double v) const {
        if (v >= danger) return QColor(0xff, 0x3b, 0x5c);
        if (v >= warning) return QColor(0xff, 0xb0, 0x20);
        return color;
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        const double side = std::min(width(), height()) - 16.0;
        if (side < 40) return;
        const QRectF ring((width() - side) / 2.0, (height() - side) / 2.0, side, side);
        const double thick = std::max(6.0, side * 0.07);
        const double span = max - min;
        const double ratio = span > 0 ? (shown - min) / span : 0.0;

        // Background disc
        QRadialGradient bg(ring.center(), side / 2.0);
        bg.setColorAt(0.0, QColor(0x0d, 0x14, 0x26));
        bg.setColorAt(1.0, QColor(0x05, 0x07, 0x0f));
        p.setPen(QPen(QColor(0x1b, 0x3a, 0x4b), 1));
        p.setBrush(bg);
        p.drawEllipse(ring.adjusted(-4, -4, 4, 4));

        const QRectF arc = ring.adjusted(thick, thick, -thick, -thick);
        // Empty track
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(QColor(0x1b, 0x2a, 0x3b), thick, Qt::SolidLine, Qt::FlatCap));
        p.drawArc(arc, static_cast<int>(kStartAngle * 16), static_cast<int>(-kSweep * 16));

        // Tick marks
        p.setPen(QPen(QColor(0x3b, 0x5a, 0x6b), 1.5));
        for (int i = 0; i <= 10; ++i) {
            const double a = (kStartAngle - kSweep * i / 10.0) * kPi / 180.0;
            const double r1 = arc.width() / 2.0 - thick * 0.9;
            const double r2 = r1 - (i % 5 == 0 ? thick * 0.9 : thick * 0.5);
            const QPointF c = arc.center();
            p.drawLine(QPointF(c.x() + r1 * std::cos(a), c.y() - r1 * std::sin(a)),
                       QPointF(c.x() + r2 * std::cos(a), c.y() - r2 * std::sin(a)));
        }

        // Value arc with glow
        const QColor col = color_for(shown);
        const int span16 = static_cast<int>(-kSweep * std::clamp(ratio, 0.0, 1.0) * 16);
        QColor glow = col;
        glow.setAlphaF(0.25);
        p.setPen(QPen(glow, thick * 1.8, Qt::SolidLine, Qt::FlatCap));
        p.drawArc(arc, static_cast<int>(kStartAngle * 16), span16);
        p.setPen(QPen(col, thick, Qt::SolidLine, Qt::FlatCap));
        p.drawArc(arc, static_cast<int>(kStartAngle * 16), span16);

        // Needle
        const double a = (kStartAngle - kSweep * std::clamp(ratio, 0.0, 1.0)) * kPi / 180.0;
        const QPointF c = arc.center();
        const double len = arc.width() / 2.0 - thick * 1.6;
        p.setPen(QPen(col, 2.5, Qt::SolidLine, Qt::RoundCap));
        p.drawLine(c, QPointF(c.x() + len * std::cos(a), c.y() - len * std::sin(a)));
        p.setPen(Qt::NoPen);
        p.setBrush(col);
        p.drawEllipse(c, 5, 5);

        // Readout
        QFont f = font();
        f.setBold(true);
        f.setPixelSize(std::max(10, static_cast<int>(side * 0.16)));
        p.setFont(f);
        p.setPen(QColor(0xe0, 0xfb, 0xff));
        QString text = QString::number(shown, 'f', decimals);
        if (!units.isEmpty()) text += QLatin1Char(' ') + units;
        p.drawText(QRectF(ring.left(), c.y() + side * 0.08, ring.width(), side * 0.2), Qt::AlignCenter, text);
        if (!title.isEmpty()) {
            f.setBold(false);
            f.setPixelSize(std::max(8, static_cast<int>(side * 0.07)));
            f.setLetterSpacing(QFont::AbsoluteSpacing, 2);
            p.setFont(f);
            p.setPen(QColor(0x6c, 0x8a, 0x99));
            p.drawText(QRectF(ring.left(), c.y() + side * 0.27, ring.width(), side * 0.12), Qt::AlignCenter, title);
        }
    }

private:
    QVariantAnimation* anim_;
};

}  // namespace

struct RadialGauge::Impl {
    QPointer<GaugeWidget> widget = new GaugeWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

RadialGauge::RadialGauge(const std::string& title, double min, double max) : pimpl(std::make_shared<Impl>()) {
    set_title(title);
    set_range(min, max);
    pimpl->widget->go_to(min);
}

RadialGauge::~RadialGauge() = default;

void RadialGauge::set_value(double value) {
    if (pimpl->widget && std::isfinite(value)) pimpl->widget->go_to(value);
}

double RadialGauge::value() const { return pimpl->widget ? pimpl->widget->target : 0.0; }

void RadialGauge::set_range(double min, double max) {
    auto* w = pimpl->widget.data();
    if (!w || !std::isfinite(min) || !std::isfinite(max)) return;
    if (min > max) std::swap(min, max);
    if (min == max) max = min + 1.0;
    w->min = min;
    w->max = max;
    w->go_to(w->target);
}

void RadialGauge::set_title(const std::string& title) {
    if (!pimpl->widget) return;
    pimpl->widget->title = detail::qs(title).toUpper();
    pimpl->widget->update();
}

void RadialGauge::set_units(const std::string& units) {
    if (!pimpl->widget) return;
    pimpl->widget->units = detail::qs(units);
    pimpl->widget->update();
}

void RadialGauge::set_decimals(int decimals) {
    if (!pimpl->widget) return;
    pimpl->widget->decimals = std::clamp(decimals, 0, 4);
    pimpl->widget->update();
}

void RadialGauge::set_color(const std::string& color) {
    if (!pimpl->widget) return;
    pimpl->widget->color = detail::parse_color(color, pimpl->widget->color);
    pimpl->widget->update();
}

void RadialGauge::set_thresholds(double warning, double danger) {
    if (!pimpl->widget) return;
    pimpl->widget->warning = warning;
    pimpl->widget->danger = danger;
    pimpl->widget->update();
}

void RadialGauge::set_animated(bool animated) {
    if (pimpl->widget) pimpl->widget->animated = animated;
}

QWidget* RadialGauge::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
