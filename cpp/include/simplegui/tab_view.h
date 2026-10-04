#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// Pages with clickable tabs along the top (Delphi: TPageControl).
class TabView : public Control {
public:
    TabView();
    ~TabView() override;

    // Adds a new page. `content` is usually a VBox holding the page's controls.
    void add_tab(const std::string& title, std::shared_ptr<Control> content);
    int tab_count() const;
    int current_index() const;              // 0 = first tab, -1 = no tabs
    void set_current_index(int index);
    void set_tab_title(int index, const std::string& title);

    // Runs when the user switches tabs. Receives the new tab index.
    EventConnection on_change(std::function<void(int index)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
