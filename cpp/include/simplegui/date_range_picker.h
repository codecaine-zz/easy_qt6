#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class DateRangePicker : public Control {
public:
    DateRangePicker(const std::string& start_date = "", const std::string& end_date = "");
    ~DateRangePicker() override;

    void set_start_date(const std::string& date);
    void set_end_date(const std::string& date);
    void set_range(const std::string& start_date, const std::string& end_date);

    std::string start_date() const;
    std::string end_date() const;

    EventConnection on_range_changed(std::function<void(const std::string& start, const std::string& end)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
