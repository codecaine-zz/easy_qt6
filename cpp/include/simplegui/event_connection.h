#pragma once
#include <functional>
#include <utility>

namespace simplegui {

class EventConnection {
public:
    EventConnection() = default;
    
    explicit EventConnection(std::function<void()> disconnect_func) 
        : disconnect_func(std::move(disconnect_func)) {}

    void disconnect() {
        if (disconnect_func) {
            disconnect_func();
            disconnect_func = nullptr;
        }
    }

private:
    std::function<void()> disconnect_func;
};

}
