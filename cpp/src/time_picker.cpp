#include "simplegui/time_picker.h"
#include <QTimeEdit>
#include <QTime>

namespace simplegui {

struct TimePicker::Impl {
    QTimeEdit* widget;
};

TimePicker::TimePicker() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QTimeEdit();
}

TimePicker::~TimePicker() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

void TimePicker::set_time(int hour, int minute) {
    pimpl->widget->setTime(QTime(hour, minute));
}

QWidget* TimePicker::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
