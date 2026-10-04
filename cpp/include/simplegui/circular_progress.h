#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

class CircularProgress : public Control {
public:
    CircularProgress();
    ~CircularProgress() override;

    void set_value(int percentage); // 0 to 100
    int get_value() const;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
