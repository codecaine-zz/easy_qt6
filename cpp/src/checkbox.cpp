#include "simplegui/checkbox.h"
#include "detail/common.h"

#include <QCheckBox>

namespace simplegui {

struct Checkbox::Impl {
    QPointer<QCheckBox> box;
    Impl(const std::string& text, bool checked) : box(new QCheckBox(detail::qs(text))) {
        box->setChecked(checked);
    }
    ~Impl() { detail::delete_if_orphan(box); }
};

Checkbox::Checkbox(const std::string& text, bool checked) : pimpl(std::make_shared<Impl>(text, checked)) {}

Checkbox::~Checkbox() = default;

bool Checkbox::is_checked() const {
    return pimpl->box && pimpl->box->isChecked();
}

void Checkbox::set_checked(bool checked) {
    if (pimpl->box) pimpl->box->setChecked(checked);
}

void Checkbox::set_text(const std::string& text) {
    if (pimpl->box) pimpl->box->setText(detail::qs(text));
}

std::string Checkbox::get_text() const {
    return pimpl->box ? detail::ss(pimpl->box->text()) : std::string();
}

EventConnection Checkbox::on_change(std::function<void(bool)> handler) {
    if (!pimpl->box || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->box.data(), &QCheckBox::toggled,
                                         [handler = std::move(handler)](bool checked) { handler(checked); }));
}

QWidget* Checkbox::get_qwidget() const {
    return pimpl->box.data();
}

}  // namespace simplegui
