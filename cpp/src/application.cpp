#include "simplegui/application.h"
#include <QApplication>

namespace simplegui {

struct Application::Impl {
    QApplication qapp;
    Impl(int& argc, char** argv) : qapp(argc, argv) {}
};

Application::Application(int& argc, char** argv)
    : pimpl(std::make_unique<Impl>(argc, argv)) {}

Application::~Application() = default;

int Application::run() {
    return pimpl->qapp.exec();
}

}
