#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct GanttTask {
    std::string name;
    int start_day;  // 0-based offset
    int duration;   // in days
    std::string color;
};

// A simplified project management chart.
class GanttChart : public Control {
public:
    GanttChart();
    ~GanttChart() override;

    void set_tasks(const std::vector<GanttTask>& tasks);
    
    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
