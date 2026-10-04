#include "simplegui/glass_panel.h"
#include "detail/box_core.h"

#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>

namespace simplegui {
namespace {

constexpr int kTitleHeight = 26;

class GlassWidget : public QWidget {
public:
    QString title;
    QColor accent = QColor(0x00, 0xe5, 0xff);
    QVBoxLayout* layout;

    GlassWidget() : layout(new QVBoxLayout(this)) { apply_margins(14); }

    void apply_margins(int m) {
        margin_ = std::max(0, m);
        layout->setContentsMargins(margin_, margin_ + (title.isEmpty() ? 0 : kTitleHeight), margin_, margin_);
        update();
    }
    int margin() const { return margin_; }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        const QRectF r = QRectF(rect()).adjusted(1, 1, -1, -1);

        // Frosted body: translucent light-to-dark gradient over whatever is behind.
        QLinearGradient body(r.topLeft(), r.bottomRight());
        body.setColorAt(0.0, QColor(255, 255, 255, 28));
        body.setColorAt(1.0, QColor(255, 255, 255, 8));
        p.setPen(Qt::NoPen);
        p.setBrush(body);
        p.drawRoundedRect(r, 14, 14);

        // Top sheen
        QLinearGradient sheen(r.topLeft(), QPointF(r.left(), r.top() + r.height() * 0.4));
        sheen.setColorAt(0.0, QColor(255, 255, 255, 22));
        sheen.setColorAt(1.0, QColor(255, 255, 255, 0));
        p.setBrush(sheen);
        p.drawRoundedRect(r, 14, 14);

        // Glowing border
        QColor edge = accent;
        edge.setAlphaF(0.15);
        p.setBrush(Qt::NoBrush);
        p.setPen(QPen(edge, 4));
        p.drawRoundedRect(r.adjusted(1, 1, -1, -1), 13, 13);
        edge.setAlphaF(0.55);
        p.setPen(QPen(edge, 1));
        p.drawRoundedRect(r, 14, 14);

        if (!title.isEmpty()) {
            QFont f = font();
            f.setBold(true);
            f.setLetterSpacing(QFont::AbsoluteSpacing, 2);
            p.setFont(f);
            p.setPen(accent);
            p.drawText(QRectF(r.left() + margin_, r.top() + 6, r.width() - 2 * margin_, kTitleHeight - 4),
                       Qt::AlignLeft | Qt::AlignVCenter, title);
        }
    }

private:
    int margin_ = 14;
};

}  // namespace

struct GlassPanel::Impl {
    QPointer<GlassWidget> widget = new GlassWidget();
    detail::BoxCore box{widget, widget->layout};
    ~Impl() { detail::delete_if_orphan(widget); }
};

GlassPanel::GlassPanel(const std::string& title) : pimpl(std::make_shared<Impl>()) {
    // Text-only children should show the glass through them, not the theme's solid color.
    detail::set_base_style(pimpl->widget, QStringLiteral(
        "QLabel, QCheckBox, QRadioButton { background: transparent; }"));
    set_title(title);
}
GlassPanel::~GlassPanel() = default;

void GlassPanel::add_child(std::shared_ptr<Control> control, int stretch) { pimpl->box.add_child(control, stretch); }
void GlassPanel::add_stretch(int stretch) { pimpl->box.add_stretch(stretch); }
void GlassPanel::add_spacing(int pixels) { pimpl->box.add_spacing(pixels); }
void GlassPanel::remove_child(std::shared_ptr<Control> control) { pimpl->box.remove_child(control); }
void GlassPanel::clear() { pimpl->box.clear(); }
int GlassPanel::child_count() const { return pimpl->box.count(); }
void GlassPanel::set_spacing(int pixels) { pimpl->box.set_spacing(pixels); }

void GlassPanel::set_margins(int pixels) {
    if (pimpl->widget) pimpl->widget->apply_margins(pixels);
}

void GlassPanel::set_title(const std::string& title) {
    auto* w = pimpl->widget.data();
    if (!w) return;
    w->title = detail::qs(title).toUpper();
    w->apply_margins(w->margin());
}

std::string GlassPanel::get_title() const {
    return pimpl->widget ? detail::ss(pimpl->widget->title) : std::string();
}

void GlassPanel::set_accent_color(const std::string& color) {
    if (!pimpl->widget) return;
    pimpl->widget->accent = detail::parse_color(color, pimpl->widget->accent);
    pimpl->widget->update();
}

QWidget* GlassPanel::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
