#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class ImageButton : public Control {
public:
    ImageButton(const std::string& image_path, const std::string& tooltip = "");
    ~ImageButton() override;

    EventConnection on_click(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
