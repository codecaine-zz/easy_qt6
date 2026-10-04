#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class SearchField : public Control {
public:
    explicit SearchField(const std::string& placeholder = "Search...");
    ~SearchField() override;

    std::string get_text() const;
    void set_text(const std::string& text);
    EventConnection on_change(std::function<void(const std::string&)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
