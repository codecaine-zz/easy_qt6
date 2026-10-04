#include "simplegui/password_input.h"
#include "detail/common.h"

#include <QLineEdit>

namespace simplegui {

struct PasswordInput::Impl {
    QPointer<QLineEdit> edit;
    explicit Impl(const std::string& placeholder) : edit(new QLineEdit()) {
        edit->setPlaceholderText(detail::qs(placeholder));
        edit->setEchoMode(QLineEdit::Password);
    }
    ~Impl() { detail::delete_if_orphan(edit); }
};

PasswordInput::PasswordInput(const std::string& placeholder) : pimpl(std::make_shared<Impl>(placeholder)) {}

PasswordInput::~PasswordInput() = default;

std::string PasswordInput::get_text() const {
    return pimpl->edit ? detail::ss(pimpl->edit->text()) : std::string();
}

void PasswordInput::set_text(const std::string& text) {
    if (pimpl->edit) pimpl->edit->setText(detail::qs(text));
}

void PasswordInput::set_placeholder(const std::string& placeholder) {
    if (pimpl->edit) pimpl->edit->setPlaceholderText(detail::qs(placeholder));
}

void PasswordInput::set_reveal(bool reveal) {
    if (pimpl->edit) pimpl->edit->setEchoMode(reveal ? QLineEdit::Normal : QLineEdit::Password);
}

void PasswordInput::clear() {
    if (pimpl->edit) pimpl->edit->clear();
}

EventConnection PasswordInput::on_change(std::function<void(const std::string&)> handler) {
    if (!pimpl->edit || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->edit.data(), &QLineEdit::textChanged,
                                         [handler = std::move(handler)](const QString& text) {
                                             handler(detail::ss(text));
                                         }));
}

EventConnection PasswordInput::on_enter(std::function<void(const std::string&)> handler) {
    if (!pimpl->edit || !handler) return {};
    QPointer<QLineEdit> edit = pimpl->edit;
    return detail::wrap(QObject::connect(pimpl->edit.data(), &QLineEdit::returnPressed,
                                         [edit, handler = std::move(handler)]() {
                                             if (edit) handler(detail::ss(edit->text()));
                                         }));
}

QWidget* PasswordInput::get_qwidget() const {
    return pimpl->edit.data();
}

}  // namespace simplegui
