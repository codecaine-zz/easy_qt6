#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

class RichTextEditor : public Control {
public:
    RichTextEditor();
    ~RichTextEditor() override;

    void set_html(const std::string& html);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
