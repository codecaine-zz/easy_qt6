#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace simplegui {

class Breadcrumbs : public Control {
public:
    explicit Breadcrumbs(const std::vector<std::string>& crumbs);
    ~Breadcrumbs() override;

    EventConnection on_click(std::function<void(int)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
