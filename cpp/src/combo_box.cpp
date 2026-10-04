#include "simplegui/combo_box.h"
#include <QComboBox>
#include <QString>
#include <QPointer>

namespace simplegui {

struct ComboBox::Impl {
    QPointer<QComboBox> qcombo;
    Impl(const std::vector<std::string>& items) {
        qcombo = new QComboBox();
        qcombo->setEditable(true); // Editable combo box
        for (const auto& item : items) {
            qcombo->addItem(QString::fromStdString(item));
        }
    }
    ~Impl() { if (qcombo && !qcombo->parent()) delete qcombo; }
};

ComboBox::ComboBox(const std::vector<std::string>& items)
    : pimpl(std::make_shared<Impl>(items)) {}

ComboBox::~ComboBox() = default;

void ComboBox::add_item(const std::string& item) {
    if (pimpl->qcombo) {
        pimpl->qcombo->addItem(QString::fromStdString(item));
    }
}

std::string ComboBox::get_text() const {
    if (pimpl->qcombo) {
        return pimpl->qcombo->currentText().toStdString();
    }
    return "";
}

void ComboBox::set_text(const std::string& text) {
    if (pimpl->qcombo) {
        pimpl->qcombo->setCurrentText(QString::fromStdString(text));
    }
}

EventConnection ComboBox::on_change(std::function<void(const std::string&)> handler) {
    if (pimpl->qcombo) {
        auto conn = QObject::connect(pimpl->qcombo.data(), &QComboBox::currentTextChanged, [handler](const QString& text) {
            handler(text.toStdString());
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* ComboBox::get_qwidget() const {
    return pimpl->qcombo.data();
}

}
