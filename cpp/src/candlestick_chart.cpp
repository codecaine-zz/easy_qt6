#include "simplegui/candlestick_chart.h"
#include "detail/common.h"
#include <QWidget>
#include <QPainter>
#include <QPointer>
#include <algorithm>
#include <cmath>

namespace simplegui {
namespace {

class CandlestickWidget : public QWidget {
public:
    std::string title;
    std::vector<CandleData> candles;
    bool draw_grid = true;

    explicit CandlestickWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setMinimumSize(320, 180);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        // Terminal background
        p.fillRect(rect(), QColor("#090d16"));
        p.setPen(QColor("#1e293b"));
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

        if (candles.empty()) return;

        double min_val = candles[0].low;
        double max_val = candles[0].high;

        for (const auto& c : candles) {
            if (c.low < min_val) min_val = c.low;
            if (c.high > max_val) max_val = c.high;
        }

        // Add 5% padding
        double span = (max_val - min_val > 0.0001) ? (max_val - min_val) : 1.0;
        min_val -= span * 0.05;
        max_val += span * 0.05;
        span = max_val - min_val;

        int left_pad = 16;
        int right_pad = 54;
        int bottom_pad = 28;

        int plot_w = width() - left_pad - right_pad;
        int plot_h = height() - top_pad - bottom_pad;
        if (plot_w <= 10 || plot_h <= 10) return;

        // Draw horizontal grid lines & price ticks on right
        int grid_steps = 4;
        QFont af = p.font();
        af.setPixelSize(10);
        p.setFont(af);

        for (int i = 0; i <= grid_steps; ++i) {
            double ratio = static_cast<double>(i) / grid_steps;
            int y = top_pad + plot_h - static_cast<int>(ratio * plot_h);

            if (draw_grid) {
                p.setPen(QPen(QColor("#1e293b"), 1, Qt::DashLine));
                p.drawLine(left_pad, y, left_pad + plot_w, y);
            }

            p.setPen(QColor("#64748b"));
            double price = min_val + ratio * span;
            char pbuf[32];
            snprintf(pbuf, sizeof(pbuf), "%.2f", price);
            p.drawText(QRect(left_pad + plot_w + 6, y - 8, right_pad - 10, 16), Qt::AlignLeft | Qt::AlignVCenter, pbuf);
        }

        // Draw candles
        int N = static_cast<int>(candles.size());
        double slot_w = static_cast<double>(plot_w) / N;
        double body_w = qBound(4.0, slot_w * 0.65, 24.0);

        auto val_to_y = [&](double v) {
            double norm = (v - min_val) / span;
            return top_pad + plot_h - norm * plot_h;
        };

        for (int i = 0; i < N; ++i) {
            const auto& c = candles[i];
            double cx = left_pad + (i + 0.5) * slot_w;

            double high_y = val_to_y(c.high);
            double low_y = val_to_y(c.low);
            double open_y = val_to_y(c.open);
            double close_y = val_to_y(c.close);

            bool bullish = (c.close >= c.open);
            QColor col = bullish ? QColor("#10b981") : QColor("#ef4444");

            // Wick
            p.setPen(QPen(col, 1.5));
            p.drawLine(QPointF(cx, high_y), QPointF(cx, low_y));

            // Candle body
            double body_top = qMin(open_y, close_y);
            double body_h = std::abs(close_y - open_y);
            if (body_h < 2.0) body_h = 2.0;

            QRectF body_rect(cx - body_w / 2.0, body_top, body_w, body_h);
            p.setPen(col);
            p.setBrush(col);
            p.drawRect(body_rect);

            // Time label
            p.setPen(QColor("#94a3b8"));
            QFont xf = p.font();
            xf.setPixelSize(10);
            p.setFont(xf);
            p.drawText(QRectF(cx - 20, top_pad + plot_h + 6, 40, 16), Qt::AlignCenter, QString::fromStdString(c.label));
        }
    }
};

}  // namespace

struct CandlestickChart::Impl {
    QPointer<CandlestickWidget> widget;
    ~Impl() { detail::delete_if_orphan(widget); }
};

CandlestickChart::CandlestickChart(const std::string& title)
    : pimpl(std::make_shared<Impl>())
{
    auto* w = new CandlestickWidget();
    pimpl->widget = w;
    w->title = title;
}

CandlestickChart::~CandlestickChart() = default;

void CandlestickChart::add_candle(const std::string& label, double open, double high, double low, double close) {
    if (pimpl->widget) {
        pimpl->widget->candles.push_back({label, open, high, low, close});
        pimpl->widget->update();
    }
}

void CandlestickChart::set_candles(const std::vector<CandleData>& candles) {
    if (pimpl->widget) {
        pimpl->widget->candles = candles;
        pimpl->widget->update();
    }
}

void CandlestickChart::clear_candles() {
    if (pimpl->widget) {
        pimpl->widget->candles.clear();
        pimpl->widget->update();
    }
}

void CandlestickChart::set_title(const std::string& title) {
    if (pimpl->widget) {
        pimpl->widget->title = title;
        pimpl->widget->update();
    }
}

void CandlestickChart::show_grid(bool show) {
    if (pimpl->widget) {
        pimpl->widget->draw_grid = show;
        pimpl->widget->update();
    }
}

QWidget* CandlestickChart::get_qwidget() const {
    return pimpl->widget.data();
}

}
