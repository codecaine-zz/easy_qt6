#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class DatePicker : public Control {
public:
    DatePicker();
    ~DatePicker() override;

    std::string get_date() const; // Format: YYYY-MM-DD
    void set_date(const std::string& date);
    EventConnection on_change(std::function<void(const std::string&)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
