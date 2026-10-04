#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// A full-sized calendar grid.
class CalendarView : public Control {
public:
    CalendarView();
    ~CalendarView() override;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
