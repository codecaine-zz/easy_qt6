#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class UserProfileCard : public Control {
public:
    UserProfileCard(const std::string& name, const std::string& handle, const std::string& role, const std::string& bio, bool is_online = true, const std::string& action_label = "Connect");
    ~UserProfileCard() override;

    void set_online_status(bool is_online);
    void set_bio(const std::string& bio);
    EventConnection on_action(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
