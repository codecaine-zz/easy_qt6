#include "simplegui/number_input.h"
#include <QSpinBox>
#include <QPointer>

namespace simplegui {

struct NumberInput::Impl {
    QPointer<QSpinBox> qspin;
    Impl(int min_val, int max_val, int initial_val) {
        qspin = new QSpinBox();
        qspin->setRange(min_val, max_val);
        qspin->setValue(initial_val);
    }
    ~Impl() { if (qspin && !qspin->parent()) delete qspin; }
};

NumberInput::NumberInput(int min_val, int max_val, int initial_val)
    : pimpl(std::make_shared<Impl>(min_val, max_val, initial_val)) {}

NumberInput::~NumberInput() = default;

int NumberInput::get_value() const {
    if (pimpl->qspin) return pimpl->qspin->value();
    return 0;
}

void NumberInput::set_value(int value) {
    if (pimpl->qspin) pimpl->qspin->setValue(value);
}

EventConnection NumberInput::on_change(std::function<void(int)> handler) {
    if (pimpl->qspin) {
        auto conn = QObject::connect(pimpl->qspin.data(), &QSpinBox::valueChanged, [handler](int value) {
            handler(value);
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* NumberInput::get_qwidget() const {
    return pimpl->qspin.data();
}

}
