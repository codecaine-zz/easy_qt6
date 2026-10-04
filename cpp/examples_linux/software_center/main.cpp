#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"
#include "simplegui/text_input.h"
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("linux_adwaita");

    simplegui::Window window("GNOME Software", 820, 560);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(20);
    root->set_spacing(16);

    // Search bar row
    auto top_bar = std::make_shared<simplegui::HBox>();
    top_bar->set_spacing(12);

    auto search = std::make_shared<simplegui::TextInput>("Search installed & Flathub apps...");
    search->set_style("padding: 10px 14px; font-size: 14px;");

    auto explore_btn = std::make_shared<simplegui::Button>("Explore");
    explore_btn->set_style("background-color: #3584e4; color: #ffffff; font-weight: 700; border-radius: 8px; padding: 10px 20px;");

    auto installed_btn = std::make_shared<simplegui::Button>("Installed (48)");
    installed_btn->set_style("background-color: #383838; color: #ffffff; font-weight: 600; border-radius: 8px; padding: 10px 16px;");

    top_bar->add_child(search);
    top_bar->add_child(explore_btn);
    top_bar->add_child(installed_btn);
    root->add_child(top_bar);

    // Section header
    auto header = std::make_shared<simplegui::Label>("Featured Flatpaks");
    header->set_style("color: #ffffff; font-size: 18px; font-weight: 700; margin-top: 8px;");
    root->add_child(header);

    // Apps list
    struct AppCard {
        std::string name;
        std::string category;
        std::string desc;
        std::string status;
        std::string size;
    };

    std::vector<AppCard> apps = {
        {"Blender 4.2", "Graphics & 3D", "Free and open source 3D creation suite.", "Installed", "820 MB"},
        {"Inkscape", "Vector Graphics", "Professional vector graphics editor for Linux.", "Install", "145 MB"},
        {"VSCodium", "Development", "Free/Libre Open Source Software Binaries of VS Code.", "Installed", "210 MB"},
        {"OBS Studio", "Audio & Video", "Live streaming and video recording software.", "Install", "310 MB"},
        {"Audacity", "Audio Production", "Multi-track audio editor and recorder.", "Installed", "95 MB"}
    };

    for (const auto& a : apps) {
        auto card = std::make_shared<simplegui::HBox>();
        card->set_margins(12);
        card->set_spacing(14);
        card->set_style("background-color: #303030; border: 1px solid #3d3d3d; border-radius: 10px;");

        auto info = std::make_shared<simplegui::VBox>();
        info->set_spacing(4);

        auto app_name = std::make_shared<simplegui::Label>(a.name + "  •  " + a.category);
        app_name->set_style("color: #ffffff; font-size: 15px; font-weight: 700;");

        auto app_desc = std::make_shared<simplegui::Label>(a.desc + " (" + a.size + ")");
        app_desc->set_style("color: #9a9996; font-size: 13px;");

        info->add_child(app_name);
        info->add_child(app_desc);
        card->add_child(info);

        auto action_btn = std::make_shared<simplegui::Button>(a.status);
        if (a.status == "Install") {
            action_btn->set_style("background-color: #3584e4; color: #ffffff; font-weight: 700; border-radius: 8px; padding: 8px 20px;");
            action_btn->on_click([action_btn]() {
                action_btn->set_text("Installed");
                action_btn->set_style("background-color: #383838; color: #57e389; font-weight: 700; border-radius: 8px; padding: 8px 20px;");
            });
        } else {
            action_btn->set_style("background-color: #383838; color: #57e389; font-weight: 700; border-radius: 8px; padding: 8px 20px;");
        }
        card->add_child(action_btn);

        root->add_child(card);
    }

    window.set_content(root);
    window.show();

    return app.run();
}
