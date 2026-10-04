#include "simplegui/rating.h"
#include "detail/common.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPolygonF>
#include <QWidget>

#include <algorithm>
#include <cmath>

namespace simplegui {
namespace {

constexpr int kStarSize = 24;
constexpr double kPi = 3.14159265358979323846;

class RatingWidget : public QWidget {
public:
    int rating = 0;
    int max_stars;
    bool read_only = false;
    std::function<void(int)> clicked;

    explicit RatingWidget(int stars) : max_stars(std::clamp(stars, 1, 20)) {
        setFixedSize(max_stars * kStarSize, kStarSize);
        setCursor(Qt::PointingHandCursor);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        for (int i = 0; i < max_stars; ++i) {
            painter.setBrush(i < rating ? QColor(0xf1, 0xc4, 0x0f) : QColor(0xbd, 0xc3, 0xc7));
            painter.drawPolygon(star_shape(QPointF(i * kStarSize + kStarSize / 2.0, kStarSize / 2.0)));
        }
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (read_only || event->button() != Qt::LeftButton) return;
        const int star = static_cast<int>(event->position().x()) / kStarSize + 1;
        if (star < 1 || star > max_stars) return;
        rating = star;
        update();
        if (clicked) clicked(rating);
    }

private:
    static QPolygonF star_shape(QPointF c) {
        QPolygonF poly;
        const double outer = kStarSize * 0.45;
        const double inner = outer * 0.45;
        for (int k = 0; k < 10; ++k) {
            const double r = (k % 2 == 0) ? outer : inner;
            const double a = -kPi / 2 + k * kPi / 5;
            poly << QPointF(c.x() + r * std::cos(a), c.y() + r * std::sin(a));
        }
        return poly;
    }
};

}  // namespace

struct Rating::Impl {
    QPointer<RatingWidget> widget;
    detail::Event<int> changed;
    explicit Impl(int stars) : widget(new RatingWidget(stars)), changed(detail::make_event<int>(widget)) {}
    ~Impl() { detail::delete_if_orphan(widget); }
};

Rating::Rating(int max_stars) : pimpl(std::make_shared<Impl>(max_stars)) {
    std::weak_ptr<Impl> weak = pimpl;
    pimpl->widget->clicked = [weak](int stars) {
        if (auto d = weak.lock()) detail::fire(d->changed, stars);
    };
}

Rating::~Rating() = default;

void Rating::set_rating(int stars) {
    if (!pimpl->widget) return;
    pimpl->widget->rating = std::clamp(stars, 0, pimpl->widget->max_stars);
    pimpl->widget->update();
}

int Rating::get_rating() const { return pimpl->widget ? pimpl->widget->rating : 0; }
int Rating::max_stars() const { return pimpl->widget ? pimpl->widget->max_stars : 0; }

void Rating::set_read_only(bool read_only) {
    if (!pimpl->widget) return;
    pimpl->widget->read_only = read_only;
    pimpl->widget->setCursor(read_only ? Qt::ArrowCursor : Qt::PointingHandCursor);
}

EventConnection Rating::on_change(std::function<void(int)> handler) {
    return detail::add_handler(pimpl->changed, std::move(handler));
}

QWidget* Rating::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
