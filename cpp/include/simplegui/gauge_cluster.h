#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A grouped set of gauges, laid out nicely (simulates a dashboard).
class GaugeCluster : public Control {
public:
    GaugeCluster();
    ~GaugeCluster() override;

    void add_gauge(std::shared_ptr<Control> gauge, const std::string& label = "");

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
