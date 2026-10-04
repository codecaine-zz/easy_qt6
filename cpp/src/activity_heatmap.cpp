#include "simplegui/activity_heatmap.h"
#include "detail/common.h"

#include <QPainter>
#include <QWidget>

#include <algorithm>

namespace simplegui {
namespace {

class HeatmapWidget : public QWidget {
public:
    int weeks;
    int days;
    std::vector<std::vector<int>> matrix;  // always exactly weeks x days
    QColor base_color = QColor(0x10, 0xb9, 0x81);

    HeatmapWidget(int w, int d) : weeks(std::clamp(w, 1, 520)), days(std::clamp(d, 1, 31)) {
        setMinimumSize(220, 100);
        matrix.assign(static_cast<size_t>(weeks), std::vector<int>(static_cast<size_t>(days), 0));
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        p.fillRect(rect(), QColor(0x12, 0x12, 0x15));
        p.setPen(QPen(QColor(0x27, 0x27, 0x2a), 1));
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 6, 6);

        const int margin = 10;
        const int gap = 3;
        const double tile_w = static_cast<double>(width() - 2 * margin - (weeks - 1) * gap) / weeks;
        const double tile_h = static_cast<double>(height() - 2 * margin - (days - 1) * gap) / days;
        const double tile = std::max(1.0, std::min(tile_w, tile_h));

        static const int alpha_for_level[] = {0, 60, 120, 190, 255};
        p.setPen(Qt::NoPen);
        for (int week = 0; week < weeks; ++week) {
            for (int day = 0; day < days; ++day) {
                const int level = std::clamp(matrix[static_cast<size_t>(week)][static_cast<size_t>(day)], 0, 4);
                QColor c = base_color;
                if (level == 0) {
                    c = QColor(0x1e, 0x1e, 0x24);
                } else {
                    c.setAlpha(alpha_for_level[level]);
                }
                p.setBrush(c);
                p.drawRoundedRect(QRectF(margin + week * (tile + gap), margin + day * (tile + gap), tile, tile), 2, 2);
            }
        }
    }
};

}  // namespace

struct ActivityHeatmap::Impl {
    QPointer<HeatmapWidget> widget;
    Impl(int weeks, int days) : widget(new HeatmapWidget(weeks, days)) {}
    ~Impl() { detail::delete_if_orphan(widget); }

    bool in_range(int week, int day) const {
        return widget && week >= 0 && day >= 0 && week < widget->weeks && day < widget->days;
    }
};

ActivityHeatmap::ActivityHeatmap(int weeks, int days)
    : pimpl(std::make_shared<Impl>(weeks, days)) {}

ActivityHeatmap::~ActivityHeatmap() = default;

void ActivityHeatmap::set_data(const std::vector<std::vector<int>>& matrix) {
    auto* w = pimpl->widget.data();
    if (!w) return;
    for (int week = 0; week < w->weeks; ++week) {
        for (int day = 0; day < w->days; ++day) {
            const bool present = week < static_cast<int>(matrix.size()) &&
                                 day < static_cast<int>(matrix[static_cast<size_t>(week)].size());
            w->matrix[static_cast<size_t>(week)][static_cast<size_t>(day)] =
                present ? std::clamp(matrix[static_cast<size_t>(week)][static_cast<size_t>(day)], 0, 4) : 0;
        }
    }
    w->update();
}

void ActivityHeatmap::set_cell(int week, int day, int intensity) {
    if (!pimpl->in_range(week, day)) return;
    pimpl->widget->matrix[static_cast<size_t>(week)][static_cast<size_t>(day)] = std::clamp(intensity, 0, 4);
    pimpl->widget->update();
}

int ActivityHeatmap::get_cell(int week, int day) const {
    if (!pimpl->in_range(week, day)) return 0;
    return pimpl->widget->matrix[static_cast<size_t>(week)][static_cast<size_t>(day)];
}

void ActivityHeatmap::clear() { set_data({}); }

void ActivityHeatmap::set_color_scale(const std::string& base_color) {
    if (!pimpl->widget) return;
    pimpl->widget->base_color = detail::parse_color(base_color, pimpl->widget->base_color);
    pimpl->widget->update();
}

QWidget* ActivityHeatmap::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
