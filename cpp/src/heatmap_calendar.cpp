#include "simplegui/heatmap_calendar.h"
#include "detail/common.h"
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QDate>

namespace simplegui {

namespace {
class HeatmapWidget : public QWidget {
public:
    std::map<QString, int> data;

    HeatmapWidget() {
        setMinimumSize(700, 150);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int cell_size = 12;
        int padding = 3;
        int cols = 52;
        int rows = 7;
        
        int start_x = 30;
        int start_y = 30;

        // Draw basic grid for the last 52 weeks
        QDate today = QDate::currentDate();
        QDate start_date = today.addDays(-(cols * 7));
        
        // Find the first Sunday of the starting week
        while (start_date.dayOfWeek() != 7) {
            start_date = start_date.addDays(-1);
        }

        const QColor colors[] = { QColor("#ebedf0"), QColor("#9be9a8"), QColor("#40c463"), QColor("#30a14e"), QColor("#216e39") };

        for (int c = 0; c < cols; ++c) {
            for (int r = 0; r < rows; ++r) {
                QDate current = start_date.addDays(c * 7 + r);
                if (current > today) break;

                QString dateStr = current.toString("yyyy-MM-dd");
                int intensity = 0;
                if (data.find(dateStr) != data.end()) {
                    intensity = data[dateStr];
                    if (intensity < 0) intensity = 0;
                    if (intensity > 4) intensity = 4;
                }

                int x = start_x + c * (cell_size + padding);
                int y = start_y + r * (cell_size + padding);
                
                p.setBrush(colors[intensity]);
                p.setPen(Qt::NoPen);
                p.drawRoundedRect(x, y, cell_size, cell_size, 2, 2);
            }
        }
    }
};
}

struct HeatmapCalendar::Impl {
    QPointer<HeatmapWidget> widget = new HeatmapWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

HeatmapCalendar::HeatmapCalendar() : pimpl(std::make_shared<Impl>()) {}
HeatmapCalendar::~HeatmapCalendar() = default;

void HeatmapCalendar::set_data(const std::map<std::string, int>& daily_intensity) {
    if (!pimpl->widget) return;
    pimpl->widget->data.clear();
    for (auto const& [date, intensity] : daily_intensity) {
        pimpl->widget->data[detail::qs(date)] = intensity;
    }
    pimpl->widget->update();
}

QWidget* HeatmapCalendar::get_qwidget() const {
    return pimpl->widget.data();
}

}
