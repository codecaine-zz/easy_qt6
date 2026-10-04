#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A drawing surface (Delphi/Lazarus: TPaintBox / TImage.Canvas).
// Coordinates are pixels: (0, 0) is the top-left corner, x grows right, y grows down.
// Drawings stay on screen until you call clear().
class Canvas : public Control {
public:
    Canvas(int min_width = 300, int min_height = 200);
    ~Canvas() override;

    void clear(const std::string& bg_color = "#0f172a");   // erase everything

    // --- Shapes --------------------------------------------------------------
    void draw_point(int x, int y, const std::string& color = "#ffffff", int size = 1);
    void draw_line(int x1, int y1, int x2, int y2, const std::string& color = "#ffffff", int line_width = 1);
    void draw_rect(int x, int y, int w, int h, const std::string& color = "#ffffff", int line_width = 1);
    void fill_rect(int x, int y, int w, int h, const std::string& color = "#3b82f6");
    void draw_rounded_rect(int x, int y, int w, int h, int radius, const std::string& color = "#ffffff", int line_width = 1);
    void fill_rounded_rect(int x, int y, int w, int h, int radius, const std::string& color = "#3b82f6");
    void draw_circle(int cx, int cy, int radius, const std::string& color = "#ffffff", int line_width = 1);
    void fill_circle(int cx, int cy, int radius, const std::string& color = "#3b82f6");
    void draw_ellipse(int x, int y, int w, int h, const std::string& color = "#ffffff", int line_width = 1);
    void fill_ellipse(int x, int y, int w, int h, const std::string& color = "#3b82f6");

    // --- Text & pictures -----------------------------------------------------
    // (x, y) is the left end of the text's baseline (the line letters sit on).
    void draw_text(int x, int y, const std::string& text, const std::string& color = "#ffffff", int font_size = 12);
    bool draw_image(int x, int y, const std::string& file_path); // false if the file can't be loaded
    bool save_to_file(const std::string& file_path) const;       // .png/.jpg/.bmp; false on failure

    void repaint();   // force a redraw (rarely needed; drawing calls do this for you)

    // --- Mouse events (x, y relative to the canvas) -------------------------
    EventConnection on_mouse_down(std::function<void(int x, int y)> handler);
    EventConnection on_mouse_move(std::function<void(int x, int y)> handler);
    EventConnection on_mouse_up(std::function<void(int x, int y)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
