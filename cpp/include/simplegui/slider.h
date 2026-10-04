#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>

namespace simplegui {

// A draggable slider for picking a whole number (Delphi: TTrackBar).
class Slider : public Control {
public:
    explicit Slider(int min_val = 0, int max_val = 100, int initial_val = 50);
    ~Slider() override;

    int get_value() const;
    void set_value(int value);              // clamped to the range
    void set_range(int min_val, int max_val);
    void set_vertical(bool vertical);       // default is horizontal

    // Runs while the slider moves. Receives the new value.
    EventConnection on_change(std::function<void(int value)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
