#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <functional>

namespace simplegui {

class FeedbackMood : public Control {
public:
    FeedbackMood(int initial_rating = 0);
    ~FeedbackMood() override;

    void set_rating(int rating); // 1 to 5, or 0 for none
    int rating() const;

    EventConnection on_change(std::function<void(int rating)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
