#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>

namespace simplegui {

// A row of clickable stars (0 to max_stars). Click a star to set the rating.
class Rating : public Control {
public:
    explicit Rating(int max_stars = 5);
    ~Rating() override;

    void set_rating(int stars);          // clamped to 0..max_stars
    int get_rating() const;
    int max_stars() const;
    void set_read_only(bool read_only);  // true = display only, clicks ignored

    // Runs when the user clicks a star.
    EventConnection on_change(std::function<void(int stars)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
