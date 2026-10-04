#pragma once
#include "simplegui/control.h"
#include <memory>
#include <vector>

namespace simplegui {

// Two (or more) panes separated by a bar the user can drag to resize them
// (Delphi: TSplitter between panels).
class SplitView : public Control {
public:
    // horizontal = true puts panes side by side; false stacks them top/bottom.
    explicit SplitView(bool horizontal = true);
    ~SplitView() override;

    void add_child(std::shared_ptr<Control> control);
    // Starting sizes in pixels, e.g. set_sizes(200, 600) for a 200px sidebar.
    void set_sizes(int first, int second);
    void set_sizes(const std::vector<int>& sizes);   // for three or more panes

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
