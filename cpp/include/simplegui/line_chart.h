#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct LineSeries {
    std::string name;
    std::vector<double> values;
    std::string color_hex;
    bool fill_gradient = true;
};

class LineChart : public Control {
public:
    explicit LineChart(const std::string& title = "");
    ~LineChart() override;

    void add_series(const std::string& name,
                    const std::vector<double>& values,
                    const std::string& color_hex = "",
                    bool fill_gradient = true);
    void set_x_labels(const std::vector<std::string>& labels);
    void clear_series();

    void set_title(const std::string& title);
    void set_smooth(bool smooth);
    void show_points(bool show);
    void show_grid(bool show);
    void show_legend(bool show);
    void set_y_range(double min, double max);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
