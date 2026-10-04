#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// A classic application menu bar.
class MenuBar : public Control {
public:
    MenuBar();
    ~MenuBar() override;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
