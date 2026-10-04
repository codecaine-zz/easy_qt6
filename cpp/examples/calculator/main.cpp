#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/grid.h"
#include "simplegui/text_input.h"
#include <iostream>

// Note: simplegui::Grid is for QTableWidget typically, but for a calculator layout
// a true layout grid might be better. However, since Grid in SimpleGUI is a table widget,
// let's build the calculator using nested HBoxes and VBoxes to demonstrate layout composition!

#include "simplegui/hbox.h"
#include "simplegui/button.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    simplegui::Window window("Calculator", 300, 400);

    auto layout = std::make_shared<simplegui::VBox>();

    auto display = std::make_shared<simplegui::TextInput>("0");
    // Ideally we'd make it read-only, but TextInput doesn't have set_readonly yet, so this works as a demo!
    layout->add_child(display);

    std::vector<std::vector<std::string>> buttons = {
        {"7", "8", "9", "/"},
        {"4", "5", "6", "*"},
        {"1", "2", "3", "-"},
        {"C", "0", "=", "+"}
    };

    for (const auto& row : buttons) {
        auto hbox = std::make_shared<simplegui::HBox>();
        for (const auto& text : row) {
            auto btn = std::make_shared<simplegui::Button>(text);
            btn->on_click([display, text]() {
                if (text == "C") {
                    display->set_text("");
                } else if (text == "=") {
                    display->set_text("Result"); // Dummy eval for demo
                } else {
                    display->set_text(display->get_text() + text);
                }
            });
            hbox->add_child(btn);
        }
        layout->add_child(hbox);
    }

    window.set_content(layout);
    window.show();

    return app.run();
}
