#include "simplegui/grid.h"
#include "detail/common.h"

#include <QHeaderView>
#include <QSignalBlocker>
#include <QStringList>
#include <QTableWidget>

#include <algorithm>

namespace simplegui {

struct Grid::Impl {
    QPointer<QTableWidget> table;
    detail::Event<int> selected;
    detail::Event<int, int, const std::string&> cell_changed;

    Impl(int rows, int cols, const std::vector<std::string>& headers)
        : table(new QTableWidget(std::max(0, rows), std::max(1, cols))) {
        selected = detail::make_event<int>(table);
        cell_changed = detail::make_event<int, int, const std::string&>(table);
        set_headers(headers);
        table->verticalHeader()->setVisible(false);
        table->verticalHeader()->setDefaultSectionSize(30);
        table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        table->setShowGrid(false);
        table->setSelectionBehavior(QAbstractItemView::SelectRows);
        table->setSelectionMode(QAbstractItemView::SingleSelection);
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);
        table->setAlternatingRowColors(true);
        table->setMinimumHeight(std::min(std::max(0, rows), 12) * 30 + 38);
        detail::set_base_style(table, QStringLiteral(
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
            "}"
            "QTableWidget::item {"
            "  padding: 6px 12px;"
            "  border: none;"
            "}"));
    }
    ~Impl() { detail::delete_if_orphan(table); }

    bool in_range(int row, int col) const {
        return table && row >= 0 && col >= 0 && row < table->rowCount() && col < table->columnCount();
    }

    void set_headers(const std::vector<std::string>& headers) {
        if (!table) return;
        if (!headers.empty() && static_cast<int>(headers.size()) > table->columnCount()) {
            table->setColumnCount(static_cast<int>(headers.size()));
        }
        QStringList labels;
        for (const auto& h : headers) labels << detail::qs(h);
        table->setHorizontalHeaderLabels(labels);
    }

    // Writes a cell without firing on_cell_changed (that event is for user edits only).
    void write(int row, int col, const std::string& text) {
        if (!in_range(row, col)) return;
        const QSignalBlocker block(table);
        if (QTableWidgetItem* item = table->item(row, col)) {
            item->setText(detail::qs(text));
        } else {
            table->setItem(row, col, new QTableWidgetItem(detail::qs(text)));
        }
    }
};

Grid::Grid(int rows, int cols, const std::vector<std::string>& headers)
    : pimpl(std::make_shared<Impl>(rows, cols, headers)) {
    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->table.data(), &QTableWidget::itemSelectionChanged, [weak]() {
        auto d = weak.lock();
        if (!d || !d->table) return;
        const auto rows_sel = d->table->selectionModel()->selectedRows();
        detail::fire(d->selected, rows_sel.isEmpty() ? -1 : rows_sel.first().row());
    });
    QObject::connect(pimpl->table.data(), &QTableWidget::cellChanged, [weak](int row, int col) {
        auto d = weak.lock();
        if (!d || !d->table) return;
        const QTableWidgetItem* item = d->table->item(row, col);
        detail::fire(d->cell_changed, row, col, item ? detail::ss(item->text()) : std::string());
    });
}

Grid::~Grid() = default;

void Grid::set_cell(int row, int col, const std::string& text) { pimpl->write(row, col, text); }

std::string Grid::get_cell(int row, int col) const {
    if (!pimpl->in_range(row, col)) return {};
    const QTableWidgetItem* item = pimpl->table->item(row, col);
    return item ? detail::ss(item->text()) : std::string();
}

int Grid::add_row(const std::vector<std::string>& values) {
    if (!pimpl->table) return -1;
    const int row = pimpl->table->rowCount();
    pimpl->table->insertRow(row);
    const int cols = std::min(static_cast<int>(values.size()), pimpl->table->columnCount());
    for (int c = 0; c < cols; ++c) pimpl->write(row, c, values[static_cast<size_t>(c)]);
    return row;
}

void Grid::remove_row(int row) {
    if (pimpl->table && row >= 0 && row < pimpl->table->rowCount()) pimpl->table->removeRow(row);
}

void Grid::clear_rows() {
    if (pimpl->table) pimpl->table->setRowCount(0);
}

void Grid::set_row_count(int rows) {
    if (pimpl->table) pimpl->table->setRowCount(std::max(0, rows));
}

int Grid::row_count() const { return pimpl->table ? pimpl->table->rowCount() : 0; }
int Grid::column_count() const { return pimpl->table ? pimpl->table->columnCount() : 0; }

void Grid::set_headers(const std::vector<std::string>& headers) { pimpl->set_headers(headers); }

int Grid::selected_row() const {
    if (!pimpl->table) return -1;
    const auto rows = pimpl->table->selectionModel()->selectedRows();
    return rows.isEmpty() ? -1 : rows.first().row();
}

void Grid::set_selected_row(int row) {
    if (!pimpl->table) return;
    if (row < 0 || row >= pimpl->table->rowCount()) {
        pimpl->table->clearSelection();
    } else {
        pimpl->table->selectRow(row);
    }
}

void Grid::set_editable(bool editable) {
    if (!pimpl->table) return;
    pimpl->table->setEditTriggers(editable ? (QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed |
                                              QAbstractItemView::AnyKeyPressed)
                                           : QAbstractItemView::NoEditTriggers);
}

EventConnection Grid::on_select(std::function<void(int)> handler) {
    return detail::add_handler(pimpl->selected, std::move(handler));
}

EventConnection Grid::on_cell_changed(std::function<void(int, int, const std::string&)> handler) {
    return detail::add_handler(pimpl->cell_changed, std::move(handler));
}

QWidget* Grid::get_qwidget() const { return pimpl->table.data(); }

}  // namespace simplegui
