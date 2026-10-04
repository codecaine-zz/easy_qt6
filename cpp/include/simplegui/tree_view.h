#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// A hierarchical list of items (Delphi: TTreeView).
class TreeView : public Control {
public:
    TreeView();
    ~TreeView() override;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
