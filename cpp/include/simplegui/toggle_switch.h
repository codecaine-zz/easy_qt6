#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A smooth, animated on/off switch (like on a phone), with an optional label.
// Works like a Checkbox: click it, or press Space when it has focus.
class ToggleSwitch : public Control {
public:
    explicit ToggleSwitch(bool on = false, const std::string& label = "");
    ~ToggleSwitch() override;

    void set_on(bool on);            // does not fire on_toggle
    bool is_on() const;
    void toggle();                   // flips the switch and fires on_toggle
    void set_text(const std::string& label);
    std::string get_text() const;
    void set_on_color(const std::string& color);   // track color when on (default cyan)

    // Runs when the user flips the switch.
    EventConnection on_toggle(std::function<void(bool on)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
