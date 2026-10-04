#include "simplegui/slider.h"
#include "detail/common.h"

#include <QSlider>

namespace simplegui {

struct Slider::Impl {
    QPointer<QSlider> slider;
    Impl(int min_val, int max_val, int initial_val) : slider(new QSlider(Qt::Horizontal)) {
        slider->setRange(std::min(min_val, max_val), std::max(min_val, max_val));
        slider->setValue(initial_val);
    }
    ~Impl() { detail::delete_if_orphan(slider); }
};

Slider::Slider(int min_val, int max_val, int initial_val)
    : pimpl(std::make_shared<Impl>(min_val, max_val, initial_val)) {}

Slider::~Slider() = default;

int Slider::get_value() const {
    return pimpl->slider ? pimpl->slider->value() : 0;
}

void Slider::set_value(int value) {
    if (pimpl->slider) pimpl->slider->setValue(value);
}

void Slider::set_range(int min_val, int max_val) {
    if (pimpl->slider) pimpl->slider->setRange(std::min(min_val, max_val), std::max(min_val, max_val));
}

void Slider::set_vertical(bool vertical) {
    if (pimpl->slider) pimpl->slider->setOrientation(vertical ? Qt::Vertical : Qt::Horizontal);
}

EventConnection Slider::on_change(std::function<void(int)> handler) {
    if (!pimpl->slider || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->slider.data(), &QSlider::valueChanged,
                                         [handler = std::move(handler)](int value) { handler(value); }));
}

QWidget* Slider::get_qwidget() const {
    return pimpl->slider.data();
}

}  // namespace simplegui
