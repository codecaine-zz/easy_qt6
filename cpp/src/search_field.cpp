#include "simplegui/search_field.h"
#include "detail/common.h"

#include <QLineEdit>

namespace simplegui {

struct SearchField::Impl {
    QPointer<QLineEdit> edit;
    explicit Impl(const std::string& placeholder) : edit(new QLineEdit()) {
        edit->setPlaceholderText(detail::qs(placeholder));
        edit->setClearButtonEnabled(true);
    }
    ~Impl() { detail::delete_if_orphan(edit); }
};

SearchField::SearchField(const std::string& placeholder) : pimpl(std::make_shared<Impl>(placeholder)) {}

SearchField::~SearchField() = default;

std::string SearchField::get_text() const {
    return pimpl->edit ? detail::ss(pimpl->edit->text()) : std::string();
}

void SearchField::set_text(const std::string& text) {
    if (pimpl->edit) pimpl->edit->setText(detail::qs(text));
}

void SearchField::set_placeholder(const std::string& placeholder) {
    if (pimpl->edit) pimpl->edit->setPlaceholderText(detail::qs(placeholder));
}

void SearchField::clear() {
    if (pimpl->edit) pimpl->edit->clear();
}

EventConnection SearchField::on_change(std::function<void(const std::string&)> handler) {
    if (!pimpl->edit || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->edit.data(), &QLineEdit::textChanged,
                                         [handler = std::move(handler)](const QString& text) {
                                             handler(detail::ss(text));
                                         }));
}

EventConnection SearchField::on_enter(std::function<void(const std::string&)> handler) {
    if (!pimpl->edit || !handler) return {};
    QPointer<QLineEdit> edit = pimpl->edit;
    return detail::wrap(QObject::connect(pimpl->edit.data(), &QLineEdit::returnPressed,
                                         [edit, handler = std::move(handler)]() {
                                             if (edit) handler(detail::ss(edit->text()));
                                         }));
}

QWidget* SearchField::get_qwidget() const {
    return pimpl->edit.data();
}

}  // namespace simplegui
