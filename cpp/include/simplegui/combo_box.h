#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A drop-down list the user can ALSO type into (Delphi: TComboBox with csDropDown).
class ComboBox : public Control {
public:
    explicit ComboBox(const std::vector<std::string>& items = {});
    ~ComboBox() override;

    void add_item(const std::string& item);
    void set_items(const std::vector<std::string>& items);
    std::vector<std::string> items() const;
    void clear();

    std::string get_text() const;           // whatever is currently shown/typed
    void set_text(const std::string& text);
    void set_placeholder(const std::string& placeholder);

    // Runs whenever the text changes (picked from the list or typed).
    EventConnection on_change(std::function<void(const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
