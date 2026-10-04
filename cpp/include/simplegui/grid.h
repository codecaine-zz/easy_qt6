#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A spreadsheet-style table of text cells (Delphi/Lazarus: TStringGrid).
// Rows and columns are counted from 0. Out-of-range cells are ignored safely.
class Grid : public Control {
public:
    Grid(int rows, int cols, const std::vector<std::string>& headers = {});
    ~Grid() override;

    // --- Cells -------------------------------------------------------------
    void set_cell(int row, int col, const std::string& text);
    std::string get_cell(int row, int col) const;   // "" when empty or out of range

    // --- Rows --------------------------------------------------------------
    int add_row(const std::vector<std::string>& values = {}); // returns the new row number
    void remove_row(int row);
    void clear_rows();                 // removes every row (headers stay)
    void set_row_count(int rows);
    int row_count() const;
    int column_count() const;
    void set_headers(const std::vector<std::string>& headers);

    // --- Selection & editing ----------------------------------------------
    int selected_row() const;          // -1 when nothing is selected
    void set_selected_row(int row);
    void set_editable(bool editable);  // false (default) = read-only cells

    // Runs when the user selects a different row (row is -1 when cleared).
    EventConnection on_select(std::function<void(int row)> handler);
    // Runs after the user edits a cell (only when set_editable(true)).
    EventConnection on_cell_changed(std::function<void(int row, int col, const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
