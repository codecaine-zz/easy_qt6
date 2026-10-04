# Changelog

All notable changes to EasyQt6 (SimpleGUI for Qt 6) are documented here.
The format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) and the project
uses [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- **Futuristic controls:** `ToggleSwitch`, `RadialGauge`, `NeonButton`, `LedIndicator`,
  `RadarScope`, `TerminalView`, `SegmentedControl`, `GlassPanel`.
- **Neon theme** (`"neon"`, `"cyber"`, `"futuristic"`).
- **RAD-style API:**
  - `Timer` and `run_later`.
  - Dialogs: `show_message`, `show_info`, `show_warning`, `show_error`, `ask_yes_no`, `ask_ok_cancel`,
    `input_box`, `input_text`, `input_number`, `input_choice`, file/folder pickers, `pick_color`.
  - `ListBox`.
  - Control names with `find<T>()` / `find_control()`.
  - Familiar aliases (`Edit`, `Memo`, `CheckBox`, `TrackBar`, `StringGrid`, `PageControl`, `Row`, `Column`, …).
- **Control (all controls):** `show`/`hide`, sizing, colors, fonts, tooltips, focus, and names.
- **Window:**
  - Menus with keyboard shortcuts, status bar text, and `on_close` (can cancel the close).
  - Title, size and position control: `center`, `maximize`, `minimize`, full screen, and `set_icon`.
- **Application:** `theme()`, `available_themes()`, `set_app_name`, `set_font`, `quit`, `process_events`.
- **Controls gained many functions**, for example:
  - `Grid`: rows API, selection, editing, and events.
  - `Canvas`: points, rounded rects, ellipses, images, and `save_to_file`.
  - `WebView`: navigation and `on_load_finished`.
  - `KanbanBoard`: `card_ids` / `card_column`.
  - `NavRail`: `set_badge`.
  - `Breadcrumbs`: `push`, `pop`, `set_crumbs`.
  - `MediaPlayer`: `position`, `duration`, previous/next events.
  - Bulk setters for `LineChart`, `RadarChart` and `CompositionBar`.
- **Tests and examples:**
  - Headless API smoke tests (`ctest`), which also run in CI.
  - Cross-platform examples `mission_control` and `classic_rad`.
- **Docs:** screenshots for every new control, and a complete beginner-friendly API reference.

### Fixed
- **Use-after-free and crash bugs:**
  - Use-after-free crashes in `Canvas`, `DateRangePicker`, `FeedbackMood` and `TokenField` handlers.
  - Out-of-bounds writes in `ActivityHeatmap` and `Grid`.
  - Item leaks in `Grid`.
- **Events:**
  - `Breadcrumbs`, `Rating` and `Canvas` events now support several handlers and disconnect correctly.
  - `KanbanBoard` cards are now clickable.
  - `move_card` can no longer lose a card.
- **Control behavior:**
  - `NavRail` badges are now displayed.
  - `MediaPlayer` now parses the duration text.
  - `ProductCard::set_in_stock(true)` restores the button text.
  - `BarChart` honors the y-axis minimum.
  - `ListBox::set_sorted(true)` sorts existing rows.
- **Window and styling:**
  - `Window::set_content` no longer deletes content that is shared elsewhere.
  - Composite controls keep their styling when you call `set_name()`.
- **Code quality:** the code builds with zero warnings under `-Wall -Wextra -Wpedantic -Wshadow`.

### Security
- Text is shown as plain text everywhere: labels, chips, list items, dialogs and terminal output. HTML is never rendered.
- Colors are validated before they reach style sheets, which prevents QSS injection.
- `Link` only opens `http`, `https` and `mailto` URLs.
- `MapView` loads Leaflet with Subresource Integrity hashes.
- `Canvas` images are capped at 8192 × 8192 pixels to prevent memory exhaustion.

### Removed
- Leftover V-language CI workflow (`.github/workflows/ci.yml`), which also auto-pushed commits.
- V-specific entries in `.gitattributes` and `.editorconfig`.
- `CompositionBar::set_height`. Use the inherited `Control::set_height` instead.
