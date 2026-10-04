#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

class Link : public Control {
public:
    Link(const std::string& text, const std::string& url);
    ~Link() override;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
