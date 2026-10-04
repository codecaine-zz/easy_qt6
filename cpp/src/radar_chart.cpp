#include "simplegui/radar_chart.h"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <cmath>

namespace simplegui {

class RadarChartWidget : public QWidget {
public:
    std::string title;
    std::vector<std::string> dimensions;
    std::vector<RadarDataset> datasets;
    bool draw_legend = true;

    explicit RadarChartWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(260, 240);
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
            tf.setPixelSize(13);
            tf.setBold(true);
            p.setFont(tf);
            p.drawText(QRect(16, 12, width() - 32, 22), Qt::AlignLeft | Qt::AlignVCenter, QString::fromStdString(title));
            top_pad = 38;
        }

        int N = static_cast<int>(dimensions.size());
        if (N < 3) return;

        int avail_w = width() - 32;
        int avail_h = height() - top_pad - (draw_legend && !datasets.empty() ? 28 : 16);
        if (avail_w <= 40 || avail_h <= 40) return;

        double cx = 16 + avail_w / 2.0;
        double cy = top_pad + avail_h / 2.0;
        double max_r = qMin(avail_w, avail_h) / 2.0 - 24.0;
        if (max_r < 20.0) max_r = 20.0;

        const double PI = 3.14159265358979323846;
        auto get_pt = [&](double angle_rad, double r) {
            return QPointF(cx + r * std::cos(angle_rad), cy + r * std::sin(angle_rad));
        };

        // Draw concentric web rings
        int rings = 4;
        p.setPen(QPen(QColor("#1e293b"), 1, Qt::SolidLine));
        p.setBrush(Qt::NoBrush);

        for (int ring = 1; ring <= rings; ++ring) {
            double r = max_r * (static_cast<double>(ring) / rings);
            QPolygonF ring_poly;
            for (int i = 0; i < N; ++i) {
                double angle = -PI / 2.0 + i * (2.0 * PI / N);
                ring_poly << get_pt(angle, r);
            }
            p.drawPolygon(ring_poly);
        }

        // Draw spokes & labels
        QFont df = p.font();
        df.setPixelSize(10);
        df.setBold(true);
        p.setFont(df);

        for (int i = 0; i < N; ++i) {
            double angle = -PI / 2.0 + i * (2.0 * PI / N);
            QPointF outer = get_pt(angle, max_r);

            p.setPen(QPen(QColor("#334155"), 1, Qt::DashLine));
            p.drawLine(QPointF(cx, cy), outer);

            // Label text slightly offset
            QPointF label_pt = get_pt(angle, max_r + 14.0);
            p.setPen(QColor("#94a3b8"));
            QRectF label_rect(label_pt.x() - 35, label_pt.y() - 8, 70, 16);
            p.drawText(label_rect, Qt::AlignCenter, QString::fromStdString(dimensions[i]));
        }

        // Draw datasets
        const char* default_colors[] = {"#3b82f6", "#10b981", "#f59e0b", "#ec4899", "#8b5cf6"};
        int col_idx = 0;

        for (const auto& ds : datasets) {
            QColor col(QString::fromStdString(ds.color_hex.empty() ? default_colors[col_idx % 5] : ds.color_hex));
            col_idx++;

            QPolygonF poly;
            for (int i = 0; i < N; ++i) {
                double val = (i < static_cast<int>(ds.values.size())) ? ds.values[i] : 0.0;
                val = qBound(0.0, val, 100.0);
                double r = max_r * (val / 100.0);
                double angle = -PI / 2.0 + i * (2.0 * PI / N);
                poly << get_pt(angle, r);
            }

            // Fill area
            QColor fill_col = col;
            fill_col.setAlpha(65);
            p.setPen(QPen(col, 2));
            p.setBrush(fill_col);
            p.drawPolygon(poly);

            // Draw vertex dots
            p.setPen(QPen(QColor("#0f172a"), 1.5));
            p.setBrush(col);
            for (const auto& pt : poly) {
                p.drawEllipse(pt, 3.5, 3.5);
            }
        }

        // Draw legend at bottom
        if (draw_legend && !datasets.empty()) {
            int leg_y = height() - 20;
            int total_leg_w = 0;
            for (const auto& ds : datasets) {
                total_leg_w += fontMetrics().horizontalAdvance(QString::fromStdString(ds.name)) + 26;
            }

            int cur_x = qMax(16, (width() - total_leg_w) / 2);
            col_idx = 0;
            QFont lf = p.font();
            lf.setPixelSize(10);
            p.setFont(lf);

            for (const auto& ds : datasets) {
                QColor col(QString::fromStdString(ds.color_hex.empty() ? default_colors[col_idx % 5] : ds.color_hex));
                col_idx++;

                p.setPen(Qt::NoPen);
                p.setBrush(col);
                p.drawRoundedRect(cur_x, leg_y + 3, 8, 8, 2, 2);

                p.setPen(QColor("#cbd5e1"));
                QString name = QString::fromStdString(ds.name);
                int nw = fontMetrics().horizontalAdvance(name);
                p.drawText(QRect(cur_x + 12, leg_y, nw + 4, 14), Qt::AlignLeft | Qt::AlignVCenter, name);

                cur_x += (nw + 26);
            }
        }
    }
};

struct RadarChart::Impl {
    QPointer<RadarChartWidget> widget;
};

RadarChart::RadarChart(const std::string& title)
    : pimpl(std::make_shared<Impl>())
{
    auto* w = new RadarChartWidget();
    pimpl->widget = w;
    w->title = title;
}

RadarChart::~RadarChart() = default;

void RadarChart::set_dimensions(const std::vector<std::string>& labels) {
    if (pimpl->widget) {
        pimpl->widget->dimensions = labels;
        pimpl->widget->update();
    }
}

void RadarChart::add_dataset(const std::string& name, const std::vector<double>& values, const std::string& color_hex) {
    if (pimpl->widget) {
        pimpl->widget->datasets.push_back({name, values, color_hex});
        pimpl->widget->update();
    }
}

void RadarChart::clear_datasets() {
    if (pimpl->widget) {
        pimpl->widget->datasets.clear();
        pimpl->widget->update();
    }
}

void RadarChart::set_title(const std::string& title) {
    if (pimpl->widget) {
        pimpl->widget->title = title;
        pimpl->widget->update();
    }
}

void RadarChart::show_legend(bool show) {
    if (pimpl->widget) {
        pimpl->widget->draw_legend = show;
        pimpl->widget->update();
    }
}

QWidget* RadarChart::get_qwidget() const {
    return pimpl->widget.data();
}

}
