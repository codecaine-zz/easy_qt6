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

### [`VBox`](../screenshots/vbox.png)
Stacks elements vertically.
```cpp
auto vbox = std::make_shared<simplegui::VBox>();
vbox->add_child(label);
vbox->add_child(button);
```

### [`HBox`](../screenshots/hbox.png)
Stacks elements horizontally.

### [`Grid`](../screenshots/grid.png)
A multi-column/multi-row table widget.
```cpp
auto grid = std::make_shared<simplegui::Grid>(rows, cols, headers);
grid->set_cell(row, col, "Text");
```

### [`SplitView`](../screenshots/split_view.png)
A resizable splitter separating two widgets.
```cpp
auto split = std::make_shared<simplegui::SplitView>(is_horizontal);
split->set_first(left_pane);
split->set_second(right_pane);
```

### [`ScrollView`](../screenshots/scroll_view.png)
A scrollable container for a single large child widget (usually a `VBox`).
```cpp
auto scroll = std::make_shared<simplegui::ScrollView>();
scroll->set_content(large_vbox);
```

### [`TabView`](../screenshots/tab_view.png)
A tabbed container.
```cpp
auto tabs = std::make_shared<simplegui::TabView>();
tabs->add_tab("Tab 1", vbox_1);
tabs->add_tab("Tab 2", vbox_2);
```

### [`GroupBox`](../screenshots/group_box.png)
A bordered container with a title.
```cpp
auto group = std::make_shared<simplegui::GroupBox>("Settings");
group->set_content(vbox);
```

---

## Controls

All controls return an `EventConnection` for their primary event handlers.

| Control | Preview | State Accessors | Event Handler | Description |
|---|:---:|---|---|---|
| `Label` | [View](../screenshots/label.png) | `set_text`, `get_text` | None | Displays text. |
| `Button` | [View](../screenshots/button.png) | N/A | `on_click([]() {})` | Clickable button. |
| `TextInput` | [View](../screenshots/text_input.png) | `set_text`, `get_text` | `on_change([](const std::string&){})` | Single-line text field. |
| `PasswordInput`| [View](../screenshots/password_input.png) | `set_text`, `get_text` | `on_change([](const std::string&){})` | Hidden text field. |
| `SearchField` | [View](../screenshots/search_field.png) | `set_text`, `get_text` | `on_change([](const std::string&){})` | Search box with clear button. |
| `Textarea` | [View](../screenshots/textarea.png) | `set_text`, `get_text` | `on_change([](const std::string&){})` | Multi-line text field. |
| `Checkbox` | [View](../screenshots/checkbox.png) | `set_checked`, `is_checked`| `on_change([](bool){})` | Toggle box. |
| `Radio` | [View](../screenshots/radio.png) | `set_checked`, `is_checked`| `on_change([](bool){})` | Exclusive toggle. |
| `Slider` | [View](../screenshots/slider.png) | `set_value`, `get_value` | `on_change([](int){})` | Horizontal slider (0-100). |
| `Knob` | [View](../screenshots/knob.png) | `set_value`, `get_value` | `on_change([](int){})` | Circular dial (0-100). |
| `NumberInput` | [View](../screenshots/number_input.png) | `set_value`, `get_value` | `on_change([](int){})` | Spinbox for integers. |
| `DatePicker` | [View](../screenshots/date_picker.png) | `set_date`, `get_date` | `on_change([](const std::string&){})` | Calendar date picker. |
| `ColorWell` | [View](../screenshots/color_well.png) | `set_color`, `get_color` | `on_change([](const std::string&){})` | Opens OS color picker. |
| `Dropdown` | [View](../screenshots/dropdown.png) | `get_selected`, `set_selected` | `on_change([](const std::string&){})` | Selection list. |
| `ComboBox` | [View](../screenshots/combo_box.png) | `add_item`, `get_selected` | `on_change([](const std::string&){})` | Editable dropdown menu. |
| `ImageButton` | [View](../screenshots/image_button.png) | N/A | `on_click([]() {})` | Button with an icon. |
| `Link` | [View](../screenshots/link.png) | `set_url` | None | Clickable URL hyperlink. |
| `ProgressIndicator` | [View](../screenshots/progress_indicator.png) | `set_value` | None | Horizontal loading bar. |

---

## Custom Controls

Controls that perform raw QPainter commands to achieve native-feeling custom widgets.

- **[`CircularProgress`](../screenshots/circular_progress.png)**: `set_value(int)`, a pie-chart style progress ring.
- **[`Rating`](../screenshots/rating.png)**: `set_rating(int)`, `on_change([](int){})`, an interactive 5-star rater.
- **[`Breadcrumbs`](../screenshots/breadcrumbs.png)**: `on_click([](int index){})`, an interactive `Home > Profile > Settings` horizontal link trail.

---

## TMOG Precision Telemetry Controls

High-precision telemetry and monitoring widgets inspired by Dave Plummer's [TMOG](https://tmog.org/) (The Mother of Graphs / Task Manager):

- **[`Sparkline`](../screenshots/sparkline.png)**: Real-time rolling telemetry line graph with gradient area fill, grid lines, and leading glow dot.
  - `add_sample(double value)`
  - `set_samples(std::vector<double>)`
  - `set_color("#06b6d4")`
  - `set_range(min, max)`
- **[`VfdMeter`](../screenshots/vfd_meter.png)**: Vacuum Fluorescent Display / LED segmented level meter with green/amber/red threshold segments and glow effect.
  - `set_value(double percentage)`
  - `set_segments(int count)`
  - `set_glow(bool enabled)`
- **[`StatCard`](../screenshots/stat_card.png)**: Prominent digital telemetry readout card with label, value, subtext, and custom accent color.
  - `set_title("CPU LOAD")`
  - `set_value("4.85 GHz")`
  - `set_subtext("Turbo Active")`
  - `set_accent_color("#06b6d4")`
- **[`CompositionBar`](../screenshots/composition_bar.png)**: Multi-segment partitioned bar showing resource/memory distribution.
  - `add_segment("Apps", 14.0, "#2563eb")`
  - `clear_segments()`
- **[`StatusPill`](../screenshots/status_pill.png)**: High-tech badge with glowing status dot indicator.
  - `set_status("LIVE 1.0.0", "#10b981")`

---

## High-Level Composite & App Parity Controls

Rich, production-grade composite controls ported from `vlang_simplegui` and enhanced with native Qt6 antialiasing, layout responsiveness, and dark styling:

- **[`KanbanBoard`](../screenshots/kanban_board.png)**: Agile task management board with columns, card badges, and interactive task cards.
  - `add_column("todo", "To Do")`
  - `add_card("todo", "c1", "Task Title", "TAG", "Task description...")`
  - `move_card("c1", "done")`
  - `on_card_clicked([](const std::string& card_id) {})`
- **[`UserProfileCard`](../screenshots/user_profile_card.png)**: User profile card with avatar initials, online status badge, handle, bio, and action button.
  - `set_online_status(bool is_online)`
  - `set_bio(const std::string& bio)`
  - `on_action([]() {})`
- **[`ProductCard`](../screenshots/product_card.png)**: Store product card with badge pill, star rating, description, price, and CTA.
  - `set_price("$149.00")`
  - `set_badge("POPULAR")`
  - `set_rating(4.9)`
  - `set_in_stock(bool in_stock)`
  - `on_buy([]() {})`
- **[`DonutChart`](../screenshots/donut_chart.png)**: Antialiased radial donut / ring progress chart with center readout.
  - `add_segment("Apps", 45.0, "#3b82f6")`
  - `set_center_text("78%", "USED")`
  - `set_thickness(22)`
- **[`ActivityHeatmap`](../screenshots/activity_heatmap.png)**: GitHub-style multi-week activity heatmap.
  - `set_cell(int week, int day, int intensity)` // 0 to 4
  - `set_color_scale("#10b981")`
- **[`StatGrid`](../screenshots/stat_grid.png)**: KPI dashboard grid with value readouts and trend indicators.
  - `add_stat("Active Users", "14,892", "+12.4%", true)`
  - `clear()`
- **[`MediaPlayer`](../screenshots/media_player.png)**: Media playback card with album artwork, title, scrub slider, and playback controls.
  - `set_track("Track Name", "Artist", "03:45")`
  - `set_position(int seconds, int total_seconds)`
  - `set_playing(bool is_playing)`
  - `on_play_pause([](bool is_playing) {})`
  - `on_seek([](int seconds) {})`
- **[`FeedbackMood`](../screenshots/feedback_mood.png)**: 5-level interactive mood satisfaction rating selector.
  - `set_rating(int rating)` // 1 to 5
  - `int rating()`
  - `on_change([](int rating) {})`
- **[`DateRangePicker`](../screenshots/date_range_picker.png)**: Dual date picker with calendar popups and quick presets.
  - `set_range("2026-10-01", "2026-10-31")`
  - `start_date()`, `end_date()`
  - `on_range_changed([](const std::string& start, const std::string& end) {})`
- **[`TokenField`](../screenshots/token_field.png)**: Interactive tag/chip input with dismissible tokens.
  - `add_token("C++20")`
  - `remove_token("C++20")`
  - `tokens()`
  - `on_tokens_changed([](const std::vector<std::string>& tokens) {})`
- **[`MaskedInput`](../screenshots/masked_input.png)**: Formatted input enforcing phone, IP, or key masks.
  - `set_mask("(999) 999-9999")`
  - `set_text("5550192834")`
  - `text()`
  - `on_change([](const std::string& text) {})`
- **[`NavRail`](../screenshots/nav_rail.png)**: Vertical compact navigation rail with icon glyphs, text labels, and active indicator.
  - `add_item("dash", "⚡", "Dash")`
  - `set_selected("dash")`
  - `on_select([](const std::string& id) {})`

---

## Graphics & Advanced Charts

Comprehensive 2D rendering and data visualization suite for analytics, telemetry, games, and financial applications:

- **[`BarChart`](../screenshots/bar_chart.png)**: Vertical categorized bar chart with rounded tops, grid lines, and value tags.
  - `add_bar("Jan", 45.0, "#3b82f6")`
  - `set_title("Monthly Revenue")`
  - `set_show_values(true)`
  - `set_show_grid(true)`
  - `set_y_range(0.0, 100.0)`
- **[`LineChart`](../screenshots/line_chart.png)**: Multi-series 2D Cartesian line chart with cubic Bezier smoothing, gradient fills, and data points.
  - `set_x_labels({"00:00", "06:00", "12:00", "18:00"})`
  - `add_series("Inbound", {12, 45, 82, 30}, "#3b82f6", true)`
  - `set_smooth(true)`
  - `show_points(true)`
  - `show_grid(true)`
  - `show_legend(true)`
- **[`PieChart`](../screenshots/pie_chart.png)**: Solid wedge pie chart with seam outlines, auto percentage calculation, and side legend.
  - `add_slice("Americas", 44.0, "#3b82f6")`
  - `show_legend(true)`
  - `show_percentages(true)`
- **[`Canvas`](../screenshots/canvas.png)**: Retained 2D vector graphics canvas with mouse tracking and immediate-mode drawing commands.
  - `clear("#090d16")`
  - `draw_line(x1, y1, x2, y2, color, width)`
  - `draw_rect(x, y, w, h, color, width)`, `fill_rect(x, y, w, h, color)`
  - `draw_circle(cx, cy, r, color, width)`, `fill_circle(cx, cy, r, color)`
  - `draw_text(x, y, text, color, font_size)`
  - `on_mouse_down([](int x, int y) {})`, `on_mouse_move([](int x, int y) {})`
- **[`RadarChart`](../screenshots/radar_chart.png)**: Multi-dimensional spider / polar chart with concentric polygonal web and translucent overlays.
  - `set_dimensions({"Speed", "UX", "Security", "Scale", "Reliability"})`
  - `add_dataset("Production", {92, 88, 84, 90, 96}, "#3b82f6")`
  - `show_legend(true)`
- **[`CandlestickChart`](../screenshots/candlestick_chart.png)**: Financial OHLC trading chart with bullish/bearish color coding and dynamic price scale.
  - `add_candle("10:00", open, high, low, close)`
  - `set_title("BTC/USDT")`
  - `show_grid(true)`

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
