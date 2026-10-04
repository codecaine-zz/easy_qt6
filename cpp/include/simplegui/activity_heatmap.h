#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A GitHub-style "contribution" grid of small colored squares.
// Each square has an intensity from 0 (empty) to 4 (brightest).
class ActivityHeatmap : public Control {
public:
    explicit ActivityHeatmap(int weeks = 16, int days_per_week = 7);
    ~ActivityHeatmap() override;

    // matrix[week][day] = intensity 0..4. Missing cells count as 0, extra cells are ignored.
    void set_data(const std::vector<std::vector<int>>& matrix);
    void set_cell(int week, int day, int intensity);  // out-of-range cells are ignored
    int get_cell(int week, int day) const;            // 0 when out of range
    void clear();                                     // every cell back to 0
    void set_color_scale(const std::string& base_color); // e.g. "#10b981"

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
