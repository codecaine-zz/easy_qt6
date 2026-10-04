#include "simplegui/knob.h"
#include <QDial>
#include <QPointer>

namespace simplegui {

struct Knob::Impl {
    QPointer<QDial> qdial;
    Impl(int min_val, int max_val, int initial_val) {
        qdial = new QDial();
        qdial->setRange(min_val, max_val);
        qdial->setValue(initial_val);
    }
    ~Impl() { if (qdial && !qdial->parent()) delete qdial; }
};

Knob::Knob(int min_val, int max_val, int initial_val)
    : pimpl(std::make_shared<Impl>(min_val, max_val, initial_val)) {}

Knob::~Knob() = default;

int Knob::get_value() const {
    if (pimpl->qdial) return pimpl->qdial->value();
    return 0;
}

void Knob::set_value(int value) {
    if (pimpl->qdial) pimpl->qdial->setValue(value);
}

EventConnection Knob::on_change(std::function<void(int)> handler) {
    if (pimpl->qdial) {
        auto conn = QObject::connect(pimpl->qdial.data(), &QDial::valueChanged, [handler](int value) {
            handler(value);
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* Knob::get_qwidget() const {
    return pimpl->qdial.data();
}

}
