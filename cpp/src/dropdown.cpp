#include "simplegui/dropdown.h"
#include <QComboBox>
#include <QString>
#include <QPointer>

namespace simplegui {

struct Dropdown::Impl {
    QPointer<QComboBox> qcombo;
    Impl(const std::vector<std::string>& items) {
        qcombo = new QComboBox();
        for (const auto& item : items) {
            qcombo->addItem(QString::fromStdString(item));
        }
    }
    ~Impl() { if (qcombo && !qcombo->parent()) delete qcombo; }
};

Dropdown::Dropdown(const std::vector<std::string>& items)
    : pimpl(std::make_shared<Impl>(items)) {}

Dropdown::~Dropdown() = default;

void Dropdown::add_item(const std::string& item) {
    if (pimpl->qcombo) {
        pimpl->qcombo->addItem(QString::fromStdString(item));
    }
}

std::string Dropdown::get_selected() const {
    if (pimpl->qcombo) {
        return pimpl->qcombo->currentText().toStdString();
    }
    return "";
}

void Dropdown::set_selected(const std::string& item) {
    if (pimpl->qcombo) {
        pimpl->qcombo->setCurrentText(QString::fromStdString(item));
    }
}

EventConnection Dropdown::on_change(std::function<void(const std::string&)> handler) {
    if (pimpl->qcombo) {
        auto conn = QObject::connect(pimpl->qcombo.data(), &QComboBox::currentTextChanged, [handler](const QString& text) {
            handler(text.toStdString());
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* Dropdown::get_qwidget() const {
    return pimpl->qcombo.data();
}

}
