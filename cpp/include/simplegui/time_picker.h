#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

class TimePicker : public Control {
public:
    TimePicker();
    ~TimePicker() override;

    void set_time(int hour, int minute);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
