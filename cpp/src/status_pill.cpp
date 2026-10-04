#include "simplegui/status_pill.h"
#include "detail/common.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>

namespace simplegui {

struct StatusPill::Impl {
    QPointer<QFrame> frame = new QFrame();
    QPointer<QLabel> dot;
    QPointer<QLabel> text_label;
    QColor color = QColor(0x10, 0xb9, 0x81);

    ~Impl() { detail::delete_if_orphan(frame); }

    void apply_color() {
        if (dot) dot->setStyleSheet(QStringLiteral("background-color: %1; border-radius: 4px;").arg(color.name()));
    }
};

StatusPill::StatusPill(const std::string& text, const std::string& dot_color) : pimpl(std::make_shared<Impl>()) {
    QFrame* frame = pimpl->frame;
    frame->setProperty("sg_role", QStringLiteral("status_pill"));
    detail::set_base_style(frame, QStringLiteral(
        "QFrame[sg_role=\"status_pill\"] { background-color: #1a1a1e; border: 1px solid #27272a; border-radius: 12px; }"));
    frame->setFixedHeight(26);

    auto* layout = new QHBoxLayout(frame);
    layout->setContentsMargins(10, 2, 10, 2);
    layout->setSpacing(6);

    pimpl->dot = new QLabel(frame);
    pimpl->dot->setFixedSize(8, 8);
    pimpl->text_label = detail::plain_label(text, frame);
    pimpl->text_label->setStyleSheet(QStringLiteral(
        "color: #e2e8f0; font-size: 11px; font-weight: 600; letter-spacing: 0.5px; background: transparent;"));
    layout->addWidget(pimpl->dot);
    layout->addWidget(pimpl->text_label);
    set_color(dot_color);
}

StatusPill::~StatusPill() = default;

void StatusPill::set_status(const std::string& text, const std::string& dot_color) {
    set_text(text);
    set_color(dot_color);
}

void StatusPill::set_text(const std::string& text) {
    if (pimpl->text_label) pimpl->text_label->setText(detail::qs(text));
}

void StatusPill::set_color(const std::string& dot_color) {
    pimpl->color = detail::parse_color(dot_color, pimpl->color);
    pimpl->apply_color();
}

std::string StatusPill::text() const {
    return pimpl->text_label ? detail::ss(pimpl->text_label->text()) : std::string();
}

QWidget* StatusPill::get_qwidget() const { return pimpl->frame.data(); }

}  // namespace simplegui
