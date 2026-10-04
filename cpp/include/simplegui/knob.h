#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>

namespace simplegui {

// A round dial you turn with the mouse, like a volume knob.
class Knob : public Control {
public:
    explicit Knob(int min_val = 0, int max_val = 100, int initial_val = 0);
    ~Knob() override;

    int get_value() const;
    void set_value(int value);
    void set_range(int min_val, int max_val);

    EventConnection on_change(std::function<void(int value)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
