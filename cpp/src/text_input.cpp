#include "simplegui/text_input.h"
#include "detail/common.h"

#include <QLineEdit>

namespace simplegui {

struct TextInput::Impl {
    QPointer<QLineEdit> edit;
    explicit Impl(const std::string& placeholder) : edit(new QLineEdit()) {
        edit->setPlaceholderText(detail::qs(placeholder));
    }
    ~Impl() { detail::delete_if_orphan(edit); }
};

TextInput::TextInput(const std::string& placeholder) : pimpl(std::make_shared<Impl>(placeholder)) {}

TextInput::~TextInput() = default;

std::string TextInput::get_text() const {
    return pimpl->edit ? detail::ss(pimpl->edit->text()) : std::string();
}

void TextInput::set_text(const std::string& text) {
    if (pimpl->edit) pimpl->edit->setText(detail::qs(text));
}

void TextInput::set_placeholder(const std::string& placeholder) {
    if (pimpl->edit) pimpl->edit->setPlaceholderText(detail::qs(placeholder));
}

void TextInput::set_read_only(bool read_only) {
    if (pimpl->edit) pimpl->edit->setReadOnly(read_only);
}

void TextInput::set_max_length(int characters) {
    if (pimpl->edit && characters > 0) pimpl->edit->setMaxLength(characters);
}

void TextInput::clear() {
    if (pimpl->edit) pimpl->edit->clear();
}

EventConnection TextInput::on_change(std::function<void(const std::string&)> handler) {
    if (!pimpl->edit || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->edit.data(), &QLineEdit::textChanged,
                                         [handler = std::move(handler)](const QString& text) {
                                             handler(detail::ss(text));
                                         }));
}

EventConnection TextInput::on_enter(std::function<void(const std::string&)> handler) {
    if (!pimpl->edit || !handler) return {};
    QPointer<QLineEdit> edit = pimpl->edit;
    return detail::wrap(QObject::connect(pimpl->edit.data(), &QLineEdit::returnPressed,
                                         [edit, handler = std::move(handler)]() {
                                             if (edit) handler(detail::ss(edit->text()));
                                         }));
}

QWidget* TextInput::get_qwidget() const {
    return pimpl->edit.data();
}

}  // namespace simplegui
