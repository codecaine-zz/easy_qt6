#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct PieSlice {
    std::string label;
    double value = 0.0;
    std::string color_hex;
};

// A classic pie chart with an optional legend and percentages.
class PieChart : public Control {
public:
    explicit PieChart(const std::string& title = "");
    ~PieChart() override;

    void add_slice(const std::string& label, double value, const std::string& color_hex = "");
    void set_slices(const std::vector<PieSlice>& slices);
    void clear_slices();

    void set_title(const std::string& title);
    void show_legend(bool show);
    void set_show_legend(bool show) { show_legend(show); }  // same as show_legend()
    void show_percentages(bool show);
    void set_show_percentages(bool show) { show_percentages(show); }  // same as show_percentages()

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
