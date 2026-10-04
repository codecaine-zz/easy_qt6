#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

class Grid : public Control {
public:
    Grid(int rows, int cols, const std::vector<std::string>& headers);
    ~Grid() override;

    void set_cell(int row, int col, const std::string& text);
    std::string get_cell(int row, int col) const;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
