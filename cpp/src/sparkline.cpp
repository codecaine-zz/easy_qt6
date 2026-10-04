#include "simplegui/sparkline.h"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QPointer>
#include <algorithm>
#include <deque>

namespace simplegui {

class SparklineWidget : public QWidget {
public:
    std::deque<double> samples;
    int max_samples = 60;
    double min_value = 0.0;
    double max_value = 100.0;
    QColor color = QColor("#06b6d4");
    bool fill_enabled = true;

    explicit SparklineWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(120, 48);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int w = width();
        int h = height();

        // Background
        p.fillRect(rect(), QColor("#121215"));
        p.setPen(QPen(QColor("#27272a"), 1));
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 6, 6);

        // Grid lines (25%, 50%, 75%)
        p.setPen(QPen(QColor(255, 255, 255, 15), 1, Qt::DashLine));
        for (int i = 1; i <= 3; ++i) {
            int y = h * i / 4;
            p.drawLine(8, y, w - 8, y);
        }

        if (samples.size() < 2) return;

        double range = max_value - min_value;
        if (range <= 0.0001) range = 1.0;

        int margin = 8;
        int plot_w = w - 2 * margin;
        int plot_h = h - 2 * margin;

        QVector<QPointF> points;
        points.reserve(static_cast<int>(samples.size()));

        double step = static_cast<double>(plot_w) / std::max(1, static_cast<int>(max_samples - 1));
        double start_x = margin + (max_samples - static_cast<int>(samples.size())) * step;

        for (size_t i = 0; i < samples.size(); ++i) {
            double norm = (samples[i] - min_value) / range;
            norm = std::clamp(norm, 0.0, 1.0);
            double x = start_x + i * step;
            double y = margin + plot_h * (1.0 - norm);
            points.append(QPointF(x, y));
        }

        QPainterPath path;
        path.moveTo(points[0]);
        for (int i = 1; i < points.size(); ++i) {
            path.lineTo(points[i]);
        }

        // Fill under curve
        if (fill_enabled) {
            QPainterPath fill_path = path;
            fill_path.lineTo(points.back().x(), h - margin);
            fill_path.lineTo(points.front().x(), h - margin);
            fill_path.closeSubpath();

            QLinearGradient grad(0, margin, 0, h - margin);
            QColor top_col = color;
            top_col.setAlpha(100);
            QColor bot_col = color;
            bot_col.setAlpha(10);
            grad.setColorAt(0.0, top_col);
            grad.setColorAt(1.0, bot_col);

            p.fillPath(fill_path, grad);
        }

        // Line
        QPen line_pen(color, 2);
        p.setPen(line_pen);
        p.drawPath(path);

        // Leading glowing dot
        p.setPen(Qt::NoPen);
        QColor dot_halo = color;
        dot_halo.setAlpha(80);
        p.setBrush(dot_halo);
        p.drawEllipse(points.back(), 5.0, 5.0);

        p.setBrush(color);
        p.drawEllipse(points.back(), 2.5, 2.5);
    }
};

struct Sparkline::Impl {
    QPointer<SparklineWidget> widget;
    Impl(const std::string& line_color) {
        widget = new SparklineWidget();
        widget->color = QColor(QString::fromStdString(line_color));
    }
    ~Impl() { if (widget && !widget->parent()) delete widget; }
};

Sparkline::Sparkline(const std::string& line_color)
    : pimpl(std::make_shared<Impl>(line_color)) {}

Sparkline::~Sparkline() = default;

void Sparkline::add_sample(double value) {
    if (pimpl->widget) {
        pimpl->widget->samples.push_back(value);
        if (static_cast<int>(pimpl->widget->samples.size()) > pimpl->widget->max_samples) {
            pimpl->widget->samples.pop_front();
        }
        pimpl->widget->update();
    }
}

void Sparkline::set_samples(const std::vector<double>& samples) {
    if (pimpl->widget) {
        pimpl->widget->samples.assign(samples.begin(), samples.end());
        while (static_cast<int>(pimpl->widget->samples.size()) > pimpl->widget->max_samples) {
            pimpl->widget->samples.pop_front();
        }
        pimpl->widget->update();
    }
}

void Sparkline::clear() {
    if (pimpl->widget) {
        pimpl->widget->samples.clear();
        pimpl->widget->update();
    }
}

void Sparkline::set_color(const std::string& hex_color) {
    if (pimpl->widget) {
        pimpl->widget->color = QColor(QString::fromStdString(hex_color));
        pimpl->widget->update();
    }
}

void Sparkline::set_fill_enabled(bool enabled) {
    if (pimpl->widget) {
        pimpl->widget->fill_enabled = enabled;
        pimpl->widget->update();
    }
}

void Sparkline::set_range(double min_val, double max_val) {
    if (pimpl->widget) {
        pimpl->widget->min_value = min_val;
        pimpl->widget->max_value = max_val;
        pimpl->widget->update();
    }
}

void Sparkline::set_max_samples(int max_count) {
    if (pimpl->widget) {
        pimpl->widget->max_samples = std::max(5, max_count);
        pimpl->widget->update();
    }
}

QWidget* Sparkline::get_qwidget() const {
    return pimpl->widget.data();
}

}
