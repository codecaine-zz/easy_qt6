#include "simplegui/timer.h"
#include "detail/common.h"

#include <QCoreApplication>
#include <QTimer>

#include <algorithm>

namespace simplegui {

struct Timer::Impl {
    // Parented to the application so it is cleaned up at shutdown even if a
    // handler keeps the Timer alive by capturing it.
    QPointer<QTimer> timer = new QTimer(QCoreApplication::instance());
    detail::Event<> tick = detail::make_event<>(timer);
    ~Impl() { delete timer.data(); }
};

Timer::Timer(int interval_ms) : pimpl(std::make_shared<Impl>()) {
    set_interval(interval_ms);
    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->timer.data(), &QTimer::timeout, [weak]() {
        if (auto d = weak.lock()) detail::fire(d->tick);
    });
}

Timer::~Timer() = default;

void Timer::set_interval(int interval_ms) {
    if (pimpl->timer) pimpl->timer->setInterval(std::max(1, interval_ms));
}

int Timer::interval() const { return pimpl->timer ? pimpl->timer->interval() : 0; }

void Timer::start() {
    if (pimpl->timer) pimpl->timer->start();
}

void Timer::stop() {
    if (pimpl->timer) pimpl->timer->stop();
}

bool Timer::is_running() const { return pimpl->timer && pimpl->timer->isActive(); }

void Timer::set_enabled(bool enabled) {
    if (enabled) {
        start();
    } else {
        stop();
    }
}

void Timer::set_single_shot(bool single) {
    if (pimpl->timer) pimpl->timer->setSingleShot(single);
}

EventConnection Timer::on_tick(std::function<void()> handler) {
    return detail::add_handler(pimpl->tick, std::move(handler));
}

void run_later(int delay_ms, std::function<void()> fn) {
    if (!fn || !QCoreApplication::instance()) return;
    // The application object is the context, so the callback is dropped safely at shutdown.
    QTimer::singleShot(std::max(0, delay_ms), QCoreApplication::instance(), std::move(fn));
}

}  // namespace simplegui
