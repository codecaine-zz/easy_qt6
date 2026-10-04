#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"
#include "simplegui/checkbox.h"
#include "simplegui/combo_box.h"
#include "simplegui/slider.h"
#include "simplegui/text_input.h"
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("linux_adwaita");

    simplegui::Window window("Preferences — Console", 560, 520);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(24);
    root->set_spacing(16);

    auto title = std::make_shared<simplegui::Label>("Terminal Preferences");
    title->set_style("color: #ffffff; font-size: 20px; font-weight: 700;");
    root->add_child(title);

    // Profile card
    auto prof_card = std::make_shared<simplegui::VBox>();
    prof_card->set_margins(16);
    prof_card->set_spacing(12);
    prof_card->set_style("background-color: #303030; border: 1px solid #3d3d3d; border-radius: 12px;");

    auto prof_title = std::make_shared<simplegui::Label>("Default Shell & Font");
    prof_title->set_style("color: #78aeed; font-size: 13px; font-weight: 700;");
    prof_card->add_child(prof_title);

    auto shell_row = std::make_shared<simplegui::HBox>();
    shell_row->set_spacing(12);
    auto shell_label = std::make_shared<simplegui::Label>("Command Shell:");
    shell_label->set_style("color: #deddda; font-size: 14px;");
    auto shell_combo = std::make_shared<simplegui::ComboBox>(std::vector<std::string>{"/bin/bash", "/usr/bin/zsh", "/usr/bin/fish"});
    shell_row->add_child(shell_label);
    shell_row->add_child(shell_combo);
    prof_card->add_child(shell_row);

    auto font_row = std::make_shared<simplegui::HBox>();
    font_row->set_spacing(12);
    auto font_label = std::make_shared<simplegui::Label>("Monospace Font:");
    font_label->set_style("color: #deddda; font-size: 14px;");
    auto font_combo = std::make_shared<simplegui::ComboBox>(std::vector<std::string>{"JetBrains Mono (11pt)", "Fira Code (11pt)", "Source Code Pro (11pt)", "Monospace (10pt)"});
    font_row->add_child(font_label);
    font_row->add_child(font_combo);
    prof_card->add_child(font_row);

    root->add_child(prof_card);

    // Appearance card
    auto app_card = std::make_shared<simplegui::VBox>();
    app_card->set_margins(16);
    app_card->set_spacing(12);
    app_card->set_style("background-color: #303030; border: 1px solid #3d3d3d; border-radius: 12px;");

    auto app_title = std::make_shared<simplegui::Label>("Window & Transparency");
    app_title->set_style("color: #57e389; font-size: 13px; font-weight: 700;");
    app_card->add_child(app_title);

    auto bell_check = std::make_shared<simplegui::Checkbox>("Audible terminal bell");
    bell_check->set_checked(false);
    app_card->add_child(bell_check);

    auto cursor_check = std::make_shared<simplegui::Checkbox>("Blinking block cursor");
    cursor_check->set_checked(true);
    app_card->add_child(cursor_check);

    auto slider_row = std::make_shared<simplegui::HBox>();
    slider_row->set_spacing(12);
    auto slider_label = std::make_shared<simplegui::Label>("Background Opacity (90%):");
    slider_label->set_style("color: #deddda; font-size: 14px;");
    auto opacity_slider = std::make_shared<simplegui::Slider>(50, 100, 90);
    opacity_slider->on_change([slider_label](int val) {
        slider_label->set_text("Background Opacity (" + std::to_string(val) + "%):");
    });
    slider_row->add_child(slider_label);
    slider_row->add_child(opacity_slider);
    app_card->add_child(slider_row);

    root->add_child(app_card);

    // Bottom action row
    auto bottom_row = std::make_shared<simplegui::HBox>();
    bottom_row->set_spacing(12);

    auto reset_btn = std::make_shared<simplegui::Button>("Reset Defaults");
    reset_btn->set_style("background-color: #383838; color: #deddda; border-radius: 8px; padding: 8px 18px;");

    auto save_btn = std::make_shared<simplegui::Button>("Save & Apply");
    save_btn->set_style("background-color: #3584e4; color: #ffffff; font-weight: 700; border-radius: 8px; padding: 8px 24px;");

    bottom_row->add_child(reset_btn);
    bottom_row->add_child(save_btn);
    root->add_child(bottom_row);

    window.set_content(root);
    window.show();

    return app.run();
}
