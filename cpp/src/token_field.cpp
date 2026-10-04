#include "simplegui/token_field.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <algorithm>

namespace simplegui {

struct TokenItem {
    std::string text;
    QPointer<QFrame> chip;
};

struct TokenField::Impl {
    QPointer<QFrame> container;
    QPointer<QHBoxLayout> chips_layout;
    QPointer<QLineEdit> input;
    std::vector<TokenItem> tokens;
    std::function<void(const std::vector<std::string>&)> change_handler;

    void notify() {
        if (change_handler) {
            std::vector<std::string> res;
            for (const auto& t : tokens) {
                res.push_back(t.text);
            }
            change_handler(res);
        }
    }
};

TokenField::TokenField(const std::string& placeholder)
    : pimpl(std::make_shared<Impl>())
{
    auto* frame = new QFrame();
    pimpl->container = frame;
    frame->setStyleSheet(
        "QFrame {"
        "  background: #1e293b;"
        "  border: 1px solid #334155;"
        "  border-radius: 8px;"
        "  padding: 3px;"
        "}"
    );

    auto* layout = new QHBoxLayout(frame);
    pimpl->chips_layout = layout;
    layout->setContentsMargins(6, 4, 6, 4);
    layout->setSpacing(6);

    auto* edit = new QLineEdit(frame);
    pimpl->input = edit;
    edit->setPlaceholderText(QString::fromStdString(placeholder));
    edit->setStyleSheet("border: none; background: transparent; color: #f8fafc; font-size: 13px;");
    layout->addWidget(edit);

    QObject::connect(edit, &QLineEdit::returnPressed, [this, edit]() {
        QString txt = edit->text().trimmed();
        if (!txt.isEmpty()) {
            add_token(txt.toStdString());
            edit->clear();
        }
    });
}

TokenField::~TokenField() = default;

void TokenField::add_token(const std::string& token) {
    if (token.empty()) return;
    for (const auto& t : pimpl->tokens) {
        if (t.text == token) return; // avoid duplicates
    }

    auto* chip = new QFrame(pimpl->container);
    chip->setStyleSheet(
        "QFrame {"
        "  background: #3b82f6;"
        "  border-radius: 5px;"
        "  padding: 2px 6px;"
        "}"
    );

    auto* chip_layout = new QHBoxLayout(chip);
    chip_layout->setContentsMargins(4, 2, 4, 2);
    chip_layout->setSpacing(4);

    auto* lbl = new QLabel(QString::fromStdString(token), chip);
    lbl->setStyleSheet("color: #ffffff; font-size: 12px; font-weight: 500; border: none; background: transparent;");
    chip_layout->addWidget(lbl);

    auto* close_btn = new QPushButton(QString::fromUtf8("✕"), chip);
    close_btn->setFixedSize(14, 14);
    close_btn->setCursor(Qt::PointingHandCursor);
    close_btn->setStyleSheet(
        "QPushButton {"
        "  color: #bfdbfe;"
        "  font-size: 10px;"
        "  font-weight: bold;"
        "  border: none;"
        "  background: transparent;"
        "}"
        "QPushButton:hover {"
        "  color: #ffffff;"
        "}"
    );

    QObject::connect(close_btn, &QPushButton::clicked, [this, token]() {
        remove_token(token);
    });

    chip_layout->addWidget(close_btn);

    // Insert before the line edit
    int insert_idx = pimpl->chips_layout->count() - 1;
    pimpl->chips_layout->insertWidget(insert_idx >= 0 ? insert_idx : 0, chip);

    TokenItem item;
    item.text = token;
    item.chip = chip;
    pimpl->tokens.push_back(item);

    pimpl->notify();
}

void TokenField::remove_token(const std::string& token) {
    auto it = std::find_if(pimpl->tokens.begin(), pimpl->tokens.end(), [&](const TokenItem& item) {
        return item.text == token;
    });

    if (it != pimpl->tokens.end()) {
        if (it->chip) {
            it->chip->deleteLater();
        }
        pimpl->tokens.erase(it);
        pimpl->notify();
    }
}

void TokenField::clear_tokens() {
    for (auto& item : pimpl->tokens) {
        if (item.chip) {
            item.chip->deleteLater();
        }
    }
    pimpl->tokens.clear();
    pimpl->notify();
}

std::vector<std::string> TokenField::tokens() const {
    std::vector<std::string> res;
    for (const auto& t : pimpl->tokens) {
        res.push_back(t.text);
    }
    return res;
}

EventConnection TokenField::on_tokens_changed(std::function<void(const std::vector<std::string>& tokens)> handler) {
    pimpl->change_handler = handler;
    return EventConnection();
}

QWidget* TokenField::get_qwidget() const {
    return pimpl->container.data();
}

}
