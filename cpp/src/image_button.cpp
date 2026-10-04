#include "simplegui/image_button.h"
#include <QPushButton>
#include <QIcon>
#include <QString>
#include <QPointer>

namespace simplegui {

struct ImageButton::Impl {
    QPointer<QPushButton> qbtn;
    Impl(const std::string& image_path, const std::string& tooltip) {
        qbtn = new QPushButton();
        qbtn->setIcon(QIcon(QString::fromStdString(image_path)));
        qbtn->setToolTip(QString::fromStdString(tooltip));
    }
    ~Impl() { if (qbtn && !qbtn->parent()) delete qbtn; }
};

ImageButton::ImageButton(const std::string& image_path, const std::string& tooltip)
    : pimpl(std::make_shared<Impl>(image_path, tooltip)) {}

ImageButton::~ImageButton() = default;

EventConnection ImageButton::on_click(std::function<void()> handler) {
    if (pimpl->qbtn) {
        auto conn = QObject::connect(pimpl->qbtn.data(), &QPushButton::clicked, [handler]() {
            handler();
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* ImageButton::get_qwidget() const {
    return pimpl->qbtn.data();
}

}
