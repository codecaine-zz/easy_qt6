#include "simplegui/vbox.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPointer>
#include <vector>

namespace simplegui {

struct VBox::Impl {
    QPointer<QWidget> container;
    QVBoxLayout* qlayout;
    std::vector<std::shared_ptr<Control>> children;

    Impl() {
        container = new QWidget();
        qlayout = new QVBoxLayout(container);
    }
    ~Impl() { if (container && !container->parent()) delete container; }
};

VBox::VBox() : pimpl(std::make_shared<Impl>()) {}

VBox::~VBox() = default;

void VBox::add_child(std::shared_ptr<Control> control) {
    pimpl->children.push_back(control);
    if (pimpl->qlayout && control) {
        pimpl->qlayout->addWidget(control->get_qwidget());
    }
}

void VBox::add_stretch(int stretch) {
    if (pimpl->qlayout) {
        pimpl->qlayout->addStretch(stretch);
    }
}

void VBox::set_spacing(int spacing) {
    if (pimpl->qlayout) {
        pimpl->qlayout->setSpacing(spacing);
    }
}

void VBox::set_margins(int margin) {
    if (pimpl->qlayout) {
        pimpl->qlayout->setContentsMargins(margin, margin, margin, margin);
    }
}

void VBox::set_margins(int left, int top, int right, int bottom) {
    if (pimpl->qlayout) {
        pimpl->qlayout->setContentsMargins(left, top, right, bottom);
    }
}

QWidget* VBox::get_qwidget() const {
    return pimpl->container.data();
}

}
