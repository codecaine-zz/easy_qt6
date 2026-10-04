#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// One bar in a BarChart. Leave color empty to use the automatic palette.
struct BarItem {
    std::string label;
    double value = 0.0;
    std::string color_hex;
};

// A vertical bar chart: chart->add_bar("Mon", 12);
class BarChart : public Control {
public:
    explicit BarChart(const std::string& title = "");
    ~BarChart() override;

    void add_bar(const std::string& label, double value, const std::string& color_hex = "");
    void set_bars(const std::vector<BarItem>& bars);
    void set_value(int index, double value);   // update one bar (ignored if out of range)
    void clear_bars();
    int bar_count() const;

    void set_title(const std::string& title);
    void set_show_values(bool show);           // numbers above the bars
    void set_show_grid(bool show);             // dashed horizontal lines
    void show_values(bool show) { set_show_values(show); }  // same as set_show_values()
    void show_grid(bool show) { set_show_grid(show); }      // same as set_show_grid()
    // Fixes the vertical axis. Call set_y_range(0, 0) to go back to automatic.
    void set_y_range(double min, double max);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
