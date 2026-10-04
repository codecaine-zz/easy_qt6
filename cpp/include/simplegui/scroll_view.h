#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

class ScrollView : public Control {
public:
    ScrollView();
    ~ScrollView() override;

    void set_content(std::shared_ptr<Control> content);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
