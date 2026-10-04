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

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);

    std::string out_dir = "screenshots";
    if (argc > 1) {
        out_dir = argv[1];
    }
    QDir().mkpath(QString::fromStdString(out_dir));

    auto capture_in_window = [&](const std::string& name, const std::string& title, std::shared_ptr<simplegui::Control> ctrl, int w = 320, int h = 120) {
        simplegui::Window win(title, w, h);
        auto vbox = std::make_shared<simplegui::VBox>();
        vbox->add_child(ctrl);
        win.set_content(vbox);
        std::string filepath = out_dir + "/" + name + ".png";
        win.save_screenshot(filepath);
        std::cout << "Saved: " << filepath << std::endl;
    };

    std::cout << "Generating high-quality native Qt6 screenshots..." << std::endl;

    // --- CONTROLS ---
    capture_in_window("button", "Button", std::make_shared<simplegui::Button>("Click Me!"), 260, 100);
    capture_in_window("label", "Label", std::make_shared<simplegui::Label>("Hello from EasyQt6!"), 280, 80);
    capture_in_window("text_input", "TextInput", std::make_shared<simplegui::TextInput>("Type your text here..."), 320, 100);
    capture_in_window("password_input", "PasswordInput", std::make_shared<simplegui::PasswordInput>("secret_password_123"), 320, 100);
    capture_in_window("search_field", "SearchField", std::make_shared<simplegui::SearchField>("Search files..."), 320, 100);
    capture_in_window("checkbox", "Checkbox", std::make_shared<simplegui::Checkbox>("Enable hardware acceleration", true), 320, 90);
    capture_in_window("radio", "Radio", std::make_shared<simplegui::Radio>("Standard Delivery (3-5 business days)"), 340, 90);

    auto slider = std::make_shared<simplegui::Slider>(0, 100, 65);
    capture_in_window("slider", "Slider", slider, 320, 90);

    capture_in_window("dropdown", "Dropdown", std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"Option A", "Option B", "Option C"}), 300, 100);
    capture_in_window("combo_box", "ComboBox", std::make_shared<simplegui::ComboBox>(std::vector<std::string>{"San Francisco", "London", "Tokyo", "Paris"}), 320, 100);
    capture_in_window("number_input", "NumberInput", std::make_shared<simplegui::NumberInput>(0, 100, 42), 240, 100);
    capture_in_window("knob", "Knob", std::make_shared<simplegui::Knob>(0, 100, 70), 180, 180);
    capture_in_window("date_picker", "DatePicker", std::make_shared<simplegui::DatePicker>(), 300, 110);
    capture_in_window("color_well", "ColorWell", std::make_shared<simplegui::ColorWell>("#3b82f6"), 220, 90);

    auto progress = std::make_shared<simplegui::ProgressIndicator>();
    progress->set_value(65);
    capture_in_window("progress_indicator", "ProgressIndicator", progress, 320, 90);

    auto circ_progress = std::make_shared<simplegui::CircularProgress>();
    circ_progress->set_value(75);
    capture_in_window("circular_progress", "CircularProgress", circ_progress, 180, 180);

    auto rating = std::make_shared<simplegui::Rating>(5);
    rating->set_rating(4);
    capture_in_window("rating", "Rating", rating, 240, 90);

    capture_in_window("breadcrumbs", "Breadcrumbs", std::make_shared<simplegui::Breadcrumbs>(std::vector<std::string>{"Home", "Projects", "EasyQt6", "Settings"}), 380, 90);
    capture_in_window("textarea", "Textarea", std::make_shared<simplegui::Textarea>("EasyQt6 provides clean, modern C++ wrappers\naround native Qt 6 widgets.\nFast, safe, and intuitive."), 380, 140);
    capture_in_window("link", "Link", std::make_shared<simplegui::Link>("Visit Qt Official Documentation", "https://doc.qt.io/"), 320, 80);
    capture_in_window("image_button", "ImageButton", std::make_shared<simplegui::ImageButton>("", "Launch Mission"), 260, 100);

    // --- LAYOUTS ---
    {
        simplegui::Window win("VBox Layout", 280, 180);
        auto vbox = std::make_shared<simplegui::VBox>();
        vbox->add_child(std::make_shared<simplegui::Button>("Top Button"));
        vbox->add_child(std::make_shared<simplegui::Button>("Middle Button"));
        vbox->add_child(std::make_shared<simplegui::Button>("Bottom Button"));
        win.set_content(vbox);
        win.save_screenshot(out_dir + "/vbox.png");
        std::cout << "Saved: " << out_dir << "/vbox.png" << std::endl;
    }

    {
        simplegui::Window win("HBox Layout", 360, 100);
        auto hbox = std::make_shared<simplegui::HBox>();
        hbox->add_child(std::make_shared<simplegui::Button>("Left"));
        hbox->add_child(std::make_shared<simplegui::Button>("Center"));
        hbox->add_child(std::make_shared<simplegui::Button>("Right"));
        win.set_content(hbox);
        win.save_screenshot(out_dir + "/hbox.png");
        std::cout << "Saved: " << out_dir << "/hbox.png" << std::endl;
    }

    {
        simplegui::Window win("Grid Layout", 380, 180);
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
        tab1->add_child(std::make_shared<simplegui::Label>("Content for General Settings"));
        tab1->add_child(std::make_shared<simplegui::Button>("Save Settings"));
        auto tab2 = std::make_shared<simplegui::VBox>();
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
        simplegui::Window win("Hello World", 320, 180);
        auto vbox = std::make_shared<simplegui::VBox>();
        auto label = std::make_shared<simplegui::Label>("Welcome to EasyQt6!");
        auto btn = std::make_shared<simplegui::Button>("Click Me");
        vbox->add_child(label);
        vbox->add_child(btn);
        win.set_content(vbox);
        win.save_screenshot(out_dir + "/example_hello_world.png");
        std::cout << "Saved: " << out_dir << "/example_hello_world.png" << std::endl;
    }

    {
        simplegui::Window win("Login", 360, 240);
        auto layout = std::make_shared<simplegui::VBox>();
        auto user_label = std::make_shared<simplegui::Label>("Username:");
        auto user_input = std::make_shared<simplegui::TextInput>("admin");
        auto pass_label = std::make_shared<simplegui::Label>("Password:");
        auto pass_input = std::make_shared<simplegui::PasswordInput>("secret");
        auto remember = std::make_shared<simplegui::Checkbox>("Remember Me", true);
        auto login_btn = std::make_shared<simplegui::Button>("Log In");

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
            for (const auto& text : row) {
                row_box->add_child(std::make_shared<simplegui::Button>(text));
            }
            layout->add_child(row_box);
        }
        win.set_content(layout);
        win.save_screenshot(out_dir + "/example_calculator.png");
        std::cout << "Saved: " << out_dir << "/example_calculator.png" << std::endl;
    }

    {
        simplegui::Window win("EasyQt6 All Controls Showcase", 520, 420);
        auto tab_view = std::make_shared<simplegui::TabView>();

        auto basic_tab = std::make_shared<simplegui::VBox>();
        basic_tab->add_child(std::make_shared<simplegui::Label>("Standard Controls"));
        basic_tab->add_child(std::make_shared<simplegui::TextInput>("Text Input field"));
        basic_tab->add_child(std::make_shared<simplegui::Button>("Submit Action"));
        auto hbox = std::make_shared<simplegui::HBox>();
        hbox->add_child(std::make_shared<simplegui::Checkbox>("Active", true));
        hbox->add_child(std::make_shared<simplegui::Slider>(0, 100, 50));
        basic_tab->add_child(hbox);

        auto extra_tab = std::make_shared<simplegui::VBox>();
        extra_tab->add_child(std::make_shared<simplegui::Radio>("Option A (Selected)"));
        extra_tab->add_child(std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"Dropdown Choice 1", "Dropdown Choice 2"}));
        auto p = std::make_shared<simplegui::ProgressIndicator>();
        p->set_value(70);
        extra_tab->add_child(p);

        tab_view->add_tab("Standard", basic_tab);
        tab_view->add_tab("Extra", extra_tab);
        win.set_content(tab_view);
        win.save_screenshot(out_dir + "/all_controls_showcase.png");
        std::cout << "Saved: " << out_dir << "/all_controls_showcase.png" << std::endl;
    }

    std::cout << "All screenshots generated successfully!" << std::endl;
    return 0;
}
