#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A clickable push button (Delphi: TButton, VB: CommandButton).
class Button : public Control {
public:
    explicit Button(const std::string& text);
    ~Button() override;

    void set_text(const std::string& text);   // the caption on the button
    std::string get_text() const;

    // Runs `handler` every time the button is clicked.
    EventConnection on_click(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
