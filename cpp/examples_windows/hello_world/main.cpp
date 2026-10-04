#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("windows_fluent");

    simplegui::Window window("Windows 11 Fluent App", 420, 240);

    auto box = std::make_shared<simplegui::VBox>();
    box->set_margins(24);
    box->set_spacing(14);

    auto title = std::make_shared<simplegui::Label>("EasyQt6 Fluent Design");
    title->set_style("color: #ffffff; font-size: 20px; font-weight: 600;");

    auto sub = std::make_shared<simplegui::Label>("Windows 11 WinUI 3 styled dark mode with Mica background.");
    sub->set_style("color: #9d9d9d; font-size: 13px; line-height: 1.4;");

    auto btn = std::make_shared<simplegui::Button>("Click Me (Fluent Accent)");
    btn->set_style("background-color: #60cdff; color: #000000; font-weight: 600; padding: 8px 24px; border-radius: 4px;");

    btn->on_click([sub]() {
        sub->set_text("Windows 11 Fluent Button activated! Native C++ performance.");
    });

    box->add_child(title);
    box->add_child(sub);
    box->add_child(btn);

    window.set_content(box);
    window.show();

    return app.run();
}
