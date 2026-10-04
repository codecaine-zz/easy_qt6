#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A retro-futuristic console: colored lines of output and (optionally) a command
// line underneath. Up/Down arrows recall earlier commands.
//   auto term = std::make_shared<TerminalView>();
//   term->print_line("SYSTEM ONLINE", "#39ff88");
//   term->on_command([term](const std::string& cmd){ term->print_line("You typed: " + cmd); });
class TerminalView : public Control {
public:
    explicit TerminalView(bool show_input = true);
    ~TerminalView() override;

    void print_line(const std::string& text, const std::string& color = "");  // adds one line
    void print(const std::string& text, const std::string& color = "");       // adds text without a new line
    void clear();
    std::string text() const;                    // everything currently shown
    std::string get_text() const { return text(); }  // same as text()
    void set_max_lines(int lines);               // oldest lines are dropped (0 = unlimited)
    void set_prompt(const std::string& prompt);  // text before the command line, default ">"
    void set_input_visible(bool visible);
    void set_output_color(const std::string& color);  // default color for printed text

    // Runs when the user presses Enter on the command line.
    EventConnection on_command(std::function<void(const std::string& command)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
