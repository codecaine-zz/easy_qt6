#include "simplegui/circular_progress.h"
#include <QWidget>
#include <QPainter>
#include <QPointer>

namespace simplegui {

class QCircularProgressWidget : public QWidget {
public:
    QCircularProgressWidget() : value(0) {
        setFixedSize(60, 60);
    }

    void setValue(int val) {
        value = std::max(0, std::min(100, val));
        update();
    }

    int getValue() const { return value; }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QRect rect(5, 5, width() - 10, height() - 10);
        
        QPen bg_pen(QColor("#E0E0E0"), 6);
        painter.setPen(bg_pen);
        painter.drawEllipse(rect);

        int spanAngle = int((value / 100.0) * -360 * 16);
        QPen fg_pen(QColor("#3498db"), 6);
        painter.setPen(fg_pen);
        painter.drawArc(rect, 90 * 16, spanAngle);
    }

private:
    int value;
};

struct CircularProgress::Impl {
    QPointer<QCircularProgressWidget> widget;
    Impl() { widget = new QCircularProgressWidget(); }
    ~Impl() { if (widget && !widget->parent()) delete widget; }
};

CircularProgress::CircularProgress() : pimpl(std::make_shared<Impl>()) {}

CircularProgress::~CircularProgress() = default;

void CircularProgress::set_value(int percentage) {
    if (pimpl->widget) pimpl->widget->setValue(percentage);
}

int CircularProgress::get_value() const {
    if (pimpl->widget) return pimpl->widget->getValue();
    return 0;
}

QWidget* CircularProgress::get_qwidget() const {
    return pimpl->widget.data();
}

}
