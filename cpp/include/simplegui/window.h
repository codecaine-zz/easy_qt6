#pragma once
#include <memory>
#include <string>

namespace simplegui {

class Control;

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    void set_content(std::shared_ptr<Control> content);
    void show();
    bool save_screenshot(const std::string& filepath);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

}
