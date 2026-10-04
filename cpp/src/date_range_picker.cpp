#include "simplegui/date_range_picker.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QDateEdit>
#include <QLabel>
#include <QPushButton>
#include <QPointer>
#include <QDate>

namespace simplegui {

struct DateRangePicker::Impl {
    QPointer<QFrame> container;
    QPointer<QDateEdit> start_edit;
    QPointer<QDateEdit> end_edit;
    std::function<void(const std::string&, const std::string&)> change_handler;
};

DateRangePicker::DateRangePicker(const std::string& initial_start, const std::string& initial_end)
    : pimpl(std::make_shared<Impl>())
{
    auto* frame = new QFrame();
    pimpl->container = frame;
    frame->setStyleSheet("background: transparent;");

    auto* layout = new QHBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    auto setup_edit = [](QDateEdit* edit) {
        edit->setDisplayFormat("yyyy-MM-dd");
        edit->setCalendarPopup(true);
        edit->setStyleSheet(
            "QDateEdit {"
            "  background: #1e293b;"
            "  color: #f8fafc;"
            "  border: 1px solid #334155;"
            "  border-radius: 6px;"
            "  padding: 6px 10px;"
            "  font-size: 13px;"
            "}"
            "QDateEdit:focus {"
            "  border-color: #3b82f6;"
            "}"
            "QDateEdit::drop-down {"
            "  subcontrol-origin: padding;"
            "  subcontrol-position: top right;"
            "  width: 20px;"
            "  border-left-width: 0px;"
            "}"
        );
    };

    auto* start_edit = new QDateEdit(frame);
    pimpl->start_edit = start_edit;
    setup_edit(start_edit);

    QDate now = QDate::currentDate();
    if (!initial_start.empty()) {
        start_edit->setDate(QDate::fromString(QString::fromStdString(initial_start), "yyyy-MM-dd"));
    } else {
        start_edit->setDate(now.addDays(-7));
    }
    layout->addWidget(start_edit);

    auto* arrow = new QLabel(QString::fromUtf8("→"), frame);
    arrow->setStyleSheet("color: #94a3b8; font-size: 14px; font-weight: bold;");
    layout->addWidget(arrow);

    auto* end_edit = new QDateEdit(frame);
    pimpl->end_edit = end_edit;
    setup_edit(end_edit);

    if (!initial_end.empty()) {
        end_edit->setDate(QDate::fromString(QString::fromStdString(initial_end), "yyyy-MM-dd"));
    } else {
        end_edit->setDate(now);
    }
    layout->addWidget(end_edit);

    // Preset buttons
    auto make_preset_btn = [&](const QString& label, int days_back) {
        auto* btn = new QPushButton(label, frame);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(
            "QPushButton {"
            "  background: #0f172a;"
            "  color: #94a3b8;"
            "  border: 1px solid #334155;"
            "  border-radius: 6px;"
            "  padding: 5px 9px;"
            "  font-size: 11px;"
            "}"
            "QPushButton:hover {"
            "  background: #1e293b;"
            "  color: #f8fafc;"
            "}"
        );
        QObject::connect(btn, &QPushButton::clicked, [this, days_back]() {
            QDate cur = QDate::currentDate();
            if (pimpl->start_edit && pimpl->end_edit) {
                pimpl->start_edit->setDate(cur.addDays(-days_back));
                pimpl->end_edit->setDate(cur);
            }
        });
        return btn;
    };

    layout->addWidget(make_preset_btn("7D", 7));
    layout->addWidget(make_preset_btn("30D", 30));

    auto notify = [this]() {
        if (pimpl->change_handler) {
            pimpl->change_handler(start_date(), end_date());
        }
    };

    QObject::connect(start_edit, &QDateEdit::dateChanged, notify);
    QObject::connect(end_edit, &QDateEdit::dateChanged, notify);
}

DateRangePicker::~DateRangePicker() = default;

void DateRangePicker::set_start_date(const std::string& date) {
    if (pimpl->start_edit) {
        pimpl->start_edit->setDate(QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd"));
    }
}

void DateRangePicker::set_end_date(const std::string& date) {
    if (pimpl->end_edit) {
        pimpl->end_edit->setDate(QDate::fromString(QString::fromStdString(date), "yyyy-MM-dd"));
    }
}

void DateRangePicker::set_range(const std::string& start_date, const std::string& end_date) {
    set_start_date(start_date);
    set_end_date(end_date);
}

std::string DateRangePicker::start_date() const {
    if (!pimpl->start_edit) return "";
    return pimpl->start_edit->date().toString("yyyy-MM-dd").toStdString();
}

std::string DateRangePicker::end_date() const {
    if (!pimpl->end_edit) return "";
    return pimpl->end_edit->date().toString("yyyy-MM-dd").toStdString();
}

EventConnection DateRangePicker::on_range_changed(std::function<void(const std::string&, const std::string&)> handler) {
    pimpl->change_handler = handler;
    return EventConnection();
}

QWidget* DateRangePicker::get_qwidget() const {
    return pimpl->container.data();
}

}
