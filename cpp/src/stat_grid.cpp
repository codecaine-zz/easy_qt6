#include "simplegui/stat_grid.h"
#include "detail/common.h"

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace simplegui {

struct StatGrid::Impl {
    QPointer<QWidget> container = new QWidget();
    QPointer<QHBoxLayout> layout = new QHBoxLayout(container);

    Impl() {
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(10);
    }
    ~Impl() { detail::delete_if_orphan(container); }

    void add_card(const StatItem& item) {
        if (!layout) return;
        auto* card = new QFrame(container);
        card->setProperty("sg_role", QStringLiteral("stat_tile"));
        card->setStyleSheet(QStringLiteral(
            "QFrame[sg_role=\"stat_tile\"] { background-color: #1a1a1e; border: 1px solid #27272a; border-radius: 8px; }"));
        auto* l = new QVBoxLayout(card);
        l->setContentsMargins(14, 10, 14, 10);
        l->setSpacing(4);

        QLabel* title = detail::plain_label("", card);
        title->setText(detail::qs(item.title).toUpper());
        title->setStyleSheet(QStringLiteral("color: #a1a1aa; font-size: 11px; font-weight: 600; background: transparent;"));
        QLabel* value = detail::plain_label(item.value, card);
        value->setStyleSheet(QStringLiteral("color: #f4f4f5; font-size: 20px; font-weight: bold; background: transparent;"));
        QLabel* trend = detail::plain_label(item.trend, card);
        trend->setStyleSheet(item.is_positive
            ? QStringLiteral("color: #10b981; font-size: 11px; font-weight: 600; background: transparent;")
            : QStringLiteral("color: #ef4444; font-size: 11px; font-weight: 600; background: transparent;"));

        l->addWidget(title);
        l->addWidget(value);
        l->addWidget(trend);
        layout->addWidget(card);
    }

    void clear() {
        if (!layout) return;
        while (QLayoutItem* item = layout->takeAt(0)) {
            if (QWidget* w = item->widget()) w->deleteLater();
            delete item;
        }
    }
};

StatGrid::StatGrid() : pimpl(std::make_shared<Impl>()) {}
StatGrid::~StatGrid() = default;

void StatGrid::add_stat(const std::string& title, const std::string& value, const std::string& trend, bool is_positive) {
    pimpl->add_card({title, value, trend, is_positive});
}

void StatGrid::set_stats(const std::vector<StatItem>& stats) {
    pimpl->clear();
    for (const auto& s : stats) pimpl->add_card(s);
}

void StatGrid::clear() { pimpl->clear(); }

int StatGrid::count() const { return pimpl->layout ? pimpl->layout->count() : 0; }

QWidget* StatGrid::get_qwidget() const { return pimpl->container.data(); }

}  // namespace simplegui
