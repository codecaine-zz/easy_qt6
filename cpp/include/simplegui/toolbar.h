#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// A row of quick-action icon buttons.
class ToolBar : public Control {
public:
    ToolBar();
    ~ToolBar() override;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
