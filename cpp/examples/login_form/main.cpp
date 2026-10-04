#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/text_input.h"
#include "simplegui/password_input.h"
#include "simplegui/checkbox.h"
#include "simplegui/button.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    simplegui::Window window("Login", 350, 250);

    auto layout = std::make_shared<simplegui::VBox>();

    // Title
    auto title = std::make_shared<simplegui::Label>("Please Sign In");
    layout->add_child(title);

    // Username Row
    auto user_row = std::make_shared<simplegui::HBox>();
    auto user_lbl = std::make_shared<simplegui::Label>("Username:");
    auto user_in = std::make_shared<simplegui::TextInput>("Enter username");
    user_row->add_child(user_lbl);
    user_row->add_child(user_in);
    layout->add_child(user_row);

    // Password Row
    auto pass_row = std::make_shared<simplegui::HBox>();
    auto pass_lbl = std::make_shared<simplegui::Label>("Password:");
    auto pass_in = std::make_shared<simplegui::PasswordInput>("Enter password");
    pass_row->add_child(pass_lbl);
    pass_row->add_child(pass_in);
    layout->add_child(pass_row);

    // Remember Me
    auto remember = std::make_shared<simplegui::Checkbox>("Remember me", false);
    layout->add_child(remember);

    // Login Button
    auto btn = std::make_shared<simplegui::Button>("Login");
    layout->add_child(btn);

    // Status Label
    auto status = std::make_shared<simplegui::Label>("");
    layout->add_child(status);

    btn->on_click([user_in, pass_in, status]() {
        if (user_in->get_text().empty() || pass_in->get_text().empty()) {
            status->set_text("Error: Credentials cannot be empty.");
        } else {
            status->set_text("Success: Welcome back, " + user_in->get_text() + "!");
        }
    });

    window.set_content(layout);
    window.show();

    return app.run();
}
