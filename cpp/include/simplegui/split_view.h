#pragma once
#include "simplegui/control.h"
#include <memory>
#include <vector>

namespace simplegui {

class SplitView : public Control {
public:
    explicit SplitView(bool horizontal = true);
    ~SplitView() override;

    void add_child(std::shared_ptr<Control> control);
    void set_sizes(int first, int second);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
