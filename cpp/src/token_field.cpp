#include "simplegui/token_field.h"
#include "detail/common.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#include <algorithm>

namespace simplegui {

struct TokenField::Impl : std::enable_shared_from_this<TokenField::Impl> {
    struct Token {
        std::string text;
        QPointer<QFrame> chip;
    };

    QPointer<QFrame> container = new QFrame();
    QPointer<QHBoxLayout> layout = new QHBoxLayout(container);
    QPointer<QLineEdit> input = new QLineEdit(container);
    std::vector<Token> items;
    detail::Event<const std::vector<std::string>&> changed =
        detail::make_event<const std::vector<std::string>&>(container);

    ~Impl() { detail::delete_if_orphan(container); }

    std::vector<std::string> texts() const {
        std::vector<std::string> out;
        out.reserve(items.size());
        for (const auto& t : items) out.push_back(t.text);
        return out;
    }

    void notify() { detail::fire(changed, texts()); }

    bool contains(const std::string& token) const {
        return std::any_of(items.begin(), items.end(), [&](const Token& t) { return t.text == token; });
    }

    bool add(const std::string& token) {
        if (token.empty() || contains(token) || !layout) return false;
        auto* chip = new QFrame(container);
        chip->setProperty("sg_role", QStringLiteral("chip"));
        chip->setStyleSheet(QStringLiteral("QFrame[sg_role=\"chip\"] { background: #3b82f6; border-radius: 5px; }"));
        auto* chip_layout = new QHBoxLayout(chip);
        chip_layout->setContentsMargins(6, 2, 4, 2);
        chip_layout->setSpacing(4);

        QLabel* label = detail::plain_label(token, chip);
        label->setStyleSheet(QStringLiteral("color: #ffffff; font-size: 12px; font-weight: 500; background: transparent;"));
        chip_layout->addWidget(label);

        auto* close_btn = new QPushButton(QString(QChar(0x2715)), chip);  // heavy multiplication X
        close_btn->setFixedSize(14, 14);
        close_btn->setCursor(Qt::PointingHandCursor);
        close_btn->setToolTip(QStringLiteral("Remove"));
        close_btn->setStyleSheet(QStringLiteral(
            "QPushButton { color: #bfdbfe; font-size: 10px; font-weight: bold; border: none; background: transparent; }"
            "QPushButton:hover { color: #ffffff; }"));
        std::weak_ptr<Impl> weak = weak_from_this();
        QObject::connect(close_btn, &QPushButton::clicked, [weak, token]() {
            if (auto d = weak.lock()) {
                if (d->remove(token)) d->notify();
            }
        });
        chip_layout->addWidget(close_btn);

        layout->insertWidget(std::max(0, layout->indexOf(input)), chip);  // chips go before the text box
        items.push_back({token, chip});
        return true;
    }

    bool remove(const std::string& token) {
        auto it = std::find_if(items.begin(), items.end(), [&](const Token& t) { return t.text == token; });
        if (it == items.end()) return false;
        if (it->chip) it->chip->deleteLater();  // deferred: we may be inside the chip's own click handler
        items.erase(it);
        return true;
    }

    void remove_all() {
        for (auto& t : items) {
            if (t.chip) t.chip->deleteLater();
        }
        items.clear();
    }
};

TokenField::TokenField(const std::string& placeholder) : pimpl(std::make_shared<Impl>()) {
    pimpl->container->setProperty("sg_role", QStringLiteral("token_field"));
    detail::set_base_style(pimpl->container, QStringLiteral(
        "QFrame[sg_role=\"token_field\"] { background: #1e293b; border: 1px solid #334155; border-radius: 8px; }"));
    pimpl->layout->setContentsMargins(6, 4, 6, 4);
    pimpl->layout->setSpacing(6);
    pimpl->input->setPlaceholderText(detail::qs(placeholder));
    pimpl->input->setStyleSheet(QStringLiteral("border: none; background: transparent; color: #f8fafc; font-size: 13px;"));
    pimpl->layout->addWidget(pimpl->input);

    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->input.data(), &QLineEdit::returnPressed, [weak]() {
        auto d = weak.lock();
        if (!d || !d->input) return;
        const std::string text = detail::ss(d->input->text().trimmed());
        d->input->clear();
        if (d->add(text)) d->notify();
    });
}

TokenField::~TokenField() = default;

void TokenField::add_token(const std::string& token) {
    if (pimpl->add(token)) pimpl->notify();
}

void TokenField::remove_token(const std::string& token) {
    if (pimpl->remove(token)) pimpl->notify();
}

void TokenField::clear_tokens() {
    if (pimpl->items.empty()) return;
    pimpl->remove_all();
    pimpl->notify();
}

void TokenField::set_tokens(const std::vector<std::string>& tokens) {
    pimpl->remove_all();
    for (const auto& t : tokens) pimpl->add(t);
    pimpl->notify();
}

std::vector<std::string> TokenField::tokens() const { return pimpl->texts(); }
bool TokenField::has_token(const std::string& token) const { return pimpl->contains(token); }

void TokenField::set_placeholder(const std::string& placeholder) {
    if (pimpl->input) pimpl->input->setPlaceholderText(detail::qs(placeholder));
}

EventConnection TokenField::on_tokens_changed(std::function<void(const std::vector<std::string>&)> handler) {
    return detail::add_handler(pimpl->changed, std::move(handler));
}

QWidget* TokenField::get_qwidget() const { return pimpl->container.data(); }

}  // namespace simplegui
