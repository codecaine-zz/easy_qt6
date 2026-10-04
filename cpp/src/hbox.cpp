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
    if (pimpl->qlayout && control) {
        pimpl->qlayout->addWidget(control->get_qwidget());
    }
}

void HBox::add_stretch(int stretch) {
    if (pimpl->qlayout) {
        pimpl->qlayout->addStretch(stretch);
    }
}

void HBox::set_spacing(int spacing) {
    if (pimpl->qlayout) {
        pimpl->qlayout->setSpacing(spacing);
    }
}

void HBox::set_margins(int margin) {
    if (pimpl->qlayout) {
        pimpl->qlayout->setContentsMargins(margin, margin, margin, margin);
    }
}

void HBox::set_margins(int left, int top, int right, int bottom) {
    if (pimpl->qlayout) {
        pimpl->qlayout->setContentsMargins(left, top, right, bottom);
    }
}

QWidget* HBox::get_qwidget() const {
    return pimpl->container.data();
}

}
