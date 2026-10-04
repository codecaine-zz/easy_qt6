#include "simplegui/vbox.h"
#include "detail/box_core.h"

#include <QVBoxLayout>

namespace simplegui {

struct VBox::Impl {
    QPointer<QWidget> container = new QWidget();
    detail::BoxCore box{container, new QVBoxLayout(container)};
    ~Impl() { detail::delete_if_orphan(container); }
};

VBox::VBox() : pimpl(std::make_shared<Impl>()) {}

VBox::~VBox() = default;

void VBox::add_child(std::shared_ptr<Control> control, int stretch) { pimpl->box.add_child(control, stretch); }
void VBox::add_stretch(int stretch) { pimpl->box.add_stretch(stretch); }
void VBox::add_spacing(int pixels) { pimpl->box.add_spacing(pixels); }
void VBox::remove_child(std::shared_ptr<Control> control) { pimpl->box.remove_child(control); }
void VBox::clear() { pimpl->box.clear(); }
int VBox::child_count() const { return pimpl->box.count(); }
void VBox::set_spacing(int pixels) { pimpl->box.set_spacing(pixels); }
void VBox::set_margins(int pixels) { pimpl->box.set_margins(pixels, pixels, pixels, pixels); }
void VBox::set_margins(int left, int top, int right, int bottom) { pimpl->box.set_margins(left, top, right, bottom); }

QWidget* VBox::get_qwidget() const {
    return pimpl->container.data();
}

}  // namespace simplegui
