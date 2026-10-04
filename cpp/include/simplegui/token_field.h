#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A tag editor: type a word, press Enter, and it becomes a removable "chip".
// Duplicate tags are ignored.
class TokenField : public Control {
public:
    explicit TokenField(const std::string& placeholder = "Type tag and press Enter...");
    ~TokenField() override;

    void add_token(const std::string& token);     // ignored if empty or already present
    void remove_token(const std::string& token);
    void clear_tokens();
    void set_tokens(const std::vector<std::string>& tokens);
    std::vector<std::string> tokens() const;
    bool has_token(const std::string& token) const;
    void set_placeholder(const std::string& placeholder);

    // Runs after tokens are added or removed, with the full new list.
    EventConnection on_tokens_changed(std::function<void(const std::vector<std::string>& tokens)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
