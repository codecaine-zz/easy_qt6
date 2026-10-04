#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/text_input.h"
#include "simplegui/button.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    simplegui::Window window("Calculator", 340, 420);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(16);
    root->set_spacing(10);

    // Display
    auto display = std::make_shared<simplegui::TextInput>("0");
    display->set_style(
        "font-size: 26px; font-weight: 600; color: #f8fafc; "
        "background-color: #1a1d26; border: 1px solid #282c37; "
        "padding: 12px; border-radius: 8px; qproperty-alignment: AlignRight;"
    );
    root->add_child(display);

    // State for simple calculator logic
    auto current_val = std::make_shared<double>(0.0);
    auto stored_val = std::make_shared<double>(0.0);
    auto pending_op = std::make_shared<std::string>("");
    auto clear_on_next = std::make_shared<bool>(true);

    const std::vector<std::vector<std::string>> button_rows = {
        {"C", "+/-", "%", "÷"},
        {"7", "8", "9", "×"},
        {"4", "5", "6", "−"},
        {"1", "2", "3", "+"},
        {"0", "", ".", "="}
    };

    for (const auto& row : button_rows) {
        auto hbox = std::make_shared<simplegui::HBox>();
        hbox->set_spacing(8);

        for (const auto& text : row) {
            if (text.empty()) continue;

            auto btn = std::make_shared<simplegui::Button>(text);

            if (text == "÷" || text == "×" || text == "−" || text == "+" || text == "=") {
                btn->set_style(
                    "background-color: #f59e0b; color: #ffffff; font-weight: bold; "
                    "border-radius: 6px; font-size: 16px; padding: 12px 0;"
                );
            } else if (text == "C" || text == "+/-" || text == "%") {
                btn->set_style(
                    "background-color: #3b4252; color: #e2e8f0; font-weight: 500; "
                    "border-radius: 6px; font-size: 14px; padding: 12px 0;"
                );
            } else {
                btn->set_style(
                    "background-color: #272a34; color: #f8fafc; font-weight: 600; "
                    "border-radius: 6px; font-size: 15px; padding: 12px 0;"
                );
            }

            btn->on_click([text, display, current_val, stored_val, pending_op, clear_on_next]() {
                if (text == "C") {
                    *current_val = 0.0;
                    *stored_val = 0.0;
                    *pending_op = "";
                    *clear_on_next = true;
                    display->set_text("0");
                } else if (text == "+/-") {
                    double val = -std::stod(display->get_text().empty() ? "0" : display->get_text());
                    std::ostringstream ss;
                    ss << val;
                    display->set_text(ss.str());
                } else if (text == "%") {
                    double val = std::stod(display->get_text().empty() ? "0" : display->get_text()) / 100.0;
                    std::ostringstream ss;
                    ss << val;
                    display->set_text(ss.str());
                } else if (text == "+" || text == "−" || text == "×" || text == "÷") {
                    *stored_val = std::stod(display->get_text().empty() ? "0" : display->get_text());
                    *pending_op = text;
                    *clear_on_next = true;
                } else if (text == "=") {
                    double second = std::stod(display->get_text().empty() ? "0" : display->get_text());
                    double result = second;
                    if (*pending_op == "+") result = *stored_val + second;
                    else if (*pending_op == "−") result = *stored_val - second;
                    else if (*pending_op == "×") result = *stored_val * second;
                    else if (*pending_op == "÷") result = second != 0 ? *stored_val / second : 0;

                    std::ostringstream ss;
                    ss << result;
                    display->set_text(ss.str());
                    *stored_val = result;
                    *pending_op = "";
                    *clear_on_next = true;
                } else {
                    // Digit or decimal
                    if (*clear_on_next || display->get_text() == "0") {
                        display->set_text(text == "." ? "0." : text);
                        *clear_on_next = false;
                    } else {
                        if (text != "." || display->get_text().find('.') == std::string::npos) {
                            display->set_text(display->get_text() + text);
                        }
                    }
                }
            });

            hbox->add_child(btn);
        }
        root->add_child(hbox);
    }

    window.set_content(root);
    window.show();

    return app.run();
}
