#include "simplegui/search_field.h"
#include <QLineEdit>
#include <QString>
#include <QPointer>

namespace simplegui {

struct SearchField::Impl {
    QPointer<QLineEdit> qline;
    Impl(const std::string& placeholder) {
        qline = new QLineEdit();
        qline->setPlaceholderText(QString::fromStdString(placeholder));
        qline->setClearButtonEnabled(true);
    }
    ~Impl() { if (qline && !qline->parent()) delete qline; }
};

SearchField::SearchField(const std::string& placeholder)
    : pimpl(std::make_shared<Impl>(placeholder)) {}

SearchField::~SearchField() = default;

std::string SearchField::get_text() const {
    if (pimpl->qline) return pimpl->qline->text().toStdString();
    return "";
}

void SearchField::set_text(const std::string& text) {
    if (pimpl->qline) pimpl->qline->setText(QString::fromStdString(text));
}

EventConnection SearchField::on_change(std::function<void(const std::string&)> handler) {
    if (pimpl->qline) {
        auto conn = QObject::connect(pimpl->qline.data(), &QLineEdit::textChanged, [handler](const QString& text) {
            handler(text.toStdString());
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* SearchField::get_qwidget() const {
    return pimpl->qline.data();
}

}
