#include "simplegui/group_box.h"
#include "detail/box_core.h"

#include <QGroupBox>
#include <QVBoxLayout>

namespace simplegui {

struct GroupBox::Impl {
    QPointer<QGroupBox> group;
    detail::BoxCore box;

    explicit Impl(const std::string& title)
        : group(new QGroupBox(detail::qs(title))), box(group, new QVBoxLayout(group)) {}
    ~Impl() { detail::delete_if_orphan(group); }
};

GroupBox::GroupBox(const std::string& title) : pimpl(std::make_shared<Impl>(title)) {}

GroupBox::~GroupBox() = default;

void GroupBox::add_child(std::shared_ptr<Control> control, int stretch) { pimpl->box.add_child(control, stretch); }
void GroupBox::add_stretch(int stretch) { pimpl->box.add_stretch(stretch); }
void GroupBox::add_spacing(int pixels) { pimpl->box.add_spacing(pixels); }
void GroupBox::remove_child(std::shared_ptr<Control> control) { pimpl->box.remove_child(control); }
void GroupBox::clear() { pimpl->box.clear(); }
void GroupBox::set_spacing(int pixels) { pimpl->box.set_spacing(pixels); }
void GroupBox::set_margins(int pixels) { pimpl->box.set_margins(pixels, pixels, pixels, pixels); }

void GroupBox::set_title(const std::string& title) {
    if (pimpl->group) pimpl->group->setTitle(detail::qs(title));
}

std::string GroupBox::get_title() const {
    return pimpl->group ? detail::ss(pimpl->group->title()) : std::string();
}

QWidget* GroupBox::get_qwidget() const {
    return pimpl->group.data();
}

}  // namespace simplegui
