#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <functional>

namespace simplegui {

class Rating : public Control {
public:
    explicit Rating(int max_stars = 5);
    ~Rating() override;

    void set_rating(int stars);
    int get_rating() const;

    EventConnection on_change(std::function<void(int)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
