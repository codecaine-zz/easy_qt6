#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A one-line text box (Delphi: TEdit, VB: TextBox).
class TextInput : public Control {
public:
    // `placeholder` is the grey hint shown while the box is empty.
    explicit TextInput(const std::string& placeholder = "");
    ~TextInput() override;

    std::string get_text() const;
    void set_text(const std::string& text);
    void set_placeholder(const std::string& placeholder);
    void set_read_only(bool read_only);      // true = can select/copy but not type
    void set_max_length(int characters);     // limit how much can be typed
    void clear();

    // Runs every time the text changes (each key press).
    EventConnection on_change(std::function<void(const std::string& text)> handler);
    // Runs when the user presses Enter / Return.
    EventConnection on_enter(std::function<void(const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
