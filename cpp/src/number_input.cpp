#include "simplegui/number_input.h"
#include "detail/common.h"

#include <QSpinBox>

namespace simplegui {

struct NumberInput::Impl {
    QPointer<QSpinBox> spin;
    Impl(int min_val, int max_val, int initial_val) : spin(new QSpinBox()) {
        spin->setRange(std::min(min_val, max_val), std::max(min_val, max_val));
        spin->setValue(initial_val);
    }
    ~Impl() { detail::delete_if_orphan(spin); }
};

NumberInput::NumberInput(int min_val, int max_val, int initial_val)
    : pimpl(std::make_shared<Impl>(min_val, max_val, initial_val)) {}

NumberInput::~NumberInput() = default;

int NumberInput::get_value() const {
    return pimpl->spin ? pimpl->spin->value() : 0;
}

void NumberInput::set_value(int value) {
    if (pimpl->spin) pimpl->spin->setValue(value);
}

void NumberInput::set_range(int min_val, int max_val) {
    if (pimpl->spin) pimpl->spin->setRange(std::min(min_val, max_val), std::max(min_val, max_val));
}

void NumberInput::set_step(int step) {
    if (pimpl->spin && step > 0) pimpl->spin->setSingleStep(step);
}

void NumberInput::set_suffix(const std::string& suffix) {
    if (pimpl->spin) pimpl->spin->setSuffix(detail::qs(suffix));
}

EventConnection NumberInput::on_change(std::function<void(int)> handler) {
    if (!pimpl->spin || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->spin.data(), &QSpinBox::valueChanged,
                                         [handler = std::move(handler)](int value) { handler(value); }));
}

QWidget* NumberInput::get_qwidget() const {
    return pimpl->spin.data();
}

}  // namespace simplegui
