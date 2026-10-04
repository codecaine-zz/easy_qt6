#include "simplegui/slider.h"
#include <QSlider>
#include <QPointer>

namespace simplegui {

struct Slider::Impl {
    QPointer<QSlider> qslider;
    Impl(int min_val, int max_val, int initial_val) {
        qslider = new QSlider(Qt::Horizontal);
        qslider->setRange(min_val, max_val);
        qslider->setValue(initial_val);
    }
    ~Impl() { if (qslider && !qslider->parent()) delete qslider; }
};

Slider::Slider(int min_val, int max_val, int initial_val)
    : pimpl(std::make_shared<Impl>(min_val, max_val, initial_val)) {}

Slider::~Slider() = default;

int Slider::get_value() const {
    if (pimpl->qslider) {
        return pimpl->qslider->value();
    }
    return 0;
}

void Slider::set_value(int value) {
    if (pimpl->qslider) {
        pimpl->qslider->setValue(value);
    }
}

EventConnection Slider::on_change(std::function<void(int)> handler) {
    if (pimpl->qslider) {
        auto conn = QObject::connect(pimpl->qslider.data(), &QSlider::valueChanged, [handler](int value) {
            handler(value);
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* Slider::get_qwidget() const {
    return pimpl->qslider.data();
}

}
