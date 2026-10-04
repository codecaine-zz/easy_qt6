#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A box for typing whole numbers, with up/down arrows (Delphi: TSpinEdit).
class NumberInput : public Control {
public:
    explicit NumberInput(int min_val = 0, int max_val = 100, int initial_val = 0);
    ~NumberInput() override;

    int get_value() const;
    void set_value(int value);
    void set_range(int min_val, int max_val);
    void set_step(int step);                    // how much one arrow click changes the value
    void set_suffix(const std::string& suffix); // e.g. " px" or " %"

    EventConnection on_change(std::function<void(int value)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
