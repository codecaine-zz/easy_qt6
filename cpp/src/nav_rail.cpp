#include "simplegui/nav_rail.h"
#include "detail/common.h"

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <algorithm>
#include <vector>

namespace simplegui {

struct NavRail::Impl {
    struct Item {
        std::string id;
        QPointer<QPushButton> button;
        QPointer<QLabel> badge;
    };

    QPointer<QFrame> container = new QFrame();
    QPointer<QVBoxLayout> layout = new QVBoxLayout(container);
    std::vector<Item> items;
    std::string selected_id;
    detail::Event<const std::string&> selected = detail::make_event<const std::string&>(container);

    ~Impl() { detail::delete_if_orphan(container); }

    Item* find(const std::string& id) {
        auto it = std::find_if(items.begin(), items.end(), [&](const Item& i) { return i.id == id; });
        return it == items.end() ? nullptr : &*it;
    }

    void update_styles() {
        for (auto& item : items) {
            if (!item.button) continue;
            item.button->setStyleSheet(item.id == selected_id
                ? QStringLiteral("QPushButton { background: #1e293b; border: none; border-left: 3px solid #3b82f6;"
                                 "  border-radius: 4px; color: #38bdf8; }")
                : QStringLiteral("QPushButton { background: transparent; border: none; border-radius: 4px; color: #94a3b8; }"
                                 "QPushButton:hover { background: #1e293b; color: #f1f5f9; }"));
        }
    }

    static void show_badge(QLabel* badge, int count) {
        if (!badge) return;
        badge->setVisible(count > 0);
        badge->setText(count > 99 ? QStringLiteral("99+") : QString::number(count));
        badge->adjustSize();
        badge->move(60 - badge->width() - 2, 2);
    }
};

NavRail::NavRail() : pimpl(std::make_shared<Impl>()) {
    QFrame* frame = pimpl->container;
    frame->setFixedWidth(72);
    frame->setProperty("sg_role", QStringLiteral("nav_rail"));
    detail::set_base_style(frame, QStringLiteral(
        "QFrame[sg_role=\"nav_rail\"] { background: #0f172a; border-right: 1px solid #1e293b; }"));
    pimpl->layout->setContentsMargins(6, 12, 6, 12);
    pimpl->layout->setSpacing(8);
    pimpl->layout->addStretch();
}

NavRail::~NavRail() = default;

bool NavRail::add_item(const std::string& id, const std::string& icon_glyph, const std::string& label, int badge_count) {
    if (id.empty() || pimpl->find(id) || !pimpl->layout) return false;

    auto* btn = new QPushButton(pimpl->container);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFixedSize(60, 56);
    btn->setToolTip(detail::qs(label));

    auto* btn_layout = new QVBoxLayout(btn);
    btn_layout->setContentsMargins(2, 6, 2, 6);
    btn_layout->setSpacing(3);
    QLabel* icon = detail::plain_label(icon_glyph, btn);
    icon->setAlignment(Qt::AlignCenter);
    icon->setStyleSheet(QStringLiteral("font-size: 18px; background: transparent;"));
    QLabel* text = detail::plain_label(label, btn);
    text->setAlignment(Qt::AlignCenter);
    text->setStyleSheet(QStringLiteral("font-size: 10px; font-weight: 500; background: transparent;"));
    btn_layout->addWidget(icon);
    btn_layout->addWidget(text);
    icon->setAttribute(Qt::WA_TransparentForMouseEvents);
    text->setAttribute(Qt::WA_TransparentForMouseEvents);

    auto* badge = new QLabel(btn);  // floats over the top-right corner, outside the layout
    badge->setAlignment(Qt::AlignCenter);
    badge->setMinimumWidth(16);
    badge->setStyleSheet(QStringLiteral(
        "background: #ef4444; color: white; font-size: 9px; font-weight: bold; border-radius: 7px; padding: 1px 4px;"));
    badge->setAttribute(Qt::WA_TransparentForMouseEvents);
    Impl::show_badge(badge, badge_count);

    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(btn, &QPushButton::clicked, [weak, id]() {
        auto d = weak.lock();
        if (!d) return;
        d->selected_id = id;
        d->update_styles();
        detail::fire(d->selected, id);
    });

    pimpl->layout->insertWidget(std::max(0, pimpl->layout->count() - 1), btn);  // before the stretch
    pimpl->items.push_back({id, btn, badge});
    if (pimpl->selected_id.empty()) pimpl->selected_id = id;
    pimpl->update_styles();
    return true;
}

void NavRail::set_badge(const std::string& id, int badge_count) {
    if (auto* item = pimpl->find(id)) Impl::show_badge(item->badge, badge_count);
}

void NavRail::set_selected(const std::string& id) {
    if (!pimpl->find(id)) return;
    pimpl->selected_id = id;
    pimpl->update_styles();
}

std::string NavRail::selected() const { return pimpl->selected_id; }

EventConnection NavRail::on_select(std::function<void(const std::string&)> handler) {
    return detail::add_handler(pimpl->selected, std::move(handler));
}

QWidget* NavRail::get_qwidget() const { return pimpl->container.data(); }

}  // namespace simplegui
