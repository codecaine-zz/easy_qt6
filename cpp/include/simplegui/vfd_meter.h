#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A retro glowing segment meter (like an old hi-fi VU meter), value 0..100.
class VfdMeter : public Control {
public:
    explicit VfdMeter(int segment_count = 20, bool vertical = true);
    ~VfdMeter() override;

    void set_value(double percentage); // 0.0 to 100.0
    double get_value() const;
    void set_segments(int count);
    void set_glow(bool glow);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
