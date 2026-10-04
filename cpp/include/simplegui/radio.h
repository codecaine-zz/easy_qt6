#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A round option button (Delphi: TRadioButton, VB: OptionButton).
// Radio buttons placed in the same layout (VBox, HBox, GroupBox...) form a group:
// choosing one automatically un-chooses the others.
class Radio : public Control {
public:
    explicit Radio(const std::string& text, bool checked = false);
    ~Radio() override;

    bool is_checked() const;
    void set_checked(bool checked);
    void set_text(const std::string& text);
    std::string get_text() const;

    // Runs when this option becomes selected (true) or unselected (false).
    EventConnection on_change(std::function<void(bool checked)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
