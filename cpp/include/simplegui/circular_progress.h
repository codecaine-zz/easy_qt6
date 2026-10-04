#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A ring that fills up clockwise from 0 to 100 percent.
class CircularProgress : public Control {
public:
    CircularProgress();
    ~CircularProgress() override;

    void set_value(int percentage);              // clamped to 0..100
    int get_value() const;
    void set_color(const std::string& color);    // ring color, e.g. "#3498db"
    void set_show_text(bool show);               // show "42%" in the middle
    void set_diameter(int pixels);               // default 60

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
