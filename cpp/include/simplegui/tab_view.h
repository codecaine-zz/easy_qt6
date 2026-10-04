#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

class TabView : public Control {
public:
    TabView();
    ~TabView() override;

    void add_tab(const std::string& title, std::shared_ptr<Control> content);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
