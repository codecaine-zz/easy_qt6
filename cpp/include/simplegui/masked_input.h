#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A text box that only accepts a fixed pattern (Delphi/Lazarus: TMaskEdit).
// Mask characters:  9 = digit required, 0 = digit optional, A = letter required,
// a = letter optional, N = letter/digit required, X = any character required.
// Everything else is shown literally.  Example phone mask: "(999) 999-9999"
class MaskedInput : public Control {
public:
    MaskedInput(const std::string& mask = "", const std::string& text = "");
    ~MaskedInput() override;

    void set_mask(const std::string& mask);   // "" removes the mask
    void set_text(const std::string& text);
    void set_placeholder(const std::string& placeholder);
    void clear();

    std::string text() const;
    std::string get_text() const { return text(); }   // same as text()
    bool is_complete() const;   // true when every required mask position is filled

    EventConnection on_change(std::function<void(const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
