#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct StatItem {
    std::string title;
    std::string value;
    std::string trend;       // e.g. "+14.2%" or "-3.1%"
    bool is_positive;        // true = green, false = red
};

class StatGrid : public Control {
public:
    StatGrid();
    ~StatGrid() override;

    void add_stat(const std::string& title, const std::string& value, const std::string& trend, bool is_positive);
    void clear();

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
