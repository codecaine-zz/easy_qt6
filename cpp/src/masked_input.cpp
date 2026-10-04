#include "simplegui/masked_input.h"
#include <QLineEdit>
#include <QPointer>

namespace simplegui {

struct MaskedInput::Impl {
    QPointer<QLineEdit> line_edit;
};

MaskedInput::MaskedInput(const std::string& mask, const std::string& text)
    : pimpl(std::make_shared<Impl>())
{
    auto* edit = new QLineEdit();
    pimpl->line_edit = edit;

    edit->setStyleSheet(
        "QLineEdit {"
        "  background: #1e293b;"
        "  color: #f8fafc;"
        "  border: 1px solid #334155;"
        "  border-radius: 6px;"
        "  padding: 8px 12px;"
        "  font-family: monospace;"
        "  font-size: 13px;"
        "}"
        "QLineEdit:focus {"
        "  border-color: #3b82f6;"
        "}"
    );

    if (!mask.empty()) {
        edit->setInputMask(QString::fromStdString(mask));
    }
    if (!text.empty()) {
        edit->setText(QString::fromStdString(text));
    }
}

MaskedInput::~MaskedInput() = default;

void MaskedInput::set_mask(const std::string& mask) {
    if (pimpl->line_edit) {
        pimpl->line_edit->setInputMask(QString::fromStdString(mask));
    }
}

void MaskedInput::set_text(const std::string& text) {
    if (pimpl->line_edit) {
        pimpl->line_edit->setText(QString::fromStdString(text));
    }
}

void MaskedInput::set_placeholder(const std::string& placeholder) {
    if (pimpl->line_edit) {
        pimpl->line_edit->setPlaceholderText(QString::fromStdString(placeholder));
    }
}

std::string MaskedInput::text() const {
    if (!pimpl->line_edit) return "";
    return pimpl->line_edit->text().toStdString();
}

EventConnection MaskedInput::on_change(std::function<void(const std::string& text)> handler) {
    if (!pimpl->line_edit) return {};
    auto conn = QObject::connect(pimpl->line_edit, &QLineEdit::textChanged, [handler](const QString& qs) {
        if (handler) handler(qs.toStdString());
    });
    return EventConnection([conn]() { QObject::disconnect(conn); });
}

QWidget* MaskedInput::get_qwidget() const {
    return pimpl->line_edit.data();
}

}
