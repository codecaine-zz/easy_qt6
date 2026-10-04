#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A sci-fi speedometer: a glowing arc with a needle and a big number in the middle.
// The arc turns amber above the warning level and red above the danger level.
//   auto rpm = std::make_shared<RadialGauge>("ENGINE", 0, 8000);
//   rpm->set_units("RPM");
//   rpm->set_value(3500);   // the needle glides smoothly to the new value
class RadialGauge : public Control {
public:
    explicit RadialGauge(const std::string& title = "", double min = 0.0, double max = 100.0);
    ~RadialGauge() override;

    void set_value(double value);                 // clamped to the range, animated
    double value() const;
    double get_value() const { return value(); }      // same as value()
    void set_range(double min, double max);
    void set_title(const std::string& title);     // small text under the number
    void set_units(const std::string& units);     // e.g. "%", "km/h", "°C"
    void set_decimals(int decimals);              // digits after the decimal point (0..4)
    void set_color(const std::string& color);     // normal arc color (default cyan)
    // Values at or above `warning` are amber, at or above `danger` are red.
    // Pass the maximum (or larger) to switch a zone off.
    void set_thresholds(double warning, double danger);
    void set_animated(bool animated);             // false = jump instantly

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
