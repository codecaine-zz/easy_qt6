#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A small dashboard tile: a caption, a big number, and an optional note.
//   StatCard("CPU", "42%", "4 cores", "#10b981")
class StatCard : public Control {
public:
    StatCard(const std::string& title, const std::string& value, const std::string& subtext = "",
             const std::string& accent_color = "#3b82f6");
    ~StatCard() override;

    void set_title(const std::string& title);
    void set_value(const std::string& value);
    void set_subtext(const std::string& subtext);           // "" hides the note
    void set_accent_color(const std::string& hex_color);    // color of the big number
    std::string value() const;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
