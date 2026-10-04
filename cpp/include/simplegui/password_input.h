#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A one-line text box that hides what is typed with dots (Delphi: TEdit with PasswordChar).
class PasswordInput : public Control {
public:
    explicit PasswordInput(const std::string& placeholder = "");
    ~PasswordInput() override;

    std::string get_text() const;
    void set_text(const std::string& text);
    void set_placeholder(const std::string& placeholder);
    void set_reveal(bool reveal);   // true = temporarily show the real characters
    void clear();

    EventConnection on_change(std::function<void(const std::string& text)> handler);
    EventConnection on_enter(std::function<void(const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
