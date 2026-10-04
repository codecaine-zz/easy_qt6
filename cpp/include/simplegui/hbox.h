#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// Places controls side by side from left to right, like a row.
class HBox : public Control {
public:
    HBox();
    ~HBox() override;

    // Adds a control on the right. `stretch` > 0 makes it grow to fill spare space.
    void add_child(std::shared_ptr<Control> control, int stretch = 0);
    void add_stretch(int stretch = 0);   // invisible spring that pushes controls apart
    void add_spacing(int pixels);        // fixed empty gap
    void remove_child(std::shared_ptr<Control> control);
    void clear();
    int child_count() const;

    void set_spacing(int pixels);
    void set_margins(int pixels);
    void set_margins(int left, int top, int right, int bottom);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
