#include "simplegui/window.h"
#include "simplegui/control.h"
#include <QMainWindow>
#include <QWidget>
#include <QString>

namespace simplegui {

struct Window::Impl {
    QMainWindow qwindow;
    std::shared_ptr<Control> main_content;
    
    Impl(const std::string& title, int width, int height) {
        qwindow.setWindowTitle(QString::fromStdString(title));
        qwindow.resize(width, height);
    }
};

Window::Window(const std::string& title, int width, int height)
    : pimpl(std::make_unique<Impl>(title, width, height)) {}

Window::~Window() = default;

void Window::set_content(std::shared_ptr<Control> content) {
    pimpl->main_content = content; 
    pimpl->qwindow.setCentralWidget(content->get_qwidget());
}

void Window::show() {
    pimpl->qwindow.show();
}

}
