#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

// A text editor configured for code (monospace font, no wrap).
class CodeEditor : public Control {
public:
    explicit CodeEditor(const std::string& text = "");
    ~CodeEditor() override;

    void set_text(const std::string& text);
    std::string get_text() const;

    EventConnection on_change(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
