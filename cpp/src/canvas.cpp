#include "simplegui/canvas.h"
#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QPointer>

namespace simplegui {

class CanvasWidget : public QWidget {
public:
    QPixmap buffer;
    std::function<void(int, int)> mouse_down_handler;
    std::function<void(int, int)> mouse_move_handler;
    std::function<void(int, int)> mouse_up_handler;

    CanvasWidget(int min_w, int min_h, QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(min_w, min_h);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMouseTracking(true);
        init_buffer(min_w, min_h);
    }

    void init_buffer(int w, int h) {
        buffer = QPixmap(qMax(w, 100), qMax(h, 100));
        buffer.fill(QColor("#0f172a"));
    }

    void ensure_buffer_size(int w, int h) {
        if (buffer.isNull() || buffer.width() < w || buffer.height() < h) {
            int new_w = qMax(w, buffer.width());
            int new_h = qMax(h, buffer.height());
            QPixmap new_buf(new_w, new_h);
            new_buf.fill(QColor("#0f172a"));
            QPainter p(&new_buf);
            p.drawPixmap(0, 0, buffer);
            p.end();
            buffer = new_buf;
        }
    }

protected:
    void resizeEvent(QResizeEvent* event) override {
        QWidget::resizeEvent(event);
        ensure_buffer_size(width(), height());
    }

    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter p(this);
        p.drawPixmap(0, 0, buffer);
    }

    void mousePressEvent(QMouseEvent* event) override {
        if (mouse_down_handler) {
            mouse_down_handler(event->pos().x(), event->pos().y());
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        if (mouse_move_handler) {
            mouse_move_handler(event->pos().x(), event->pos().y());
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (mouse_up_handler) {
            mouse_up_handler(event->pos().x(), event->pos().y());
        }
    }
};

struct Canvas::Impl {
    QPointer<CanvasWidget> widget;
};

Canvas::Canvas(int min_width, int min_height)
    : pimpl(std::make_shared<Impl>())
{
    auto* w = new CanvasWidget(min_width, min_height);
    pimpl->widget = w;
}

Canvas::~Canvas() = default;

void Canvas::clear(const std::string& bg_color) {
    if (pimpl->widget) {
        pimpl->widget->buffer.fill(QColor(QString::fromStdString(bg_color)));
        pimpl->widget->update();
    }
}

void Canvas::draw_line(int x1, int y1, int x2, int y2, const std::string& color, int line_width) {
    if (pimpl->widget) {
        pimpl->widget->ensure_buffer_size(qMax(x1, x2) + 10, qMax(y1, y2) + 10);
        QPainter p(&pimpl->widget->buffer);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen(QColor(QString::fromStdString(color)), line_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawLine(x1, y1, x2, y2);
        p.end();
        pimpl->widget->update();
    }
}

void Canvas::draw_rect(int x, int y, int w, int h, const std::string& color, int line_width) {
    if (pimpl->widget) {
        pimpl->widget->ensure_buffer_size(x + w + 10, y + h + 10);
        QPainter p(&pimpl->widget->buffer);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen(QColor(QString::fromStdString(color)), line_width));
        p.setBrush(Qt::NoBrush);
        p.drawRect(x, y, w, h);
        p.end();
        pimpl->widget->update();
    }
}

void Canvas::fill_rect(int x, int y, int w, int h, const std::string& color) {
    if (pimpl->widget) {
        pimpl->widget->ensure_buffer_size(x + w + 10, y + h + 10);
        QPainter p(&pimpl->widget->buffer);
        p.fillRect(QRect(x, y, w, h), QColor(QString::fromStdString(color)));
        p.end();
        pimpl->widget->update();
    }
}

void Canvas::draw_circle(int cx, int cy, int radius, const std::string& color, int line_width) {
    if (pimpl->widget) {
        pimpl->widget->ensure_buffer_size(cx + radius + 10, cy + radius + 10);
        QPainter p(&pimpl->widget->buffer);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen(QColor(QString::fromStdString(color)), line_width));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(QPoint(cx, cy), radius, radius);
        p.end();
        pimpl->widget->update();
    }
}

void Canvas::fill_circle(int cx, int cy, int radius, const std::string& color) {
    if (pimpl->widget) {
        pimpl->widget->ensure_buffer_size(cx + radius + 10, cy + radius + 10);
        QPainter p(&pimpl->widget->buffer);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(QString::fromStdString(color)));
        p.drawEllipse(QPoint(cx, cy), radius, radius);
        p.end();
        pimpl->widget->update();
    }
}

void Canvas::draw_text(int x, int y, const std::string& text, const std::string& color, int font_size) {
    if (pimpl->widget) {
        QPainter p(&pimpl->widget->buffer);
        p.setRenderHint(QPainter::Antialiasing);
        QFont f = p.font();
        f.setPixelSize(font_size);
        p.setFont(f);
        p.setPen(QColor(QString::fromStdString(color)));
        p.drawText(x, y, QString::fromStdString(text));
        p.end();
        pimpl->widget->update();
    }
}

void Canvas::repaint() {
    if (pimpl->widget) {
        pimpl->widget->update();
    }
}

EventConnection Canvas::on_mouse_down(std::function<void(int x, int y)> handler) {
    pimpl->widget->mouse_down_handler = handler;
    return EventConnection([this]() {
        if (pimpl->widget) pimpl->widget->mouse_down_handler = nullptr;
    });
}

EventConnection Canvas::on_mouse_move(std::function<void(int x, int y)> handler) {
    pimpl->widget->mouse_move_handler = handler;
    return EventConnection([this]() {
        if (pimpl->widget) pimpl->widget->mouse_move_handler = nullptr;
    });
}

EventConnection Canvas::on_mouse_up(std::function<void(int x, int y)> handler) {
    pimpl->widget->mouse_up_handler = handler;
    return EventConnection([this]() {
        if (pimpl->widget) pimpl->widget->mouse_up_handler = nullptr;
    });
}

QWidget* Canvas::get_qwidget() const {
    return pimpl->widget.data();
}

}
