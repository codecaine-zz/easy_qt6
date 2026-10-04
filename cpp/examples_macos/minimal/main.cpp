#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/label.h"
#include "simplegui/text_input.h"
#include "simplegui/button.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/checkbox.h"
#include "simplegui/slider.h"
#include "simplegui/radio.h"
#include "simplegui/dropdown.h"
#include "simplegui/textarea.h"
#include "simplegui/progress_indicator.h"
#include "simplegui/number_input.h"
#include "simplegui/knob.h"
#include "simplegui/date_picker.h"
#include "simplegui/group_box.h"
#include "simplegui/tab_view.h"
#include "simplegui/color_well.h"
#include "simplegui/combo_box.h"
#include "simplegui/grid.h"
#include "simplegui/link.h"
#include "simplegui/password_input.h"
#include "simplegui/search_field.h"
#include "simplegui/image_button.h"
#include "simplegui/image.h"
#include "simplegui/split_view.h"
#include "simplegui/scroll_view.h"
#include "simplegui/circular_progress.h"
#include "simplegui/rating.h"
#include "simplegui/breadcrumbs.h"
#include "simplegui/web_view.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);

    simplegui::Window window("Minimal C++/Qt Example", 400, 300);

    auto layout = std::make_shared<simplegui::VBox>();
    
    auto label = std::make_shared<simplegui::Label>("Hello, SimpleGUI!");
    auto input = std::make_shared<simplegui::TextInput>("Type your name here");
    auto button = std::make_shared<simplegui::Button>("Update Label");
    
    auto hbox = std::make_shared<simplegui::HBox>();
    auto checkbox = std::make_shared<simplegui::Checkbox>("Enable Slider", true);
    auto slider = std::make_shared<simplegui::Slider>(0, 100, 50);

    hbox->add_child(checkbox);
    hbox->add_child(slider);

    auto radio = std::make_shared<simplegui::Radio>("Option 1");
    auto dropdown = std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"Item A", "Item B", "Item C"});
    auto progress = std::make_shared<simplegui::ProgressIndicator>();
    auto textarea = std::make_shared<simplegui::Textarea>("Multiline text\ngoes here...");

    button->on_click([label, input, dropdown]() {
        label->set_text("Hello, " + input->get_text() + "! (Selected: " + dropdown->get_selected() + ")");
    });
    
    checkbox->on_change([slider](bool checked) {
        slider->set_enabled(checked);
    });

    slider->on_change([label, progress](int value) {
        label->set_text("Slider value: " + std::to_string(value));
        progress->set_value(value);
    });

    auto basic_tab = std::make_shared<simplegui::VBox>();
    basic_tab->add_child(label);
    basic_tab->add_child(input);
    basic_tab->add_child(button);
    basic_tab->add_child(hbox);

    auto extra_tab = std::make_shared<simplegui::VBox>();
    auto password = std::make_shared<simplegui::PasswordInput>("Password");
    auto search = std::make_shared<simplegui::SearchField>("Search...");
    auto image_btn = std::make_shared<simplegui::ImageButton>("", "Image Button");
    auto image = std::make_shared<simplegui::Image>(""); // No valid path right now
    
    extra_tab->add_child(radio);
    extra_tab->add_child(dropdown);
    extra_tab->add_child(progress);
    extra_tab->add_child(textarea);
    extra_tab->add_child(password);
    extra_tab->add_child(search);
    extra_tab->add_child(image_btn);
    extra_tab->add_child(image);

    auto advanced_tab = std::make_shared<simplegui::VBox>();
    auto number_input = std::make_shared<simplegui::NumberInput>(0, 100, 42);
    auto knob = std::make_shared<simplegui::Knob>(0, 100, 25);
    auto date_picker = std::make_shared<simplegui::DatePicker>();
    auto group_box = std::make_shared<simplegui::GroupBox>("Advanced Controls");
    
    auto color_well = std::make_shared<simplegui::ColorWell>("#3498db");
    auto combo_box = std::make_shared<simplegui::ComboBox>(std::vector<std::string>{"Edit me 1", "Edit me 2"});
    auto link = std::make_shared<simplegui::Link>("Visit Example.com", "https://example.com");
    auto grid = std::make_shared<simplegui::Grid>(2, 2, std::vector<std::string>{"Col 1", "Col 2"});
    grid->set_cell(0, 0, "A1");
    grid->set_cell(0, 1, "B1");
    grid->set_cell(1, 0, "A2");
    grid->set_cell(1, 1, "B2");

    auto circular_progress = std::make_shared<simplegui::CircularProgress>();
    circular_progress->set_value(75);
    auto rating = std::make_shared<simplegui::Rating>(5);
    rating->set_rating(3);
    auto breadcrumbs = std::make_shared<simplegui::Breadcrumbs>(std::vector<std::string>{"Home", "Settings", "Profile"});

    advanced_tab->add_child(breadcrumbs);
    advanced_tab->add_child(circular_progress);
    advanced_tab->add_child(rating);
    advanced_tab->add_child(number_input);
    advanced_tab->add_child(knob);
    advanced_tab->add_child(date_picker);
    advanced_tab->add_child(color_well);
    advanced_tab->add_child(combo_box);
    advanced_tab->add_child(link);
    advanced_tab->add_child(grid);
    advanced_tab->add_child(group_box);

    auto scroll_view = std::make_shared<simplegui::ScrollView>();
    scroll_view->set_content(advanced_tab);

    auto web_tab = std::make_shared<simplegui::VBox>();
    auto map_view = std::make_shared<simplegui::MapView>(48.8584, 2.2945); // Eiffel Tower
    auto html_view = std::make_shared<simplegui::HtmlView>("<h1>Hello WebEngine</h1><p>This is rendered in QtWebEngine!</p>");
    web_tab->add_child(html_view);
    web_tab->add_child(map_view);

    auto tab_view = std::make_shared<simplegui::TabView>();
    tab_view->add_tab("Basic", basic_tab);
    tab_view->add_tab("Extra", extra_tab);
    tab_view->add_tab("Advanced", scroll_view);
    tab_view->add_tab("Web Views", web_tab);

    auto left_pane = std::make_shared<simplegui::Label>("Side Panel");
    auto split_view = std::make_shared<simplegui::SplitView>(true);
    split_view->add_child(left_pane);
    split_view->add_child(tab_view);

    window.set_content(split_view);
    window.show();

    return app.run();
}
