#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// A layout that aligns labels and inputs automatically (Delphi: no direct equivalent, Qt: QFormLayout).
class FormLayout : public Control {
public:
    FormLayout();
    ~FormLayout() override;

    // Adds a row with a text label and a control on the right.
    void add_row(const std::string& label, std::shared_ptr<Control> field);
    
    // Adds a control that spans both columns.
    void add_row(std::shared_ptr<Control> field);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
