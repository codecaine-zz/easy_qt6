#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct CompositionSegment {
    std::string label;
    double value = 0.0;
    std::string color;
};

// A single horizontal bar split into colored parts (like a disk-usage bar).
// Each part's width is proportional to its value.
class CompositionBar : public Control {
public:
    CompositionBar();
    ~CompositionBar() override;

    void add_segment(const std::string& label, double value, const std::string& color);
    void set_segments(const std::vector<CompositionSegment>& segments);   // replace every part
    void clear_segments();

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
