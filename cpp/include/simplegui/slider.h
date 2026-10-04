#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <functional>

namespace simplegui {

class Slider : public Control {
public:
    explicit Slider(int min_val = 0, int max_val = 100, int initial_val = 50);
    ~Slider() override;

    int get_value() const;
    void set_value(int value);
    EventConnection on_change(std::function<void(int)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
