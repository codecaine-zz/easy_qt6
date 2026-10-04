#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    simplegui::Window window("Hello World", 400, 220);

    auto box = std::make_shared<simplegui::VBox>();
    box->set_margins(20);
    box->set_spacing(14);

    auto title = std::make_shared<simplegui::Label>("Welcome to EasyQt6");
    title->set_style("color: #f8fafc; font-size: 18px; font-weight: 700;");

    auto sub = std::make_shared<simplegui::Label>("Zero-boilerplate C++20 GUI library with native Qt6 performance.");
    sub->set_style("color: #94a3b8; font-size: 13px;");

    auto btn = std::make_shared<simplegui::Button>("Get Started");
    btn->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 10px 24px; border-radius: 6px;");

    btn->on_click([sub]() {
        sub->set_text("Button clicked! Enjoy building simple, high-performance Qt6 apps.");
    });

    box->add_child(title);
    box->add_child(sub);
    box->add_child(btn);

    window.set_content(box);
    window.show();

    return app.run();
}
