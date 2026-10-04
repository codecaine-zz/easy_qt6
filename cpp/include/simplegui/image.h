#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// Shows a picture from a file such as "logo.png" (Delphi: TImage, VB: PictureBox).
// Supported formats include PNG, JPG, BMP, GIF (first frame) and SVG.
class Image : public Control {
public:
    explicit Image(const std::string& image_path = "");
    ~Image() override;

    // Loads a new picture. Returns false if the file is missing or not an image.
    bool set_image(const std::string& image_path);
    // true = stretch/shrink the picture to fill the control (keeps its shape).
    void set_scaled(bool scaled);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
