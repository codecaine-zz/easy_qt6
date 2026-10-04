#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/text_input.h"
#include "simplegui/password_input.h"
#include "simplegui/checkbox.h"
#include "simplegui/button.h"
#include <iostream>

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    simplegui::Window window("Sign In", 420, 320);

    auto layout = std::make_shared<simplegui::VBox>();
    layout->set_margins(24);
    layout->set_spacing(12);

    auto sec_lbl = std::make_shared<simplegui::Label>("APPLE ACCOUNT SIGN IN");
    sec_lbl->set_style("color: #818cf8; font-size: 11px; font-weight: 700; letter-spacing: 0.8px;");
    layout->add_child(sec_lbl);

    auto user_input = std::make_shared<simplegui::TextInput>("developer@easyqt6.org");
    layout->add_child(user_input);

    auto pass_lbl = std::make_shared<simplegui::Label>("PASSWORD / PASSKEY");
    pass_lbl->set_style("color: #818cf8; font-size: 11px; font-weight: 700; letter-spacing: 0.8px;");
    layout->add_child(pass_lbl);

    auto pass_input = std::make_shared<simplegui::PasswordInput>("super_secret_master_password");
    layout->add_child(pass_input);

    auto remember = std::make_shared<simplegui::Checkbox>("Keep me signed in on this Mac", true);
    layout->add_child(remember);

    auto login_btn = std::make_shared<simplegui::Button>("Sign In with Apple");
    login_btn->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 10px 18px; border-radius: 6px; font-size: 14px;");

    login_btn->on_click([user_input]() {
        std::cout << "Signed in as: " << user_input->get_text() << std::endl;
    });

    layout->add_child(login_btn);

    window.set_content(layout);
    window.show();

    return app.run();
}
