#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A glowing outlined button that lights up when the mouse moves over it.
// Works exactly like Button: on_click, set_text, keyboard Space/Enter.
class NeonButton : public Control {
public:
    explicit NeonButton(const std::string& text = "", const std::string& color = "#00e5ff");
    ~NeonButton() override;

    void set_text(const std::string& text);
    std::string get_text() const;
    void set_color(const std::string& color);   // glow + text color
    void click();                               // press it from code (fires on_click)

    EventConnection on_click(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
