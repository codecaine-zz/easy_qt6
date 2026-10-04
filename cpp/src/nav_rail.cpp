#include "simplegui/nav_rail.h"
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPointer>
#include <vector>

namespace simplegui {

struct NavRailItem {
    std::string id;
    std::string icon;
    std::string label;
    int badge_count = 0;
    QPointer<QPushButton> button;
};

struct NavRail::Impl {
    QPointer<QFrame> container;
    QPointer<QVBoxLayout> layout;
    std::vector<NavRailItem> items;
    std::string selected_id;
    std::function<void(const std::string&)> select_handler;

    void update_styles() {
        for (auto& item : items) {
            if (!item.button) continue;
            bool active = (item.id == selected_id);
            if (active) {
                item.button->setStyleSheet(
                    "QPushButton {"
                    "  background: #1e293b;"
                    "  border-left: 3px solid #3b82f6;"
                    "  border-top: none;"
                    "  border-right: none;"
                    "  border-bottom: none;"
                    "  border-radius: 4px;"
                    "  padding: 10px 4px;"
                    "  color: #38bdf8;"
                    "}"
                );
            } else {
                item.button->setStyleSheet(
                    "QPushButton {"
                    "  background: transparent;"
                    "  border: none;"
                    "  border-radius: 4px;"
                    "  padding: 10px 4px;"
                    "  color: #94a3b8;"
                    "}"
                    "QPushButton:hover {"
                    "  background: #1e293b;"
                    "  color: #f1f5f9;"
                    "}"
                );
            }
        }
    }
};

NavRail::NavRail()
    : pimpl(std::make_shared<Impl>())
{
    auto* frame = new QFrame();
    pimpl->container = frame;
    frame->setFixedWidth(72);
    frame->setStyleSheet(
        "QFrame {"
        "  background: #0f172a;"
        "  border-right: 1px solid #1e293b;"
        "}"
    );

    auto* layout = new QVBoxLayout(frame);
    pimpl->layout = layout;
    layout->setContentsMargins(6, 12, 6, 12);
    layout->setSpacing(8);
    layout->addStretch();
}

NavRail::~NavRail() = default;

void NavRail::add_item(const std::string& id, const std::string& icon_glyph, const std::string& label, int badge_count) {
    auto* btn = new QPushButton();
    btn->setCursor(Qt::PointingHandCursor);
    btn->setFixedWidth(60);

    auto* btn_layout = new QVBoxLayout(btn);
    btn_layout->setContentsMargins(2, 4, 2, 4);
    btn_layout->setSpacing(3);

    auto* icon_lbl = new QLabel(QString::fromStdString(icon_glyph), btn);
    icon_lbl->setAlignment(Qt::AlignCenter);
    icon_lbl->setStyleSheet("font-size: 18px; border: none; background: transparent;");
    btn_layout->addWidget(icon_lbl);

    auto* text_lbl = new QLabel(QString::fromStdString(label), btn);
    text_lbl->setAlignment(Qt::AlignCenter);
    text_lbl->setStyleSheet("font-size: 10px; font-weight: 500; border: none; background: transparent;");
    btn_layout->addWidget(text_lbl);

    QObject::connect(btn, &QPushButton::clicked, [this, id]() {
        set_selected(id);
        if (pimpl->select_handler) {
            pimpl->select_handler(id);
        }
    });

    int count = pimpl->layout->count();
    pimpl->layout->insertWidget(count > 0 ? count - 1 : 0, btn);

    NavRailItem item;
    item.id = id;
    item.icon = icon_glyph;
    item.label = label;
    item.badge_count = badge_count;
    item.button = btn;
    pimpl->items.push_back(item);

    if (pimpl->selected_id.empty()) {
        pimpl->selected_id = id;
    }
    pimpl->update_styles();
}

void NavRail::set_selected(const std::string& id) {
    pimpl->selected_id = id;
    pimpl->update_styles();
}

std::string NavRail::selected() const {
    return pimpl->selected_id;
}

EventConnection NavRail::on_select(std::function<void(const std::string& id)> handler) {
    pimpl->select_handler = handler;
    return EventConnection();
}

QWidget* NavRail::get_qwidget() const {
    return pimpl->container.data();
}

}
