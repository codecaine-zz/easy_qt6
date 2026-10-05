#include "simplegui/signature_pad.h"
#include "detail/common.h"
#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>

namespace simplegui {

namespace {
class SignatureWidget : public QWidget {
public:
    QImage image;
    QPoint lastPoint;
    bool drawing = false;
    QColor penColor = Qt::black;
    int penWidth = 2;

    SignatureWidget(int w, int h) {
        setMinimumSize(w, h);
        image = QImage(size(), QImage::Format_RGB32);
        image.fill(Qt::white);
    }

    void clear() {
        image.fill(Qt::white);
        update();
    }

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton) {
            lastPoint = event->pos();
            drawing = true;
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if ((event->buttons() & Qt::LeftButton) && drawing) {
            QPainter painter(&image);
            painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(lastPoint, event->pos());
            lastPoint = event->pos();
            update();
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && drawing) {
            QPainter painter(&image);
            painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(lastPoint, event->pos());
            drawing = false;
            update();
        }
    }

    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        QRect dirtyRect = event->rect();
        painter.drawImage(dirtyRect, image, dirtyRect);
    }

    void resizeEvent(QResizeEvent *event) override {
        if (size() != image.size()) {
            QImage newImage(size(), QImage::Format_RGB32);
            newImage.fill(Qt::white);
            QPainter painter(&newImage);
            painter.drawImage(QPoint(0, 0), image);
            image = newImage;
        }
        QWidget::resizeEvent(event);
    }
};
}

struct SignaturePad::Impl {
    QPointer<SignatureWidget> widget;
    ~Impl() { detail::delete_if_orphan(widget); }
};

SignaturePad::SignaturePad(int width, int height) : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new SignatureWidget(width, height);
}

SignaturePad::~SignaturePad() = default;

void SignaturePad::clear() {
    if (pimpl->widget) pimpl->widget->clear();
}

bool SignaturePad::save_to_file(const std::string& file_path) const {
    if (!pimpl->widget) return false;
    return pimpl->widget->image.save(detail::qs(file_path));
}

void SignaturePad::set_pen_color(const std::string& color) {
    if (pimpl->widget) pimpl->widget->penColor = QColor(detail::qs(color));
}

void SignaturePad::set_pen_width(int width) {
    if (pimpl->widget) pimpl->widget->penWidth = width;
}

QWidget* SignaturePad::get_qwidget() const {
    return pimpl->widget.data();
}

}
