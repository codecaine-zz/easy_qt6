#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    simplegui::Window window("Hello World", 300, 200);

    auto vbox = std::make_shared<simplegui::VBox>();
    auto label = std::make_shared<simplegui::Label>("Welcome to SimpleGUI!");
    auto btn = std::make_shared<simplegui::Button>("Click Me");

    btn->on_click([label]() {
        label->set_text("Hello from C++ and Qt6!");
    });

    vbox->add_child(label);
    vbox->add_child(btn);

    window.set_content(vbox);
    window.show();

    return app.run();
}
