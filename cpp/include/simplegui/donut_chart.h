#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct DonutSlice {
    std::string label;
    double value;
    std::string color_hex;
};

class DonutChart : public Control {
public:
    DonutChart(const std::string& center_title = "", const std::string& center_subtitle = "");
    ~DonutChart() override;

    void add_segment(const std::string& label, double value, const std::string& color_hex);
    void set_segments(const std::vector<DonutSlice>& segments);
    void clear_segments();
    void set_center_text(const std::string& title, const std::string& subtitle = "");
    void set_thickness(int thickness_px);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
