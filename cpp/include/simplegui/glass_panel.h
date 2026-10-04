#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A frosted-glass card that holds other controls in a column (like a VBox with a
// futuristic glowing border and an optional title).
class GlassPanel : public Control {
public:
    explicit GlassPanel(const std::string& title = "");
    ~GlassPanel() override;

    // Same layout functions as VBox.
    void add_child(std::shared_ptr<Control> control, int stretch = 0);
    void add_stretch(int stretch = 0);
    void add_spacing(int pixels);
    void remove_child(std::shared_ptr<Control> control);
    void clear();
    int child_count() const;
    void set_spacing(int pixels);
    void set_margins(int pixels);

    void set_title(const std::string& title);
    std::string get_title() const;
    void set_accent_color(const std::string& color);   // border glow and title color

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
