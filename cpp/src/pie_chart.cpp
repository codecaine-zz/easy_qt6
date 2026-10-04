#include "simplegui/pie_chart.h"
#include <QWidget>
#include <QPainter>
#include <QPointer>
#include <numeric>
#include <cmath>

namespace simplegui {

class PieChartWidget : public QWidget {
public:
    std::string title;
    std::vector<PieSlice> slices;
    bool draw_legend = true;
    bool draw_percentages = true;

    explicit PieChartWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(280, 180);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Panel background
        p.fillRect(rect(), QColor("#111827"));
        p.setPen(QColor("#1f2937"));
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 8, 8);

        int top_pad = 16;
        if (!title.empty()) {
            p.setPen(QColor("#f8fafc"));
            QFont tf = p.font();
            tf.setPixelSize(13);
            tf.setBold(true);
            p.setFont(tf);
            p.drawText(QRect(16, 12, width() - 32, 22), Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(title));
            top_pad = 38;
        }

        double total = 0.0;
        for (const auto& s : slices) {
            if (s.value > 0) total += s.value;
        }

        int available_w = width() - 32;
        int available_h = height() - top_pad - 16;
        if (available_w <= 20 || available_h <= 20) return;

        int pie_diameter = qMin(available_h, draw_legend ? (available_w * 5 / 10) : available_w);
        pie_diameter = qMax(40, pie_diameter - 10);

        int pie_x = 16 + (draw_legend ? 8 : (available_w - pie_diameter) / 2);
        int pie_y = top_pad + (available_h - pie_diameter) / 2;
        QRectF pie_rect(pie_x, pie_y, pie_diameter, pie_diameter);

        const char* default_colors[] = {
            "#3b82f6", "#10b981", "#f59e0b", "#ec4899", "#8b5cf6", "#06b6d4"
        };

        if (total <= 0.0001) {
            p.setPen(QColor("#1e293b"));
            p.setBrush(QColor("#1e293b"));
            p.drawEllipse(pie_rect);
            return;
        }

        double start_angle = 90.0 * 16.0;
        int col_idx = 0;

        for (const auto& s : slices) {
            if (s.value <= 0) continue;
            double span = (s.value / total) * 360.0 * 16.0;

            QColor col(QString::fromStdString(s.color_hex.empty() ? default_colors[col_idx % 6] : s.color_hex));
            col_idx++;

            p.setPen(QPen(QColor("#111827"), 2));
            p.setBrush(col);
            p.drawPie(pie_rect, static_cast<int>(start_angle), static_cast<int>(-span));

            start_angle -= span;
        }

        // Draw Legend on right side
        if (draw_legend) {
            int leg_left = pie_x + pie_diameter + 24;
            int leg_top = top_pad + 10;
            int leg_w = width() - leg_left - 16;

            if (leg_w > 50) {
                int item_y = leg_top;
                col_idx = 0;

                QFont lf = p.font();
                lf.setPixelSize(11);
                p.setFont(lf);

                for (const auto& s : slices) {
                    if (item_y + 20 > height() - 10) break;

                    QColor col(QString::fromStdString(s.color_hex.empty() ? default_colors[col_idx % 6] : s.color_hex));
                    col_idx++;

                    // Swatch
                    p.setPen(Qt::NoPen);
                    p.setBrush(col);
                    p.drawRoundedRect(leg_left, item_y + 3, 10, 10, 2, 2);

                    // Label & percentage
                    p.setPen(QColor("#e2e8f0"));
                    double pct = (total > 0) ? (s.value / total * 100.0) : 0.0;
                    char buf[64];
                    if (draw_percentages) {
                        snprintf(buf, sizeof(buf), "%s (%.1f%%)", s.label.c_str(), pct);
                    } else {
                        snprintf(buf, sizeof(buf), "%s", s.label.c_str());
                    }
                    p.drawText(QRect(leg_left + 18, item_y, leg_w - 18, 16), Qt::AlignLeft | Qt::AlignVCenter, QString::fromUtf8(buf));

                    item_y += 22;
                }
            }
        }
    }
};

struct PieChart::Impl {
    QPointer<PieChartWidget> widget;
};

PieChart::PieChart(const std::string& title)
    : pimpl(std::make_shared<Impl>())
{
    auto* w = new PieChartWidget();
    pimpl->widget = w;
    w->title = title;
}

PieChart::~PieChart() = default;

void PieChart::add_slice(const std::string& label, double value, const std::string& color_hex) {
    if (pimpl->widget) {
        pimpl->widget->slices.push_back({label, value, color_hex});
        pimpl->widget->update();
    }
}

void PieChart::set_slices(const std::vector<PieSlice>& slices) {
    if (pimpl->widget) {
        pimpl->widget->slices = slices;
        pimpl->widget->update();
    }
}

void PieChart::clear_slices() {
    if (pimpl->widget) {
        pimpl->widget->slices.clear();
        pimpl->widget->update();
    }
}

void PieChart::set_title(const std::string& title) {
    if (pimpl->widget) {
        pimpl->widget->title = title;
        pimpl->widget->update();
    }
}

void PieChart::show_legend(bool show) {
    if (pimpl->widget) {
        pimpl->widget->draw_legend = show;
        pimpl->widget->update();
    }
}

void PieChart::show_percentages(bool show) {
    if (pimpl->widget) {
        pimpl->widget->draw_percentages = show;
        pimpl->widget->update();
    }
}

QWidget* PieChart::get_qwidget() const {
    return pimpl->widget.data();
}

}
