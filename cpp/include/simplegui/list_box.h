#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A scrollable list of text lines the user can pick from (Delphi/Lazarus/VB: ListBox).
// Rows are counted from 0. -1 means "nothing selected".
class ListBox : public Control {
public:
    explicit ListBox(const std::vector<std::string>& items = {});
    ~ListBox() override;

    void add_item(const std::string& text);
    void insert_item(int index, const std::string& text);
    void set_item(int index, const std::string& text);
    void remove_item(int index);
    void set_items(const std::vector<std::string>& items);
    void clear();
    std::vector<std::string> items() const;
    std::string item(int index) const;   // "" when out of range
    int count() const;

    int selected_index() const;
    std::string selected_text() const;
    void set_selected_index(int index);  // -1 clears the selection
    void set_sorted(bool sorted);        // keep items in alphabetical order

    // Runs when the selection changes (index -1 = nothing selected).
    EventConnection on_select(std::function<void(int index, const std::string& text)> handler);
    // Runs when a row is double-clicked (or Enter is pressed on it).
    EventConnection on_double_click(std::function<void(int index, const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
