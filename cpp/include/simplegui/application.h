#pragma once
#include <memory>
#include <string>

namespace simplegui {

class Application {
public:
    Application(int& argc, char** argv);
    ~Application();
    int run();

    void set_theme(const std::string& theme_name);
    void set_stylesheet(const std::string& qss);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

}
