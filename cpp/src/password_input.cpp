#include "simplegui/password_input.h"
#include <QLineEdit>
#include <QString>
#include <QPointer>

namespace simplegui {

struct PasswordInput::Impl {
    QPointer<QLineEdit> qline;
    Impl(const std::string& placeholder) {
        qline = new QLineEdit();
        qline->setPlaceholderText(QString::fromStdString(placeholder));
        qline->setEchoMode(QLineEdit::Password);
    }
    ~Impl() { if (qline && !qline->parent()) delete qline; }
};

PasswordInput::PasswordInput(const std::string& placeholder)
    : pimpl(std::make_shared<Impl>(placeholder)) {}

PasswordInput::~PasswordInput() = default;

std::string PasswordInput::get_text() const {
    if (pimpl->qline) return pimpl->qline->text().toStdString();
    return "";
}

void PasswordInput::set_text(const std::string& text) {
    if (pimpl->qline) pimpl->qline->setText(QString::fromStdString(text));
}

EventConnection PasswordInput::on_change(std::function<void(const std::string&)> handler) {
    if (pimpl->qline) {
        auto conn = QObject::connect(pimpl->qline.data(), &QLineEdit::textChanged, [handler](const QString& text) {
            handler(text.toStdString());
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* PasswordInput::get_qwidget() const {
    return pimpl->qline.data();
}

}
