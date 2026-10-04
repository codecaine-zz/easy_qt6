#include "simplegui/canvas.h"
#include "detail/common.h"

#include <QImage>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QWidget>

#include <algorithm>

namespace simplegui {
namespace {

// Upper bound for the backing image so huge coordinates cannot exhaust memory.
constexpr int kMaxCanvasSide = 8192;

class CanvasWidget : public QWidget {
public:
    QPixmap buffer;
    QColor background = QColor(0x0f, 0x17, 0x2a);
    std::function<void(int, int)> mouse_down;
    std::function<void(int, int)> mouse_move;
    std::function<void(int, int)> mouse_up;

    CanvasWidget(int min_w, int min_h) {
        min_w = std::clamp(min_w, 1, kMaxCanvasSide);
        min_h = std::clamp(min_h, 1, kMaxCanvasSide);
        setMinimumSize(min_w, min_h);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMouseTracking(true);
        buffer = QPixmap(std::max(min_w, 100), std::max(min_h, 100));
        buffer.fill(background);
    }

    // Grows the backing image (never shrinks) so drawings outside the current area are kept.
    void ensure_size(int w, int h) {
        w = std::clamp(w, 1, kMaxCanvasSide);
        h = std::clamp(h, 1, kMaxCanvasSide);
        if (buffer.width() >= w && buffer.height() >= h) return;
        QPixmap bigger(std::max(w, buffer.width()), std::max(h, buffer.height()));
        bigger.fill(background);
        QPainter p(&bigger);
        p.drawPixmap(0, 0, buffer);
        p.end();
        buffer = bigger;
    }

protected:
    void resizeEvent(QResizeEvent* event) override {
        QWidget::resizeEvent(event);
        ensure_size(width(), height());
    }
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.drawPixmap(0, 0, buffer);
    }
    void mousePressEvent(QMouseEvent* e) override {
        if (mouse_down) mouse_down(e->position().toPoint().x(), e->position().toPoint().y());
    }
    void mouseMoveEvent(QMouseEvent* e) override {
        if (mouse_move) mouse_move(e->position().toPoint().x(), e->position().toPoint().y());
    }
    void mouseReleaseEvent(QMouseEvent* e) override {
        if (mouse_up) mouse_up(e->position().toPoint().x(), e->position().toPoint().y());
    }
};

}  // namespace

struct Canvas::Impl {
    QPointer<CanvasWidget> widget;
    detail::Event<int, int> down, move, up;

    Impl(int w, int h) : widget(new CanvasWidget(w, h)) {
        down = detail::make_event<int, int>(widget);
        move = detail::make_event<int, int>(widget);
        up = detail::make_event<int, int>(widget);
    }
    ~Impl() { detail::delete_if_orphan(widget); }

    // Runs `draw` with a painter on the backing image that covers (right, bottom).
    template <typename Fn>
    void paint(int right, int bottom, Fn&& draw) {
        if (!widget) return;
        widget->ensure_size(right + 10, bottom + 10);
        QPainter p(&widget->buffer);
        p.setRenderHint(QPainter::Antialiasing);
        draw(p);
        p.end();
        widget->update();
    }

    QColor color(const std::string& text) const { return detail::parse_color(text, Qt::white); }
};

Canvas::Canvas(int min_width, int min_height) : pimpl(std::make_shared<Impl>(min_width, min_height)) {
    std::weak_ptr<Impl> weak = pimpl;
    pimpl->widget->mouse_down = [weak](int x, int y) { if (auto d = weak.lock()) detail::fire(d->down, x, y); };
    pimpl->widget->mouse_move = [weak](int x, int y) { if (auto d = weak.lock()) detail::fire(d->move, x, y); };
    pimpl->widget->mouse_up = [weak](int x, int y) { if (auto d = weak.lock()) detail::fire(d->up, x, y); };
}

Canvas::~Canvas() = default;

void Canvas::clear(const std::string& bg_color) {
    if (!pimpl->widget) return;
    pimpl->widget->background = detail::parse_color(bg_color, pimpl->widget->background);
    pimpl->widget->buffer.fill(pimpl->widget->background);
    pimpl->widget->update();
}

void Canvas::draw_point(int x, int y, const std::string& color, int size) {
    const int s = std::max(1, size);
    pimpl->paint(x + s, y + s, [&](QPainter& p) {
        p.setPen(QPen(pimpl->color(color), s, Qt::SolidLine, Qt::RoundCap));
        p.drawPoint(x, y);
    });
}

void Canvas::draw_line(int x1, int y1, int x2, int y2, const std::string& color, int line_width) {
    pimpl->paint(std::max(x1, x2), std::max(y1, y2), [&](QPainter& p) {
        p.setPen(QPen(pimpl->color(color), std::max(1, line_width), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        p.drawLine(x1, y1, x2, y2);
    });
}

void Canvas::draw_rect(int x, int y, int w, int h, const std::string& color, int line_width) {
    pimpl->paint(x + w, y + h, [&](QPainter& p) {
        p.setPen(QPen(pimpl->color(color), std::max(1, line_width)));
        p.setBrush(Qt::NoBrush);
        p.drawRect(x, y, w, h);
    });
}

void Canvas::fill_rect(int x, int y, int w, int h, const std::string& color) {
    pimpl->paint(x + w, y + h, [&](QPainter& p) { p.fillRect(QRect(x, y, w, h), pimpl->color(color)); });
}

void Canvas::draw_rounded_rect(int x, int y, int w, int h, int radius, const std::string& color, int line_width) {
    pimpl->paint(x + w, y + h, [&](QPainter& p) {
        p.setPen(QPen(pimpl->color(color), std::max(1, line_width)));
        p.setBrush(Qt::NoBrush);
        p.drawRoundedRect(QRect(x, y, w, h), radius, radius);
    });
}

void Canvas::fill_rounded_rect(int x, int y, int w, int h, int radius, const std::string& color) {
    pimpl->paint(x + w, y + h, [&](QPainter& p) {
        p.setPen(Qt::NoPen);
        p.setBrush(pimpl->color(color));
        p.drawRoundedRect(QRect(x, y, w, h), radius, radius);
    });
}

void Canvas::draw_circle(int cx, int cy, int radius, const std::string& color, int line_width) {
    pimpl->paint(cx + radius, cy + radius, [&](QPainter& p) {
        p.setPen(QPen(pimpl->color(color), std::max(1, line_width)));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(QPoint(cx, cy), radius, radius);
    });
}

void Canvas::fill_circle(int cx, int cy, int radius, const std::string& color) {
    pimpl->paint(cx + radius, cy + radius, [&](QPainter& p) {
        p.setPen(Qt::NoPen);
        p.setBrush(pimpl->color(color));
        p.drawEllipse(QPoint(cx, cy), radius, radius);
    });
}

void Canvas::draw_ellipse(int x, int y, int w, int h, const std::string& color, int line_width) {
    pimpl->paint(x + w, y + h, [&](QPainter& p) {
        p.setPen(QPen(pimpl->color(color), std::max(1, line_width)));
        p.setBrush(Qt::NoBrush);
        p.drawEllipse(QRect(x, y, w, h));
    });
}

void Canvas::fill_ellipse(int x, int y, int w, int h, const std::string& color) {
    pimpl->paint(x + w, y + h, [&](QPainter& p) {
        p.setPen(Qt::NoPen);
        p.setBrush(pimpl->color(color));
        p.drawEllipse(QRect(x, y, w, h));
    });
}

void Canvas::draw_text(int x, int y, const std::string& text, const std::string& color, int font_size) {
    pimpl->paint(x, y, [&](QPainter& p) {
        QFont f = p.font();
        f.setPixelSize(std::clamp(font_size, 1, 512));
        p.setFont(f);
        p.setPen(pimpl->color(color));
        p.drawText(x, y, detail::qs(text));
    });
}

bool Canvas::draw_image(int x, int y, const std::string& file_path) {
    QImage img;
    if (file_path.empty() || !img.load(detail::qs(file_path))) return false;
    pimpl->paint(x + img.width(), y + img.height(), [&](QPainter& p) { p.drawImage(x, y, img); });
    return true;
}

bool Canvas::save_to_file(const std::string& file_path) const {
    if (!pimpl->widget || file_path.empty()) return false;
    const QWidget* w = pimpl->widget.data();
    return pimpl->widget->buffer.copy(0, 0, std::max(1, w->width()), std::max(1, w->height()))
        .save(detail::qs(file_path));
}

void Canvas::repaint() {
    if (pimpl->widget) pimpl->widget->update();
}

EventConnection Canvas::on_mouse_down(std::function<void(int, int)> handler) {
    return detail::add_handler(pimpl->down, std::move(handler));
}
EventConnection Canvas::on_mouse_move(std::function<void(int, int)> handler) {
    return detail::add_handler(pimpl->move, std::move(handler));
}
EventConnection Canvas::on_mouse_up(std::function<void(int, int)> handler) {
    return detail::add_handler(pimpl->up, std::move(handler));
}

QWidget* Canvas::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
