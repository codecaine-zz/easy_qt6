#pragma once
#include "simplegui/control.h"
#include <memory>

namespace simplegui {

// Adds scroll bars around one large control (Delphi: TScrollBox).
// Put a VBox full of controls inside, and the user can scroll through it.
class ScrollView : public Control {
public:
    ScrollView();
    ~ScrollView() override;

    void set_content(std::shared_ptr<Control> content);
    void scroll_to_top();
    void scroll_to_bottom();

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
