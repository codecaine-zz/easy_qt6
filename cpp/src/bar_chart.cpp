#include "simplegui/bar_chart.h"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <algorithm>
#include <cmath>

namespace simplegui {

class BarChartWidget : public QWidget {
public:
    std::string title;
    std::vector<BarItem> bars;
    bool show_values = true;
    bool show_grid = true;
    double custom_min = 0.0;
    double custom_max = 0.0; // 0 = auto

    explicit BarChartWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(280, 160);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Background
        p.fillRect(rect(), QColor("#111827"));
        p.setPen(QColor("#1f2937"));
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 8, 8);

        int top_pad = 16;
        if (!title.empty()) {
            p.setPen(QColor("#f8fafc"));
            QFont tf = p.font();
            tf.setPixelSize(12);
            tf.setBold(true);
            p.setFont(tf);
            p.drawText(QRect(14, 10, width() - 28, 20), Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(title));
            top_pad = 36;
        }

        int left_pad = 42;
        int right_pad = 16;
        int bottom_pad = 28;

        int plot_w = width() - left_pad - right_pad;
        int plot_h = height() - top_pad - bottom_pad;

        if (plot_w <= 10 || plot_h <= 10) return;

        double max_val = custom_max;
        if (max_val <= 0.0) {
            for (const auto& b : bars) {
                if (b.value > max_val) max_val = b.value;
            }
            if (max_val <= 0.0) max_val = 100.0;
            // Round up to nice number
            double magnitude = std::pow(10.0, std::floor(std::log10(max_val)));
            max_val = std::ceil(max_val / magnitude) * magnitude;
        }

        // Draw horizontal grid lines & Y labels
        int grid_steps = 4;
        QFont axis_font = p.font();
        axis_font.setPixelSize(10);
        axis_font.setBold(false);
        p.setFont(axis_font);

        for (int i = 0; i <= grid_steps; ++i) {
            double ratio = static_cast<double>(i) / grid_steps;
            int y = top_pad + plot_h - static_cast<int>(ratio * plot_h);

            if (show_grid) {
                p.setPen(QPen(QColor("#1e293b"), 1, Qt::DashLine));
                p.drawLine(left_pad, y, left_pad + plot_w, y);
            }

            p.setPen(QColor("#64748b"));
            double val = ratio * max_val;
            char val_buf[32];
            if (val >= 1000.0) {
                snprintf(val_buf, sizeof(val_buf), "%.0fk", val / 1000.0);
            } else {
                snprintf(val_buf, sizeof(val_buf), "%.0f", val);
            }
            p.drawText(QRect(4, y - 8, left_pad - 8, 16), Qt::AlignRight | Qt::AlignVCenter, val_buf);
        }

        if (bars.empty()) return;

        // Draw bars
        int count = static_cast<int>(bars.size());
        double slot_w = static_cast<double>(plot_w) / count;
        double bar_w = qMax(8.0, slot_w * 0.65);

        const char* default_colors[] = {
            "#3b82f6", "#06b6d4", "#10b981", "#f59e0b", "#ec4899", "#8b5cf6"
        };

        for (int i = 0; i < count; ++i) {
            const auto& b = bars[i];
            double x_center = left_pad + (i + 0.5) * slot_w;
            double bar_h = (b.value / max_val) * plot_h;
            bar_h = qBound(0.0, bar_h, static_cast<double>(plot_h));

            double bar_x = x_center - bar_w / 2.0;
            double bar_y = top_pad + plot_h - bar_h;

            QColor col(QString::fromStdString(b.color_hex.empty() ? default_colors[i % 6] : b.color_hex));
            
            QPainterPath bar_path;
            bar_path.addRoundedRect(QRectF(bar_x, bar_y, bar_w, bar_h), 4, 4);
            p.fillPath(bar_path, col);

            // Value text on top
            if (show_values && bar_h > 12) {
                p.setPen(QColor("#f1f5f9"));
                char vbuf[32];
                if (b.value == static_cast<int>(b.value)) {
                    snprintf(vbuf, sizeof(vbuf), "%d", static_cast<int>(b.value));
                } else {
                    snprintf(vbuf, sizeof(vbuf), "%.1f", b.value);
                }
                QFont vf = p.font();
                vf.setPixelSize(10);
                vf.setBold(true);
                p.setFont(vf);
                p.drawText(QRectF(bar_x - 10, bar_y - 16, bar_w + 20, 14), Qt::AlignCenter, vbuf);
            }

            // X-axis label
            p.setPen(QColor("#94a3b8"));
            QFont xf = p.font();
            xf.setPixelSize(10);
            xf.setBold(false);
            p.setFont(xf);
            p.drawText(QRectF(bar_x - 15, top_pad + plot_h + 4, bar_w + 30, 20),
                       Qt::AlignHCenter | Qt::AlignTop,
                       QString::fromStdString(b.label));
        }
    }
};

struct BarChart::Impl {
    QPointer<BarChartWidget> widget;
};

BarChart::BarChart(const std::string& title)
    : pimpl(std::make_shared<Impl>())
{
    auto* w = new BarChartWidget();
    pimpl->widget = w;
    w->title = title;
}

BarChart::~BarChart() = default;

void BarChart::add_bar(const std::string& label, double value, const std::string& color_hex) {
    if (pimpl->widget) {
        pimpl->widget->bars.push_back({label, value, color_hex});
        pimpl->widget->update();
    }
}

void BarChart::set_bars(const std::vector<BarItem>& bars) {
    if (pimpl->widget) {
        pimpl->widget->bars = bars;
        pimpl->widget->update();
    }
}

void BarChart::clear_bars() {
    if (pimpl->widget) {
        pimpl->widget->bars.clear();
        pimpl->widget->update();
    }
}

void BarChart::set_title(const std::string& title) {
    if (pimpl->widget) {
        pimpl->widget->title = title;
        pimpl->widget->update();
    }
}

void BarChart::set_show_values(bool show) {
    if (pimpl->widget) {
        pimpl->widget->show_values = show;
        pimpl->widget->update();
    }
}

void BarChart::set_show_grid(bool show) {
    if (pimpl->widget) {
        pimpl->widget->show_grid = show;
        pimpl->widget->update();
    }
}

void BarChart::set_y_range(double min, double max) {
    if (pimpl->widget) {
        pimpl->widget->custom_min = min;
        pimpl->widget->custom_max = max;
        pimpl->widget->update();
    }
}

QWidget* BarChart::get_qwidget() const {
    return pimpl->widget.data();
}

}
