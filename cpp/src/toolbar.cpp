#include "simplegui/toolbar.h"
#include <QToolBar>

namespace simplegui {

struct ToolBar::Impl {
    QToolBar* widget;
};

ToolBar::ToolBar() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QToolBar();
}

ToolBar::~ToolBar() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

QWidget* ToolBar::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
