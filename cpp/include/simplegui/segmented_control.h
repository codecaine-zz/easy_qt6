#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A row of joined buttons where exactly one is selected, like the
// "Day | Week | Month" switch in calendar apps. Indexes start at 0.
class SegmentedControl : public Control {
public:
    explicit SegmentedControl(const std::vector<std::string>& items = {}, int selected_index = 0);
    ~SegmentedControl() override;

    void set_items(const std::vector<std::string>& items);   // selection resets to 0
    std::vector<std::string> items() const;
    int count() const;
    void set_selected_index(int index);    // does not fire on_change; unknown indexes are ignored
    int selected_index() const;            // -1 when there are no items
    std::string selected_text() const;
    void set_accent_color(const std::string& color);

    // Runs when the user picks a different segment.
    EventConnection on_change(std::function<void(int index, const std::string& text)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
