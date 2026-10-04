#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace simplegui {

class TokenField : public Control {
public:
    TokenField(const std::string& placeholder = "Type tag and press Enter...");
    ~TokenField() override;

    void add_token(const std::string& token);
    void remove_token(const std::string& token);
    void clear_tokens();
    std::vector<std::string> tokens() const;

    EventConnection on_tokens_changed(std::function<void(const std::vector<std::string>& tokens)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
