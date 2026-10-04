#include "simplegui/stat_card.h"
#include "detail/common.h"

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

namespace simplegui {

struct StatCard::Impl {
    QPointer<QFrame> frame = new QFrame();
    QPointer<QLabel> lbl_title;
    QPointer<QLabel> lbl_value;
    QPointer<QLabel> lbl_subtext;
    QColor accent = QColor(0x3b, 0x82, 0xf6);

    ~Impl() { detail::delete_if_orphan(frame); }

    void apply_accent() {
        if (!lbl_value) return;
        // QColor::name() output is always "#rrggbb", so user text never reaches the style sheet.
        lbl_value->setStyleSheet(QStringLiteral(
            "color: %1; font-size: 22px; font-weight: bold; font-family: monospace; background: transparent;")
                                     .arg(accent.name()));
    }
};

StatCard::StatCard(const std::string& title, const std::string& value, const std::string& subtext,
                   const std::string& accent_color)
    : pimpl(std::make_shared<Impl>()) {
    QFrame* frame = pimpl->frame;
    frame->setProperty("sg_role", QStringLiteral("stat_card"));
    detail::set_base_style(frame, QStringLiteral(
        "QFrame[sg_role=\"stat_card\"] { background-color: #1a1a1e; border: 1px solid #27272a; border-radius: 8px; }"));
    frame->setMinimumWidth(130);

    auto* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(14, 12, 14, 12);
    layout->setSpacing(4);

    pimpl->lbl_title = detail::plain_label("", frame);
    pimpl->lbl_title->setStyleSheet(QStringLiteral("color: #a1a1aa; font-size: 11px; font-weight: 600; background: transparent;"));
    pimpl->lbl_value = detail::plain_label(value, frame);
    pimpl->lbl_subtext = detail::plain_label("", frame);
    pimpl->lbl_subtext->setStyleSheet(QStringLiteral("color: #71717a; font-size: 11px; background: transparent;"));
    layout->addWidget(pimpl->lbl_title);
    layout->addWidget(pimpl->lbl_value);
    layout->addWidget(pimpl->lbl_subtext);

    set_title(title);
    set_subtext(subtext);
    set_accent_color(accent_color);
}

StatCard::~StatCard() = default;

void StatCard::set_title(const std::string& title) {
    // Captions are shown in capitals, matching the dashboard look.
    if (pimpl->lbl_title) pimpl->lbl_title->setText(detail::qs(title).toUpper());
}

void StatCard::set_value(const std::string& value) {
    if (pimpl->lbl_value) pimpl->lbl_value->setText(detail::qs(value));
}

void StatCard::set_subtext(const std::string& subtext) {
    if (!pimpl->lbl_subtext) return;
    pimpl->lbl_subtext->setText(detail::qs(subtext));
    pimpl->lbl_subtext->setVisible(!subtext.empty());
}

void StatCard::set_accent_color(const std::string& hex_color) {
    pimpl->accent = detail::parse_color(hex_color, pimpl->accent);
    pimpl->apply_accent();
}

std::string StatCard::value() const {
    return pimpl->lbl_value ? detail::ss(pimpl->lbl_value->text()) : std::string();
}

QWidget* StatCard::get_qwidget() const { return pimpl->frame.data(); }

}  // namespace simplegui
