#include "simplegui/textarea.h"
#include "detail/common.h"

#include <QPlainTextEdit>

namespace simplegui {

struct Textarea::Impl {
    QPointer<QPlainTextEdit> edit;
    explicit Impl(const std::string& text) : edit(new QPlainTextEdit()) {
        edit->setPlainText(detail::qs(text));
    }
    ~Impl() { detail::delete_if_orphan(edit); }
};

Textarea::Textarea(const std::string& text) : pimpl(std::make_shared<Impl>(text)) {}

Textarea::~Textarea() = default;

std::string Textarea::get_text() const {
    return pimpl->edit ? detail::ss(pimpl->edit->toPlainText()) : std::string();
}

void Textarea::set_text(const std::string& text) {
    if (pimpl->edit) pimpl->edit->setPlainText(detail::qs(text));
}

void Textarea::append_text(const std::string& line) {
    if (pimpl->edit) pimpl->edit->appendPlainText(detail::qs(line));
}

void Textarea::set_placeholder(const std::string& placeholder) {
    if (pimpl->edit) pimpl->edit->setPlaceholderText(detail::qs(placeholder));
}

void Textarea::set_read_only(bool read_only) {
    if (pimpl->edit) pimpl->edit->setReadOnly(read_only);
}

void Textarea::clear() {
    if (pimpl->edit) pimpl->edit->clear();
}

EventConnection Textarea::on_change(std::function<void(const std::string&)> handler) {
    if (!pimpl->edit || !handler) return {};
    // Capture the widget (not `this`) so the handler can never touch a destroyed object.
    QPointer<QPlainTextEdit> edit = pimpl->edit;
    return detail::wrap(QObject::connect(pimpl->edit.data(), &QPlainTextEdit::textChanged,
                                         [edit, handler = std::move(handler)]() {
                                             if (edit) handler(detail::ss(edit->toPlainText()));
                                         }));
}

QWidget* Textarea::get_qwidget() const {
    return pimpl->edit.data();
}

}  // namespace simplegui
