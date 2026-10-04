#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A social-style profile card: avatar initial, name, online dot, @handle, role,
// short bio, and one action button (e.g. "Connect" or "Message").
class UserProfileCard : public Control {
public:
    UserProfileCard(const std::string& name, const std::string& handle, const std::string& role,
                    const std::string& bio, bool is_online = true, const std::string& action_label = "Connect");
    ~UserProfileCard() override;

    void set_online_status(bool is_online);   // green dot = online, grey = offline
    bool is_online() const;
    void set_bio(const std::string& bio);
    void set_action_text(const std::string& text);

    // Runs when the action button is clicked.
    EventConnection on_action(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
