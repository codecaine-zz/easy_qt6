#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A date box with a pop-up calendar (Delphi: TDateTimePicker).
// Dates are always text in the form "YYYY-MM-DD", for example "2026-10-31".
class DatePicker : public Control {
public:
    DatePicker();                                // starts on today's date
    explicit DatePicker(const std::string& date);
    ~DatePicker() override;

    std::string get_date() const;
    // Returns false (and changes nothing) if `date` is not a real "YYYY-MM-DD" date.
    bool set_date(const std::string& date);

    EventConnection on_change(std::function<void(const std::string& date)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
