#include "simplegui/line_chart.h"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <algorithm>
#include <cmath>

namespace simplegui {

class LineChartWidget : public QWidget {
public:
    std::string title;
    std::vector<LineSeries> series_list;
    std::vector<std::string> x_labels;
    bool is_smooth = true;
    bool draw_points = true;
    bool draw_grid = true;
    bool draw_legend = true;
    double custom_min = 0.0;
    double custom_max = 0.0;

    explicit LineChartWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(320, 180);
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

        int top_pad = 20;
        int header_h = (!title.empty() || draw_legend) ? 32 : 12;
        top_pad += header_h;

        // Draw title & legend
        if (!title.empty()) {
            p.setPen(QColor("#f8fafc"));
            QFont tf = p.font();
            tf.setPixelSize(13);
            tf.setBold(true);
            p.setFont(tf);
            p.drawText(QRect(16, 12, 200, 24), Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(title));
        }

        if (draw_legend && !series_list.empty()) {
            int leg_x = width() - 16;
            QFont lf = p.font();
            lf.setPixelSize(11);
            p.setFont(lf);

            for (auto it = series_list.rbegin(); it != series_list.rend(); ++it) {
                QString name = QString::fromStdString(it->name);
                int text_w = fontMetrics().horizontalAdvance(name);
                leg_x -= (text_w + 24);

                p.setPen(Qt::NoPen);
                p.setBrush(QColor(QString::fromStdString(it->color_hex.empty() ? "#3b82f6" : it->color_hex)));
                p.drawEllipse(leg_x, 18, 8, 8);

                p.setPen(QColor("#cbd5e1"));
                p.drawText(QRect(leg_x + 12, 12, text_w + 4, 20), Qt::AlignLeft | Qt::AlignVCenter, name);
            }
        }

        int left_pad = 42;
        int right_pad = 20;
        int bottom_pad = 28;

        int plot_w = width() - left_pad - right_pad;
        int plot_h = height() - top_pad - bottom_pad;

        if (plot_w <= 10 || plot_h <= 10) return;

        // Compute max / min
        double min_val = custom_min;
        double max_val = custom_max;
        if (max_val <= min_val) {
            max_val = 10.0;
            min_val = 0.0;
            bool first = true;
            for (const auto& s : series_list) {
                for (double v : s.values) {
                    if (first) {
                        min_val = max_val = v;
                        first = false;
                    } else {
                        if (v > max_val) max_val = v;
                        if (v < min_val) min_val = v;
                    }
                }
            }
            if (min_val > 0.0) min_val = 0.0;
            if (max_val <= min_val) max_val = min_val + 10.0;
            double span = max_val - min_val;
            double mag = std::pow(10.0, std::floor(std::log10(span)));
            max_val = std::ceil(max_val / mag) * mag;
        }

        double val_span = (max_val - min_val > 0.0001) ? (max_val - min_val) : 1.0;

        // Draw Y grid
        int grid_steps = 4;
        QFont af = p.font();
        af.setPixelSize(10);
        af.setBold(false);
        p.setFont(af);

        for (int i = 0; i <= grid_steps; ++i) {
            double ratio = static_cast<double>(i) / grid_steps;
            int y = top_pad + plot_h - static_cast<int>(ratio * plot_h);

            if (draw_grid) {
                p.setPen(QPen(QColor("#1e293b"), 1, Qt::DashLine));
                p.drawLine(left_pad, y, left_pad + plot_w, y);
            }

            p.setPen(QColor("#64748b"));
            double val = min_val + ratio * val_span;
            char vbuf[32];
            if (std::abs(val) >= 1000.0) {
                snprintf(vbuf, sizeof(vbuf), "%.0fk", val / 1000.0);
            } else {
                snprintf(vbuf, sizeof(vbuf), "%.0f", val);
            }
            p.drawText(QRect(4, y - 8, left_pad - 8, 16), Qt::AlignRight | Qt::AlignVCenter, vbuf);
        }

        // Determine number of X slots
        size_t max_pts = 0;
        for (const auto& s : series_list) {
            if (s.values.size() > max_pts) max_pts = s.values.size();
        }
        if (!x_labels.empty() && x_labels.size() > max_pts) {
            max_pts = x_labels.size();
        }

        if (max_pts < 2) return;

        // Draw X-labels
        if (!x_labels.empty()) {
            p.setPen(QColor("#94a3b8"));
            for (size_t i = 0; i < x_labels.size(); ++i) {
                double x = left_pad + (static_cast<double>(i) / (max_pts - 1)) * plot_w;
                p.drawText(QRectF(x - 25, top_pad + plot_h + 6, 50, 18),
                           Qt::AlignHCenter | Qt::AlignTop,
                           QString::fromStdString(x_labels[i]));
            }
        }

        // Draw series
        const char* default_colors[] = {"#3b82f6", "#10b981", "#f59e0b", "#ec4899", "#8b5cf6"};
        int col_idx = 0;

        for (const auto& s : series_list) {
            if (s.values.size() < 2) continue;
            QColor col(QString::fromStdString(s.color_hex.empty() ? default_colors[col_idx % 5] : s.color_hex));
            col_idx++;

            std::vector<QPointF> points;
            for (size_t i = 0; i < s.values.size(); ++i) {
                double x = left_pad + (static_cast<double>(i) / (max_pts - 1)) * plot_w;
                double norm = (s.values[i] - min_val) / val_span;
                norm = qBound(0.0, norm, 1.0);
                double y = top_pad + plot_h - norm * plot_h;
                points.emplace_back(x, y);
            }

            QPainterPath line_path;
            line_path.moveTo(points[0]);

            if (is_smooth && points.size() > 2) {
                for (size_t i = 0; i < points.size() - 1; ++i) {
                    QPointF p0 = (i > 0) ? points[i - 1] : points[i];
                    QPointF p1 = points[i];
                    QPointF p2 = points[i + 1];
                    QPointF p3 = (i + 2 < points.size()) ? points[i + 2] : p2;

                    double c1x = p1.x() + (p2.x() - p0.x()) / 6.0;
                    double c1y = p1.y() + (p2.y() - p0.y()) / 6.0;
                    double c2x = p2.x() - (p3.x() - p1.x()) / 6.0;
                    double c2y = p2.y() - (p3.y() - p1.y()) / 6.0;

                    line_path.cubicTo(QPointF(c1x, c1y), QPointF(c2x, c2y), p2);
                }
            } else {
                for (size_t i = 1; i < points.size(); ++i) {
                    line_path.lineTo(points[i]);
                }
            }

            // Fill gradient under curve
            if (s.fill_gradient) {
                QPainterPath fill_path = line_path;
                fill_path.lineTo(points.back().x(), top_pad + plot_h);
                fill_path.lineTo(points.front().x(), top_pad + plot_h);
                fill_path.closeSubpath();

                QLinearGradient grad(0, top_pad, 0, top_pad + plot_h);
                QColor gcol = col;
                gcol.setAlpha(60);
                grad.setColorAt(0.0, gcol);
                gcol.setAlpha(0);
                grad.setColorAt(1.0, gcol);

                p.fillPath(fill_path, grad);
            }

            // Draw line stroke
            QPen line_pen(col, 2.5);
            line_pen.setCapStyle(Qt::RoundCap);
            line_pen.setJoinStyle(Qt::RoundJoin);
            p.setPen(line_pen);
            p.setBrush(Qt::NoBrush);
            p.drawPath(line_path);

            // Draw points
            if (draw_points) {
                p.setPen(QPen(QColor("#0f172a"), 2));
                p.setBrush(col);
                for (const auto& pt : points) {
                    p.drawEllipse(pt, 4.0, 4.0);
                }
            }
        }
    }
};

struct LineChart::Impl {
    QPointer<LineChartWidget> widget;
};

LineChart::LineChart(const std::string& title)
    : pimpl(std::make_shared<Impl>())
{
    auto* w = new LineChartWidget();
    pimpl->widget = w;
    w->title = title;
}

LineChart::~LineChart() = default;

void LineChart::add_series(const std::string& name,
                           const std::vector<double>& values,
                           const std::string& color_hex,
                           bool fill_gradient)
{
    if (pimpl->widget) {
        pimpl->widget->series_list.push_back({name, values, color_hex, fill_gradient});
        pimpl->widget->update();
    }
}

void LineChart::set_x_labels(const std::vector<std::string>& labels) {
    if (pimpl->widget) {
        pimpl->widget->x_labels = labels;
        pimpl->widget->update();
    }
}

void LineChart::clear_series() {
    if (pimpl->widget) {
        pimpl->widget->series_list.clear();
        pimpl->widget->update();
    }
}

void LineChart::set_title(const std::string& title) {
    if (pimpl->widget) {
        pimpl->widget->title = title;
        pimpl->widget->update();
    }
}

void LineChart::set_smooth(bool smooth) {
    if (pimpl->widget) {
        pimpl->widget->is_smooth = smooth;
        pimpl->widget->update();
    }
}

void LineChart::show_points(bool show) {
    if (pimpl->widget) {
        pimpl->widget->draw_points = show;
        pimpl->widget->update();
    }
}

void LineChart::show_grid(bool show) {
    if (pimpl->widget) {
        pimpl->widget->draw_grid = show;
        pimpl->widget->update();
    }
}

void LineChart::show_legend(bool show) {
    if (pimpl->widget) {
        pimpl->widget->draw_legend = show;
        pimpl->widget->update();
    }
}

void LineChart::set_y_range(double min, double max) {
    if (pimpl->widget) {
        pimpl->widget->custom_min = min;
        pimpl->widget->custom_max = max;
        pimpl->widget->update();
    }
}

QWidget* LineChart::get_qwidget() const {
    return pimpl->widget.data();
}

}
