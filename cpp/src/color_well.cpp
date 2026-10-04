#include "simplegui/color_well.h"
#include <QPushButton>
#include <QColorDialog>
#include <QPointer>
#include <QColor>

namespace simplegui {

struct ColorWell::Impl {
    QPointer<QPushButton> qbtn;
    QColor current_color;
    std::function<void(const std::string&)> on_change_handler;

    Impl(const std::string& hex_color) {
        qbtn = new QPushButton();
        set_color(QString::fromStdString(hex_color));
    }
    ~Impl() { if (qbtn && !qbtn->parent()) delete qbtn; }

    void set_color(const QColor& color) {
        current_color = color;
        QString style = QString("background-color: %1; border: 1px solid #ccc;").arg(color.name());
        if (qbtn) qbtn->setStyleSheet(style);
    }
};

ColorWell::ColorWell(const std::string& hex_color)
    : pimpl(std::make_shared<Impl>(hex_color)) {
    
    QObject::connect(pimpl->qbtn.data(), &QPushButton::clicked, [this]() {
        QColor new_color = QColorDialog::getColor(pimpl->current_color, pimpl->qbtn, "Select Color");
        if (new_color.isValid()) {
            pimpl->set_color(new_color);
            if (pimpl->on_change_handler) {
                pimpl->on_change_handler(new_color.name().toStdString());
            }
        }
    });
}

ColorWell::~ColorWell() = default;

std::string ColorWell::get_color() const {
    return pimpl->current_color.name().toStdString();
}

void ColorWell::set_color(const std::string& hex_color) {
    pimpl->set_color(QColor(QString::fromStdString(hex_color)));
}

EventConnection ColorWell::on_change(std::function<void(const std::string&)> handler) {
    pimpl->on_change_handler = handler;
    return EventConnection();
}

QWidget* ColorWell::get_qwidget() const {
    return pimpl->qbtn.data();
}

}
