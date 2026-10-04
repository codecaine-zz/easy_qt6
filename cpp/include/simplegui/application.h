#pragma once
#include <memory>

namespace simplegui {

class Application {
public:
    Application(int& argc, char** argv);
    ~Application();
    int run();

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

}
