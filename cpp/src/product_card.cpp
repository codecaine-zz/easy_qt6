#include "simplegui/product_card.h"
#include "detail/common.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace simplegui {
namespace {

QString rating_text(double rating) {
    return QString(QChar(0x2605)) + QLatin1Char(' ') + QString::number(rating, 'f', 1);  // black star
}

}  // namespace

struct ProductCard::Impl {
    QPointer<QFrame> frame = new QFrame();
    QPointer<QLabel> badge_label;
    QPointer<QLabel> title_label;
    QPointer<QLabel> desc_label;
    QPointer<QLabel> price_label;
    QPointer<QLabel> rating_label;
    QPointer<QPushButton> buy_btn;
    QString button_text;
    bool in_stock = true;

    ~Impl() { detail::delete_if_orphan(frame); }
};

ProductCard::ProductCard(const std::string& title, const std::string& description, const std::string& price,
                         const std::string& badge, double rating, const std::string& button_text)
    : pimpl(std::make_shared<Impl>()) {
    QFrame* frame = pimpl->frame;
    frame->setProperty("sg_role", QStringLiteral("product_card"));
    detail::set_base_style(frame, QStringLiteral(
        "QFrame[sg_role=\"product_card\"] {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1a2230, stop:1 #131822);"
        "  border: 1px solid #2d3748; border-radius: 12px; }"
        "QLabel { border: none; background: transparent; }"));

    auto* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(10);

    auto* top_row = new QHBoxLayout();
    pimpl->badge_label = detail::plain_label("", frame);
    pimpl->badge_label->setStyleSheet(QStringLiteral(
        "QLabel { background: #3b82f6; color: #ffffff; font-size: 11px; font-weight: bold;"
        "  padding: 3px 8px; border-radius: 6px; }"));
    top_row->addWidget(pimpl->badge_label);
    top_row->addStretch();
    pimpl->rating_label = new QLabel(frame);
    pimpl->rating_label->setStyleSheet(QStringLiteral("color: #f59e0b; font-weight: bold; font-size: 12px;"));
    top_row->addWidget(pimpl->rating_label);
    layout->addLayout(top_row);

    pimpl->title_label = detail::plain_label(title, frame);
    pimpl->title_label->setWordWrap(true);
    pimpl->title_label->setStyleSheet(QStringLiteral("color: #f8fafc; font-size: 16px; font-weight: bold;"));
    layout->addWidget(pimpl->title_label);

    pimpl->desc_label = detail::plain_label(description, frame);
    pimpl->desc_label->setWordWrap(true);
    pimpl->desc_label->setStyleSheet(QStringLiteral("color: #94a3b8; font-size: 13px;"));
    layout->addWidget(pimpl->desc_label);
    layout->addStretch();

    auto* bottom_row = new QHBoxLayout();
    bottom_row->setContentsMargins(0, 8, 0, 0);
    pimpl->price_label = detail::plain_label(price, frame);
    pimpl->price_label->setStyleSheet(QStringLiteral("color: #38bdf8; font-size: 20px; font-weight: bold;"));
    bottom_row->addWidget(pimpl->price_label);
    bottom_row->addStretch();

    pimpl->buy_btn = new QPushButton(frame);
    pimpl->buy_btn->setCursor(Qt::PointingHandCursor);
    pimpl->buy_btn->setStyleSheet(QStringLiteral(
        "QPushButton { background: #2563eb; color: white; font-weight: 600; font-size: 13px;"
        "  border: none; border-radius: 8px; padding: 8px 16px; }"
        "QPushButton:hover { background: #1d4ed8; }"
        "QPushButton:pressed { background: #1e40af; }"
        "QPushButton:disabled { background: #374151; color: #6b7280; }"));
    bottom_row->addWidget(pimpl->buy_btn);
    layout->addLayout(bottom_row);

    set_badge(badge);
    set_rating(rating);
    set_button_text(button_text);
}

ProductCard::~ProductCard() = default;

void ProductCard::set_title(const std::string& title) {
    if (pimpl->title_label) pimpl->title_label->setText(detail::qs(title));
}

void ProductCard::set_description(const std::string& description) {
    if (pimpl->desc_label) pimpl->desc_label->setText(detail::qs(description));
}

void ProductCard::set_price(const std::string& price) {
    if (pimpl->price_label) pimpl->price_label->setText(detail::qs(price));
}

void ProductCard::set_badge(const std::string& badge) {
    if (!pimpl->badge_label) return;
    pimpl->badge_label->setText(detail::qs(badge));
    pimpl->badge_label->setVisible(!badge.empty());
}

void ProductCard::set_rating(double rating) {
    if (pimpl->rating_label) pimpl->rating_label->setText(rating_text(rating));
}

void ProductCard::set_button_text(const std::string& text) {
    pimpl->button_text = detail::qs(text);
    if (pimpl->buy_btn && pimpl->in_stock) pimpl->buy_btn->setText(pimpl->button_text);
}

void ProductCard::set_in_stock(bool in_stock) {
    pimpl->in_stock = in_stock;
    if (!pimpl->buy_btn) return;
    pimpl->buy_btn->setEnabled(in_stock);
    pimpl->buy_btn->setText(in_stock ? pimpl->button_text : QStringLiteral("Out of Stock"));
}

EventConnection ProductCard::on_buy(std::function<void()> handler) {
    if (!pimpl->buy_btn || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->buy_btn.data(), &QPushButton::clicked,
                                         [handler = std::move(handler)]() { handler(); }));
}

QWidget* ProductCard::get_qwidget() const { return pimpl->frame.data(); }

}  // namespace simplegui
