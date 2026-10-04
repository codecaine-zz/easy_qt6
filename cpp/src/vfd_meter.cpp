#include "simplegui/vfd_meter.h"
#include "detail/common.h"
#include <QWidget>
#include <QPainter>
#include <QPointer>
#include <algorithm>

namespace simplegui {
namespace {

class VfdMeterWidget : public detail::PaintedWidget {
public:
    int segment_count = 20;
    bool is_vertical = true;
    double percentage = 0.0;
    bool glow_effect = true;

    explicit VfdMeterWidget(QWidget* parent = nullptr) : detail::PaintedWidget(parent) {
        setMinimumSize(24, 120);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        int w = width();
        int h = height();

        // Dark housing
        p.fillRect(rect(), QColor("#121215"));
        p.setPen(QPen(QColor("#27272a"), 1));
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 4, 4);

        int lit_count = static_cast<int>((std::clamp(percentage, 0.0, 100.0) / 100.0) * segment_count);

        int margin = 4;
        int gap = 2;

        if (is_vertical) {
            int total_height = h - 2 * margin;
            double seg_h = static_cast<double>(total_height - (segment_count - 1) * gap) / segment_count;
            int seg_w = w - 2 * margin;

            for (int i = 0; i < segment_count; ++i) {
                // Bottom to top
                int index_from_bottom = i;
                double y = h - margin - (i + 1) * seg_h - i * gap;
                QRectF seg_rect(margin, y, seg_w, seg_h);

                bool is_lit = (index_from_bottom < lit_count);

                // Color calculation: bottom 60% green/cyan, next 25% amber, top 15% red
                double seg_pos = static_cast<double>(i) / segment_count;
                QColor active_color;
                if (seg_pos < 0.60) {
                    active_color = QColor("#10b981"); // green
                } else if (seg_pos < 0.85) {
                    active_color = QColor("#f59e0b"); // amber
                } else {
                    active_color = QColor("#ef4444"); // red
                }

                if (is_lit) {
                    p.setPen(Qt::NoPen);
                    p.setBrush(active_color);
                    p.drawRoundedRect(seg_rect, 2, 2);

                    if (glow_effect) {
                        QColor halo = active_color;
                        halo.setAlpha(40);
                        p.setBrush(halo);
                        p.drawRoundedRect(seg_rect.adjusted(-1, -1, 1, 1), 3, 3);
                    }
                } else {
                    p.setPen(Qt::NoPen);
                    QColor unlit = active_color;
                    unlit.setAlpha(30);
                    p.setBrush(unlit);
                    p.drawRoundedRect(seg_rect, 2, 2);
                }
            }
        } else {
            // Horizontal meter
            int total_width = w - 2 * margin;
            double seg_w = static_cast<double>(total_width - (segment_count - 1) * gap) / segment_count;
            int seg_h = h - 2 * margin;

            for (int i = 0; i < segment_count; ++i) {
                double x = margin + i * (seg_w + gap);
                QRectF seg_rect(x, margin, seg_w, seg_h);

                bool is_lit = (i < lit_count);
                double seg_pos = static_cast<double>(i) / segment_count;
                QColor active_color;
                if (seg_pos < 0.60) {
                    active_color = QColor("#10b981");
                } else if (seg_pos < 0.85) {
                    active_color = QColor("#f59e0b");
                } else {
                    active_color = QColor("#ef4444");
                }

                if (is_lit) {
                    p.setPen(Qt::NoPen);
                    p.setBrush(active_color);
                    p.drawRoundedRect(seg_rect, 2, 2);
                } else {
                    p.setPen(Qt::NoPen);
                    QColor unlit = active_color;
                    unlit.setAlpha(30);
                    p.setBrush(unlit);
                    p.drawRoundedRect(seg_rect, 2, 2);
                }
            }
        }
    }
};

}  // namespace

struct VfdMeter::Impl {
    QPointer<VfdMeterWidget> widget;
    Impl(int segment_count, bool vertical) {
        widget = new VfdMeterWidget();
        widget->segment_count = std::max(3, segment_count);
        widget->is_vertical = vertical;
        if (!vertical) {
            widget->setMinimumSize(120, 24);
        }
    }
    ~Impl() { detail::delete_if_orphan(widget); }
};

VfdMeter::VfdMeter(int segment_count, bool vertical)
    : pimpl(std::make_shared<Impl>(segment_count, vertical)) {}

VfdMeter::~VfdMeter() = default;

void VfdMeter::set_value(double percentage) {
    if (pimpl->widget) {
        pimpl->widget->percentage = std::clamp(percentage, 0.0, 100.0);
        pimpl->widget->update();
    }
}

double VfdMeter::get_value() const {
    return pimpl->widget ? pimpl->widget->percentage : 0.0;
}

void VfdMeter::set_segments(int count) {
    if (pimpl->widget) {
        pimpl->widget->segment_count = std::max(3, count);
        pimpl->widget->update();
    }
}

void VfdMeter::set_glow(bool glow) {
    if (pimpl->widget) {
        pimpl->widget->glow_effect = glow;
        pimpl->widget->update();
    }
}

QWidget* VfdMeter::get_qwidget() const {
    return pimpl->widget.data();
}

}
