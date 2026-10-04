#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class Button : public Control {
public:
    explicit Button(const std::string& text);
    ~Button() override;

    void set_text(const std::string& text);
    std::string get_text() const;

    EventConnection on_click(std::function<void()> handler);
    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
