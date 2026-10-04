#include "simplegui/product_card.h"
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QPointer>

namespace simplegui {

struct ProductCard::Impl {
    QPointer<QFrame> frame;
    QPointer<QLabel> badge_label;
    QPointer<QLabel> title_label;
    QPointer<QLabel> desc_label;
    QPointer<QLabel> price_label;
    QPointer<QLabel> rating_label;
    QPointer<QPushButton> buy_btn;
};

ProductCard::ProductCard(const std::string& title,
                         const std::string& description,
                         const std::string& price,
                         const std::string& badge,
                         double rating,
                         const std::string& button_text)
    : pimpl(std::make_shared<Impl>())
{
    auto* frame = new QFrame();
    pimpl->frame = frame;
    frame->setObjectName("product_card_frame");
    frame->setStyleSheet(
        "QFrame#product_card_frame {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #1a2230, stop:1 #131822);"
        "  border: 1px solid #2d3748;"
        "  border-radius: 12px;"
        "}"
        "QLabel { border: none; background: transparent; }"
    );

    auto* layout = new QVBoxLayout(frame);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(10);

    // Top row: Badge and Rating
    auto* top_row = new QHBoxLayout();
    top_row->setContentsMargins(0, 0, 0, 0);

    auto* badge_lbl = new QLabel(QString::fromStdString(badge), frame);
    pimpl->badge_label = badge_lbl;
    badge_lbl->setStyleSheet(
        "QLabel {"
        "  background: #3b82f6;"
        "  color: #ffffff;"
        "  font-size: 11px;"
        "  font-weight: bold;"
        "  padding: 3px 8px;"
        "  border-radius: 6px;"
        "}"
    );
    if (badge.empty()) {
        badge_lbl->setVisible(false);
    }
    top_row->addWidget(badge_lbl);
    top_row->addStretch();

    char rating_buf[32];
    snprintf(rating_buf, sizeof(rating_buf), "★ %.1f", rating);
    auto* rating_lbl = new QLabel(QString::fromUtf8(rating_buf), frame);
    pimpl->rating_label = rating_lbl;
    rating_lbl->setStyleSheet("color: #f59e0b; font-weight: bold; font-size: 12px;");
    top_row->addWidget(rating_lbl);
    layout->addLayout(top_row);

    // Title
    auto* title_lbl = new QLabel(QString::fromStdString(title), frame);
    pimpl->title_label = title_lbl;
    title_lbl->setWordWrap(true);
    title_lbl->setStyleSheet("color: #f8fafc; font-size: 16px; font-weight: bold;");
    layout->addWidget(title_lbl);

    // Description
    auto* desc_lbl = new QLabel(QString::fromStdString(description), frame);
    pimpl->desc_label = desc_lbl;
    desc_lbl->setWordWrap(true);
    desc_lbl->setStyleSheet("color: #94a3b8; font-size: 13px; line-height: 1.4;");
    layout->addWidget(desc_lbl);

    layout->addStretch();

    // Bottom row: Price and Buy Button
    auto* bottom_row = new QHBoxLayout();
    bottom_row->setContentsMargins(0, 8, 0, 0);

    auto* price_lbl = new QLabel(QString::fromStdString(price), frame);
    pimpl->price_label = price_lbl;
    price_lbl->setStyleSheet("color: #38bdf8; font-size: 20px; font-weight: bold;");
    bottom_row->addWidget(price_lbl);

    bottom_row->addStretch();

    auto* buy_btn = new QPushButton(QString::fromStdString(button_text), frame);
    pimpl->buy_btn = buy_btn;
    buy_btn->setCursor(Qt::PointingHandCursor);
    buy_btn->setStyleSheet(
        "QPushButton {"
        "  background: #2563eb;"
        "  color: white;"
        "  font-weight: 600;"
        "  font-size: 13px;"
        "  border: none;"
        "  border-radius: 8px;"
        "  padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "  background: #1d4ed8;"
        "}"
        "QPushButton:pressed {"
        "  background: #1e40af;"
        "}"
        "QPushButton:disabled {"
        "  background: #374151;"
        "  color: #6b7280;"
        "}"
    );
    bottom_row->addWidget(buy_btn);

    layout->addLayout(bottom_row);
}

ProductCard::~ProductCard() = default;

void ProductCard::set_price(const std::string& price) {
    if (pimpl->price_label) {
        pimpl->price_label->setText(QString::fromStdString(price));
    }
}

void ProductCard::set_badge(const std::string& badge) {
    if (pimpl->badge_label) {
        pimpl->badge_label->setText(QString::fromStdString(badge));
        pimpl->badge_label->setVisible(!badge.empty());
    }
}

void ProductCard::set_rating(double rating) {
    if (pimpl->rating_label) {
        char rating_buf[32];
        snprintf(rating_buf, sizeof(rating_buf), "★ %.1f", rating);
        pimpl->rating_label->setText(QString::fromUtf8(rating_buf));
    }
}

void ProductCard::set_in_stock(bool in_stock) {
    if (pimpl->buy_btn) {
        pimpl->buy_btn->setEnabled(in_stock);
        if (!in_stock) {
            pimpl->buy_btn->setText("Out of Stock");
        }
    }
}

EventConnection ProductCard::on_buy(std::function<void()> handler) {
    if (!pimpl->buy_btn) return {};
    auto conn = QObject::connect(pimpl->buy_btn, &QPushButton::clicked, [handler]() {
        if (handler) handler();
    });
    return EventConnection([conn]() { QObject::disconnect(conn); });
}

QWidget* ProductCard::get_qwidget() const {
    return pimpl->frame.data();
}

}
