#include "simplegui/activity_heatmap.h"
#include <QWidget>
#include <QPainter>
#include <QPointer>
#include <algorithm>

namespace simplegui {

class HeatmapWidget : public QWidget {
public:
    int weeks = 16;
    int days = 7;
    std::vector<std::vector<int>> matrix;
    QColor base_color = QColor("#10b981");

    explicit HeatmapWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(220, 100);
        matrix.resize(weeks, std::vector<int>(days, 0));
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int w = width();
        int h = height();

        p.fillRect(rect(), QColor("#121215"));
        p.setPen(QPen(QColor("#27272a"), 1));
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 6, 6);

        int margin = 10;
        int gap = 3;
        int avail_w = w - 2 * margin;
        int avail_h = h - 2 * margin;

        double tile_w = static_cast<double>(avail_w - (weeks - 1) * gap) / weeks;
        double tile_h = static_cast<double>(avail_h - (days - 1) * gap) / days;
        double tile_size = std::min(tile_w, tile_h);

        p.setPen(Qt::NoPen);

        for (int week = 0; week < weeks; ++week) {
            for (int day = 0; day < days; ++day) {
                double x = margin + week * (tile_size + gap);
                double y = margin + day * (tile_size + gap);
                QRectF r(x, y, tile_size, tile_size);

                int val = 0;
                if (week < static_cast<int>(matrix.size()) && day < static_cast<int>(matrix[week].size())) {
                    val = std::clamp(matrix[week][day], 0, 4);
                }

                QColor cell_color;
                if (val == 0) {
                    cell_color = QColor("#1e1e24");
                } else if (val == 1) {
                    cell_color = base_color;
                    cell_color.setAlpha(60);
                } else if (val == 2) {
                    cell_color = base_color;
                    cell_color.setAlpha(120);
                } else if (val == 3) {
                    cell_color = base_color;
                    cell_color.setAlpha(190);
                } else {
                    cell_color = base_color;
                    cell_color.setAlpha(255);
                }

                p.setBrush(cell_color);
                p.drawRoundedRect(r, 2, 2);
            }
        }
    }
};

struct ActivityHeatmap::Impl {
    QPointer<HeatmapWidget> widget;
    Impl(int weeks, int days) {
        widget = new HeatmapWidget();
        widget->weeks = weeks;
        widget->days = days;
        widget->matrix.resize(weeks, std::vector<int>(days, 0));
    }
    ~Impl() { if (widget && !widget->parent()) delete widget; }
};

ActivityHeatmap::ActivityHeatmap(int weeks, int days)
    : pimpl(std::make_shared<Impl>(weeks, days)) {}

ActivityHeatmap::~ActivityHeatmap() = default;

void ActivityHeatmap::set_data(const std::vector<std::vector<int>>& matrix) {
    if (pimpl->widget) {
        pimpl->widget->matrix = matrix;
        pimpl->widget->update();
    }
}

void ActivityHeatmap::set_cell(int week, int day, int intensity) {
    if (pimpl->widget) {
        if (week >= 0 && week < pimpl->widget->weeks && day >= 0 && day < pimpl->widget->days) {
            pimpl->widget->matrix[week][day] = intensity;
            pimpl->widget->update();
        }
    }
}

void ActivityHeatmap::set_color_scale(const std::string& base_hex) {
    if (pimpl->widget) {
        pimpl->widget->base_color = QColor(QString::fromStdString(base_hex));
        pimpl->widget->update();
    }
}

QWidget* ActivityHeatmap::get_qwidget() const {
    return pimpl->widget.data();
}

}
