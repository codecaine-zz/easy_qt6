#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A drop-down list where the user picks one item (Delphi: TComboBox with csDropDownList).
// The user cannot type their own value - use ComboBox for that.
class Dropdown : public Control {
public:
    explicit Dropdown(const std::vector<std::string>& items = {});
    ~Dropdown() override;

    void add_item(const std::string& item);
    void set_items(const std::vector<std::string>& items);  // replaces all items
    std::vector<std::string> items() const;
    void clear();
    int count() const;

    std::string get_selected() const;          // "" when nothing is selected
    void set_selected(const std::string& item); // ignored if the item is not in the list
    int selected_index() const;                // -1 when nothing is selected
    void set_selected_index(int index);

    // Runs when the user picks a different item. Receives the item text.
    EventConnection on_change(std::function<void(const std::string& item)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
