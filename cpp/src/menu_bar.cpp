#include "simplegui/menu_bar.h"
#include <QMenuBar>

namespace simplegui {

struct MenuBar::Impl {
    QMenuBar* widget;
};

MenuBar::MenuBar() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QMenuBar();
}

MenuBar::~MenuBar() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

QWidget* MenuBar::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
