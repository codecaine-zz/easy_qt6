#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A button that shows a picture (icon) instead of, or next to, text
// (Delphi: TSpeedButton / TBitBtn).
class ImageButton : public Control {
public:
    ImageButton(const std::string& image_path, const std::string& tooltip = "");
    ~ImageButton() override;

    bool set_image(const std::string& image_path); // false if the file can't be loaded
    void set_icon_size(int pixels);                // default is the platform icon size
    void set_text(const std::string& text);        // optional caption next to the icon

    EventConnection on_click(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
