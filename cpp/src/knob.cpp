#include "simplegui/knob.h"
#include "detail/common.h"

#include <QDial>

namespace simplegui {

struct Knob::Impl {
    QPointer<QDial> dial;
    Impl(int min_val, int max_val, int initial_val) : dial(new QDial()) {
        dial->setRange(std::min(min_val, max_val), std::max(min_val, max_val));
        dial->setValue(initial_val);
    }
    ~Impl() { detail::delete_if_orphan(dial); }
};

Knob::Knob(int min_val, int max_val, int initial_val)
    : pimpl(std::make_shared<Impl>(min_val, max_val, initial_val)) {}

Knob::~Knob() = default;

int Knob::get_value() const {
    return pimpl->dial ? pimpl->dial->value() : 0;
}

void Knob::set_value(int value) {
    if (pimpl->dial) pimpl->dial->setValue(value);
}

void Knob::set_range(int min_val, int max_val) {
    if (pimpl->dial) pimpl->dial->setRange(std::min(min_val, max_val), std::max(min_val, max_val));
}

EventConnection Knob::on_change(std::function<void(int)> handler) {
    if (!pimpl->dial || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->dial.data(), &QDial::valueChanged,
                                         [handler = std::move(handler)](int value) { handler(value); }));
}

QWidget* Knob::get_qwidget() const {
    return pimpl->dial.data();
}

}  // namespace simplegui
