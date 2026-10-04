#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace simplegui {

class Dropdown : public Control {
public:
    explicit Dropdown(const std::vector<std::string>& items = {});
    ~Dropdown() override;

    void add_item(const std::string& item);
    std::string get_selected() const;
    void set_selected(const std::string& item);
    EventConnection on_change(std::function<void(const std::string&)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
