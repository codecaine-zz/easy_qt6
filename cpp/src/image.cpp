#include "simplegui/image.h"
#include <QLabel>
#include <QPixmap>
#include <QString>
#include <QPointer>

namespace simplegui {

struct Image::Impl {
    QPointer<QLabel> qlabel;
    Impl(const std::string& image_path) {
        qlabel = new QLabel();
        qlabel->setPixmap(QPixmap(QString::fromStdString(image_path)));
        qlabel->setAlignment(Qt::AlignCenter);
    }
    ~Impl() { if (qlabel && !qlabel->parent()) delete qlabel; }
};

Image::Image(const std::string& image_path)
    : pimpl(std::make_shared<Impl>(image_path)) {}

Image::~Image() = default;

void Image::set_image(const std::string& image_path) {
    if (pimpl->qlabel) {
        pimpl->qlabel->setPixmap(QPixmap(QString::fromStdString(image_path)));
    }
}

QWidget* Image::get_qwidget() const {
    return pimpl->qlabel.data();
}

}
