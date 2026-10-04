#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct BarItem {
    std::string label;
    double value;
    std::string color_hex;
};

class BarChart : public Control {
public:
    explicit BarChart(const std::string& title = "");
    ~BarChart() override;

    void add_bar(const std::string& label, double value, const std::string& color_hex = "");
    void set_bars(const std::vector<BarItem>& bars);
    void clear_bars();

    void set_title(const std::string& title);
    void set_show_values(bool show);
    void set_show_grid(bool show);
    void set_y_range(double min, double max);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
