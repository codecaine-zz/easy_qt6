#pragma once
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

class Control;

// A top-level window with a title bar (Delphi/Lazarus: TForm, VB: Form).
// Put ONE control inside it with set_content() - usually a VBox or HBox that
// holds everything else.
class Window {
public:
    Window(const std::string& title = "SimpleGUI", int width = 640, int height = 480);
    ~Window();

    // --- Content ----------------------------------------------------------
    void set_content(std::shared_ptr<Control> content);   // replaces any previous content
    std::shared_ptr<Control> content() const;

    // --- Showing & hiding --------------------------------------------------
    void show();
    void hide();
    void close();                    // like clicking the close button (runs on_close)
    bool is_visible() const;
    void maximize();
    void minimize();
    void set_fullscreen(bool fullscreen);

    // --- Title, size & position -------------------------------------------
    void set_title(const std::string& title);
    std::string title() const;
    void set_size(int width, int height);
    void set_min_size(int width, int height);
    void set_fixed_size(int width, int height);   // user cannot resize
    int width() const;
    int height() const;
    void set_position(int x, int y);              // top-left corner on the screen
    void center();                                // move to the middle of the screen
    bool set_icon(const std::string& image_path); // false if the image can't be loaded

    // --- Status bar & menus -----------------------------------------------
    void set_status_text(const std::string& text);  // text at the bottom of the window
    // Adds an item to a menu in the menu bar (the menu is created on first use).
    // shortcut examples: "Ctrl+S", "Ctrl+Shift+N", "F5" ("Ctrl" means Cmd on macOS).
    EventConnection add_menu_item(const std::string& menu, const std::string& item,
                                  std::function<void()> handler, const std::string& shortcut = "");
    void add_menu_separator(const std::string& menu);

    // --- Events -----------------------------------------------------------
    // Runs when the user tries to close the window. Return false to keep it open
    // (for example after asking "Save changes?").
    EventConnection on_close(std::function<bool()> handler);

    // Saves a picture of the window to a .png/.jpg file. Returns false on failure.
    bool save_screenshot(const std::string& filepath);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

}  // namespace simplegui
