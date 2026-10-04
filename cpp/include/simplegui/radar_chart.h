#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct RadarDataset {
    std::string name;
    std::vector<double> values; // 0.0 to 100.0
    std::string color_hex;
};

class RadarChart : public Control {
public:
    explicit RadarChart(const std::string& title = "");
    ~RadarChart() override;

    void set_dimensions(const std::vector<std::string>& labels);
    void add_dataset(const std::string& name, const std::vector<double>& values, const std::string& color_hex = "");
    void clear_datasets();

    void set_title(const std::string& title);
    void show_legend(bool show);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
