#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A rounded "pill" with a colored dot and a short status word, e.g.  ● LIVE
class StatusPill : public Control {
public:
    explicit StatusPill(const std::string& text = "LIVE", const std::string& dot_color = "#10b981");
    ~StatusPill() override;

    void set_status(const std::string& text, const std::string& dot_color);
    void set_text(const std::string& text);
    void set_color(const std::string& dot_color);
    std::string text() const;
    std::string get_text() const { return text(); }   // same as text()

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
