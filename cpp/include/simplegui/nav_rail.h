#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace simplegui {

class NavRail : public Control {
public:
    NavRail();
    ~NavRail() override;

    void add_item(const std::string& id, const std::string& icon_glyph, const std::string& label, int badge_count = 0);
    void set_selected(const std::string& id);
    std::string selected() const;

    EventConnection on_select(std::function<void(const std::string& id)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
