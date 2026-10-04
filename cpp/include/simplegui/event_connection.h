#pragma once
#include <functional>
#include <utility>

namespace simplegui {

// Every on_xxx(...) event function returns an EventConnection "ticket".
//
//   * Ignore it, and your handler keeps working for as long as the control exists.
//   * Keep it, and you can stop the handler later with disconnect().
//
// Letting an EventConnection go out of scope does NOT disconnect the handler.
class EventConnection {
public:
    EventConnection() = default;

    explicit EventConnection(std::function<void()> disconnect_func)
        : disconnect_func(std::move(disconnect_func)) {}

    // Stops the handler. Safe to call more than once, and safe to call after
    // the control has been destroyed.
    void disconnect() {
        if (disconnect_func) {
            auto func = std::move(disconnect_func);
            disconnect_func = nullptr;
            func();
        }
    }

    // True until disconnect() is called on this ticket.
    bool connected() const { return static_cast<bool>(disconnect_func); }

private:
    std::function<void()> disconnect_func;
};

}  // namespace simplegui
