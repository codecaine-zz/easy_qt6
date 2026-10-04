#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// A temporary popup notification.
class Toast : public Control {
public:
    Toast();
    ~Toast() override;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
