#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A small round status light (like the LEDs on a hardware panel), optionally blinking.
//   auto led = std::make_shared<LedIndicator>("#22c55e");
//   led->set_blinking(true);
class LedIndicator : public Control {
public:
    explicit LedIndicator(const std::string& color = "#22c55e", bool on = true);
    ~LedIndicator() override;

    void set_on(bool on);
    bool is_on() const;
    void set_color(const std::string& color);
    void set_blinking(bool blinking, int interval_ms = 500);
    bool is_blinking() const;
    void set_diameter(int pixels);   // default 16
    void set_label(const std::string& text);  // optional text to the right of the light

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
