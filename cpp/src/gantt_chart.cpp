#include "simplegui/gantt_chart.h"
#include "detail/common.h"
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

namespace simplegui {

namespace {
class GanttWidget : public QWidget {
public:
    std::vector<GanttTask> tasks;

    GanttWidget() {
        setMinimumSize(400, 200);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        if (tasks.empty()) return;
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int max_day = 0;
        for (const auto& t : tasks) {
            if (t.start_day + t.duration > max_day) max_day = t.start_day + t.duration;
        }
        if (max_day == 0) max_day = 1;

        int row_height = 40;
        int name_width = 150;
        int chart_width = width() - name_width - 20;
        double day_width = (double)chart_width / max_day;

        for (size_t i = 0; i < tasks.size(); ++i) {
            int y = 10 + i * row_height;
            const auto& t = tasks[i];

            // Draw name
            p.setPen(Qt::black);
            p.drawText(QRect(10, y, name_width - 10, row_height - 10), Qt::AlignLeft | Qt::AlignVCenter, detail::qs(t.name));

            // Draw bar
            int x = name_width + t.start_day * day_width;
            int w = t.duration * day_width;
            
            QColor c = t.color.empty() ? QColor("#3b82f6") : QColor(detail::qs(t.color));
            p.setBrush(c);
            p.setPen(Qt::NoPen);
            p.drawRoundedRect(x, y + 5, w, row_height - 15, 4, 4);
        }
    }
};
}

struct GanttChart::Impl {
    QPointer<GanttWidget> widget = new GanttWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

GanttChart::GanttChart() : pimpl(std::make_shared<Impl>()) {}
GanttChart::~GanttChart() = default;

void GanttChart::set_tasks(const std::vector<GanttTask>& tasks) {
    if (!pimpl->widget) return;
    pimpl->widget->tasks = tasks;
    pimpl->widget->setMinimumHeight(20 + tasks.size() * 40);
    pimpl->widget->update();
}

QWidget* GanttChart::get_qwidget() const {
    return pimpl->widget.data();
}

}
