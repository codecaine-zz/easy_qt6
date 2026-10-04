#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct CompositionSegment {
    std::string label;
    double value;
    std::string color;
};

class CompositionBar : public Control {
public:
    CompositionBar();
    ~CompositionBar() override;

    void add_segment(const std::string& label, double value, const std::string& color);
    void clear_segments();
    void set_height(int height);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
