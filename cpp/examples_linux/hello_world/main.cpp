#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("linux_adwaita");

    simplegui::Window window("Hello GNOME", 420, 240);

    auto box = std::make_shared<simplegui::VBox>();
    box->set_margins(24);
    box->set_spacing(14);

    auto title = std::make_shared<simplegui::Label>("EasyQt6 on Linux");
    title->set_style("color: #ffffff; font-size: 18px; font-weight: 700;");

    auto sub = std::make_shared<simplegui::Label>("Native Qt6 C++20 with GNOME Libadwaita Dark Theme.");
    sub->set_style("color: #9a9996; font-size: 13px; line-height: 1.4;");

    auto btn = std::make_shared<simplegui::Button>("Click Me (Libadwaita)");
    btn->set_style("background-color: #3584e4; color: #ffffff; font-weight: 700; padding: 10px 24px; border-radius: 8px;");

    btn->on_click([sub]() {
        sub->set_text("GNOME Libadwaita button clicked! Fast native Linux execution.");
    });

    box->add_child(title);
    box->add_child(sub);
    box->add_child(btn);

    window.set_content(box);
    window.show();

    return app.run();
}
