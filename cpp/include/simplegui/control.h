#pragma once
#include <memory>
#include <string>

class QWidget;

namespace simplegui {

// Control is the base class of every visible thing in SimpleGUI: buttons, labels,
// charts, and layouts. Everything listed here works on *every* control.
//
// Colors accept "#RRGGBB" hex codes or color names like "red" and "teal".
// Invalid colors are ignored.
class Control : public std::enable_shared_from_this<Control> {
public:
    virtual ~Control();

    // --- Visibility & enabled state ---------------------------------------
    virtual void set_enabled(bool enabled);   // false = greyed out, cannot be clicked
    virtual bool is_enabled() const;
    virtual void set_visible(bool visible);   // false = hidden (takes no space)
    virtual bool is_visible() const;
    void show();                              // same as set_visible(true)
    void hide();                              // same as set_visible(false)

    // --- Size (in pixels) -------------------------------------------------
    void set_width(int width);                // fixed width
    void set_height(int height);              // fixed height
    void set_size(int width, int height);     // fixed width and height
    void set_min_size(int width, int height); // never smaller than this
    void set_max_size(int width, int height); // never larger than this
    int width() const;                        // current on-screen width
    int height() const;                       // current on-screen height

    // --- Look & feel ------------------------------------------------------
    void set_tooltip(const std::string& text); // text shown when the mouse hovers
    std::string tooltip() const;
    void set_text_color(const std::string& color);
    void set_background_color(const std::string& color);
    void set_font_size(int pixels);
    void set_bold(bool bold);
    void set_font(const std::string& family);  // e.g. "Courier New"
    // Advanced: apply a Qt style sheet (CSS-like). Replaces the control's
    // built-in style but keeps the colors/fonts set with the methods above.
    virtual void set_style(const std::string& style);

    // --- Accessibility ----------------------------------------------------
    void set_accessible_name(const std::string& name);
    void set_accessible_description(const std::string& description);

    // --- Animations -------------------------------------------------------
    void fade_in(int duration_ms = 250);
    void fade_out(int duration_ms = 250);
    void slide_to(int x, int y, int duration_ms = 250);

    // --- Keyboard focus ---------------------------------------------------
    void set_focus();                         // put the typing cursor here
    bool has_focus() const;

    // --- Names (like Delphi's Name property or vlang_simplegui IDs) -------
    // Give a control a name, then look it up later from anywhere with
    // simplegui::find<Button>("save_button").
    void set_name(const std::string& name);
    std::string name() const;

    // Advanced: the underlying Qt widget. Only needed when mixing raw Qt code
    // with SimpleGUI. Most programs never call this.
    virtual QWidget* get_qwidget() const = 0;

private:
    std::string name_;
};

// Finds a control by the name given with set_name(). Returns nullptr when no
// living control has that name.
std::shared_ptr<Control> find_control(const std::string& name);

// Typed lookup: auto btn = simplegui::find<simplegui::Button>("ok");
// Returns nullptr when the name is unknown or the control is a different type.
template <typename T>
std::shared_ptr<T> find(const std::string& name) {
    return std::dynamic_pointer_cast<T>(find_control(name));
}

}  // namespace simplegui
