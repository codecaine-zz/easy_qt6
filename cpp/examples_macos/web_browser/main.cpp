#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/button.h"
#include "simplegui/text_input.h"
#include "simplegui/web_view.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    simplegui::Window window("Simple Browser", 1024, 768);

    auto layout = std::make_shared<simplegui::VBox>();

    // Toolbar
    auto toolbar = std::make_shared<simplegui::HBox>();
    auto url_bar = std::make_shared<simplegui::TextInput>("https://www.google.com");
    auto go_btn = std::make_shared<simplegui::Button>("Go");
    
    toolbar->add_child(url_bar);
    toolbar->add_child(go_btn);

    // Browser View
    auto web_view = std::make_shared<simplegui::WebView>("https://www.google.com");

    layout->add_child(toolbar);
    layout->add_child(web_view);

    go_btn->on_click([url_bar, web_view]() {
        web_view->set_url(url_bar->get_text());
    });

    window.set_content(layout);
    window.show();

    return app.run();
}
