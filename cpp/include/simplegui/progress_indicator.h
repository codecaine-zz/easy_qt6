#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// A horizontal loading bar (Delphi: TProgressBar).
class ProgressIndicator : public Control {
public:
    explicit ProgressIndicator(int min_val = 0, int max_val = 100, int initial_val = 0);
    ~ProgressIndicator() override;

    int get_value() const;
    void set_value(int value);
    void set_range(int min_val, int max_val);
    // true = endless "busy" animation when you don't know how long something takes.
    void set_indeterminate(bool indeterminate);
    void set_show_text(bool show);   // show/hide the "42%" text

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
