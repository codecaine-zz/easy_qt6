#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

class Sparkline : public Control {
public:
    explicit Sparkline(const std::string& line_color = "#06b6d4");
    ~Sparkline() override;

    void add_sample(double value);
    void set_samples(const std::vector<double>& samples);
    void clear();
    void set_color(const std::string& hex_color);
    void set_fill_enabled(bool enabled);
    void set_range(double min_val, double max_val);
    void set_max_samples(int max_count);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
