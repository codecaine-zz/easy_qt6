#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A box with a border and a title that groups related controls
// (Delphi: TGroupBox, VB: Frame). Children are stacked top to bottom.
class GroupBox : public Control {
public:
    explicit GroupBox(const std::string& title = "");
    ~GroupBox() override;

    void add_child(std::shared_ptr<Control> control, int stretch = 0);
    void add_stretch(int stretch = 0);  // invisible spring that pushes controls apart
    void add_spacing(int pixels);
    void remove_child(std::shared_ptr<Control> control);
    void clear();

    void set_title(const std::string& title);
    std::string get_title() const;
    void set_spacing(int pixels);
    void set_margins(int pixels);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
