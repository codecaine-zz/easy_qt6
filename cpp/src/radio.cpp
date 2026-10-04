#include "simplegui/radio.h"
#include "detail/common.h"

#include <QRadioButton>

namespace simplegui {

struct Radio::Impl {
    QPointer<QRadioButton> radio;
    Impl(const std::string& text, bool checked) : radio(new QRadioButton(detail::qs(text))) {
        radio->setChecked(checked);
    }
    ~Impl() { detail::delete_if_orphan(radio); }
};

Radio::Radio(const std::string& text, bool checked) : pimpl(std::make_shared<Impl>(text, checked)) {}

Radio::~Radio() = default;

bool Radio::is_checked() const {
    return pimpl->radio && pimpl->radio->isChecked();
}

void Radio::set_checked(bool checked) {
    if (pimpl->radio) pimpl->radio->setChecked(checked);
}

void Radio::set_text(const std::string& text) {
    if (pimpl->radio) pimpl->radio->setText(detail::qs(text));
}

std::string Radio::get_text() const {
    return pimpl->radio ? detail::ss(pimpl->radio->text()) : std::string();
}

EventConnection Radio::on_change(std::function<void(bool)> handler) {
    if (!pimpl->radio || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->radio.data(), &QRadioButton::toggled,
                                         [handler = std::move(handler)](bool checked) { handler(checked); }));
}

QWidget* Radio::get_qwidget() const {
    return pimpl->radio.data();
}

}  // namespace simplegui
