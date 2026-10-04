#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

class Image : public Control {
public:
    explicit Image(const std::string& image_path);
    ~Image() override;

    void set_image(const std::string& image_path);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
