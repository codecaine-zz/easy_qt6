#include "simplegui/radio.h"
#include <QRadioButton>
#include <QString>
#include <QPointer>

namespace simplegui {

struct Radio::Impl {
    QPointer<QRadioButton> qradio;
    Impl(const std::string& text, bool checked) {
        qradio = new QRadioButton(QString::fromStdString(text));
        qradio->setChecked(checked);
    }
    ~Impl() { if (qradio && !qradio->parent()) delete qradio; }
};

Radio::Radio(const std::string& text, bool checked)
    : pimpl(std::make_shared<Impl>(text, checked)) {}

Radio::~Radio() = default;

bool Radio::is_checked() const {
    if (pimpl->qradio) {
        return pimpl->qradio->isChecked();
    }
    return false;
}

void Radio::set_checked(bool checked) {
    if (pimpl->qradio) {
        pimpl->qradio->setChecked(checked);
    }
}

EventConnection Radio::on_change(std::function<void(bool)> handler) {
    if (pimpl->qradio) {
        auto conn = QObject::connect(pimpl->qradio.data(), &QRadioButton::toggled, [handler](bool checked) {
            handler(checked);
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* Radio::get_qwidget() const {
    return pimpl->qradio.data();
}

}
