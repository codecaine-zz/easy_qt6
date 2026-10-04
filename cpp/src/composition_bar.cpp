#include "simplegui/composition_bar.h"
#include "detail/common.h"
#include <QWidget>
#include <QPainter>
#include <QPointer>
#include <numeric>

namespace simplegui {
namespace {

class CompositionBarWidget : public QWidget {
public:
    std::vector<CompositionSegment> segments;

    explicit CompositionBarWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(160, 18);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
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
        p.drawRoundedRect(rect().adjusted(0, 0, -1, -1), 4, 4);

        if (segments.empty()) return;

        double total = 0.0;
        for (const auto& s : segments) {
            total += std::max(0.0, s.value);
        }

        if (total <= 0.0001) return;

        int margin = 2;
        int bar_w = w - 2 * margin;
        int bar_h = h - 2 * margin;

        double current_x = margin;

        p.setClipRect(rect().adjusted(margin, margin, -margin, -margin));

        for (size_t i = 0; i < segments.size(); ++i) {
            const auto& s = segments[i];
            double seg_w = (s.value / total) * bar_w;
            if (seg_w < 1.0) continue;

            QRectF r(current_x, margin, seg_w, bar_h);
            p.fillRect(r, detail::parse_color(s.color, QColor(0x64, 0x74, 0x8b)));

            // Subtle divider line
            if (i + 1 < segments.size()) {
                p.setPen(QColor("#18181b"));
                p.drawLine(QPointF(current_x + seg_w, margin), QPointF(current_x + seg_w, margin + bar_h));
            }

            current_x += seg_w;
        }
    }
};

}  // namespace

struct CompositionBar::Impl {
    QPointer<CompositionBarWidget> widget;
    Impl() {
        widget = new CompositionBarWidget();
    }
    ~Impl() { detail::delete_if_orphan(widget); }
};

CompositionBar::CompositionBar() : pimpl(std::make_shared<Impl>()) {}

CompositionBar::~CompositionBar() = default;

void CompositionBar::add_segment(const std::string& label, double value, const std::string& color) {
    if (pimpl->widget) {
        pimpl->widget->segments.push_back({label, value, color});
        pimpl->widget->update();
    }
}

void CompositionBar::set_segments(const std::vector<CompositionSegment>& segments) {
    if (pimpl->widget) {
        pimpl->widget->segments = segments;
        pimpl->widget->update();
    }
}

void CompositionBar::clear_segments() {
    if (pimpl->widget) {
        pimpl->widget->segments.clear();
        pimpl->widget->update();
    }
}

QWidget* CompositionBar::get_qwidget() const {
    return pimpl->widget.data();
}

}
