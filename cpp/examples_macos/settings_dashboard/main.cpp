#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/text_input.h"
#include "simplegui/password_input.h"
#include "simplegui/button.h"
#include "simplegui/checkbox.h"
#include "simplegui/dropdown.h"
#include "simplegui/combo_box.h"
#include "simplegui/slider.h"
#include "simplegui/knob.h"
#include "simplegui/color_well.h"
#include "simplegui/rating.h"
#include "simplegui/group_box.h"
#include "simplegui/tab_view.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    simplegui::Window window("Application Preferences", 520, 480);

    auto main_layout = std::make_shared<simplegui::VBox>();
    main_layout->set_margins(16);
    main_layout->set_spacing(12);

    auto tabs = std::make_shared<simplegui::TabView>();

    // --- TAB 1: Profile & Account ---
    auto tab_profile = std::make_shared<simplegui::VBox>();
    tab_profile->set_margins(12);
    tab_profile->set_spacing(12);

    auto grp_user = std::make_shared<simplegui::GroupBox>("User Information");
    auto user_vbox = std::make_shared<simplegui::VBox>();
    user_vbox->set_margins(10);
    user_vbox->set_spacing(8);

    auto row_name = std::make_shared<simplegui::HBox>();
    row_name->set_spacing(10);
    auto lbl_name = std::make_shared<simplegui::Label>("Full Name:");
    auto input_name = std::make_shared<simplegui::TextInput>("Jerome Scott");
    row_name->add_child(lbl_name);
    row_name->add_child(input_name);

    auto row_email = std::make_shared<simplegui::HBox>();
    row_email->set_spacing(10);
    auto lbl_email = std::make_shared<simplegui::Label>("Email Address:");
    auto input_email = std::make_shared<simplegui::TextInput>("developer@easyqt6.org");
    row_email->add_child(lbl_email);
    row_email->add_child(input_email);

    auto row_pass = std::make_shared<simplegui::HBox>();
    row_pass->set_spacing(10);
    auto lbl_pass = std::make_shared<simplegui::Label>("API Key / Secret:");
    auto input_pass = std::make_shared<simplegui::PasswordInput>("eqt6_live_9837428472");
    row_pass->add_child(lbl_pass);
    row_pass->add_child(input_pass);

    user_vbox->add_child(row_name);
    user_vbox->add_child(row_email);
    user_vbox->add_child(row_pass);
    grp_user->add_child(user_vbox);

    auto grp_region = std::make_shared<simplegui::GroupBox>("Regional Settings");
    auto region_vbox = std::make_shared<simplegui::VBox>();
    region_vbox->set_margins(10);
    region_vbox->set_spacing(8);

    auto row_lang = std::make_shared<simplegui::HBox>();
    row_lang->set_spacing(10);
    auto lbl_lang = std::make_shared<simplegui::Label>("Language:");
    auto drop_lang = std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"English (US)", "Spanish", "German", "Japanese"});
    row_lang->add_child(lbl_lang);
    row_lang->add_child(drop_lang);

    auto row_tz = std::make_shared<simplegui::HBox>();
    row_tz->set_spacing(10);
    auto lbl_tz = std::make_shared<simplegui::Label>("Timezone:");
    auto drop_tz = std::make_shared<simplegui::ComboBox>(std::vector<std::string>{"UTC-05:00 Eastern", "UTC-06:00 Central", "UTC-08:00 Pacific", "UTC+00:00 London"});
    row_tz->add_child(lbl_tz);
    row_tz->add_child(drop_tz);

    region_vbox->add_child(row_lang);
    region_vbox->add_child(row_tz);
    grp_region->add_child(region_vbox);

    tab_profile->add_child(grp_user);
    tab_profile->add_child(grp_region);
    tab_profile->add_stretch();

    // --- TAB 2: Appearance & Hardware ---
    auto tab_appearance = std::make_shared<simplegui::VBox>();
    tab_appearance->set_margins(12);
    tab_appearance->set_spacing(12);

    auto grp_theme = std::make_shared<simplegui::GroupBox>("Theme & Colors");
    auto theme_vbox = std::make_shared<simplegui::VBox>();
    theme_vbox->set_margins(10);
    theme_vbox->set_spacing(8);

    auto chk_dark = std::make_shared<simplegui::Checkbox>("Enable High-Contrast Dark Mode", true);
    auto chk_anim = std::make_shared<simplegui::Checkbox>("Hardware-accelerated animations", true);

    auto row_color = std::make_shared<simplegui::HBox>();
    row_color->set_spacing(10);
    auto lbl_color = std::make_shared<simplegui::Label>("Accent Color:");
    auto color_well = std::make_shared<simplegui::ColorWell>("#2563eb");
    row_color->add_child(lbl_color);
    row_color->add_child(color_well);
    row_color->add_stretch();

    auto row_scale = std::make_shared<simplegui::HBox>();
    row_scale->set_spacing(10);
    auto lbl_scale = std::make_shared<simplegui::Label>("UI Scaling (%):");
    auto slider_scale = std::make_shared<simplegui::Slider>(50, 200, 100);
    row_scale->add_child(lbl_scale);
    row_scale->add_child(slider_scale);

    theme_vbox->add_child(chk_dark);
    theme_vbox->add_child(chk_anim);
    theme_vbox->add_child(row_color);
    theme_vbox->add_child(row_scale);
    grp_theme->add_child(theme_vbox);

    auto grp_audio = std::make_shared<simplegui::GroupBox>("Audio & Controls");
    auto audio_hbox = std::make_shared<simplegui::HBox>();
    audio_hbox->set_margins(10);
    audio_hbox->set_spacing(16);

    auto audio_left = std::make_shared<simplegui::VBox>();
    audio_left->add_child(std::make_shared<simplegui::Label>("Master Volume"));
    auto knob = std::make_shared<simplegui::Knob>(0, 100, 75);
    audio_left->add_child(knob);

    auto audio_right = std::make_shared<simplegui::VBox>();
    audio_right->add_child(std::make_shared<simplegui::Label>("User Satisfaction"));
    auto rating = std::make_shared<simplegui::Rating>(5);
    rating->set_rating(5);
    audio_right->add_child(rating);

    audio_hbox->add_child(audio_left);
    audio_hbox->add_child(audio_right);
    audio_hbox->add_stretch();
    grp_audio->add_child(audio_hbox);

    tab_appearance->add_child(grp_theme);
    tab_appearance->add_child(grp_audio);
    tab_appearance->add_stretch();

    tabs->add_tab("Profile & Account", tab_profile);
    tabs->add_tab("Appearance & Audio", tab_appearance);

    // --- BOTTOM ACTION BAR ---
    auto bottom_bar = std::make_shared<simplegui::HBox>();
    bottom_bar->set_spacing(10);

    auto btn_reset = std::make_shared<simplegui::Button>("Reset Defaults");
    auto btn_cancel = std::make_shared<simplegui::Button>("Cancel");
    auto btn_save = std::make_shared<simplegui::Button>("Save Changes");
    btn_save->set_style("background-color: #2563eb; color: #ffffff; font-weight: bold; border: 1px solid #3b82f6;");

    bottom_bar->add_child(btn_reset);
    bottom_bar->add_stretch();
    bottom_bar->add_child(btn_cancel);
    bottom_bar->add_child(btn_save);

    main_layout->add_child(tabs);
    main_layout->add_child(bottom_bar);

    window.set_content(main_layout);
    window.show();

    return app.run();
}
