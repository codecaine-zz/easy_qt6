#include "simplegui/date_range_picker.h"
#include "detail/common.h"

#include <QDate>
#include <QDateEdit>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalBlocker>

#include <algorithm>

namespace simplegui {
namespace {

const QString kDateFormat = QStringLiteral("yyyy-MM-dd");

QDate parse_date(const std::string& text) { return QDate::fromString(detail::qs(text).trimmed(), kDateFormat); }

}  // namespace

struct DateRangePicker::Impl {
    QPointer<QFrame> container = new QFrame();
    QPointer<QDateEdit> start_edit;
    QPointer<QDateEdit> end_edit;
    detail::Event<const std::string&, const std::string&> changed;

    Impl() { changed = detail::make_event<const std::string&, const std::string&>(container); }
    ~Impl() { detail::delete_if_orphan(container); }

    std::string start() const { return start_edit ? detail::ss(start_edit->date().toString(kDateFormat)) : std::string(); }
    std::string end() const { return end_edit ? detail::ss(end_edit->date().toString(kDateFormat)) : std::string(); }
    void notify() { detail::fire(changed, start(), end()); }

    // Sets both dates and fires a single change event.
    void set_both(const QDate& s, const QDate& e) {
        if (!start_edit || !end_edit) return;
        {
            const QSignalBlocker b1(start_edit);
            const QSignalBlocker b2(end_edit);
            start_edit->setDate(s);
            end_edit->setDate(e);
        }
        notify();
    }
};

DateRangePicker::DateRangePicker(const std::string& initial_start, const std::string& initial_end)
    : pimpl(std::make_shared<Impl>()) {
    QFrame* frame = pimpl->container;
    frame->setStyleSheet(QStringLiteral("background: transparent;"));

    auto* layout = new QHBoxLayout(frame);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    auto make_edit = [frame](const QDate& date) {
        auto* edit = new QDateEdit(frame);
        edit->setDisplayFormat(kDateFormat);
        edit->setCalendarPopup(true);
        edit->setDate(date);
        edit->setStyleSheet(QStringLiteral(
            "QDateEdit { background: #1e293b; color: #f8fafc; border: 1px solid #334155;"
            "  border-radius: 6px; padding: 6px 10px; font-size: 13px; }"
            "QDateEdit:focus { border-color: #3b82f6; }"
            "QDateEdit::drop-down { subcontrol-origin: padding; subcontrol-position: top right;"
            "  width: 20px; border-left-width: 0px; }"));
        return edit;
    };

    const QDate today = QDate::currentDate();
    const QDate s = parse_date(initial_start);
    const QDate e = parse_date(initial_end);
    pimpl->start_edit = make_edit(s.isValid() ? s : today.addDays(-7));
    pimpl->end_edit = make_edit(e.isValid() ? e : today);

    auto* arrow = new QLabel(QString(QChar(0x2192)), frame);
    arrow->setStyleSheet(QStringLiteral("color: #94a3b8; font-size: 14px; font-weight: bold;"));

    layout->addWidget(pimpl->start_edit);
    layout->addWidget(arrow);
    layout->addWidget(pimpl->end_edit);

    std::weak_ptr<Impl> weak = pimpl;
    for (int days : {7, 30}) {
        auto* btn = new QPushButton(QStringLiteral("%1D").arg(days), frame);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setStyleSheet(QStringLiteral(
            "QPushButton { background: #0f172a; color: #94a3b8; border: 1px solid #334155;"
            "  border-radius: 6px; padding: 5px 9px; font-size: 11px; }"
            "QPushButton:hover { background: #1e293b; color: #f8fafc; }"));
        QObject::connect(btn, &QPushButton::clicked, [weak, days]() {
            if (auto d = weak.lock()) d->set_both(QDate::currentDate().addDays(-days), QDate::currentDate());
        });
        layout->addWidget(btn);
    }

    auto notify = [weak]() {
        if (auto d = weak.lock()) d->notify();
    };
    QObject::connect(pimpl->start_edit.data(), &QDateEdit::dateChanged, notify);
    QObject::connect(pimpl->end_edit.data(), &QDateEdit::dateChanged, notify);
}

DateRangePicker::~DateRangePicker() = default;

bool DateRangePicker::set_start_date(const std::string& date) {
    const QDate d = parse_date(date);
    if (!d.isValid() || !pimpl->start_edit) return false;
    pimpl->start_edit->setDate(d);
    return true;
}

bool DateRangePicker::set_end_date(const std::string& date) {
    const QDate d = parse_date(date);
    if (!d.isValid() || !pimpl->end_edit) return false;
    pimpl->end_edit->setDate(d);
    return true;
}

bool DateRangePicker::set_range(const std::string& start_date, const std::string& end_date) {
    const QDate s = parse_date(start_date);
    const QDate e = parse_date(end_date);
    if (!s.isValid() || !e.isValid()) return false;
    pimpl->set_both(s, e);
    return true;
}

void DateRangePicker::set_last_days(int days) {
    const QDate today = QDate::currentDate();
    pimpl->set_both(today.addDays(-std::max(0, days)), today);
}

std::string DateRangePicker::start_date() const { return pimpl->start(); }
std::string DateRangePicker::end_date() const { return pimpl->end(); }

EventConnection DateRangePicker::on_range_changed(std::function<void(const std::string&, const std::string&)> handler) {
    return detail::add_handler(pimpl->changed, std::move(handler));
}

QWidget* DateRangePicker::get_qwidget() const { return pimpl->container.data(); }

}  // namespace simplegui
