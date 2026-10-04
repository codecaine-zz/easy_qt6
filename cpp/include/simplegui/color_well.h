#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A colored button that opens the system color picker when clicked.
// Colors are "#RRGGBB" text, for example "#3498db".
class ColorWell : public Control {
public:
    explicit ColorWell(const std::string& hex_color = "#FFFFFF");
    ~ColorWell() override;

    std::string get_color() const;              // always "#rrggbb"
    void set_color(const std::string& hex_color); // invalid colors are ignored

    // Runs after the user picks a new color.
    EventConnection on_change(std::function<void(const std::string& hex_color)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
