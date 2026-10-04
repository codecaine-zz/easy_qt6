#include "simplegui/button.h"
#include "detail/common.h"

#include <QPushButton>

namespace simplegui {

struct Button::Impl {
    QPointer<QPushButton> qbutton;
    explicit Impl(const std::string& text) : qbutton(new QPushButton(detail::qs(text))) {}
    ~Impl() { detail::delete_if_orphan(qbutton); }
};

Button::Button(const std::string& text) : pimpl(std::make_shared<Impl>(text)) {}

Button::~Button() = default;

void Button::set_text(const std::string& text) {
    if (pimpl->qbutton) pimpl->qbutton->setText(detail::qs(text));
}

std::string Button::get_text() const {
    return pimpl->qbutton ? detail::ss(pimpl->qbutton->text()) : std::string();
}

EventConnection Button::on_click(std::function<void()> handler) {
    if (!pimpl->qbutton || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->qbutton.data(), &QPushButton::clicked,
                                         [handler = std::move(handler)]() { handler(); }));
}

QWidget* Button::get_qwidget() const {
    return pimpl->qbutton.data();
}

}  // namespace simplegui
