#include "simplegui/bar_chart.h"
#include "detail/common.h"

#include <QPainter>
#include <QPainterPath>
#include <QWidget>

#include <algorithm>
#include <cmath>

namespace simplegui {
namespace {

QString format_number(double v) {
    if (std::fabs(v) >= 1000.0) return QString::number(v / 1000.0, 'f', 0) + QLatin1Char('k');
    if (v == std::floor(v)) return QString::number(static_cast<long long>(v));
    return QString::number(v, 'f', 1);
}

class BarChartWidget : public QWidget {
public:
    QString title;
    std::vector<BarItem> bars;
    bool show_values = true;
    bool show_grid = true;
    double custom_min = 0.0;
    double custom_max = 0.0;  // custom range is used only when max > min

    BarChartWidget() {
        setMinimumSize(280, 160);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        p.fillRect(rect(), QColor(0x11, 0x18, 0x27));
        p.setPen(QColor(0x1f, 0x29, 0x37));
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 8, 8);

        int top_pad = 16;
        if (!title.isEmpty()) {
            p.setPen(QColor(0xf8, 0xfa, 0xfc));
            QFont tf = p.font();
            tf.setPixelSize(12);
            tf.setBold(true);
            p.setFont(tf);
            p.drawText(QRect(14, 10, width() - 28, 20), Qt::AlignLeft | Qt::AlignVCenter, title);
            top_pad = 36;
        }

        const int left_pad = 42;
        const int right_pad = 16;
        const int bottom_pad = 28;
        const int plot_w = width() - left_pad - right_pad;
        const int plot_h = height() - top_pad - bottom_pad;
        if (plot_w <= 10 || plot_h <= 10) return;

        double min_val = 0.0;
        double max_val = 0.0;
        if (custom_max > custom_min) {
            min_val = custom_min;
            max_val = custom_max;
        } else {
            for (const auto& b : bars) max_val = std::max(max_val, b.value);
            if (max_val <= 0.0) max_val = 100.0;
            const double magnitude = std::pow(10.0, std::floor(std::log10(max_val)));
            max_val = std::ceil(max_val / magnitude) * magnitude;  // round up to a "nice" number
        }
        const double span = max_val - min_val;

        QFont axis_font = p.font();
        axis_font.setPixelSize(10);
        axis_font.setBold(false);
        p.setFont(axis_font);

        const int grid_steps = 4;
        for (int i = 0; i <= grid_steps; ++i) {
            const double ratio = static_cast<double>(i) / grid_steps;
            const int y = top_pad + plot_h - static_cast<int>(ratio * plot_h);
            if (show_grid) {
                p.setPen(QPen(QColor(0x1e, 0x29, 0x3b), 1, Qt::DashLine));
                p.drawLine(left_pad, y, left_pad + plot_w, y);
            }
            p.setPen(QColor(0x64, 0x74, 0x8b));
            p.drawText(QRect(4, y - 8, left_pad - 8, 16), Qt::AlignRight | Qt::AlignVCenter,
                       format_number(min_val + ratio * span));
        }

        if (bars.empty()) return;

        static const QColor palette[] = {QColor(0x3b, 0x82, 0xf6), QColor(0x06, 0xb6, 0xd4), QColor(0x10, 0xb9, 0x81),
                                         QColor(0xf5, 0x9e, 0x0b), QColor(0xec, 0x48, 0x99), QColor(0x8b, 0x5c, 0xf6)};
        const int count = static_cast<int>(bars.size());
        const double slot_w = static_cast<double>(plot_w) / count;
        const double bar_w = std::max(4.0, slot_w * 0.65);

        for (int i = 0; i < count; ++i) {
            const auto& b = bars[static_cast<size_t>(i)];
            const double x_center = left_pad + (i + 0.5) * slot_w;
            const double bar_h = std::clamp((b.value - min_val) / span * plot_h, 0.0, static_cast<double>(plot_h));
            const double bar_x = x_center - bar_w / 2.0;
            const double bar_y = top_pad + plot_h - bar_h;

            QPainterPath bar_path;
            bar_path.addRoundedRect(QRectF(bar_x, bar_y, bar_w, bar_h), 4, 4);
            p.fillPath(bar_path, detail::parse_color(b.color_hex, palette[i % 6]));

            if (show_values && bar_h > 12) {
                p.setPen(QColor(0xf1, 0xf5, 0xf9));
                QFont vf = p.font();
                vf.setBold(true);
                p.setFont(vf);
                p.drawText(QRectF(bar_x - 10, bar_y - 16, bar_w + 20, 14), Qt::AlignCenter, format_number(b.value));
            }

            p.setPen(QColor(0x94, 0xa3, 0xb8));
            QFont xf = p.font();
            xf.setBold(false);
            p.setFont(xf);
            p.drawText(QRectF(bar_x - 15, top_pad + plot_h + 4, bar_w + 30, 20), Qt::AlignHCenter | Qt::AlignTop,
                       detail::qs(b.label));
        }
    }
};

}  // namespace

struct BarChart::Impl {
    QPointer<BarChartWidget> widget = new BarChartWidget();
    ~Impl() { detail::delete_if_orphan(widget); }
};

BarChart::BarChart(const std::string& title) : pimpl(std::make_shared<Impl>()) {
    pimpl->widget->title = detail::qs(title);
}

BarChart::~BarChart() = default;

void BarChart::add_bar(const std::string& label, double value, const std::string& color_hex) {
    if (!pimpl->widget) return;
    pimpl->widget->bars.push_back({label, value, color_hex});
    pimpl->widget->update();
}

void BarChart::set_bars(const std::vector<BarItem>& bars) {
    if (!pimpl->widget) return;
    pimpl->widget->bars = bars;
    pimpl->widget->update();
}

void BarChart::set_value(int index, double value) {
    if (!pimpl->widget || index < 0 || index >= static_cast<int>(pimpl->widget->bars.size())) return;
    pimpl->widget->bars[static_cast<size_t>(index)].value = value;
    pimpl->widget->update();
}

void BarChart::clear_bars() {
    if (!pimpl->widget) return;
    pimpl->widget->bars.clear();
    pimpl->widget->update();
}

int BarChart::bar_count() const { return pimpl->widget ? static_cast<int>(pimpl->widget->bars.size()) : 0; }

void BarChart::set_title(const std::string& title) {
    if (!pimpl->widget) return;
    pimpl->widget->title = detail::qs(title);
    pimpl->widget->update();
}

void BarChart::set_show_values(bool show) {
    if (!pimpl->widget) return;
    pimpl->widget->show_values = show;
    pimpl->widget->update();
}

void BarChart::set_show_grid(bool show) {
    if (!pimpl->widget) return;
    pimpl->widget->show_grid = show;
    pimpl->widget->update();
}

void BarChart::set_y_range(double min, double max) {
    if (!pimpl->widget) return;
    pimpl->widget->custom_min = min;
    pimpl->widget->custom_max = max;
    pimpl->widget->update();
}

QWidget* BarChart::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
