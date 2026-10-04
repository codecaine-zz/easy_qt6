#include "simplegui/image_button.h"
#include "detail/common.h"

#include <QIcon>
#include <QPixmap>
#include <QPushButton>

namespace simplegui {

struct ImageButton::Impl {
    QPointer<QPushButton> button = new QPushButton();
    ~Impl() { detail::delete_if_orphan(button); }
};

ImageButton::ImageButton(const std::string& image_path, const std::string& tooltip)
    : pimpl(std::make_shared<Impl>()) {
    pimpl->button->setCursor(Qt::PointingHandCursor);
    if (!image_path.empty()) set_image(image_path);
    if (!tooltip.empty()) set_tooltip(tooltip);
}

ImageButton::~ImageButton() = default;

bool ImageButton::set_image(const std::string& image_path) {
    if (!pimpl->button) return false;
    QPixmap pixmap(detail::qs(image_path));
    pimpl->button->setIcon(QIcon(pixmap));
    return !pixmap.isNull();
}

void ImageButton::set_icon_size(int pixels) {
    if (pimpl->button && pixels > 0) pimpl->button->setIconSize(QSize(pixels, pixels));
}

void ImageButton::set_text(const std::string& text) {
    if (pimpl->button) pimpl->button->setText(detail::qs(text));
}

EventConnection ImageButton::on_click(std::function<void()> handler) {
    if (!pimpl->button || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->button.data(), &QPushButton::clicked,
                                         [handler = std::move(handler)]() { handler(); }));
}

QWidget* ImageButton::get_qwidget() const {
    return pimpl->button.data();
}

}  // namespace simplegui
