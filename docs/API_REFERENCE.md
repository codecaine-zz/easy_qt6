# SimpleGUI C++ API Reference

SimpleGUI is a modern, lightweight, and zero-boilerplate C++ wrapper around Qt6. It is designed to be as straightforward to use as Visual Basic or Delphi - allowing rapid UI development without touching raw Qt classes, macros, or memory management.

## Table of Contents
1. [Core Concepts](#core-concepts)
2. [Application & Window](#application--window)
3. [Layouts](#layouts)
4. [Controls](#controls)
5. [Custom Controls](#custom-controls)
6. [Web Views](#web-views)
7. [Events](#events)

---

## Core Concepts

### Memory Management
All controls and layouts in SimpleGUI are managed using `std::shared_ptr`. You instantiate them using `std::make_shared<simplegui::ControlName>()`.
You **never** use `new` or `delete`. When the window is closed, Qt natively destroys the UI tree safely, and C++ smart pointers release their memory.

### The PIMPL Idiom
Your application code never needs to `#include <QPushButton>` or any Qt headers. The library uses the PIMPL (Pointer to Implementation) idiom, isolating compilation times and removing complex Qt macro requirements from your code.

### Security Notes
- **Web Views**: The `WebView`, `HtmlView`, and `MapView` components utilize `QtWebEngine` (Chromium). By default, JavaScript is enabled to support maps and dynamic HTML. If you load remote URLs, be aware that standard browser security implications apply.
- **Memory Safety**: Event lambdas are memory-safe. The returned `EventConnection` automatically disconnects signals when destroyed or manually instructed, preventing dangling pointers.

---

## Application & Window

### `Application`
The entry point of your UI. It initializes the underlying Qt application.
```cpp
#include "simplegui/application.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    // ... setup window
    return app.run(); // Blocks until window is closed
}
```

### `Window`
The main desktop window.
```cpp
#include "simplegui/window.h"

// Window(const std::string& title, int width, int height)
simplegui::Window window("My App", 800, 600);
window.set_content(my_layout);
window.show();
```

---

## Layouts

Layouts inherit from `Control` and manage the positioning of child elements.

### `VBox`
Stacks elements vertically.
```cpp
auto vbox = std::make_shared<simplegui::VBox>();
vbox->add_child(label);
vbox->add_child(button);
```

### `HBox`
Stacks elements horizontally.

### `Grid`
A multi-column/multi-row table widget.
```cpp
auto grid = std::make_shared<simplegui::Grid>(rows, cols, headers);
grid->set_cell(row, col, "Text");
```

### `SplitView`
A resizable splitter separating two widgets.
```cpp
auto split = std::make_shared<simplegui::SplitView>(is_horizontal);
split->set_first(left_pane);
split->set_second(right_pane);
```

### `ScrollView`
A scrollable container for a single large child widget (usually a `VBox`).
```cpp
auto scroll = std::make_shared<simplegui::ScrollView>();
scroll->set_content(large_vbox);
```

### `TabView`
A tabbed container.
```cpp
auto tabs = std::make_shared<simplegui::TabView>();
tabs->add_tab("Tab 1", vbox_1);
tabs->add_tab("Tab 2", vbox_2);
```

### `GroupBox`
A bordered container with a title.
```cpp
auto group = std::make_shared<simplegui::GroupBox>("Settings");
group->set_content(vbox);
```

---

## Controls

All controls return an `EventConnection` for their primary event handlers.

| Control | State Accessors | Event Handler | Description |
|---|---|---|---|
| `Label` | `set_text`, `get_text` | None | Displays text. |
| `Button` | N/A | `on_click([]() {})` | Clickable button. |
| `TextInput` | `set_text`, `get_text` | `on_change([](const std::string&){})` | Single-line text field. |
| `PasswordInput`| `set_text`, `get_text` | `on_change([](const std::string&){})` | Hidden text field. |
| `Textarea` | `set_text`, `get_text` | `on_change([](const std::string&){})` | Multi-line text field. |
| `Checkbox` | `set_checked`, `is_checked`| `on_change([](bool){})` | Toggle box. |
| `Radio` | `set_checked`, `is_checked`| `on_change([](bool){})` | Exclusive toggle. |
| `Slider` | `set_value`, `get_value` | `on_change([](int){})` | Horizontal slider (0-100). |
| `Knob` | `set_value`, `get_value` | `on_change([](int){})` | Circular dial (0-100). |
| `NumberInput` | `set_value`, `get_value` | `on_change([](int){})` | Spinbox for integers. |
| `DatePicker` | `set_date`, `get_date` | `on_change([](const std::string&){})` | Calendar date picker. |
| `ColorWell` | `set_color`, `get_color` | `on_change([](const std::string&){})` | Opens OS color picker. |
| `ComboBox` | `add_item`, `get_selected` | `on_change([](const std::string&){})` | Dropdown menu. |
| `ImageButton` | N/A | `on_click([]() {})` | Button with an icon. |
| `Image` | `set_image_path` | None | Displays a picture. |
| `Link` | `set_url` | None | Clickable URL hyperlink. |
| `ProgressIndicator` | `set_value` | None | Horizontal loading bar. |

---

## Custom Controls

Controls that perform raw QPainter commands to achieve native-feeling custom widgets.

- **`CircularProgress`**: `set_value(int)`, a pie-chart style progress ring.
- **`Rating`**: `set_rating(int)`, `on_change([](int){})`, an interactive 5-star rater.
- **`Breadcrumbs`**: `on_click([](int index){})`, an interactive `Home > Profile > Settings` horizontal link trail.

---

## Web Views

Advanced Chromium-backed rendering engines utilizing `QtWebEngine`.

- **`WebView`**: Core engine. Use `set_url(std::string)` or `set_html(std::string)`.
- **`HtmlView`**: Alias for `WebView`. Displays raw HTML code.
- **`PdfView`**: Alias for `WebView`. Pass a local file path (`file:///path/to/doc.pdf`) to render a PDF using Chrome's native PDF renderer.
- **`MapView`**: Interactive maps. Use `set_coordinates(double lat, double lng)` to jump to a location via embedded Leaflet/OpenStreetMap.

---

## Events

Event handling uses modern C++ standard lambdas.
When you register an event, it returns an `EventConnection`. 
If you simply ignore the return value, the event lives as long as the widget does.
If you need to unsubscribe from the event later, store the token and call `.disconnect()`.

```cpp
auto btn = std::make_shared<simplegui::Button>("Click Me");

// Basic usage
btn->on_click([]() {
    std::cout << "Clicked!" << std::endl;
});

// Advanced usage: disconnect later
simplegui::EventConnection conn = btn->on_click([]() {
    std::cout << "This will only fire once." << std::endl;
});
conn.disconnect();
```
