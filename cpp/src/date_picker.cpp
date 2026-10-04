#include "simplegui/date_picker.h"
#include "detail/common.h"

#include <QDate>
#include <QDateEdit>

namespace simplegui {

namespace {
constexpr const char* kDateFormat = "yyyy-MM-dd";
}

struct DatePicker::Impl {
    QPointer<QDateEdit> edit;
    Impl() : edit(new QDateEdit()) {
        edit->setCalendarPopup(true);
        edit->setDisplayFormat(kDateFormat);
        edit->setDate(QDate::currentDate());
    }
    ~Impl() { detail::delete_if_orphan(edit); }
};

DatePicker::DatePicker() : pimpl(std::make_shared<Impl>()) {}

DatePicker::DatePicker(const std::string& date) : DatePicker() {
    set_date(date);
}

DatePicker::~DatePicker() = default;

std::string DatePicker::get_date() const {
    return pimpl->edit ? detail::ss(pimpl->edit->date().toString(kDateFormat)) : std::string();
}

bool DatePicker::set_date(const std::string& date) {
    const QDate parsed = QDate::fromString(detail::qs(date), kDateFormat);
    if (!pimpl->edit || !parsed.isValid()) return false;
    pimpl->edit->setDate(parsed);
    return true;
}

EventConnection DatePicker::on_change(std::function<void(const std::string&)> handler) {
    if (!pimpl->edit || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->edit.data(), &QDateEdit::dateChanged,
                                         [handler = std::move(handler)](const QDate& date) {
                                             handler(detail::ss(date.toString(kDateFormat)));
                                         }));
}

QWidget* DatePicker::get_qwidget() const {
    return pimpl->edit.data();
}

}  // namespace simplegui
