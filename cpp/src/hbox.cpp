#include "simplegui/hbox.h"
#include "detail/box_core.h"

#include <QHBoxLayout>

namespace simplegui {

struct HBox::Impl {
    QPointer<QWidget> container = new QWidget();
    detail::BoxCore box{container, new QHBoxLayout(container)};
    ~Impl() { detail::delete_if_orphan(container); }
};

HBox::HBox() : pimpl(std::make_shared<Impl>()) {}

HBox::~HBox() = default;

void HBox::add_child(std::shared_ptr<Control> control, int stretch) { pimpl->box.add_child(control, stretch); }
void HBox::add_stretch(int stretch) { pimpl->box.add_stretch(stretch); }
void HBox::add_spacing(int pixels) { pimpl->box.add_spacing(pixels); }
void HBox::remove_child(std::shared_ptr<Control> control) { pimpl->box.remove_child(control); }
void HBox::clear() { pimpl->box.clear(); }
int HBox::child_count() const { return pimpl->box.count(); }
void HBox::set_spacing(int pixels) { pimpl->box.set_spacing(pixels); }
void HBox::set_margins(int pixels) { pimpl->box.set_margins(pixels, pixels, pixels, pixels); }
void HBox::set_margins(int left, int top, int right, int bottom) { pimpl->box.set_margins(left, top, right, bottom); }

QWidget* HBox::get_qwidget() const {
    return pimpl->container.data();
}

}  // namespace simplegui
