#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

class ActivityHeatmap : public Control {
public:
    explicit ActivityHeatmap(int weeks = 16, int days_per_week = 7);
    ~ActivityHeatmap() override;

    void set_data(const std::vector<std::vector<int>>& matrix);
    void set_cell(int week, int day, int intensity);
    void set_color_scale(const std::string& base_hex);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
