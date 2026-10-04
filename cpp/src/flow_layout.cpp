#include "simplegui/flow_layout.h"
#include <QWidget>

namespace simplegui {

struct FlowLayout::Impl {
    QWidget* widget;
};

FlowLayout::FlowLayout() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QWidget();
}

FlowLayout::~FlowLayout() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

QWidget* FlowLayout::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
