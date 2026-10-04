#include "simplegui/kanban_board.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QPointer>
#include <unordered_map>
#include <vector>

namespace simplegui {

struct KanbanCardData {
    std::string card_id;
    std::string title;
    std::string tag;
    std::string description;
    QPointer<QFrame> widget;
};

struct KanbanColumnData {
    std::string column_id;
    std::string title;
    QPointer<QFrame> container;
    QPointer<QVBoxLayout> card_layout;
    QPointer<QLabel> badge_label;
    std::vector<KanbanCardData> cards;
};

struct KanbanBoard::Impl {
    QPointer<QFrame> main_frame;
    QPointer<QHBoxLayout> columns_layout;
    std::vector<KanbanColumnData> columns;
    std::function<void(const std::string&)> card_click_handler;

    void update_badges() {
        for (auto& col : columns) {
            if (col.badge_label) {
                col.badge_label->setText(QString::number(col.cards.size()));
            }
        }
    }
};

KanbanBoard::KanbanBoard()
    : pimpl(std::make_shared<Impl>())
{
    auto* frame = new QFrame();
    pimpl->main_frame = frame;
    frame->setStyleSheet("background: transparent;");

    auto* layout = new QHBoxLayout(frame);
    pimpl->columns_layout = layout;
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(12);
}

KanbanBoard::~KanbanBoard() = default;

void KanbanBoard::add_column(const std::string& column_id, const std::string& title) {
    auto* col_frame = new QFrame(pimpl->main_frame);
    col_frame->setStyleSheet(
        "QFrame {"
        "  background: #111827;"
        "  border: 1px solid #1f2937;"
        "  border-radius: 10px;"
        "}"
    );

    auto* col_vbox = new QVBoxLayout(col_frame);
    col_vbox->setContentsMargins(12, 12, 12, 12);
    col_vbox->setSpacing(10);

    // Column header
    auto* header_row = new QHBoxLayout();
    header_row->setContentsMargins(0, 0, 0, 0);

    auto* title_lbl = new QLabel(QString::fromStdString(title), col_frame);
    title_lbl->setStyleSheet("color: #f1f5f9; font-weight: bold; font-size: 14px; border: none; background: transparent;");
    header_row->addWidget(title_lbl);

    header_row->addStretch();

    auto* badge_lbl = new QLabel("0", col_frame);
    badge_lbl->setStyleSheet(
        "QLabel {"
        "  background: #374151;"
        "  color: #94a3b8;"
        "  font-size: 11px;"
        "  font-weight: bold;"
        "  padding: 2px 7px;"
        "  border-radius: 9px;"
        "  border: none;"
        "}"
    );
    header_row->addWidget(badge_lbl);
    col_vbox->addLayout(header_row);

    // Card container area
    auto* scroll = new QScrollArea(col_frame);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setStyleSheet("background: transparent; border: none;");

    auto* scroll_content = new QWidget();
    scroll_content->setStyleSheet("background: transparent;");
    auto* cards_layout = new QVBoxLayout(scroll_content);
    cards_layout->setContentsMargins(0, 4, 0, 0);
    cards_layout->setSpacing(8);
    cards_layout->addStretch();

    scroll->setWidget(scroll_content);
    col_vbox->addWidget(scroll);

    pimpl->columns_layout->addWidget(col_frame);

    KanbanColumnData data;
    data.column_id = column_id;
    data.title = title;
    data.container = col_frame;
    data.card_layout = cards_layout;
    data.badge_label = badge_lbl;
    pimpl->columns.push_back(std::move(data));
}

void KanbanBoard::add_card(const std::string& column_id,
                           const std::string& card_id,
                           const std::string& title,
                           const std::string& tag,
                           const std::string& description)
{
    for (auto& col : pimpl->columns) {
        if (col.column_id == column_id) {
            auto* card_frame = new QFrame();
            card_frame->setCursor(Qt::PointingHandCursor);
            card_frame->setStyleSheet(
                "QFrame {"
                "  background: #1e293b;"
                "  border: 1px solid #334155;"
                "  border-radius: 8px;"
                "}"
                "QFrame:hover {"
                "  border-color: #3b82f6;"
                "  background: #253349;"
                "}"
            );

            auto* card_vbox = new QVBoxLayout(card_frame);
            card_vbox->setContentsMargins(10, 10, 10, 10);
            card_vbox->setSpacing(5);

            if (!tag.empty()) {
                auto* tag_lbl = new QLabel(QString::fromStdString(tag), card_frame);
                tag_lbl->setStyleSheet("color: #60a5fa; font-size: 10px; font-weight: bold; text-transform: uppercase; border: none; background: transparent;");
                card_vbox->addWidget(tag_lbl);
            }

            auto* title_lbl = new QLabel(QString::fromStdString(title), card_frame);
            title_lbl->setWordWrap(true);
            title_lbl->setStyleSheet("color: #e2e8f0; font-size: 13px; font-weight: 600; border: none; background: transparent;");
            card_vbox->addWidget(title_lbl);

            if (!description.empty()) {
                auto* desc_lbl = new QLabel(QString::fromStdString(description), card_frame);
                desc_lbl->setWordWrap(true);
                desc_lbl->setStyleSheet("color: #94a3b8; font-size: 11px; border: none; background: transparent;");
                card_vbox->addWidget(desc_lbl);
            }

            // Insert before stretch
            int count = col.card_layout->count();
            col.card_layout->insertWidget(count > 0 ? count - 1 : 0, card_frame);

            KanbanCardData cdata;
            cdata.card_id = card_id;
            cdata.title = title;
            cdata.tag = tag;
            cdata.description = description;
            cdata.widget = card_frame;
            col.cards.push_back(std::move(cdata));

            pimpl->update_badges();
            break;
        }
    }
}

void KanbanBoard::move_card(const std::string& card_id, const std::string& target_column_id) {
    KanbanCardData found_card;
    bool found = false;

    for (auto& col : pimpl->columns) {
        for (auto it = col.cards.begin(); it != col.cards.end(); ++it) {
            if (it->card_id == card_id) {
                found_card = *it;
                col.cards.erase(it);
                found = true;
                break;
            }
        }
        if (found) break;
    }

    if (!found) return;

    for (auto& col : pimpl->columns) {
        if (col.column_id == target_column_id) {
            if (found_card.widget) {
                int count = col.card_layout->count();
                col.card_layout->insertWidget(count > 0 ? count - 1 : 0, found_card.widget);
            }
            col.cards.push_back(std::move(found_card));
            break;
        }
    }

    pimpl->update_badges();
}

void KanbanBoard::clear_column(const std::string& column_id) {
    for (auto& col : pimpl->columns) {
        if (col.column_id == column_id) {
            for (auto& c : col.cards) {
                if (c.widget) {
                    c.widget->deleteLater();
                }
            }
            col.cards.clear();
            break;
        }
    }
    pimpl->update_badges();
}

EventConnection KanbanBoard::on_card_clicked(std::function<void(const std::string& card_id)> handler) {
    pimpl->card_click_handler = handler;
    return EventConnection();
}

QWidget* KanbanBoard::get_qwidget() const {
    return pimpl->main_frame.data();
}

}
