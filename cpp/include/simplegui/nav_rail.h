#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A slim vertical navigation bar (like the left edge of many modern apps).
// Each item has an ID, an icon (any emoji or symbol, e.g. "🏠"), a short label,
// and an optional red badge number (0 = no badge).
class NavRail : public Control {
public:
    NavRail();
    ~NavRail() override;

    // Returns false if the ID is empty or already used. The first item becomes selected.
    bool add_item(const std::string& id, const std::string& icon_glyph, const std::string& label, int badge_count = 0);
    void set_badge(const std::string& id, int badge_count);   // 0 hides the badge
    void set_selected(const std::string& id);                 // does not fire on_select
    std::string selected() const;

    // Runs when the user clicks an item.
    EventConnection on_select(std::function<void(const std::string& id)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
