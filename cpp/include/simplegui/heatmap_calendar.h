#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <map>

namespace simplegui {

// A github-style contribution calendar view.
class HeatmapCalendar : public Control {
public:
    HeatmapCalendar();
    ~HeatmapCalendar() override;

    // date formatted as "YYYY-MM-DD", intensity usually 0 to 4
    void set_data(const std::map<std::string, int>& daily_intensity);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
