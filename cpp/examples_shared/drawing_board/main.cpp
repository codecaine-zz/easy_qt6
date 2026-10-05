#include "simplegui/simplegui.h"
#include <string>
#include <cmath>
#include <algorithm>
#include <cstdlib>

using namespace simplegui;

enum class Tool { Freehand, Line, Rectangle, Circle };

int main(int argc, char* argv[]) {
    Application app(argc, argv);
    Window window("Drawing Board & Image Viewer", 1000, 600);

    // Left pane: Reference Image
    auto left_pane = std::make_shared<VBox>();
    auto btn_load_image = std::make_shared<Button>("Load Reference Image");
    auto image_view = std::make_shared<Image>();
    image_view->set_scaled(true);
    
    btn_load_image->on_click([image_view]() {
        std::string path = open_file_dialog("Load Image", "Images (*.png *.jpg *.bmp *.svg *.gif)", "");
        if (!path.empty()) {
            if (!image_view->set_image(path)) {
                show_error("Failed to load image.");
            }
        }
    });

    left_pane->add_child(btn_load_image);
    left_pane->add_child(image_view);

    // Right pane: Drawing Board
    auto right_pane = std::make_shared<VBox>();
    auto toolbar = std::make_shared<HBox>();
    
    auto canvas = std::make_shared<Canvas>(600, 500);
    canvas->clear("#ffffff"); // white background
    
    // Tools State
    auto current_tool = std::make_shared<Tool>(Tool::Freehand);
    auto current_color = std::make_shared<std::string>("#000000");
    auto is_drawing = std::make_shared<bool>(false);
    auto start_x = std::make_shared<int>(0);
    auto start_y = std::make_shared<int>(0);
    auto last_x = std::make_shared<int>(0);
    auto last_y = std::make_shared<int>(0);

    // Toolbar buttons
    auto btn_freehand = std::make_shared<Button>("Freehand");
    btn_freehand->on_click([current_tool]() { *current_tool = Tool::Freehand; });
    
    auto btn_line = std::make_shared<Button>("Line");
    btn_line->on_click([current_tool]() { *current_tool = Tool::Line; });
    
    auto btn_rect = std::make_shared<Button>("Rectangle");
    btn_rect->on_click([current_tool]() { *current_tool = Tool::Rectangle; });
    
    auto btn_circle = std::make_shared<Button>("Circle");
    btn_circle->on_click([current_tool]() { *current_tool = Tool::Circle; });
    
    auto btn_color = std::make_shared<Button>("Color...");
    btn_color->on_click([current_color]() {
        std::string col = pick_color(*current_color, "Select Color");
        if (!col.empty()) *current_color = col;
    });
    
    auto btn_clear = std::make_shared<Button>("Clear");
    btn_clear->on_click([canvas]() { canvas->clear("#ffffff"); });

    toolbar->add_child(btn_freehand);
    toolbar->add_child(btn_line);
    toolbar->add_child(btn_rect);
    toolbar->add_child(btn_circle);
    toolbar->add_child(btn_color);
    toolbar->add_child(btn_clear);

    right_pane->add_child(toolbar);
    right_pane->add_child(canvas);

    // Canvas Events
    canvas->on_mouse_down([is_drawing, start_x, start_y, last_x, last_y, current_tool, current_color, canvas](int x, int y) {
        *is_drawing = true;
        *start_x = x;
        *start_y = y;
        *last_x = x;
        *last_y = y;
        
        if (*current_tool == Tool::Freehand) {
            canvas->draw_point(x, y, *current_color, 2);
        }
    });

    canvas->on_mouse_move([is_drawing, last_x, last_y, current_tool, current_color, canvas](int x, int y) {
        if (*is_drawing && *current_tool == Tool::Freehand) {
            canvas->draw_line(*last_x, *last_y, x, y, *current_color, 2);
            *last_x = x;
            *last_y = y;
        }
    });

    canvas->on_mouse_up([is_drawing, start_x, start_y, current_tool, current_color, canvas](int x, int y) {
        if (!*is_drawing) return;
        *is_drawing = false;
        
        if (*current_tool == Tool::Line) {
            canvas->draw_line(*start_x, *start_y, x, y, *current_color, 2);
        } else if (*current_tool == Tool::Rectangle) {
            int rx = std::min(*start_x, x);
            int ry = std::min(*start_y, y);
            int rw = std::abs(x - *start_x);
            int rh = std::abs(y - *start_y);
            canvas->draw_rect(rx, ry, rw, rh, *current_color, 2);
        } else if (*current_tool == Tool::Circle) {
            int radius = static_cast<int>(std::hypot(x - *start_x, y - *start_y));
            canvas->draw_circle(*start_x, *start_y, radius, *current_color, 2);
        }
    });

    // Main layout
    auto split = std::make_shared<SplitView>(true);
    split->add_child(left_pane);
    split->add_child(right_pane);
    split->set_sizes(300, 700);

    window.set_content(split);
    window.show();
    return app.run();
}
