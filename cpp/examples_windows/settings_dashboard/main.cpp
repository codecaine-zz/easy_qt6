#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"
#include "simplegui/checkbox.h"
#include "simplegui/combo_box.h"
#include "simplegui/slider.h"
#include "simplegui/split_view.h"
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("windows_fluent");

    simplegui::Window window("Settings", 920, 620);

    // Left Navigation Rail
    auto nav = std::make_shared<simplegui::VBox>();
    nav->set_margins(16);
    nav->set_spacing(4);
    nav->set_style("background-color: #202020; border-right: 1px solid #2d2d2d;");

    auto user_info = std::make_shared<simplegui::VBox>();
    user_info->set_margins(8);
    user_info->set_spacing(2);
    auto user_name = std::make_shared<simplegui::Label>("User Name");
    user_name->set_style("color: #ffffff; font-size: 15px; font-weight: 600;");
    auto user_email = std::make_shared<simplegui::Label>("user@example.com");
    user_email->set_style("color: #9d9d9d; font-size: 12px; margin-bottom: 12px;");
    user_info->add_child(user_name);
    user_info->add_child(user_email);
    nav->add_child(user_info);

    std::vector<std::pair<std::string, bool>> nav_items = {
        {"System", true},
        {"Bluetooth & devices", false},
        {"Network & internet", false},
        {"Personalization", false},
        {"Apps", false},
        {"Accounts", false},
        {"Time & language", false},
        {"Windows Update", false}
    };

    for (const auto& item : nav_items) {
        auto nav_btn = std::make_shared<simplegui::Button>(item.first);
        if (item.second) {
            nav_btn->set_style(
                "background-color: #2d2d2d; color: #ffffff; font-weight: 600; "
                "text-align: left; padding: 10px 14px; border-radius: 4px; border-left: 3px solid #60cdff;"
            );
        } else {
            nav_btn->set_style(
                "background-color: transparent; color: #d0d0d0; font-weight: 400; "
                "text-align: left; padding: 10px 14px; border-radius: 4px;"
            );
        }
        nav->add_child(nav_btn);
    }

    // Right Content Area (Mica settings cards)
    auto content = std::make_shared<simplegui::VBox>();
    content->set_margins(28);
    content->set_spacing(14);
    content->set_style("background-color: #1b1b1b;");

    auto page_title = std::make_shared<simplegui::Label>("System");
    page_title->set_style("color: #ffffff; font-size: 26px; font-weight: 600; margin-bottom: 4px;");
    content->add_child(page_title);

    // Device Info Card
    auto dev_card = std::make_shared<simplegui::HBox>();
    dev_card->set_margins(16);
    dev_card->set_spacing(16);
    dev_card->set_style("background-color: #2b2b2b; border: 1px solid #363636; border-radius: 6px;");

    auto dev_info = std::make_shared<simplegui::VBox>();
    dev_info->set_spacing(4);
    auto pc_name = std::make_shared<simplegui::Label>("DESKTOP-EASYQT6");
    pc_name->set_style("color: #ffffff; font-size: 16px; font-weight: 600;");
    auto win_ver = std::make_shared<simplegui::Label>("Windows 11 Pro  •  Version 24H2  •  Build 26100");
    win_ver->set_style("color: #9d9d9d; font-size: 13px;");
    dev_info->add_child(pc_name);
    dev_info->add_child(win_ver);
    dev_card->add_child(dev_info);

    auto rename_btn = std::make_shared<simplegui::Button>("Rename");
    rename_btn->set_style("background-color: #383838; color: #ffffff; border-radius: 4px; padding: 8px 18px; font-weight: 600;");
    dev_card->add_child(rename_btn);
    content->add_child(dev_card);

    // Settings Setting Cards
    struct SettingItem {
        std::string title;
        std::string desc;
        std::string action;
    };

    std::vector<SettingItem> settings = {
        {"Display", "Monitors, brightness, night light, display profile", "HDR On"},
        {"Sound", "Volume levels, output and input devices", "72%"},
        {"Notifications", "Alerts from apps and system, do not disturb", "On"},
        {"Power & battery", "Sleep, battery usage, battery saver, power mode", "Best performance"},
        {"Storage", "Storage space, drive cleanup recommendations", "312 GB free"}
    };

    for (const auto& s : settings) {
        auto card = std::make_shared<simplegui::HBox>();
        card->set_margins(14);
        card->set_spacing(16);
        card->set_style("background-color: #2b2b2b; border: 1px solid #363636; border-radius: 6px;");

        auto label_col = std::make_shared<simplegui::VBox>();
        label_col->set_spacing(2);

        auto title_lbl = std::make_shared<simplegui::Label>(s.title);
        title_lbl->set_style("color: #ffffff; font-size: 14px; font-weight: 600;");

        auto desc_lbl = std::make_shared<simplegui::Label>(s.desc);
        desc_lbl->set_style("color: #9d9d9d; font-size: 12px;");

        label_col->add_child(title_lbl);
        label_col->add_child(desc_lbl);
        card->add_child(label_col);

        auto action_badge = std::make_shared<simplegui::Label>(s.action + "  ›");
        action_badge->set_style("color: #60cdff; font-size: 13px; font-weight: 600;");
        card->add_child(action_badge);

        content->add_child(card);
    }

    // SplitView
    auto split = std::make_shared<simplegui::SplitView>(true);
    split->add_child(nav);
    split->add_child(content);
    split->set_sizes(240, 680);

    window.set_content(split);
    window.show();

    return app.run();
}
