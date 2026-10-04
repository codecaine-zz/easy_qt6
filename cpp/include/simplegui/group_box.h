#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

class GroupBox : public Control {
public:
    explicit GroupBox(const std::string& title);
    ~GroupBox() override;

    void add_child(std::shared_ptr<Control> control);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
