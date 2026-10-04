#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// Two date boxes ("from" -> "to") plus quick "7D" and "30D" preset buttons.
// Dates are text in "YYYY-MM-DD" form, e.g. "2026-12-31".
// Empty start = 7 days ago, empty end = today.
class DateRangePicker : public Control {
public:
    DateRangePicker(const std::string& start_date = "", const std::string& end_date = "");
    ~DateRangePicker() override;

    // Each setter returns false (and changes nothing) if the date text is invalid.
    bool set_start_date(const std::string& date);
    bool set_end_date(const std::string& date);
    bool set_range(const std::string& start_date, const std::string& end_date);
    void set_last_days(int days);   // end = today, start = today - days

    std::string start_date() const;
    std::string end_date() const;

    // Runs whenever either date changes.
    EventConnection on_range_changed(std::function<void(const std::string& start, const std::string& end)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
