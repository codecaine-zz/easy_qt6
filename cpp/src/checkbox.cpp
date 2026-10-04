#include "simplegui/checkbox.h"
#include <QCheckBox>
#include <QString>
#include <QPointer>

namespace simplegui {

struct Checkbox::Impl {
    QPointer<QCheckBox> qcheckbox;
    Impl(const std::string& text, bool checked) {
        qcheckbox = new QCheckBox(QString::fromStdString(text));
        qcheckbox->setChecked(checked);
    }
    ~Impl() { if (qcheckbox && !qcheckbox->parent()) delete qcheckbox; }
};

Checkbox::Checkbox(const std::string& text, bool checked)
    : pimpl(std::make_shared<Impl>(text, checked)) {}

Checkbox::~Checkbox() = default;

bool Checkbox::is_checked() const {
    if (pimpl->qcheckbox) {
        return pimpl->qcheckbox->isChecked();
    }
    return false;
}

void Checkbox::set_checked(bool checked) {
    if (pimpl->qcheckbox) {
        pimpl->qcheckbox->setChecked(checked);
    }
}

EventConnection Checkbox::on_change(std::function<void(bool)> handler) {
    if (pimpl->qcheckbox) {
        auto conn = QObject::connect(pimpl->qcheckbox.data(), &QCheckBox::toggled, [handler](bool checked) {
            handler(checked);
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* Checkbox::get_qwidget() const {
    return pimpl->qcheckbox.data();
}

}
