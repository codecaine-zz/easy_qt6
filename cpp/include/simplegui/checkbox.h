#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class Checkbox : public Control {
public:
    explicit Checkbox(const std::string& text, bool checked = false);
    ~Checkbox() override;

    bool is_checked() const;
    void set_checked(bool checked);
    EventConnection on_change(std::function<void(bool)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
