#include "simplegui/button.h"
#include <QPushButton>
#include <QString>
#include <QPointer>

namespace simplegui {

struct Button::Impl {
    QPointer<QPushButton> qbutton;
    Impl(const std::string& text) { qbutton = new QPushButton(QString::fromStdString(text)); }
    ~Impl() { if (qbutton && !qbutton->parent()) delete qbutton; }
};

Button::Button(const std::string& text)
    : pimpl(std::make_shared<Impl>(text)) {}

Button::~Button() = default;

EventConnection Button::on_click(std::function<void()> handler) {
    if (pimpl->qbutton) {
        auto conn = QObject::connect(pimpl->qbutton.data(), &QPushButton::clicked, [handler]() {
            handler();
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* Button::get_qwidget() const {
    return pimpl->qbutton.data();
}

}
