#include "simplegui/kanban_board.h"
#include "detail/common.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QScrollArea>
#include <QVBoxLayout>

#include <algorithm>

namespace simplegui {
namespace {

// A card frame that reports left-clicks.
class CardFrame : public QFrame {
public:
    explicit CardFrame(std::function<void()> click) : click_(std::move(click)) {}

protected:
    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && rect().contains(event->position().toPoint()) && click_) click_();
        QFrame::mouseReleaseEvent(event);
    }

private:
    std::function<void()> click_;
};

struct Card {
    std::string id;
    QPointer<QFrame> widget;
};

struct Column {
    std::string id;
    QPointer<QVBoxLayout> card_layout;
    QPointer<QLabel> badge;
    std::vector<Card> cards;
};

}  // namespace

struct KanbanBoard::Impl {
    QPointer<QFrame> main_frame = new QFrame();
    QPointer<QHBoxLayout> columns_layout = new QHBoxLayout(main_frame);
    std::vector<Column> columns;
    detail::Event<const std::string&> card_clicked = detail::make_event<const std::string&>(main_frame);

    ~Impl() { detail::delete_if_orphan(main_frame); }

    Column* column(const std::string& id) {
        auto it = std::find_if(columns.begin(), columns.end(), [&](const Column& c) { return c.id == id; });
        return it == columns.end() ? nullptr : &*it;
    }

    // Finds a card; returns its column and index.
    bool locate(const std::string& card_id, Column*& col, size_t& index) {
        for (auto& c : columns) {
            for (size_t i = 0; i < c.cards.size(); ++i) {
                if (c.cards[i].id == card_id) {
                    col = &c;
                    index = i;
                    return true;
                }
            }
        }
        return false;
    }

    static void insert_card_widget(Column& col, QWidget* w) {
        if (!col.card_layout || !w) return;
        const int count = col.card_layout->count();  // last item is the stretch
        col.card_layout->insertWidget(std::max(0, count - 1), w);
    }

    void update_badges() {
        for (auto& c : columns) {
            if (c.badge) c.badge->setText(QString::number(c.cards.size()));
        }
    }
};

KanbanBoard::KanbanBoard() : pimpl(std::make_shared<Impl>()) {
    pimpl->main_frame->setStyleSheet(QStringLiteral("background: transparent;"));
    pimpl->columns_layout->setContentsMargins(0, 0, 0, 0);
    pimpl->columns_layout->setSpacing(12);
}

KanbanBoard::~KanbanBoard() = default;

bool KanbanBoard::add_column(const std::string& column_id, const std::string& title) {
    if (column_id.empty() || pimpl->column(column_id) || !pimpl->columns_layout) return false;

    auto* col_frame = new QFrame(pimpl->main_frame);
    col_frame->setProperty("sg_role", QStringLiteral("kanban_column"));
    col_frame->setStyleSheet(QStringLiteral(
        "QFrame[sg_role=\"kanban_column\"] { background: #111827; border: 1px solid #1f2937; border-radius: 10px; }"));
    auto* col_vbox = new QVBoxLayout(col_frame);
    col_vbox->setContentsMargins(12, 12, 12, 12);
    col_vbox->setSpacing(10);

    auto* header = new QHBoxLayout();
    QLabel* title_lbl = detail::plain_label(title, col_frame);
    title_lbl->setStyleSheet(QStringLiteral("color: #f1f5f9; font-weight: bold; font-size: 14px; background: transparent;"));
    header->addWidget(title_lbl);
    header->addStretch();
    auto* badge = new QLabel(QStringLiteral("0"), col_frame);
    badge->setStyleSheet(QStringLiteral(
        "QLabel { background: #374151; color: #94a3b8; font-size: 11px; font-weight: bold;"
        "  padding: 2px 7px; border-radius: 9px; }"));
    header->addWidget(badge);
    col_vbox->addLayout(header);

    auto* scroll = new QScrollArea(col_frame);
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setStyleSheet(QStringLiteral("QScrollArea { background: transparent; border: none; }"));
    auto* content = new QWidget();
    content->setStyleSheet(QStringLiteral("background: transparent;"));
    auto* cards_layout = new QVBoxLayout(content);
    cards_layout->setContentsMargins(0, 4, 0, 0);
    cards_layout->setSpacing(8);
    cards_layout->addStretch();
    scroll->setWidget(content);
    col_vbox->addWidget(scroll);

    pimpl->columns_layout->addWidget(col_frame);
    pimpl->columns.push_back({column_id, cards_layout, badge, {}});
    return true;
}

bool KanbanBoard::add_card(const std::string& column_id, const std::string& card_id, const std::string& title,
                           const std::string& tag, const std::string& description) {
    Column* col = pimpl->column(column_id);
    Column* existing = nullptr;
    size_t idx = 0;
    if (!col || card_id.empty() || pimpl->locate(card_id, existing, idx)) return false;

    std::weak_ptr<Impl> weak = pimpl;
    auto* card = new CardFrame([weak, card_id]() {
        if (auto d = weak.lock()) detail::fire(d->card_clicked, card_id);
    });
    card->setCursor(Qt::PointingHandCursor);
    card->setProperty("sg_role", QStringLiteral("kanban_card"));
    card->setStyleSheet(QStringLiteral(
        "QFrame[sg_role=\"kanban_card\"] { background: #1e293b; border: 1px solid #334155; border-radius: 8px; }"
        "QFrame[sg_role=\"kanban_card\"]:hover { border-color: #3b82f6; background: #253349; }"));
    auto* vbox = new QVBoxLayout(card);
    vbox->setContentsMargins(10, 10, 10, 10);
    vbox->setSpacing(5);

    if (!tag.empty()) {
        QLabel* tag_lbl = detail::plain_label(tag, card);
        tag_lbl->setStyleSheet(QStringLiteral("color: #60a5fa; font-size: 10px; font-weight: bold; background: transparent;"));
        vbox->addWidget(tag_lbl);
    }
    QLabel* title_lbl = detail::plain_label(title, card);
    title_lbl->setWordWrap(true);
    title_lbl->setStyleSheet(QStringLiteral("color: #e2e8f0; font-size: 13px; font-weight: 600; background: transparent;"));
    vbox->addWidget(title_lbl);
    if (!description.empty()) {
        QLabel* desc_lbl = detail::plain_label(description, card);
        desc_lbl->setWordWrap(true);
        desc_lbl->setStyleSheet(QStringLiteral("color: #94a3b8; font-size: 11px; background: transparent;"));
        vbox->addWidget(desc_lbl);
    }
    // Let clicks on the labels reach the card.
    for (QLabel* l : card->findChildren<QLabel*>()) l->setAttribute(Qt::WA_TransparentForMouseEvents);

    Impl::insert_card_widget(*col, card);
    col->cards.push_back({card_id, card});
    pimpl->update_badges();
    return true;
}

bool KanbanBoard::move_card(const std::string& card_id, const std::string& target_column_id) {
    Column* target = pimpl->column(target_column_id);
    Column* source = nullptr;
    size_t idx = 0;
    if (!target || !pimpl->locate(card_id, source, idx)) return false;  // check both before touching anything
    if (source == target) return true;
    Card card = source->cards[idx];
    source->cards.erase(source->cards.begin() + static_cast<std::ptrdiff_t>(idx));
    Impl::insert_card_widget(*target, card.widget);
    target->cards.push_back(card);
    pimpl->update_badges();
    return true;
}

bool KanbanBoard::remove_card(const std::string& card_id) {
    Column* col = nullptr;
    size_t idx = 0;
    if (!pimpl->locate(card_id, col, idx)) return false;
    if (col->cards[idx].widget) col->cards[idx].widget->deleteLater();
    col->cards.erase(col->cards.begin() + static_cast<std::ptrdiff_t>(idx));
    pimpl->update_badges();
    return true;
}

void KanbanBoard::clear_column(const std::string& column_id) {
    Column* col = pimpl->column(column_id);
    if (!col) return;
    for (auto& c : col->cards) {
        if (c.widget) c.widget->deleteLater();
    }
    col->cards.clear();
    pimpl->update_badges();
}

std::vector<std::string> KanbanBoard::card_ids(const std::string& column_id) const {
    std::vector<std::string> out;
    if (Column* col = pimpl->column(column_id)) {
        for (const auto& c : col->cards) out.push_back(c.id);
    }
    return out;
}

std::string KanbanBoard::card_column(const std::string& card_id) const {
    Column* col = nullptr;
    size_t idx = 0;
    return pimpl->locate(card_id, col, idx) ? col->id : std::string();
}

EventConnection KanbanBoard::on_card_clicked(std::function<void(const std::string&)> handler) {
    return detail::add_handler(pimpl->card_clicked, std::move(handler));
}

QWidget* KanbanBoard::get_qwidget() const { return pimpl->main_frame.data(); }

}  // namespace simplegui
