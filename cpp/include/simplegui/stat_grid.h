#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// One tile in a StatGrid.
struct StatItem {
    std::string title;
    std::string value;
    std::string trend;          // e.g. "+14.2%" or "-3.1%"
    bool is_positive = true;    // true = green trend text, false = red
};

// A row of KPI tiles (title, big value, colored trend), like a finance dashboard.
class StatGrid : public Control {
public:
    StatGrid();
    ~StatGrid() override;

    void add_stat(const std::string& title, const std::string& value, const std::string& trend, bool is_positive = true);
    void set_stats(const std::vector<StatItem>& stats);   // replace every tile
    void clear();
    int count() const;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
