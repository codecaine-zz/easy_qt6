#pragma once
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>

namespace simplegui {

// Runs code again and again at a fixed interval (Delphi/Lazarus: TTimer, VB: Timer).
// A Timer is not visible, so it is not added to a window.
//
//   auto clock = std::make_shared<simplegui::Timer>(1000);  // every 1000 ms = 1 second
//   clock->on_tick([]{ /* update the clock label */ });
//   clock->start();
//
// Keep the Timer alive (for example as a variable in main) for as long as it should run.
class Timer {
public:
    explicit Timer(int interval_ms = 1000);
    ~Timer();

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    void set_interval(int interval_ms);   // minimum 1 ms
    int interval() const;
    void start();
    void stop();
    bool is_running() const;
    void set_enabled(bool enabled);       // Delphi-style: true = start, false = stop
    void set_single_shot(bool single);    // true = tick only once per start()

    EventConnection on_tick(std::function<void()> handler);

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

// Runs `fn` once after `delay_ms` milliseconds, without creating a Timer yourself.
void run_later(int delay_ms, std::function<void()> fn);

}  // namespace simplegui
