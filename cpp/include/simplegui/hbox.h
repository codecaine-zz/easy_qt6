#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

class HBox : public Control {
public:
    HBox();
    ~HBox() override;

    void add_child(std::shared_ptr<Control> control);
    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
