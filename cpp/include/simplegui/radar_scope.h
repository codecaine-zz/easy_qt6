#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// An animated radar screen with a rotating sweep line and "blips" (targets).
// Angles are in degrees: 0 = up (north), 90 = right (east), and so on clockwise.
// Distance is 0.0 (centre) to 1.0 (outer ring).
class RadarScope : public Control {
public:
    RadarScope();
    ~RadarScope() override;

    void start();                                   // begin sweeping (starts automatically)
    void stop();
    bool is_running() const;
    void set_sweep_speed(double degrees_per_second); // default 90
    void set_color(const std::string& color);        // default green
    int add_blip(double angle_deg, double distance, const std::string& color = "");  // returns blip id
    void move_blip(int id, double angle_deg, double distance);
    void remove_blip(int id);
    void clear_blips();
    int blip_count() const;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
