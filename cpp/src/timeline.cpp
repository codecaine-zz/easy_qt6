#include "simplegui/timeline.h"
#include "detail/common.h"
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

namespace simplegui {

namespace {
class TimelineWidget : public QWidget {
public:
    std::vector<QString> steps;
    int current_step = 0;

    TimelineWidget() {
        setMinimumHeight(60);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        if (steps.empty()) return;
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int count = steps.size();
        int margin = 40;
        int width = this->width() - 2 * margin;
        int step_width = count > 1 ? width / (count - 1) : 0;
        int y = height() / 2 - 10;

        // Draw line
        p.setPen(QPen(QColor("#e2e8f0"), 4));
        if (count > 1) {
            p.drawLine(margin, y, margin + width, y);
        }
        
        // Draw progress line
        if (current_step > 0 && count > 1) {
            p.setPen(QPen(QColor("#3b82f6"), 4));
            p.drawLine(margin, y, margin + std::min(current_step, count - 1) * step_width, y);
        }

        // Draw points and text
        for (int i = 0; i < count; ++i) {
            int cx = margin + i * step_width;
            bool active = i <= current_step;

            p.setPen(Qt::NoPen);
            p.setBrush(active ? QColor("#3b82f6") : QColor("#cbd5e1"));
            p.drawEllipse(QPoint(cx, y), 8, 8);
            if (active) {
                p.setBrush(Qt::white);
                p.drawEllipse(QPoint(cx, y), 3, 3);
            }

            p.setPen(active ? QColor("#1e293b") : QColor("#94a3b8"));
            QFont f = p.font();
            f.setBold(active);
            p.setFont(f);
            
            QRect textRect(cx - 50, y + 15, 100, 30);
            p.drawText(textRect, Qt::AlignCenter | Qt::TextWordWrap, steps[i]);
        }
    }
};
}

struct Timeline::Impl {
    QPointer<TimelineWidget> widget = new TimelineWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

Timeline::Timeline() : pimpl(std::make_shared<Impl>()) {}
Timeline::~Timeline() = default;

void Timeline::set_steps(const std::vector<std::string>& steps) {
    if (!pimpl->widget) return;
    pimpl->widget->steps.clear();
    for (const auto& s : steps) {
        pimpl->widget->steps.push_back(detail::qs(s));
    }
    pimpl->widget->update();
}

void Timeline::set_current_step(int step_index) {
    if (!pimpl->widget) return;
    pimpl->widget->current_step = step_index;
    pimpl->widget->update();
}

QWidget* Timeline::get_qwidget() const {
    return pimpl->widget.data();
}

}
