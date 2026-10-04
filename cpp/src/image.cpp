#include "simplegui/image.h"
#include "detail/common.h"

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>

namespace simplegui {

namespace {

// A label that can scale its picture while keeping the aspect ratio.
class ImageLabel : public QLabel {
public:
    QPixmap original;
    bool scaled = false;

    ImageLabel() {
        setAlignment(Qt::AlignCenter);
        setMinimumSize(1, 1);
    }

    void refresh() {
        if (original.isNull()) {
            clear();
        } else if (scaled) {
            setPixmap(original.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            setPixmap(original);
        }
    }

protected:
    void resizeEvent(QResizeEvent* event) override {
        QLabel::resizeEvent(event);
        if (scaled) refresh();
    }
};

}  // namespace

struct Image::Impl {
    QPointer<ImageLabel> label = new ImageLabel();
    ~Impl() { detail::delete_if_orphan(label); }
};

Image::Image(const std::string& image_path) : pimpl(std::make_shared<Impl>()) {
    if (!image_path.empty()) set_image(image_path);
}

Image::~Image() = default;

bool Image::set_image(const std::string& image_path) {
    if (!pimpl->label) return false;
    QPixmap pixmap(detail::qs(image_path));
    pimpl->label->original = pixmap;
    pimpl->label->refresh();
    return !pixmap.isNull();
}

void Image::set_scaled(bool scaled) {
    if (!pimpl->label) return;
    pimpl->label->scaled = scaled;
    pimpl->label->refresh();
}

QWidget* Image::get_qwidget() const {
    return pimpl->label.data();
}

}  // namespace simplegui
