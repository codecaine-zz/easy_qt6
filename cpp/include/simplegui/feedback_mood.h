#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>

namespace simplegui {

// Five emoji buttons (angry ... delighted) for quick "how did we do?" feedback.
// Rating 1 = worst, 5 = best, 0 = nothing chosen yet.
class FeedbackMood : public Control {
public:
    explicit FeedbackMood(int initial_rating = 0);
    ~FeedbackMood() override;

    void set_rating(int rating);   // clamped to 0..5; does not fire on_change
    int rating() const;

    // Runs when the user clicks one of the faces.
    EventConnection on_change(std::function<void(int rating)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
