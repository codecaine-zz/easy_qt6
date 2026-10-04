#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A one-line search box with a built-in "x" clear button.
class SearchField : public Control {
public:
    explicit SearchField(const std::string& placeholder = "Search...");
    ~SearchField() override;

    std::string get_text() const;
    void set_text(const std::string& text);
    void set_placeholder(const std::string& placeholder);
    void clear();

    // Runs on every key press - perfect for "filter as you type".
    EventConnection on_change(std::function<void(const std::string& text)> handler);
    // Runs when the user presses Enter / Return.
    EventConnection on_enter(std::function<void(const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
