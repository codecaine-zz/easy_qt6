#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

class Label : public Control {
public:
    explicit Label(const std::string& text);
    ~Label() override;

    void set_text(const std::string& text);
    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
