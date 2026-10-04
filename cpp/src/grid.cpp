#include "simplegui/grid.h"
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>
#include <QPointer>

namespace simplegui {

struct Grid::Impl {
    QPointer<QTableWidget> qtable;

    Impl(int rows, int cols, const std::vector<std::string>& headers) {
        qtable = new QTableWidget(rows, cols);
        QStringList qt_headers;
        for (const auto& h : headers) {
            qt_headers << QString::fromStdString(h);
        }
        qtable->setHorizontalHeaderLabels(qt_headers);
        qtable->verticalHeader()->setVisible(false);
        qtable->verticalHeader()->setDefaultSectionSize(30);
        qtable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        qtable->setShowGrid(false);
        qtable->setSelectionBehavior(QAbstractItemView::SelectRows);
        qtable->setAlternatingRowColors(true);
        qtable->setMinimumHeight(rows * 30 + 38);
        qtable->setStyleSheet(
            "QTableWidget {"
            "  background-color: #12141a;"
            "  alternate-background-color: #181b22;"
            "  border: 1px solid #282c37;"
            "  border-radius: 8px;"
            "  color: #f1f5f9;"
            "  gridline-color: transparent;"
            "}"
            "QHeaderView::section {"
            "  background-color: #1a1d26;"
            "  color: #8b92a5;"
            "  font-weight: 600;"
            "  font-size: 11px;"
            "  padding: 6px 12px;"
            "  border: none;"
            "  border-bottom: 1px solid #282c37;"
            "  text-transform: uppercase;"
            "}"
            "QTableWidget::item {"
            "  padding: 6px 12px;"
            "  border: none;"
            "}"
        );
    }
    ~Impl() { if (qtable && !qtable->parent()) delete qtable; }
};

Grid::Grid(int rows, int cols, const std::vector<std::string>& headers)
    : pimpl(std::make_shared<Impl>(rows, cols, headers)) {}

Grid::~Grid() = default;

void Grid::set_cell(int row, int col, const std::string& text) {
    if (pimpl->qtable) {
        auto item = new QTableWidgetItem(QString::fromStdString(text));
        pimpl->qtable->setItem(row, col, item);
    }
}

std::string Grid::get_cell(int row, int col) const {
    if (pimpl->qtable) {
        auto item = pimpl->qtable->item(row, col);
        if (item) return item->text().toStdString();
    }
    return "";
}

QWidget* Grid::get_qwidget() const {
    return pimpl->qtable.data();
}

}
