#include "simplegui/accordion.h"
#include <QWidget>

namespace simplegui {

struct Accordion::Impl {
    QWidget* widget;
};

Accordion::Accordion() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QWidget();
}

Accordion::~Accordion() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

QWidget* Accordion::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
