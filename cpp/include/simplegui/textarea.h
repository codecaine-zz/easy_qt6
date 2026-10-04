#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A multi-line text box (Delphi: TMemo, VB: multi-line TextBox).
class Textarea : public Control {
public:
    explicit Textarea(const std::string& text = "");
    ~Textarea() override;

    std::string get_text() const;
    void set_text(const std::string& text);
    void append_text(const std::string& line);   // adds a new line at the end
    void set_placeholder(const std::string& placeholder);
    void set_read_only(bool read_only);
    void clear();

    EventConnection on_change(std::function<void(const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
