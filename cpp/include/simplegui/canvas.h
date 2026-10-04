#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class Canvas : public Control {
public:
    Canvas(int min_width = 300, int min_height = 200);
    ~Canvas() override;

    void clear(const std::string& bg_color = "#0f172a");

    void draw_line(int x1, int y1, int x2, int y2, const std::string& color = "#ffffff", int line_width = 1);
    void draw_rect(int x, int y, int w, int h, const std::string& color = "#ffffff", int line_width = 1);
    void fill_rect(int x, int y, int w, int h, const std::string& color = "#3b82f6");
    void draw_circle(int cx, int cy, int radius, const std::string& color = "#ffffff", int line_width = 1);
    void fill_circle(int cx, int cy, int radius, const std::string& color = "#3b82f6");
    void draw_text(int x, int y, const std::string& text, const std::string& color = "#ffffff", int font_size = 12);

    void repaint();

    EventConnection on_mouse_down(std::function<void(int x, int y)> handler);
    EventConnection on_mouse_move(std::function<void(int x, int y)> handler);
    EventConnection on_mouse_up(std::function<void(int x, int y)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
