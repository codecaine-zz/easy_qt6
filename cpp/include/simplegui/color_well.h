#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class ColorWell : public Control {
public:
    explicit ColorWell(const std::string& hex_color = "#FFFFFF");
    ~ColorWell() override;

    std::string get_color() const; // Format: #RRGGBB
    void set_color(const std::string& hex_color);
    EventConnection on_change(std::function<void(const std::string&)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
