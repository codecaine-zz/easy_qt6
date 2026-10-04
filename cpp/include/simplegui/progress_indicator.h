#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

class ProgressIndicator : public Control {
public:
    explicit ProgressIndicator(int min_val = 0, int max_val = 100, int initial_val = 0);
    ~ProgressIndicator() override;

    int get_value() const;
    void set_value(int value);
    void set_indeterminate(bool indeterminate);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
