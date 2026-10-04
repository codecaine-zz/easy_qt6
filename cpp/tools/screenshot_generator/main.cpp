#include <QApplication>
#include <QDir>
#include <iostream>
#include <memory>
#include <vector>

#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/label.h"
#include "simplegui/text_input.h"
#include "simplegui/password_input.h"
#include "simplegui/search_field.h"
#include "simplegui/button.h"
#include "simplegui/image_button.h"
#include "simplegui/image.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/checkbox.h"
#include "simplegui/slider.h"
#include "simplegui/radio.h"
#include "simplegui/dropdown.h"
#include "simplegui/combo_box.h"
#include "simplegui/textarea.h"
#include "simplegui/progress_indicator.h"
#include "simplegui/circular_progress.h"
#include "simplegui/number_input.h"
#include "simplegui/knob.h"
#include "simplegui/date_picker.h"
#include "simplegui/group_box.h"
#include "simplegui/tab_view.h"
#include "simplegui/color_well.h"
#include "simplegui/grid.h"
#include "simplegui/link.h"
#include "simplegui/rating.h"
#include "simplegui/breadcrumbs.h"
#include "simplegui/split_view.h"
#include "simplegui/scroll_view.h"
#include "simplegui/sparkline.h"
#include "simplegui/vfd_meter.h"
#include "simplegui/stat_card.h"
#include "simplegui/composition_bar.h"
#include "simplegui/status_pill.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    std::string out_dir = "screenshots";
    if (argc > 1) {
        out_dir = argv[1];
    }
    QDir().mkpath(QString::fromStdString(out_dir));

    auto capture_in_window = [&](const std::string& name, const std::string& title, std::shared_ptr<simplegui::Control> ctrl, int w = 320, int h = 120) {
        simplegui::Window win(title, w, h);
        auto vbox = std::make_shared<simplegui::VBox>();
        vbox->set_margins(16);
        vbox->add_child(ctrl);
        win.set_content(vbox);
        std::string filepath = out_dir + "/" + name + ".png";
        win.save_screenshot(filepath);
        std::cout << "Saved: " << filepath << std::endl;
    };

    std::cout << "Generating polished Qt6 screenshots with modern design..." << std::endl;

    // --- CONTROLS ---
    auto btn = std::make_shared<simplegui::Button>("Primary Action");
    btn->set_style("background-color: #2563eb; color: #ffffff; font-weight: 600; border: 1px solid #3b82f6;");
    capture_in_window("button", "Button", btn, 260, 90);

    capture_in_window("label", "Label", std::make_shared<simplegui::Label>("EasyQt6: Zero-boilerplate C++ GUI"), 300, 80);
    capture_in_window("text_input", "TextInput", std::make_shared<simplegui::TextInput>("Type your text here..."), 320, 90);
    capture_in_window("password_input", "PasswordInput", std::make_shared<simplegui::PasswordInput>("secret_password_123"), 320, 90);
    capture_in_window("search_field", "SearchField", std::make_shared<simplegui::SearchField>("Search documents..."), 320, 90);
    capture_in_window("checkbox", "Checkbox", std::make_shared<simplegui::Checkbox>("Enable hardware acceleration", true), 320, 80);
    capture_in_window("radio", "Radio", std::make_shared<simplegui::Radio>("Standard Delivery (3-5 business days)"), 340, 80);

    auto slider = std::make_shared<simplegui::Slider>(0, 100, 65);
    capture_in_window("slider", "Slider", slider, 320, 80);

    capture_in_window("dropdown", "Dropdown", std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"Option A", "Option B", "Option C"}), 300, 90);
    capture_in_window("combo_box", "ComboBox", std::make_shared<simplegui::ComboBox>(std::vector<std::string>{"San Francisco", "London", "Tokyo", "Paris"}), 320, 90);
    capture_in_window("number_input", "NumberInput", std::make_shared<simplegui::NumberInput>(0, 100, 42), 240, 90);
    capture_in_window("knob", "Knob", std::make_shared<simplegui::Knob>(0, 100, 70), 160, 160);
    capture_in_window("date_picker", "DatePicker", std::make_shared<simplegui::DatePicker>(), 300, 100);
    capture_in_window("color_well", "ColorWell", std::make_shared<simplegui::ColorWell>("#2563eb"), 200, 90);

    auto progress = std::make_shared<simplegui::ProgressIndicator>();
    progress->set_value(65);
    capture_in_window("progress_indicator", "ProgressIndicator", progress, 320, 80);

    auto circ_progress = std::make_shared<simplegui::CircularProgress>();
    circ_progress->set_value(75);
    capture_in_window("circular_progress", "CircularProgress", circ_progress, 160, 160);

    auto rating = std::make_shared<simplegui::Rating>(5);
    rating->set_rating(4);
    capture_in_window("rating", "Rating", rating, 240, 80);

    capture_in_window("breadcrumbs", "Breadcrumbs", std::make_shared<simplegui::Breadcrumbs>(std::vector<std::string>{"Home", "Projects", "EasyQt6", "Settings"}), 380, 80);
    capture_in_window("textarea", "Textarea", std::make_shared<simplegui::Textarea>("EasyQt6 provides clean, modern C++ wrappers\naround native Qt 6 widgets.\nFast, safe, and intuitive."), 380, 130);
    capture_in_window("link", "Link", std::make_shared<simplegui::Link>("Visit Qt Official Documentation", "https://doc.qt.io/"), 320, 80);
    capture_in_window("image_button", "ImageButton", std::make_shared<simplegui::ImageButton>("", "Launch Mission"), 260, 90);

    // TMOG Precision Telemetry Controls
    auto spark = std::make_shared<simplegui::Sparkline>("#06b6d4");
    spark->set_samples({15, 20, 24, 18, 30, 45, 60, 52, 40, 35, 48, 70, 85, 65, 42, 38, 50});
    capture_in_window("sparkline", "Sparkline Telemetry Graph", spark, 340, 100);

    auto vfd = std::make_shared<simplegui::VfdMeter>(16, false);
    vfd->set_value(78.0);
    capture_in_window("vfd_meter", "VFD Segmented Level Meter", vfd, 300, 70);

    auto stat = std::make_shared<simplegui::StatCard>("CPU LOAD", "4.85 GHz", "Turbo Boost Active", "#06b6d4");
    capture_in_window("stat_card", "StatCard Telemetry Readout", stat, 220, 100);

    auto comp = std::make_shared<simplegui::CompositionBar>();
    comp->add_segment("App", 16.0, "#2563eb");
    comp->add_segment("Wired", 8.0, "#06b6d4");
    comp->add_segment("Cache", 6.0, "#f59e0b");
    comp->add_segment("Free", 34.0, "#27272a");
    capture_in_window("composition_bar", "CompositionBar Segmented Bar", comp, 320, 70);

    auto pill = std::make_shared<simplegui::StatusPill>("LIVE 1.0.0 RTM", "#10b981");
    capture_in_window("status_pill", "StatusPill Badge Indicator", pill, 200, 70);

    // --- LAYOUTS ---
    {
        simplegui::Window win("VBox Layout", 280, 180);
        auto vbox = std::make_shared<simplegui::VBox>();
        vbox->set_margins(16);
        vbox->set_spacing(10);
        vbox->add_child(std::make_shared<simplegui::Button>("Top Button"));
        vbox->add_child(std::make_shared<simplegui::Button>("Middle Button"));
        vbox->add_child(std::make_shared<simplegui::Button>("Bottom Button"));
        win.set_content(vbox);
        win.save_screenshot(out_dir + "/vbox.png");
        std::cout << "Saved: " << out_dir << "/vbox.png" << std::endl;
    }

    {
        simplegui::Window win("HBox Layout", 360, 90);
        auto hbox = std::make_shared<simplegui::HBox>();
        hbox->set_margins(16);
        hbox->set_spacing(10);
        hbox->add_child(std::make_shared<simplegui::Button>("Left"));
        hbox->add_child(std::make_shared<simplegui::Button>("Center"));
        hbox->add_child(std::make_shared<simplegui::Button>("Right"));
        win.set_content(hbox);
        win.save_screenshot(out_dir + "/hbox.png");
        std::cout << "Saved: " << out_dir << "/hbox.png" << std::endl;
    }

    {
        simplegui::Window win("Grid Layout", 400, 180);
        auto grid = std::make_shared<simplegui::Grid>(3, 3, std::vector<std::string>{"ID", "Item", "Status"});
        grid->set_cell(0, 0, "101");
        grid->set_cell(0, 1, "Widgets");
        grid->set_cell(0, 2, "Active");
        grid->set_cell(1, 0, "102");
        grid->set_cell(1, 1, "WebEngine");
        grid->set_cell(1, 2, "Ready");
        grid->set_cell(2, 0, "103");
        grid->set_cell(2, 1, "Layouts");
        grid->set_cell(2, 2, "Compiled");
        win.set_content(grid);
        win.save_screenshot(out_dir + "/grid.png");
        std::cout << "Saved: " << out_dir << "/grid.png" << std::endl;
    }

    {
        simplegui::Window win("GroupBox Layout", 360, 150);
        auto group_box = std::make_shared<simplegui::GroupBox>("User Preferences");
        auto group_vbox = std::make_shared<simplegui::VBox>();
        group_vbox->set_margins(12);
        group_vbox->set_spacing(8);
        group_vbox->add_child(std::make_shared<simplegui::Checkbox>("Receive email notifications", true));
        group_vbox->add_child(std::make_shared<simplegui::Checkbox>("Automatic updates", false));
        group_box->add_child(group_vbox);
        win.set_content(group_box);
        win.save_screenshot(out_dir + "/group_box.png");
        std::cout << "Saved: " << out_dir << "/group_box.png" << std::endl;
    }

    {
        simplegui::Window win("TabView Layout", 400, 200);
        auto tab_view = std::make_shared<simplegui::TabView>();
        auto tab1 = std::make_shared<simplegui::VBox>();
        tab1->set_margins(12);
        tab1->set_spacing(8);
        tab1->add_child(std::make_shared<simplegui::Label>("Content for General Settings"));
        tab1->add_child(std::make_shared<simplegui::Button>("Save Settings"));
        auto tab2 = std::make_shared<simplegui::VBox>();
        tab2->set_margins(12);
        tab2->add_child(std::make_shared<simplegui::Label>("Content for Account Details"));
        tab_view->add_tab("General", tab1);
        tab_view->add_tab("Account", tab2);
        win.set_content(tab_view);
        win.save_screenshot(out_dir + "/tab_view.png");
        std::cout << "Saved: " << out_dir << "/tab_view.png" << std::endl;
    }

    {
        simplegui::Window win("SplitView Layout", 420, 160);
        auto split_view = std::make_shared<simplegui::SplitView>(true);
        split_view->add_child(std::make_shared<simplegui::Label>("Left Sidebar Navigation"));
        split_view->add_child(std::make_shared<simplegui::Label>("Main Content Area"));
        win.set_content(split_view);
        win.save_screenshot(out_dir + "/split_view.png");
        std::cout << "Saved: " << out_dir << "/split_view.png" << std::endl;
    }

    {
        simplegui::Window win("ScrollView Layout", 340, 180);
        auto scroll_view = std::make_shared<simplegui::ScrollView>();
        auto content = std::make_shared<simplegui::VBox>();
        content->set_margins(12);
        content->set_spacing(6);
        for (int i = 1; i <= 8; ++i) {
            content->add_child(std::make_shared<simplegui::Label>("Scrollable list entry #" + std::to_string(i)));
        }
        scroll_view->set_content(content);
        win.set_content(scroll_view);
        win.save_screenshot(out_dir + "/scroll_view.png");
        std::cout << "Saved: " << out_dir << "/scroll_view.png" << std::endl;
    }

    // --- FULL DEMO APPLICATIONS ---
    {
        simplegui::Window win("Hello World", 320, 160);
        auto vbox = std::make_shared<simplegui::VBox>();
        vbox->set_margins(16);
        vbox->set_spacing(10);
        auto label = std::make_shared<simplegui::Label>("Welcome to EasyQt6!");
        auto click_btn = std::make_shared<simplegui::Button>("Click Me");
        click_btn->set_style("background-color: #2563eb; color: #ffffff; font-weight: 600; border: 1px solid #3b82f6;");
        vbox->add_child(label);
        vbox->add_child(click_btn);
        win.set_content(vbox);
        win.save_screenshot(out_dir + "/example_hello_world.png");
        std::cout << "Saved: " << out_dir << "/example_hello_world.png" << std::endl;
    }

    {
        simplegui::Window win("Login", 360, 260);
        auto layout = std::make_shared<simplegui::VBox>();
        layout->set_margins(20);
        layout->set_spacing(10);

        auto user_label = std::make_shared<simplegui::Label>("Username:");
        auto user_input = std::make_shared<simplegui::TextInput>("admin");
        auto pass_label = std::make_shared<simplegui::Label>("Password:");
        auto pass_input = std::make_shared<simplegui::PasswordInput>("secret");
        auto remember = std::make_shared<simplegui::Checkbox>("Remember Me", true);
        auto login_btn = std::make_shared<simplegui::Button>("Log In");
        login_btn->set_style("background-color: #2563eb; color: #ffffff; font-weight: 600; border: 1px solid #3b82f6;");

        layout->add_child(user_label);
        layout->add_child(user_input);
        layout->add_child(pass_label);
        layout->add_child(pass_input);
        layout->add_child(remember);
        layout->add_child(login_btn);
        win.set_content(layout);
        win.save_screenshot(out_dir + "/example_login_form.png");
        std::cout << "Saved: " << out_dir << "/example_login_form.png" << std::endl;
    }

    {
        simplegui::Window win("Calculator", 320, 360);
        auto layout = std::make_shared<simplegui::VBox>();
        layout->set_margins(16);
        layout->set_spacing(10);
        auto display = std::make_shared<simplegui::TextInput>("1337");
        layout->add_child(display);

        const std::vector<std::vector<std::string>> buttons = {
            {"7", "8", "9", "/"},
            {"4", "5", "6", "*"},
            {"1", "2", "3", "-"},
            {"0", "C", "=", "+"}
        };

        for (const auto& row : buttons) {
            auto row_box = std::make_shared<simplegui::HBox>();
            row_box->set_spacing(8);
            for (const auto& text : row) {
                auto b = std::make_shared<simplegui::Button>(text);
                if (text == "=" || text == "C") {
                    b->set_style("background-color: #2563eb; color: #ffffff; font-weight: bold; border: 1px solid #3b82f6;");
                }
                row_box->add_child(b);
            }
            layout->add_child(row_box);
        }
        win.set_content(layout);
        win.save_screenshot(out_dir + "/example_calculator.png");
        std::cout << "Saved: " << out_dir << "/example_calculator.png" << std::endl;
    }

    // 4. MIXED LAYOUT: Settings Dashboard
    {
        simplegui::Window win("Application Preferences", 540, 480);
        auto main_layout = std::make_shared<simplegui::VBox>();
        main_layout->set_margins(16);
        main_layout->set_spacing(12);

        auto tabs = std::make_shared<simplegui::TabView>();

        // Tab 1: Profile
        auto tab_profile = std::make_shared<simplegui::VBox>();
        tab_profile->set_margins(12);
        tab_profile->set_spacing(10);

        auto grp_user = std::make_shared<simplegui::GroupBox>("User Information");
        auto user_vbox = std::make_shared<simplegui::VBox>();
        user_vbox->set_margins(10);
        user_vbox->set_spacing(8);

        auto row_name = std::make_shared<simplegui::HBox>();
        row_name->set_spacing(10);
        row_name->add_child(std::make_shared<simplegui::Label>("Full Name:"));
        row_name->add_child(std::make_shared<simplegui::TextInput>("Jerome Scott"));

        auto row_email = std::make_shared<simplegui::HBox>();
        row_email->set_spacing(10);
        row_email->add_child(std::make_shared<simplegui::Label>("Email Address:"));
        row_email->add_child(std::make_shared<simplegui::TextInput>("developer@easyqt6.org"));

        user_vbox->add_child(row_name);
        user_vbox->add_child(row_email);
        grp_user->add_child(user_vbox);

        auto grp_region = std::make_shared<simplegui::GroupBox>("Regional Settings");
        auto region_vbox = std::make_shared<simplegui::VBox>();
        region_vbox->set_margins(10);
        region_vbox->set_spacing(8);

        auto row_lang = std::make_shared<simplegui::HBox>();
        row_lang->set_spacing(10);
        row_lang->add_child(std::make_shared<simplegui::Label>("Language:"));
        row_lang->add_child(std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"English (US)", "Spanish", "German"}));

        region_vbox->add_child(row_lang);
        grp_region->add_child(region_vbox);

        tab_profile->add_child(grp_user);
        tab_profile->add_child(grp_region);
        tab_profile->add_stretch();

        // Tab 2: Appearance & Audio
        auto tab_appearance = std::make_shared<simplegui::VBox>();
        tab_appearance->set_margins(12);
        tab_appearance->set_spacing(10);

        auto grp_theme = std::make_shared<simplegui::GroupBox>("Theme & Colors");
        auto theme_vbox = std::make_shared<simplegui::VBox>();
        theme_vbox->set_margins(10);
        theme_vbox->set_spacing(8);

        auto chk_dark = std::make_shared<simplegui::Checkbox>("Enable High-Contrast Dark Mode", true);
        auto row_color = std::make_shared<simplegui::HBox>();
        row_color->set_spacing(10);
        row_color->add_child(std::make_shared<simplegui::Label>("Accent Color:"));
        row_color->add_child(std::make_shared<simplegui::ColorWell>("#2563eb"));
        row_color->add_stretch();

        auto row_scale = std::make_shared<simplegui::HBox>();
        row_scale->set_spacing(10);
        row_scale->add_child(std::make_shared<simplegui::Label>("UI Scale:"));
        row_scale->add_child(std::make_shared<simplegui::Slider>(50, 150, 100));

        theme_vbox->add_child(chk_dark);
        theme_vbox->add_child(row_color);
        theme_vbox->add_child(row_scale);
        grp_theme->add_child(theme_vbox);

        auto grp_audio = std::make_shared<simplegui::GroupBox>("Audio & Controls");
        auto audio_hbox = std::make_shared<simplegui::HBox>();
        audio_hbox->set_margins(10);
        audio_hbox->set_spacing(16);

        auto audio_left = std::make_shared<simplegui::VBox>();
        audio_left->add_child(std::make_shared<simplegui::Label>("Master Volume"));
        audio_left->add_child(std::make_shared<simplegui::Knob>(0, 100, 75));

        auto audio_right = std::make_shared<simplegui::VBox>();
        audio_right->add_child(std::make_shared<simplegui::Label>("Satisfaction"));
        auto rtg = std::make_shared<simplegui::Rating>(5);
        rtg->set_rating(5);
        audio_right->add_child(rtg);

        audio_hbox->add_child(audio_left);
        audio_hbox->add_child(audio_right);
        audio_hbox->add_stretch();
        grp_audio->add_child(audio_hbox);

        tab_appearance->add_child(grp_theme);
        tab_appearance->add_child(grp_audio);
        tab_appearance->add_stretch();

        tabs->add_tab("Profile & Account", tab_profile);
        tabs->add_tab("Appearance & Audio", tab_appearance);

        // Bottom Bar
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
        win.set_content(main_layout);
        win.save_screenshot(out_dir + "/example_settings_dashboard.png");
        std::cout << "Saved: " << out_dir << "/example_settings_dashboard.png" << std::endl;
    }

    // 5. MIXED LAYOUT: Data Explorer
    {
        simplegui::Window win("Enterprise Data Explorer", 680, 480);
        auto root = std::make_shared<simplegui::VBox>();
        root->set_margins(16);
        root->set_spacing(10);

        auto crumbs = std::make_shared<simplegui::Breadcrumbs>(std::vector<std::string>{"Organization", "Analytics", "Live Transactions"});

        auto toolbar = std::make_shared<simplegui::HBox>();
        toolbar->set_spacing(8);
        auto search = std::make_shared<simplegui::SearchField>("Search customers...");
        auto status_filter = std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"All Statuses", "Paid", "Pending", "Failed"});
        auto btn_add = std::make_shared<simplegui::Button>("+ New Record");
        btn_add->set_style("background-color: #2563eb; color: #ffffff; font-weight: 600; border: 1px solid #3b82f6;");
        auto btn_refresh = std::make_shared<simplegui::Button>("Refresh");

        toolbar->add_child(search);
        toolbar->add_child(status_filter);
        toolbar->add_child(btn_add);
        toolbar->add_child(btn_refresh);

        auto split = std::make_shared<simplegui::SplitView>(true);

        auto sidebar = std::make_shared<simplegui::VBox>();
        sidebar->set_margins(8);
        sidebar->set_spacing(10);

        auto grp_summary = std::make_shared<simplegui::GroupBox>("Overview");
        auto summary_vbox = std::make_shared<simplegui::VBox>();
        summary_vbox->set_spacing(6);
        summary_vbox->add_child(std::make_shared<simplegui::Label>("Revenue: $148,200"));
        auto circ = std::make_shared<simplegui::CircularProgress>();
        circ->set_value(84);
        summary_vbox->add_child(circ);
        grp_summary->add_child(summary_vbox);

        auto grp_filter = std::make_shared<simplegui::GroupBox>("Quick Filters");
        auto filter_vbox = std::make_shared<simplegui::VBox>();
        filter_vbox->set_spacing(6);
        filter_vbox->add_child(std::make_shared<simplegui::Checkbox>("VIP Only", false));
        filter_vbox->add_child(std::make_shared<simplegui::Checkbox>("High Value", true));
        grp_filter->add_child(filter_vbox);

        sidebar->add_child(grp_summary);
        sidebar->add_child(grp_filter);
        sidebar->add_stretch();

        auto grid = std::make_shared<simplegui::Grid>(6, 5, std::vector<std::string>{"ID", "Customer", "Tier", "Amount", "Status"});
        const std::vector<std::vector<std::string>> data = {
            {"#401", "Acme Corporation", "Enterprise", "$12,450", "Paid"},
            {"#402", "Nova Solutions LLC", "Pro", "$3,200", "Paid"},
            {"#403", "Apex Systems Inc", "Basic", "$850", "Pending"},
            {"#404", "Quantum Dynamics", "Enterprise", "$24,000", "Paid"},
            {"#405", "Vanguard Media", "Pro", "$4,100", "Failed"},
            {"#406", "Horizon Robotics", "Enterprise", "$18,900", "Paid"}
        };

        for (int r = 0; r < static_cast<int>(data.size()); ++r) {
            for (int c = 0; c < 5; ++c) {
                grid->set_cell(r, c, data[r][c]);
            }
        }

        split->add_child(sidebar);
        split->add_child(grid);

        auto status_bar = std::make_shared<simplegui::HBox>();
        status_bar->set_spacing(12);
        auto lbl_status = std::make_shared<simplegui::Label>("6 of 1,420 records | Synced");
        auto sync_progress = std::make_shared<simplegui::ProgressIndicator>();
        sync_progress->set_value(100);
        auto btn_export = std::make_shared<simplegui::Button>("Export CSV");

        status_bar->add_child(lbl_status);
        status_bar->add_stretch();
        status_bar->add_child(sync_progress);
        status_bar->add_child(btn_export);

        root->add_child(crumbs);
        root->add_child(toolbar);
        root->add_child(split);
        root->add_child(status_bar);

        win.set_content(root);
        win.save_screenshot(out_dir + "/example_data_explorer.png");
        std::cout << "Saved: " << out_dir << "/example_data_explorer.png" << std::endl;
    }

    // 6. TMOG SYSTEM MONITOR DASHBOARD
    {
        simplegui::Window win("TMOG Inspired System Telemetry", 740, 560);
        auto root = std::make_shared<simplegui::VBox>();
        root->set_margins(16);
        root->set_spacing(12);

        auto header = std::make_shared<simplegui::HBox>();
        header->set_spacing(10);
        auto title = std::make_shared<simplegui::Label>("TMOG / PRECISION TELEMETRY");
        title->set_style("font-weight: bold; font-size: 14px; letter-spacing: 1px; color: #f4f4f5;");
        auto pill_live = std::make_shared<simplegui::StatusPill>("LIVE 1.0.0", "#10b981");
        auto pill_sensors = std::make_shared<simplegui::StatusPill>("SENSORS ONLINE", "#06b6d4");
        header->add_child(title);
        header->add_child(pill_live);
        header->add_stretch();
        header->add_child(pill_sensors);

        auto stat_row = std::make_shared<simplegui::HBox>();
        stat_row->set_spacing(10);
        stat_row->add_child(std::make_shared<simplegui::StatCard>("CPU LOAD", "34.8%", "16 Cores · 4.80 GHz", "#06b6d4"));
        stat_row->add_child(std::make_shared<simplegui::StatCard>("MEMORY", "14.2 GB", "64 GB Total (22%)", "#10b981"));
        stat_row->add_child(std::make_shared<simplegui::StatCard>("DISK THROUGHPUT", "1.42 GB/s", "NVMe PCIe 4.0", "#f59e0b"));
        stat_row->add_child(std::make_shared<simplegui::StatCard>("PACKAGE POWER", "46.5 W", "Efficiency High", "#ec4899"));

        auto mid_row = std::make_shared<simplegui::HBox>();
        mid_row->set_spacing(12);

        auto grp_graph = std::make_shared<simplegui::GroupBox>("CPU Utilization History (Rolling Telemetry)");
        auto graph_vbox = std::make_shared<simplegui::VBox>();
        graph_vbox->set_margins(12);
        graph_vbox->set_spacing(10);

        auto sparkline = std::make_shared<simplegui::Sparkline>("#06b6d4");
        sparkline->set_range(0.0, 100.0);
        const std::vector<double> history = {
            18, 22, 25, 20, 19, 35, 48, 62, 58, 45, 
            30, 28, 32, 40, 55, 78, 85, 92, 70, 52, 
            41, 38, 35, 33, 30, 36, 42, 50, 48, 35
        };
        sparkline->set_samples(history);

        auto comp_label = std::make_shared<simplegui::Label>("Memory: Apps (14GB) | Wired (6GB) | Cache (8GB) | Free (36GB)");
        comp_label->set_style("color: #a1a1aa; font-size: 11px;");

        auto comp_bar = std::make_shared<simplegui::CompositionBar>();
        comp_bar->add_segment("Apps", 14.0, "#2563eb");
        comp_bar->add_segment("Wired", 6.0, "#06b6d4");
        comp_bar->add_segment("Cached", 8.0, "#f59e0b");
        comp_bar->add_segment("Free", 36.0, "#27272a");

        graph_vbox->add_child(sparkline);
        graph_vbox->add_child(comp_label);
        graph_vbox->add_child(comp_bar);
        grp_graph->add_child(graph_vbox);

        auto grp_vfd = std::make_shared<simplegui::GroupBox>("Core VU Levels");
        auto vfd_hbox = std::make_shared<simplegui::HBox>();
        vfd_hbox->set_margins(12);
        vfd_hbox->set_spacing(10);

        auto vfd1 = std::make_shared<simplegui::VfdMeter>(18, true);
        vfd1->set_value(88.0);
        auto vfd2 = std::make_shared<simplegui::VfdMeter>(18, true);
        vfd2->set_value(45.0);
        auto vfd3 = std::make_shared<simplegui::VfdMeter>(18, true);
        vfd3->set_value(95.0);
        auto vfd4 = std::make_shared<simplegui::VfdMeter>(18, true);
        vfd4->set_value(32.0);

        vfd_hbox->add_child(vfd1);
        vfd_hbox->add_child(vfd2);
        vfd_hbox->add_child(vfd3);
        vfd_hbox->add_child(vfd4);
        grp_vfd->add_child(vfd_hbox);

        mid_row->add_child(grp_graph);
        mid_row->add_child(grp_vfd);

        auto grp_procs = std::make_shared<simplegui::GroupBox>("Top Resource Processes");
        auto procs_vbox = std::make_shared<simplegui::VBox>();
        procs_vbox->set_margins(10);

        auto grid = std::make_shared<simplegui::Grid>(5, 5, std::vector<std::string>{"PID", "Process Name", "CPU %", "Memory", "Energy"});
        const std::vector<std::vector<std::string>> proc_data = {
            {"1082", "clang++ (EasyQt6 Build)", "68.4%", "1.24 GB", "Very High"},
            {"429", "WindowServer", "12.8%", "680 MB", "Medium"},
            {"8891", "QtWebEngineProcess", "8.2%", "512 MB", "Low"},
            {"1", "launchd (init)", "0.1%", "28 MB", "Very Low"},
            {"7420", "tmog_telemetry_core", "0.4%", "18 MB", "Very Low"}
        };

        for (int r = 0; r < 5; ++r) {
            for (int c = 0; c < 5; ++c) {
                grid->set_cell(r, c, proc_data[r][c]);
            }
        }
        procs_vbox->add_child(grid);
        grp_procs->add_child(procs_vbox);

        root->add_child(header);
        root->add_child(stat_row);
        root->add_child(mid_row);
        root->add_child(grp_procs);

        win.set_content(root);
        win.save_screenshot(out_dir + "/example_system_monitor.png");
        std::cout << "Saved: " << out_dir << "/example_system_monitor.png" << std::endl;
    }

    std::cout << "All polished screenshots generated successfully!" << std::endl;
    return 0;
}
