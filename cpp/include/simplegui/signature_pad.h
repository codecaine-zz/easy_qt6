#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A widget tailored for collecting handwritten signatures.
class SignaturePad : public Control {
public:
    SignaturePad(int width = 400, int height = 200);
    ~SignaturePad() override;

    void clear();
    bool save_to_file(const std::string& file_path) const;
    void set_pen_color(const std::string& color);
    void set_pen_width(int width);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
