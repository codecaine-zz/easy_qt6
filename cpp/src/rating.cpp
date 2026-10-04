#include "simplegui/rating.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPointer>

namespace simplegui {

class QRatingWidget : public QWidget {
public:
    int rating = 0;
    int max_stars = 5;
    std::function<void(int)> on_change_handler;

    QRatingWidget(int max_stars) : max_stars(max_stars) {
        setFixedSize(max_stars * 24, 24);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        for (int i = 0; i < max_stars; ++i) {
            QRect rect(i * 24, 0, 24, 24);
            if (i < rating) {
                painter.setBrush(QColor("#f1c40f")); // Gold
            } else {
                painter.setBrush(QColor("#bdc3c7")); // Gray
            }
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(rect.adjusted(4, 4, -4, -4)); // Just draw circles as stars for simplicity
        }
    }

    void mousePressEvent(QMouseEvent* event) override {
        int star = (event->pos().x() / 24) + 1;
        if (star <= max_stars && star > 0) {
            rating = star;
            update();
            if (on_change_handler) on_change_handler(rating);
        }
    }
};

struct Rating::Impl {
    QPointer<QRatingWidget> widget;
    Impl(int max_stars) { widget = new QRatingWidget(max_stars); }
    ~Impl() { if (widget && !widget->parent()) delete widget; }
};

Rating::Rating(int max_stars) : pimpl(std::make_shared<Impl>(max_stars)) {}

Rating::~Rating() = default;

void Rating::set_rating(int stars) {
    if (pimpl->widget) {
        pimpl->widget->rating = stars;
        pimpl->widget->update();
    }
}

int Rating::get_rating() const {
    if (pimpl->widget) return pimpl->widget->rating;
    return 0;
}

EventConnection Rating::on_change(std::function<void(int)> handler) {
    if (pimpl->widget) {
        pimpl->widget->on_change_handler = handler;
        auto weak_widget = std::weak_ptr<QPointer<QRatingWidget>>(std::make_shared<QPointer<QRatingWidget>>(pimpl->widget));
        return EventConnection([weak_widget]() {
            if (auto p = weak_widget.lock()) {
                if (*p) (*p)->on_change_handler = nullptr;
            }
        });
    }
    return EventConnection();
}

QWidget* Rating::get_qwidget() const {
    return pimpl->widget.data();
}

}
