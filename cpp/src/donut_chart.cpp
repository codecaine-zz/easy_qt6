#include "simplegui/donut_chart.h"
#include "detail/common.h"
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QPointer>
#include <numeric>

namespace simplegui {
namespace {

class DonutWidget : public QWidget {
public:
    std::vector<DonutSlice> slices;
    QString center_title;
    QString center_subtitle;
    int thickness = 22;

    explicit DonutWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(140, 140);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int side = qMin(width(), height());
        if (side <= 20) return;

        double total = 0.0;
        for (const auto& s : slices) {
            total += (s.value > 0 ? s.value : 0);
        }

        QRectF bounds((width() - side) / 2.0 + 10.0, (height() - side) / 2.0 + 10.0, side - 20.0, side - 20.0);
        int ring_thick = qBound(12, thickness, side / 4);

        if (total <= 0.0001) {
            // Draw empty ring
            QPen pen(QColor("#334155"), ring_thick);
            pen.setCapStyle(Qt::FlatCap);
            p.setPen(pen);
            p.setBrush(Qt::NoBrush);
            QRectF ring_bounds = bounds.adjusted(ring_thick / 2.0, ring_thick / 2.0, -ring_thick / 2.0, -ring_thick / 2.0);
            p.drawEllipse(ring_bounds);
        } else {
            QRectF ring_bounds = bounds.adjusted(ring_thick / 2.0, ring_thick / 2.0, -ring_thick / 2.0, -ring_thick / 2.0);
            double start_angle = 90.0 * 16.0; // Start at 12 o'clock

            for (const auto& s : slices) {
                if (s.value <= 0) continue;
                double span_angle = -(s.value / total) * 360.0 * 16.0;

                QPen pen(QColor(QString::fromStdString(s.color_hex.empty() ? "#3b82f6" : s.color_hex)), ring_thick);
                pen.setCapStyle(Qt::FlatCap);
                p.setPen(pen);
                p.setBrush(Qt::NoBrush);

                p.drawArc(ring_bounds, static_cast<int>(start_angle), static_cast<int>(span_angle));
                start_angle += span_angle;
            }
        }

        // Draw center text
        if (!center_title.isEmpty() || !center_subtitle.isEmpty()) {
            QRectF inner_rect = bounds.adjusted(ring_thick + 4, ring_thick + 4, -(ring_thick + 4), -(ring_thick + 4));
            
            p.setPen(QColor("#f8fafc"));
            QFont title_font = p.font();
            title_font.setPixelSize(qMax(14, qMin(24, side / 9)));
            title_font.setBold(true);
            p.setFont(title_font);

            if (!center_subtitle.isEmpty()) {
                QRectF t_rect = inner_rect;
                t_rect.setBottom(inner_rect.center().y() + 4);
                p.drawText(t_rect, Qt::AlignHCenter | Qt::AlignBottom, center_title);

                QFont sub_font = p.font();
                sub_font.setPixelSize(qMax(10, qMin(13, side / 15)));
                sub_font.setBold(false);
                p.setFont(sub_font);
                p.setPen(QColor("#94a3b8"));

                QRectF s_rect = inner_rect;
                s_rect.setTop(inner_rect.center().y() + 6);
                p.drawText(s_rect, Qt::AlignHCenter | Qt::AlignTop, center_subtitle);
            } else {
                p.drawText(inner_rect, Qt::AlignCenter, center_title);
            }
        }
    }
};

}  // namespace

struct DonutChart::Impl {
    QPointer<DonutWidget> widget;
    ~Impl() { detail::delete_if_orphan(widget); }
};

DonutChart::DonutChart(const std::string& center_title, const std::string& center_subtitle)
    : pimpl(std::make_shared<Impl>())
{
    auto* w = new DonutWidget();
    pimpl->widget = w;
    w->center_title = QString::fromStdString(center_title);
    w->center_subtitle = QString::fromStdString(center_subtitle);
}

DonutChart::~DonutChart() = default;

void DonutChart::add_segment(const std::string& label, double value, const std::string& color_hex) {
    if (pimpl->widget) {
        pimpl->widget->slices.push_back({label, value, color_hex});
        pimpl->widget->update();
    }
}

void DonutChart::set_segments(const std::vector<DonutSlice>& segments) {
    if (pimpl->widget) {
        pimpl->widget->slices = segments;
        pimpl->widget->update();
    }
}

void DonutChart::clear_segments() {
    if (pimpl->widget) {
        pimpl->widget->slices.clear();
        pimpl->widget->update();
    }
}

void DonutChart::set_center_text(const std::string& title, const std::string& subtitle) {
    if (pimpl->widget) {
        pimpl->widget->center_title = QString::fromStdString(title);
        pimpl->widget->center_subtitle = QString::fromStdString(subtitle);
        pimpl->widget->update();
    }
}

void DonutChart::set_thickness(int thickness_px) {
    if (pimpl->widget) {
        pimpl->widget->thickness = thickness_px;
        pimpl->widget->update();
    }
}

QWidget* DonutChart::get_qwidget() const {
    return pimpl->widget.data();
}

}
