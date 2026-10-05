#pragma once
#include "simplegui/control.h"
#include <memory>
#include <vector>
#include <functional>

namespace simplegui {

// A swipeable slider of child controls or images.
class Carousel : public Control {
public:
    Carousel();
    ~Carousel() override;

    void add_child(std::shared_ptr<Control> control);
    void set_current_index(int index);
    int current_index() const;
    int count() const;

    void next();
    void previous();

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
