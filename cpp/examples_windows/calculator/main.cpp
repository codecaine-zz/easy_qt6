#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>

static std::string format_calc_num(double val) {
    std::ostringstream ss;
    ss << std::setprecision(10) << val;
    return ss.str();
}

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("windows_fluent");

    simplegui::Window window("Calculator", 330, 490);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(12);
    root->set_spacing(6);

    // Top mode label
    auto title = std::make_shared<simplegui::Label>("Standard ");
    title->set_style("color: #ffffff; font-size: 16px; font-weight: 600; margin-bottom: 2px;");
    root->add_child(title);

    // Expression preview
    auto expr_label = std::make_shared<simplegui::Label>("");
    expr_label->set_style("color: #9d9d9d; font-size: 13px;");
    root->add_child(expr_label);

    // Main display
    auto display = std::make_shared<simplegui::Label>("0");
    display->set_style(
        "color: #ffffff; font-size: 42px; font-weight: 600; "
        "background-color: transparent; padding: 4px 6px;"
    );
    root->add_child(display);

    // Calculator state
    auto current_val = std::make_shared<std::string>("0");
    auto stored_val = std::make_shared<double>(0.0);
    auto pending_op = std::make_shared<std::string>("");
    auto should_reset = std::make_shared<bool>(false);

    auto update_display = [display, current_val]() {
        display->set_text(*current_val);
    };

    auto digit_click = [current_val, should_reset, update_display](const std::string& d) {
        if (*should_reset || *current_val == "0") {
            *current_val = d;
            *should_reset = false;
        } else {
            *current_val += d;
        }
        update_display();
    };

    auto op_click = [current_val, stored_val, pending_op, should_reset, expr_label](const std::string& op) {
        try {
            *stored_val = std::stod(*current_val);
        } catch (...) {
            *stored_val = 0.0;
        }
        *pending_op = op;
        *should_reset = true;
        expr_label->set_text(format_calc_num(*stored_val) + " " + op);
    };

    auto equal_click = [current_val, stored_val, pending_op, should_reset, expr_label, update_display]() {
        if (pending_op->empty()) return;
        double current = 0.0;
        try {
            current = std::stod(*current_val);
        } catch (...) {
            current = 0.0;
        }
        double res = current;
        if (*pending_op == "+") res = *stored_val + current;
        else if (*pending_op == "−") res = *stored_val - current;
        else if (*pending_op == "×") res = *stored_val * current;
        else if (*pending_op == "÷") {
            if (current != 0.0) res = *stored_val / current;
            else res = 0.0;
        }

        expr_label->set_text(format_calc_num(*stored_val) + " " + *pending_op + " " + format_calc_num(current) + " =");
        *current_val = format_calc_num(res);
        *pending_op = "";
        *should_reset = true;
        update_display();
    };

    // Button grid styling
    std::string btn_num_style = 
        "background-color: #3b3b3b; color: #ffffff; border: 1px solid #333333; "
        "border-radius: 4px; font-size: 16px; font-weight: 600; padding: 10px 0;";
    std::string btn_op_style = 
        "background-color: #323232; color: #ffffff; border: 1px solid #333333; "
        "border-radius: 4px; font-size: 16px; font-weight: 400; padding: 10px 0;";
    std::string btn_eq_style = 
        "background-color: #60cdff; color: #000000; border: none; "
        "border-radius: 4px; font-size: 18px; font-weight: 700; padding: 10px 0;";

    std::vector<std::vector<std::string>> layout = {
        {"CE", "C", "⌫", "÷"},
        {"7", "8", "9", "×"},
        {"4", "5", "6", "−"},
        {"1", "2", "3", "+"},
        {"±", "0", ".", "="}
    };

    for (const auto& row_keys : layout) {
        auto row = std::make_shared<simplegui::HBox>();
        row->set_spacing(4);

        for (const auto& key : row_keys) {
            auto btn = std::make_shared<simplegui::Button>(key);
            if (key == "=") {
                btn->set_style(btn_eq_style);
                btn->on_click(equal_click);
            } else if (key == "+" || key == "−" || key == "×" || key == "÷") {
                btn->set_style(btn_op_style);
                btn->on_click([op_click, key]() { op_click(key); });
            } else if (key == "C" || key == "CE") {
                btn->set_style(btn_op_style);
                btn->on_click([current_val, stored_val, pending_op, expr_label, update_display]() {
                    *current_val = "0";
                    *stored_val = 0.0;
                    *pending_op = "";
                    expr_label->set_text("");
                    update_display();
                });
            } else if (key == "⌫") {
                btn->set_style(btn_op_style);
                btn->on_click([current_val, update_display]() {
                    if (current_val->length() > 1) {
                        *current_val = current_val->substr(0, current_val->length() - 1);
                    } else {
                        *current_val = "0";
                    }
                    update_display();
                });
            } else if (key == "±") {
                btn->set_style(btn_num_style);
                btn->on_click([current_val, update_display]() {
                    if (*current_val != "0") {
                        if ((*current_val)[0] == '-') *current_val = current_val->substr(1);
                        else *current_val = "-" + *current_val;
                        update_display();
                    }
                });
            } else if (key == ".") {
                btn->set_style(btn_num_style);
                btn->on_click([current_val, should_reset, update_display]() {
                    if (*should_reset) {
                        *current_val = "0.";
                        *should_reset = false;
                    } else if (current_val->find('.') == std::string::npos) {
                        *current_val += ".";
                    }
                    update_display();
                });
            } else {
                btn->set_style(btn_num_style);
                btn->on_click([digit_click, key]() { digit_click(key); });
            }
            row->add_child(btn);
        }
        root->add_child(row);
    }

    window.set_content(root);
    window.show();

    return app.run();
}
