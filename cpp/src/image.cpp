#include "simplegui/image.h"
#include "detail/common.h"

#include <QLabel>
#include <QPixmap>
#include <QResizeEvent>
#include <QMovie>

namespace simplegui {

namespace {

// A label that can scale its picture while keeping the aspect ratio.
class ImageLabel : public QLabel {
public:
    QPixmap original_pixmap;
    QMovie* movie = nullptr;
    bool scaled = false;

    ImageLabel() {
        setAlignment(Qt::AlignCenter);
        setMinimumSize(1, 1);
    }

    void refresh() {
        if (movie) {
            if (scaled) {
                QSize origSize = movie->frameRect().size();
                if (origSize.isValid() && !origSize.isNull()) {
                    QSize newSize = origSize.scaled(size(), Qt::KeepAspectRatio);
                    movie->setScaledSize(newSize);
                }
            } else {
                movie->setScaledSize(QSize());
            }
        } else {
            if (original_pixmap.isNull()) {
                clear();
            } else if (scaled) {
                setPixmap(original_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                setPixmap(original_pixmap);
            }
        }
    }

protected:
    void resizeEvent(QResizeEvent* event) override {
        QLabel::resizeEvent(event);
        refresh();
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
    
    if (pimpl->label->movie) {
        pimpl->label->movie->stop();
        pimpl->label->movie->deleteLater();
        pimpl->label->movie = nullptr;
    }

    QString qpath = detail::qs(image_path);
    
    if (qpath.endsWith(".gif", Qt::CaseInsensitive)) {
        QMovie* movie = new QMovie(qpath, QByteArray(), pimpl->label.data());
        if (movie->isValid()) {
            pimpl->label->movie = movie;
            pimpl->label->original_pixmap = QPixmap();
            pimpl->label->setMovie(movie);
            movie->start();
            
            // Wait for it to start so the frameRect is loaded, then refresh size
            QObject::connect(movie, &QMovie::started, pimpl->label.data(), [lbl = pimpl->label.data()]() {
                if (lbl) lbl->refresh();
            });
            
            pimpl->label->refresh();
            return true;
        }
        delete movie;
    }
    
    QPixmap pixmap(qpath);
    pimpl->label->original_pixmap = pixmap;
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
