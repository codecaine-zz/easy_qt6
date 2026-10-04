#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct PieSlice {
    std::string label;
    double value;
    std::string color_hex;
};

class PieChart : public Control {
public:
    explicit PieChart(const std::string& title = "");
    ~PieChart() override;

    void add_slice(const std::string& label, double value, const std::string& color_hex = "");
    void set_slices(const std::vector<PieSlice>& slices);
    void clear_slices();

    void set_title(const std::string& title);
    void show_legend(bool show);
    void show_percentages(bool show);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
