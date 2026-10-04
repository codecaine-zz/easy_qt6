#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// Stacks controls from top to bottom, like a column.
class VBox : public Control {
public:
    VBox();
    ~VBox() override;

    // Adds a control at the bottom. `stretch` > 0 makes it grow to fill spare
    // space (a child with stretch 2 gets twice as much as one with stretch 1).
    void add_child(std::shared_ptr<Control> control, int stretch = 0);
    void add_stretch(int stretch = 0);   // invisible spring that pushes controls apart
    void add_spacing(int pixels);        // fixed empty gap
    void remove_child(std::shared_ptr<Control> control);
    void clear();                        // removes everything
    int child_count() const;

    void set_spacing(int pixels);        // gap between every child
    void set_margins(int pixels);        // empty border around all children
    void set_margins(int left, int top, int right, int bottom);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
