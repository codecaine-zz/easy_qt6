#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

class StatusPill : public Control {
public:
    explicit StatusPill(const std::string& text = "LIVE", const std::string& dot_color = "#10b981");
    ~StatusPill() override;

    void set_status(const std::string& text, const std::string& dot_color);
    void set_text(const std::string& text);
    void set_color(const std::string& dot_color);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
