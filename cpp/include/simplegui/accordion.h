#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// A vertically stacked list of collapsible panels.
class Accordion : public Control {
public:
    Accordion();
    ~Accordion() override;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
