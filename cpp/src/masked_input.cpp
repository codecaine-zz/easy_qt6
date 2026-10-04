#include "simplegui/masked_input.h"
#include "detail/common.h"

#include <QLineEdit>

namespace simplegui {

struct MaskedInput::Impl {
    QPointer<QLineEdit> line_edit = new QLineEdit();
    ~Impl() { detail::delete_if_orphan(line_edit); }
};

MaskedInput::MaskedInput(const std::string& mask, const std::string& text) : pimpl(std::make_shared<Impl>()) {
    detail::set_base_style(pimpl->line_edit, QStringLiteral(
        "QLineEdit { background: #1e293b; color: #f8fafc; border: 1px solid #334155;"
        "  border-radius: 6px; padding: 8px 12px; font-family: monospace; font-size: 13px; }"
        "QLineEdit:focus { border-color: #3b82f6; }"));
    set_mask(mask);
    set_text(text);
}

MaskedInput::~MaskedInput() = default;

void MaskedInput::set_mask(const std::string& mask) {
    if (pimpl->line_edit) pimpl->line_edit->setInputMask(detail::qs(mask));
}

void MaskedInput::set_text(const std::string& text) {
    if (pimpl->line_edit) pimpl->line_edit->setText(detail::qs(text));
}

void MaskedInput::set_placeholder(const std::string& placeholder) {
    if (pimpl->line_edit) pimpl->line_edit->setPlaceholderText(detail::qs(placeholder));
}

void MaskedInput::clear() {
    if (pimpl->line_edit) pimpl->line_edit->clear();
}

std::string MaskedInput::text() const {
    return pimpl->line_edit ? detail::ss(pimpl->line_edit->text()) : std::string();
}

bool MaskedInput::is_complete() const {
    return pimpl->line_edit && pimpl->line_edit->hasAcceptableInput();
}

EventConnection MaskedInput::on_change(std::function<void(const std::string&)> handler) {
    if (!pimpl->line_edit || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->line_edit.data(), &QLineEdit::textChanged,
                                         [handler = std::move(handler)](const QString& t) { handler(detail::ss(t)); }));
}

QWidget* MaskedInput::get_qwidget() const { return pimpl->line_edit.data(); }

}  // namespace simplegui
