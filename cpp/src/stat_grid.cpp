#include "simplegui/stat_grid.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPointer>
#include <QString>

namespace simplegui {

struct StatGrid::Impl {
    QPointer<QWidget> container;
    QHBoxLayout* layout;

    Impl() {
        container = new QWidget();
        layout = new QHBoxLayout(container);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(10);
    }
    ~Impl() { if (container && !container->parent()) delete container; }

    void add_card(const StatItem& item) {
        QFrame* card = new QFrame();
        card->setStyleSheet(
            "QFrame {"
            "  background-color: #1a1a1e;"
            "  border: 1px solid #27272a;"
            "  border-radius: 8px;"
            "  padding: 10px;"
            "}"
        );
        QVBoxLayout* l = new QVBoxLayout(card);
        l->setContentsMargins(10, 8, 10, 8);
        l->setSpacing(4);

        QLabel* t = new QLabel(QString::fromStdString(item.title));
        t->setStyleSheet("color: #a1a1aa; font-size: 11px; font-weight: 600; text-transform: uppercase; border: none; background: transparent;");

        QLabel* v = new QLabel(QString::fromStdString(item.value));
        v->setStyleSheet("color: #f4f4f5; font-size: 20px; font-weight: bold; border: none; background: transparent;");

        QLabel* tr = new QLabel(QString::fromStdString(item.trend));
        QString tr_col = item.is_positive ? "#10b981" : "#ef4444";
        tr->setStyleSheet(QString("color: %1; font-size: 11px; font-weight: 600; border: none; background: transparent;").arg(tr_col));

        l->addWidget(t);
        l->addWidget(v);
        l->addWidget(tr);

        layout->addWidget(card);
    }
};

StatGrid::StatGrid() : pimpl(std::make_shared<Impl>()) {}

StatGrid::~StatGrid() = default;

void StatGrid::add_stat(const std::string& title, const std::string& value, const std::string& trend, bool is_positive) {
    pimpl->add_card({title, value, trend, is_positive});
}

void StatGrid::clear() {
    QLayoutItem* item;
    while ((item = pimpl->layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
}

QWidget* StatGrid::get_qwidget() const {
    return pimpl->container.data();
}

}
