#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// A layout that wraps elements to the next line.
class FlowLayout : public Control {
public:
    FlowLayout();
    ~FlowLayout() override;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
