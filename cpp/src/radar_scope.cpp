#include "simplegui/radar_scope.h"
#include "detail/common.h"

#include <QConicalGradient>
#include <QElapsedTimer>
#include <QPainter>
#include <QTimer>
#include <QWidget>

#include <algorithm>
#include <cmath>
#include <map>

namespace simplegui {
namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr int kMaxBlips = 1000;

struct Blip {
    double angle;     // degrees clockwise from north
    double distance;  // 0..1
    QColor color;     // invalid = use the scope color
};

class RadarWidget : public QWidget {
public:
    QColor color = QColor(0x39, 0xff, 0x88);
    double sweep = 0.0;   // current sweep angle, degrees clockwise from north
    double speed = 90.0;  // degrees per second
    std::map<int, Blip> blips;
    int next_id = 1;
    QTimer* frame_timer;
    QElapsedTimer clock;

    RadarWidget() : frame_timer(new QTimer(this)) {
        setMinimumSize(180, 180);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        frame_timer->setInterval(16);  // ~60 frames per second
        QObject::connect(frame_timer, &QTimer::timeout, this, [this]() {
            const double dt = clock.restart() / 1000.0;
            sweep = std::fmod(sweep + speed * dt, 360.0);
            if (sweep < 0) sweep += 360.0;
            if (isVisible()) update();  // no repaint work while hidden
        });
    }

    void start() {
        clock.restart();
        frame_timer->start();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.fillRect(rect(), QColor(0x02, 0x08, 0x05));
        const double side = std::min(width(), height()) - 12.0;
        if (side < 20) return;
        const QPointF c(width() / 2.0, height() / 2.0);
        const double r = side / 2.0;

        QColor dim = color;
        dim.setAlphaF(0.25);
        p.setPen(QPen(dim, 1));
        p.setBrush(Qt::NoBrush);
        for (int i = 1; i <= 4; ++i) p.drawEllipse(c, r * i / 4.0, r * i / 4.0);
        p.drawLine(QPointF(c.x() - r, c.y()), QPointF(c.x() + r, c.y()));
        p.drawLine(QPointF(c.x(), c.y() - r), QPointF(c.x(), c.y() + r));

        // Sweep trail: Qt's conical gradient angle is counter-clockwise from 3 o'clock.
        const double qt_angle = 90.0 - sweep;
        QConicalGradient trail(c, qt_angle);
        QColor head = color;
        head.setAlphaF(0.55);
        QColor tail = color;
        tail.setAlphaF(0.0);
        trail.setColorAt(0.0, head);
        trail.setColorAt(0.18, tail);
        trail.setColorAt(1.0, tail);
        p.setPen(Qt::NoPen);
        p.setBrush(trail);
        p.drawEllipse(c, r, r);

        const double rad = sweep * kPi / 180.0;
        p.setPen(QPen(color, 2));
        p.drawLine(c, QPointF(c.x() + r * std::sin(rad), c.y() - r * std::cos(rad)));

        // Blips glow brightly right after the sweep passes them, then fade.
        for (const auto& entry : blips) {
            const Blip& b = entry.second;
            const double since = std::fmod(sweep - b.angle + 360.0, 360.0);
            const double fade = std::max(0.15, 1.0 - since / 360.0);
            QColor bc = b.color.isValid() ? b.color : color;
            bc.setAlphaF(fade);
            const double a = b.angle * kPi / 180.0;
            const QPointF pt(c.x() + r * b.distance * std::sin(a), c.y() - r * b.distance * std::cos(a));
            p.setPen(Qt::NoPen);
            p.setBrush(bc);
            p.drawEllipse(pt, 4.0, 4.0);
            QColor halo = bc;
            halo.setAlphaF(fade * 0.3);
            p.setBrush(halo);
            p.drawEllipse(pt, 8.0, 8.0);
        }
        p.setPen(QPen(color, 1.5));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(c, r, r);
    }
};

double norm_angle(double a) {
    if (!std::isfinite(a)) return 0.0;
    a = std::fmod(a, 360.0);
    return a < 0 ? a + 360.0 : a;
}

double norm_distance(double d) { return std::isfinite(d) ? std::clamp(d, 0.0, 1.0) : 0.0; }

}  // namespace

struct RadarScope::Impl {
    QPointer<RadarWidget> widget = new RadarWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

RadarScope::RadarScope() : pimpl(std::make_shared<Impl>()) { start(); }
RadarScope::~RadarScope() = default;

void RadarScope::start() {
    if (pimpl->widget) pimpl->widget->start();
}

void RadarScope::stop() {
    if (pimpl->widget) pimpl->widget->frame_timer->stop();
}

bool RadarScope::is_running() const { return pimpl->widget && pimpl->widget->frame_timer->isActive(); }

void RadarScope::set_sweep_speed(double degrees_per_second) {
    if (pimpl->widget && std::isfinite(degrees_per_second))
        pimpl->widget->speed = std::clamp(degrees_per_second, -3600.0, 3600.0);
}

void RadarScope::set_color(const std::string& color) {
    if (!pimpl->widget) return;
    pimpl->widget->color = detail::parse_color(color, pimpl->widget->color);
    pimpl->widget->update();
}

int RadarScope::add_blip(double angle_deg, double distance, const std::string& color) {
    auto* w = pimpl->widget.data();
    if (!w || static_cast<int>(w->blips.size()) >= kMaxBlips) return -1;
    const int id = w->next_id++;
    w->blips[id] = {norm_angle(angle_deg), norm_distance(distance), detail::parse_color(color, QColor())};
    w->update();
    return id;
}

void RadarScope::move_blip(int id, double angle_deg, double distance) {
    if (!pimpl->widget) return;
    auto it = pimpl->widget->blips.find(id);
    if (it == pimpl->widget->blips.end()) return;
    it->second.angle = norm_angle(angle_deg);
    it->second.distance = norm_distance(distance);
    pimpl->widget->update();
}

void RadarScope::remove_blip(int id) {
    if (!pimpl->widget) return;
    pimpl->widget->blips.erase(id);
    pimpl->widget->update();
}

void RadarScope::clear_blips() {
    if (!pimpl->widget) return;
    pimpl->widget->blips.clear();
    pimpl->widget->update();
}

int RadarScope::blip_count() const { return pimpl->widget ? static_cast<int>(pimpl->widget->blips.size()) : 0; }

QWidget* RadarScope::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
