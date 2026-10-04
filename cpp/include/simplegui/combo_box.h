#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace simplegui {

class ComboBox : public Control {
public:
    explicit ComboBox(const std::vector<std::string>& items = {});
    ~ComboBox() override;

    void add_item(const std::string& item);
    std::string get_text() const;
    void set_text(const std::string& text);
    EventConnection on_change(std::function<void(const std::string&)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
