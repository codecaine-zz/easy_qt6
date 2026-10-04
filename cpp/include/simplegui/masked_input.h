#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class MaskedInput : public Control {
public:
    MaskedInput(const std::string& mask = "", const std::string& text = "");
    ~MaskedInput() override;

    void set_mask(const std::string& mask);
    void set_text(const std::string& text);
    void set_placeholder(const std::string& placeholder);

    std::string text() const;

    EventConnection on_change(std::function<void(const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
