#include "simplegui/textarea.h"
#include <QTextEdit>
#include <QString>
#include <QPointer>

namespace simplegui {

struct Textarea::Impl {
    QPointer<QTextEdit> qtext;
    Impl(const std::string& text) {
        qtext = new QTextEdit();
        qtext->setPlainText(QString::fromStdString(text));
    }
    ~Impl() { if (qtext && !qtext->parent()) delete qtext; }
};

Textarea::Textarea(const std::string& text)
    : pimpl(std::make_shared<Impl>(text)) {}

Textarea::~Textarea() = default;

std::string Textarea::get_text() const {
    if (pimpl->qtext) {
        return pimpl->qtext->toPlainText().toStdString();
    }
    return "";
}

void Textarea::set_text(const std::string& text) {
    if (pimpl->qtext) {
        pimpl->qtext->setPlainText(QString::fromStdString(text));
    }
}

EventConnection Textarea::on_change(std::function<void(const std::string&)> handler) {
    if (pimpl->qtext) {
        auto conn = QObject::connect(pimpl->qtext.data(), &QTextEdit::textChanged, [this, handler]() {
            handler(get_text());
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* Textarea::get_qwidget() const {
    return pimpl->qtext.data();
}

}
