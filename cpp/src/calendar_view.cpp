#include "simplegui/calendar_view.h"
#include <QCalendarWidget>

namespace simplegui {

struct CalendarView::Impl {
    QCalendarWidget* widget;
};

CalendarView::CalendarView() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QCalendarWidget();
}

CalendarView::~CalendarView() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

QWidget* CalendarView::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
