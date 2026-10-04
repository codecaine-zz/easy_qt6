#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

// A "you are here" path such as  Home > Projects > Report.
// Every part is clickable; on_click tells you which one (0 = first).
class Breadcrumbs : public Control {
public:
    explicit Breadcrumbs(const std::vector<std::string>& crumbs = {});
    ~Breadcrumbs() override;

    void set_crumbs(const std::vector<std::string>& crumbs);  // replace the whole path
    void push(const std::string& crumb);                      // add one part at the end
    void pop();                                               // remove the last part
    std::vector<std::string> crumbs() const;

    EventConnection on_click(std::function<void(int index)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
