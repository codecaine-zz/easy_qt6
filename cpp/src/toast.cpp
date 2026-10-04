#include "simplegui/toast.h"
#include <QWidget>

namespace simplegui {

struct Toast::Impl {
    QWidget* widget;
};

Toast::Toast() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QWidget();
}

Toast::~Toast() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

QWidget* Toast::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
