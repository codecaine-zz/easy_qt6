#include "simplegui/date_picker.h"
#include <QDateEdit>
#include <QPointer>
#include <QDate>

namespace simplegui {

struct DatePicker::Impl {
    QPointer<QDateEdit> qdate;
    Impl() {
        qdate = new QDateEdit();
        qdate->setCalendarPopup(true);
        qdate->setDate(QDate::currentDate());
    }
    ~Impl() { if (qdate && !qdate->parent()) delete qdate; }
};

DatePicker::DatePicker() : pimpl(std::make_shared<Impl>()) {}

DatePicker::~DatePicker() = default;

std::string DatePicker::get_date() const {
    if (pimpl->qdate) {
        return pimpl->qdate->date().toString("yyyy-MM-dd").toStdString();
    }
    return "";
}

void DatePicker::set_date(const std::string& date) {
    if (pimpl->qdate) {
        pimpl->qdate->setDate(QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd"));
    }
}

EventConnection DatePicker::on_change(std::function<void(const std::string&)> handler) {
    if (pimpl->qdate) {
        auto conn = QObject::connect(pimpl->qdate.data(), &QDateEdit::dateChanged, [handler](const QDate& date) {
            handler(date.toString(Qt::ISODate).toStdString());
        });
        return EventConnection([conn]() { QObject::disconnect(conn); });
    }
    return EventConnection();
}

QWidget* DatePicker::get_qwidget() const {
    return pimpl->qdate.data();
}

}
