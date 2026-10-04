#include "simplegui/circular_progress.h"
#include "detail/common.h"

#include <QPainter>
#include <QWidget>

#include <algorithm>

namespace simplegui {
namespace {

class CircularProgressWidget : public QWidget {
public:
    int value = 0;
    bool show_text = false;
    QColor color = QColor(0x34, 0x98, 0xdb);

    CircularProgressWidget() { setFixedSize(60, 60); }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const int pen = std::max(3, width() / 10);
        const QRect ring(pen, pen, width() - 2 * pen, height() - 2 * pen);

        painter.setPen(QPen(QColor(0xe0, 0xe0, 0xe0), pen));
        painter.drawEllipse(ring);
        painter.setPen(QPen(color, pen, Qt::SolidLine, Qt::RoundCap));
        painter.drawArc(ring, 90 * 16, static_cast<int>(value / 100.0 * -360 * 16));

        if (show_text) {
            QFont f = painter.font();
            f.setPixelSize(std::max(8, width() / 5));
            f.setBold(true);
            painter.setFont(f);
            painter.setPen(palette().color(QPalette::WindowText));
            painter.drawText(rect(), Qt::AlignCenter, QString::number(value) + QLatin1Char('%'));
        }
    }
};

}  // namespace

struct CircularProgress::Impl {
    QPointer<CircularProgressWidget> widget = new CircularProgressWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

CircularProgress::CircularProgress() : pimpl(std::make_shared<Impl>()) {}
CircularProgress::~CircularProgress() = default;

void CircularProgress::set_value(int percentage) {
    if (!pimpl->widget) return;
    pimpl->widget->value = std::clamp(percentage, 0, 100);
    pimpl->widget->update();
}

int CircularProgress::get_value() const { return pimpl->widget ? pimpl->widget->value : 0; }

void CircularProgress::set_color(const std::string& color) {
    if (!pimpl->widget) return;
    pimpl->widget->color = detail::parse_color(color, pimpl->widget->color);
    pimpl->widget->update();
}

void CircularProgress::set_show_text(bool show) {
    if (!pimpl->widget) return;
    pimpl->widget->show_text = show;
    pimpl->widget->update();
}

void CircularProgress::set_diameter(int pixels) {
    if (pimpl->widget) pimpl->widget->setFixedSize(std::max(20, pixels), std::max(20, pixels));
}

QWidget* CircularProgress::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
