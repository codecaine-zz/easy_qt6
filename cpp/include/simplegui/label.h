#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// Shows a piece of text (Delphi: TLabel, VB: Label).
// Text is always shown exactly as written - it is never interpreted as HTML.
class Label : public Control {
public:
    explicit Label(const std::string& text = "");
    ~Label() override;

    void set_text(const std::string& text);
    std::string get_text() const;

    // "left", "center" or "right". Unknown values are ignored.
    void set_alignment(const std::string& alignment);
    // true = long text wraps onto several lines instead of being cut off.
    void set_word_wrap(bool wrap);
    // true = the user can select and copy the text with the mouse.
    void set_selectable(bool selectable);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
