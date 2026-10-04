#include "simplegui/hbox.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QPointer>
#include <vector>

namespace simplegui {

struct HBox::Impl {
    QPointer<QWidget> container;
    QHBoxLayout* qlayout;
    std::vector<std::shared_ptr<Control>> children;

    Impl() {
        container = new QWidget();
        qlayout = new QHBoxLayout(container);
    }
    ~Impl() { if (container && !container->parent()) delete container; }
};

HBox::HBox() : pimpl(std::make_shared<Impl>()) {}

HBox::~HBox() = default;

void HBox::add_child(std::shared_ptr<Control> control) {
    pimpl->children.push_back(control);
    if (pimpl->qlayout) {
        pimpl->qlayout->addWidget(control->get_qwidget());
    }
}

QWidget* HBox::get_qwidget() const {
    return pimpl->container.data();
}

}
