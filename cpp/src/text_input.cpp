#include "simplegui/text_input.h"
#include <QLineEdit>
#include <QString>
#include <QPointer>

namespace simplegui {

struct TextInput::Impl {
    QPointer<QLineEdit> qlineedit;
    Impl(const std::string& placeholder) {
        qlineedit = new QLineEdit();
        qlineedit->setPlaceholderText(QString::fromStdString(placeholder));
    }
    ~Impl() { if (qlineedit && !qlineedit->parent()) delete qlineedit; }
};

TextInput::TextInput(const std::string& placeholder)
    : pimpl(std::make_shared<Impl>(placeholder)) {}

TextInput::~TextInput() = default;

std::string TextInput::get_text() const {
    if (pimpl->qlineedit) {
        return pimpl->qlineedit->text().toStdString();
    }
    return "";
}

void TextInput::set_text(const std::string& text) {
    if (pimpl->qlineedit) {
        pimpl->qlineedit->setText(QString::fromStdString(text));
    }
}

EventConnection TextInput::on_change(std::function<void(const std::string&)> handler) {
    if (pimpl->qlineedit) {
        auto conn = QObject::connect(pimpl->qlineedit.data(), &QLineEdit::textChanged, [handler](const QString& text) {
            handler(text.toStdString());
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* TextInput::get_qwidget() const {
    return pimpl->qlineedit.data();
}

}
