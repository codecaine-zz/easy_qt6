#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

class VBox : public Control {
public:
    VBox();
    ~VBox() override;

    void add_child(std::shared_ptr<Control> control);
    void add_stretch(int stretch = 0);
    void set_spacing(int spacing);
    void set_margins(int margin);
    void set_margins(int left, int top, int right, int bottom);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
