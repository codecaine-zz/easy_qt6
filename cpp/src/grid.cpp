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
        qtable->horizontalHeader()->setStretchLastSection(true);
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
