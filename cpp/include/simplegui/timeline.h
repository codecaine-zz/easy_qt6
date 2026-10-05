#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A visual tracker showing progress across sequential steps.
class Timeline : public Control {
public:
    Timeline();
    ~Timeline() override;

    void set_steps(const std::vector<std::string>& steps);
    void set_current_step(int step_index); // 0-based

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
