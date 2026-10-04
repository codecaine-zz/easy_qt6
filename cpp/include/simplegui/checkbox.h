#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A tick box with a caption (Delphi: TCheckBox, VB: CheckBox).
class Checkbox : public Control {
public:
    explicit Checkbox(const std::string& text, bool checked = false);
    ~Checkbox() override;

    bool is_checked() const;
    void set_checked(bool checked);
    void set_text(const std::string& text);
    std::string get_text() const;

    // Runs when the box is ticked or unticked. Receives the new state.
    EventConnection on_change(std::function<void(bool checked)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
