# SimpleGUI — Native macOS GUIs in V

Build real, native Cocoa desktop apps in [V](https://vlang.io) with a beginner-friendly API and a built-in drag-and-drop Visual UI Designer & Code Generator inspired by Delphi, Lazarus, and Visual Basic — no Objective-C required.

![Platform: macOS](https://img.shields.io/badge/platform-macOS-blue)
![Language: V](https://img.shields.io/badge/language-V-4f87c4)
![License: MIT](https://img.shields.io/badge/license-MIT-green)
![Demos: 70+](https://img.shields.io/badge/demos-70%2B-orange)
![RAD Studio: Visual Designer Included](https://img.shields.io/badge/RAD%20Designer-Included-8A2BE2)

![SimpleGUI — All Controls Demo (20 sections, every win.add_* control)](screenshots/all_controls_demo.png)

## Table of contents

- [Overview](#overview)
- [📘 Beginner's Core Concepts Glossary](#beginners-core-concepts--jargon-free-glossary)
- [Features](#features)
- [🎨 RAD Visual UI Designer & Code Studio](#rad-visual-ui-designer--code-studio)
- [Platform Support Matrix](#platform-support-matrix)
- [Installation](#installation)
- [Quick start](#quick-start)
- [Faster form building](#faster-form-building)
- [Best and easiest ways to create GUIs and events](#best-and-easiest-ways-to-create-guis-and-events)
- [Starter templates](#starter-templates)
- [Developer tips](#developer-tips)
- [Compiling & packaging as a macOS app](#compiling--packaging-as-a-macos-app)
- [Capturing demo screenshots](#capturing-demo-screenshots)
- [Demos](#demos)
- [Production Workstations & Studio Applications](#production-workstations--studio-applications)
- [SimpleCLI: Headless Console & RAD Toolkit](#simplecli-headless-console--rad-toolkit)
- [Developer Utility Suite (40 Modules)](#developer-utility-suite-40-modules)
- [Related GUI & RAD Desktop Projects](#related-gui--rad-desktop-projects)
- [Security & Command Injection Prevention](#security--command-injection-prevention)
- [Testing](#testing)
- [Project structure](#project-structure)
- [Documentation](#documentation)
- [Screenshots](#screenshots)
- [Contributing](#contributing)
- [License](#license)

## Overview

SimpleGUI makes building desktop software on macOS effortless and enjoyable for beginners and experienced developers alike. It combines:

- **Lightweight V Code**: Write clean, concise code in [V](https://vlang.io) without heavy frameworks or bloated web views.
- **Native macOS Cocoa Bridge**: Displays real, native Mac windows, buttons, text fields, and menus that match the macOS operating system seamlessly.
- **Beginner-Friendly API**: Add named controls, read/write input values, and attach click or change events in just a few lines of code.
- **🎨 Built-in Drag & Drop Visual UI Designer**: Design forms visually with a Delphi/VB/Lazarus-style WYSIWYG studio (`v run tools/ui_designer.v`), complete with an Object Inspector, property search filter, multi-column grid auto-layout, undo/redo history, 10+ layout presets, and 1-click V source code & HTML generation.

The goal is to provide an intuitive, event-driven GUI framework similar to classic tools like Delphi, Visual Basic, or Python UI libraries — while delivering the lightning-fast performance of compiled native software.

---

## 📘 Beginner's Core Concepts & Jargon-Free Glossary

If you are new to programming or desktop app creation, here are simple definitions for terms used throughout this guide:

| Term | Simple Explanation | Real-World Analogy |
| :--- | :--- | :--- |
| **Window (`SimpleWindow`)** | The rectangular application frame on your computer screen that contains all app controls. | A picture frame or digital canvas displaying your app. |
| **Control / Widget** | An interactive user interface element (like a button, text box, slider, or checkbox). | Building blocks like light switches, knobs, and text labels. |
| **Layout Container** | An invisible box or row that automatically aligns and arranges controls side-by-side or stacked vertically. | A bookshelf that arranges books side-by-side instead of in a messy pile. |
| **Event & Callback** | An action listener. When a user interacts with a control (e.g. clicks a button), the app triggers a "callback" function to execute code. | A doorbell: when someone presses it (event), a chime sounds (callback). |
| **Fluent Chaining** | Connecting multiple setup actions in a single line using dots (e.g. `.width(200).tooltip('Help')`). | Snapping Lego blocks together in a continuous chain. |
| **String (`string`)** | Text surrounded by quotes (e.g. `'Ada'` or `"Hello World"`). | Printed words on a label. |
| **Integer (`int`)** | A whole number without decimals (e.g. `42`, `100`, `-5`). | Counting physical items like 5 apples or 10 buttons. |
| **Float (`f64`)** | A number with decimal points (e.g. `3.14`, `0.75`). | Precise measurements like weight (1.5 kg) or percentage (99.9%). |
| **Boolean (`bool`)** | A value that is strictly `true` (YES/ON) or `false` (NO/OFF). | A light switch position: ON or OFF. |
| **Hex Color** | A color code starting with `#` followed by 6 characters (e.g. `#007aff` for Apple blue, `#ffffff` for white, `#000000` for black). | A paint swatch code from a hardware store. |

---

## Features

- **Advanced Layout & Auto-Sizing Capabilities**:
  - CSS-like Multi-Column Grid layout containers (`win.begin_grid` / `win.end_grid` and `win.grid(...)`) for responsive multi-column forms without manual row nesting.
  - Flexbox containers (`win.begin_flex_box` / `win.end_flex_box` and `win.flex_box(...)`) supporting `row`/`column` directions, main-axis justification (`start`, `center`, `end`, `space_between`, `space_around`, `fill`), and cross-axis alignment (`start`, `center`, `end`, `stretch`).
  - Native Cocoa `containerStack` backend supporting arbitrary nested container hierarchies (Grid inside Flexbox inside Cards).
  - Explicit control alignment modifiers (`.align_left()`, `.align_center()`, `.align_right()`, `.align_top()`, `.align_bottom()`) and fill expansion (`.expand_fill()`).
- **40+ Native macOS Controls**:
  - **Standard Controls**: Labels, text inputs, password fields, scrollable text areas, push buttons, checkboxes, radio groups, dropdowns, segmented controls, search fields, sliders, steppers, progress bars, and image boxes.
  - **Rich macOS Widgets**: `NSComboBox` (editable combo box with suggestions), `NSLevelIndicator` & Star Ratings, `NSTokenField` (bubble tag editor), `NSPathControl` (interactive breadcrumb path navigator), and Activity Loading Spinners.
  - **Developer & Dashboard Controls**: Breadcrumb bars, keyboard shortcut recorders, line & area charts, circular progress gauges, property inspector grids, color swatches/wells, editable spreadsheet-style grids, tree views, code editors, timeline event feeds, badges, avatar cards, stat cards, notice banners, split buttons, tag clouds, wizard steppers, status indicators, metric meters, system tray status items, collapsible sections, titlebar toolbar items, WebKit HTML preview panels, and drag-and-drop file drop zones.
- **18 Curated Production Themes & Save State**:
  - Light & Dark system palettes: `GitHub Dark` (Default), `Apple Dark`, `Apple Light`, `Deep Space OLED`, `Tokyo Night`, `Nord Arctic`, `Dracula Vampire`, `Cyberpunk Neon`, `Catppuccin Mocha`, `Monokai Pro`, `Gruvbox Dark`, `Cobalt Blue`, `Emerald Forest`, `Sunset Dusk`, `GitHub Light`, `Solarized Dark`, `Solarized Light`, and `Warm Paper & Ink`.
  - **Cross-App Theme State Persistence**: User theme choices persist automatically to `~/.config/simplegui/theme.txt` and synchronize seamlessly across all applications on startup.
  - **Theme-driven control styling**: buttons, dropdowns, inputs, and date pickers restyle from the applied theme's background — a light theme renders light controls even when macOS is in system Dark Mode (and vice versa).
- **Centralized Command Injection Prevention & Safe Execution**:
  - POSIX single-quote escaping for arguments and inputs (`simplegui.quote_arg`, `simplegui.quote_path`).
  - Safe shell isolation executors (`simplegui.exec_safe`, `simplegui.exec_safe_stdin`) neutralizing command breakout (`;`, `&&`, `|`, `` ` ``, `$()`, `>`, `<`).
  - Path traversal and metacharacter sanitization (`simplegui.sanitize_filename`).
- **Form Automation & Validation Engine**:
  - Auto-generate complete forms from V structs using compile-time reflection (`win.add_form_from_struct[T]()`).
  - Struct field attribute validation (`win.validate_struct[T]()`) supporting `@[required]`, `@[min_len]`, `@[max_len]`, `@[email]`, `@[url]`, `@[alphanumeric]`, `@[min]`, `@[max]`.
  - Built-in ready-made validators for email, numeric input, URLs, IP addresses, phone numbers, alphanumeric string constraints, and numeric ranges.
- **Neutralino-inspired OS & System APIs**:
  - Synchronous, asynchronous, and timeout shell process execution (`exec`, `exec_bg`, `exec_timeout`, `spawn_process`).
  - Production reliability helpers: structured command metadata (`exec_result`, `exec_timeout_result`), retry/backoff (`exec_retry`), command dependency checks (`command_exists`, `require_command`), and readiness waits (`wait_for_file`, `wait_for_port`).
  - Environment variable accessors, system paths lookup (`home`, `temp`, `desktop`, `documents`, `downloads`, `cache`, `app_data`).
  - Hardware & system diagnostics: Processor model, CPU physical/logical cores, frequency, architecture, RAM memory, disk space, and disk usage stats (`DiskStats`).
  - Network tools: Local IP, public external IP, TCP ping reachability, DNS lookup, Wi-Fi SSID, network interface listing, MAC address, DNS servers, default gateway, and listening ports detection.
  - Resource monitoring: CPU utilization %, RSS memory usage per process, load average (1m/5m/15m), and kernel memory pressure.
  - macOS App & Dock Integration: Bouncing dock icon for alerts, dock badges, native system notifications via AppleScript, volume/mute control, system accent color, dark mode toggle, and launch-at-login management.
  - Cross-window and external-app automation helpers: inspect/drive registered SimpleGUI windows and external macOS apps via Accessibility (AXUIElement) by PID, including enable/disable/show/hide, set text/value, flash highlights, and app frontmost/visibility controls.
- **V Standard Library High-Level Wrappers**:
  - **HTTP & WebSockets**: Synchronous HTTP GET/POST and background WebSocket client thread wrappers.
  - **Cryptography & Hashing**: SHA-256, SHA-512, SHA-1, MD5, bcrypt password hashing/verification, HMAC (SHA256/SHA512/SHA1), Wyhash, and AES CBC 128-bit block encryption/decryption.
  - **Encodings & Compression**: Hex, Base64 encoding/decoding, and Gzip, Zlib, Deflate, and Zstd data compression/decompression.
  - **Random Numbers & UUIDs**: Cryptographically secure random bytes, hex strings, UUID v4 generation, and weighted choice arrays.
  - **Parsing & Data Formats**: TOML configuration parser (`TOMLWrapperDoc`), JSON serialization/deserialization map helpers, URL parsing/building (`SimpleURL`), HTML DOM parser (`SimpleHTMLDocument`), and Markov-chain placeholder text generator (`lorem`).
  - **Networking & IPC**: Low-level stream wrappers for TCP client, UDP datagram socket, and Unix domain socket IPC.
  - **Datatypes & Collections**: Generic LIFO Stack (`SimpleStack`), FIFO Queue (`SimpleQueue`), Unique Set (`SimpleSet`), Ring Buffer (`SimpleRingBuffer`), and Min-Heap priority queue (`SimpleMinHeap`).
  - **Math & Stats**: 2D Complex number arithmetic ($e^z$, conjugate, phase), trigonometry, logarithmic functions, smoothstep interpolation, and statistical utilities (mean, median, geometric/harmonic mean, RMS).
- **Ergonomics, RAD Productivity & Threading**:
  - One-call JSON settings persistence (`save_values_to_file` / `load_values_from_file`).
  - Baseline form dirty-tracking state (`is_dirty`, `is_control_dirty`, `commit_changes`).
  - Multi-select List Box and Table helpers with Cmd/Shift selection and double-click actions.
  - Live search filtering (`bind_search_to_list`) and CSV/JSON table import/export.
  - Application top menu bar (`add_menu`) and right-click context menu binding (`add_context_menu`).
  - Async thread execution (`run_async`, `run_on_main_thread`, `run_on_main_thread_sync`) to run intensive tasks off the UI thread cleanly while supporting deterministic, blocking UI handoff when needed.
  - Native production utilities for app state and OS integration: window frame autosave/restore (`set_frame_autosave_name`, `save_frame`, `restore_frame`), window PNG capture (`capture_screenshot`), clipboard read/write (`copy_to_clipboard`, `get_clipboard_text`, `simplegui.clipboard_text()`), and Finder reveal (`simplegui.reveal_in_finder(path)`).
- **Native Keyboard Shortcuts & Overlay Levels**:
  - `CMD + F` for full screen, `CMD + Q` to quit, custom shortcut recorder widget, and window always-on-top level control.
- **RAD Visual UI Designer & Code Generator**:
  - Delphi/VB/Lazarus-inspired drag-and-drop visual design studio (`v run tools/ui_designer.v`).
  - **⚡ Object Inspector Component Selector Dropdown**: Top dropdown selector listing all controls on the form (`id: ControlType ("Caption")`) for instant selection and canvas highlighting.
  - **Object Inspector Property Search & Filter**: Live keyword filter bar in Object Inspector to instantly search properties (`color`, `width`, `text`, `hover`, etc.).
  - **Auto-Generated Event Callbacks & Code Stubs**: 1-click RAD event generator (`on_<id>_click`, `on_<id>_change`, `on_<id>_hover`, `on_<id>_hover_exit`) and generated V callback function stubs.
  - Full Undo (`Cmd+Z`) and Redo (`Cmd+Shift+Z`) state history engine.
  - Component Tree / Object Hierarchy inspector tab for z-index layer ordering (`Move Up`/`Move Down`) and locking (`Lock`/`Unlock`).
  - **Multi-Selection & Simultaneous Move/Resize**: Marquee drag selection box, `Shift`/`Cmd`-click selection, and `Cmd+A` / `Ctrl+A` Select All with simultaneous multi-control drag moving and multi-control handle resizing, plus batch property updates (width, height, text/caption, font size, font/background colors, color swatch presets, hover styles, cursor styles, position, and RAD event callbacks).
  - Instant **📄 New Form** creation to reset canvas, title, and specs cleanly for fresh layouts.
  - Full alignment and distribution toolbar (`Align Left`, `Center`, `Right`, `Top`, `Middle`, `Bottom`, `Center H Form`, `Center V Form`, `Distribute Horizontally/Vertically`, `Equal Width/Height`, `Fit Text Size`).
  - Smart snap alignment guide lines for instant pixel-perfect layout alignment.
  - Arrow key nudge controls (`1px`, or `8px` with `Shift`).
  - 25+ Supported component types: buttons, labels, inputs, password fields, textareas, checkboxes, switches, sliders, progress indicators, panel boxes, radio buttons, separators, status badges, search inputs, data grids, color wells, date pickers, metric cards, alert banners, circular progress dials, code views, drop zones, rating stars, steppers, and token fields.
  - 10 Pre-loaded layout presets: Customer Registration, Auth Login, KPI Dashboard, Settings Studio, Checkout, Enterprise Data Grid CRUD Manager, Support Ticket Reporter, REST API Client Tester, Audio Media Player, and User Profile.
  - One-click V source code generator producing clean `simplegui` code with event handler callback stubs.
  - Import / Export JSON layout specs, live V runtime state sync (`syncSpecToV()`), and launch live native preview test windows (`launch_preview_window`) executing click, hover, hover exit, and change callbacks.
- **RAD Code Explorer & Live Previewer Studio**:
  - Interactive V code studio & file explorer (`v run tools/vlang_simple_gui_previewer.v`), buildable as a standalone macOS `.app` bundle via `v run scripts/build_previewer.vsh`.
  - **📂 Workspace Folder Management & Drag & Drop**: Select any local V project directory (`w.select_folder()`) or drag-and-drop folders/files directly into the window to open them.
  - **📄 File Creation, Saving & Reverting**: Create new V source files (`📄 New File`), save edits back to disk (`💾 Save File`), or revert unsaved edits (`↺ Reset Code`) to original file contents.
  - **📚 Workspace File Explorer & Search**: Live list box with instant file search filtering (`bind_search_to_list`).
  - **#️⃣ Line Numbers & Line Gutter**: Enabled by default (`#️⃣ Line Numbers`), formatting monospaced line numbers (`001 | `, `002 | `, ...) and auto-stripping line numbers when compiling, formatting (`v fmt`), saving, or copying to clipboard.
  - **🎯 Go To Line Number ("Jump Line")**: Instant line navigation (`input_goto_line` + `btn_goto_line`) that programmatically scrolls the native Cocoa `NSTextView` to the line, highlights it, and displays line contents in the status bar and output console. Preserves keyboard input focus while typing multi-digit numbers.
  - **🔍 Find in Code Search**: Line-by-line code searching (`input_find_code` + `btn_find_code`) highlighting matching line numbers in the diagnostic console log and scrolling to the first match.
  - **📊 Live Code Metrics & Stats**: Header bar displaying file line count, word count, character count, function count (`fn ...`), and live modification indicator (`* [Modified]`).
  - **▶ Live Window Execution**: 1-click compilation and execution (`v run`) with animated loading spinners (`live_spinner`) and non-blocking background thread window launching.
  - **⚡ Integrated Auto-Formatting**: 1-click code formatting via `v fmt -w`.
  - **⚡ Quick RAD Templates**: 1-click loading of boilerplate GUI snippets (Minimal Window, Form & Input Row, Multi-Column Grid Table, Tabbed Layout View, Interactive Event Handlers, Canvas & Custom Drawing, Toolbar & Dialog Alerts).
  - **📋 RAD Code Clipboard Export**: 1-click copy clean V code to system clipboard.
  - **🛠️ Compiler Diagnostics & Console Log**: Integrated diagnostic output log console with color-coded severity levels and 1-click log clear (`tb_console_clear`).
  - **🎨 Multi-Theme Workspace**: Dynamic workspace theme switching (Dracula, Nord, Dark, Light) with high-contrast label color adjustments.

![SimpleGUI RAD Code Explorer & Live Previewer Studio](screenshots/vlang_simple_gui_previewer.png)

![SimpleGUI RAD Visual UI Designer Studio](screenshots/ui_designer.png)

The developer controls demo in [demos/developer_controls_demo.v](demos/developer_controls_demo.v) showcases these richer UI helpers in one place, while [demos/editable_grid_showcase_demo.v](demos/editable_grid_showcase_demo.v) demonstrates the editable-grid workflow with selection, filtering, sorting, and programmatic cell access.

For app code, the grid helpers are intentionally ergonomic:

- `grid_get_rows()` / `grid_set_rows()` replace the full data set in one step.
- `grid_get_row()` / `grid_set_row()` and `grid_get_column()` / `grid_set_column()` cover the common spreadsheet-style operations.
- `grid_get_selected_column()`, `grid_set_selected_column()`, and `grid_set_selected_cell()` make selection easy to drive from code.

---

## 🎨 RAD Visual UI Designer & Code Studio

SimpleGUI includes a dedicated, Delphi & Visual Basic-inspired **Visual UI Designer Studio** (`tools/ui_designer.v`) and **RAD Code Explorer & Live Previewer** (`tools/vlang_simple_gui_previewer.v`). Design forms visually, drag and drop native controls, configure properties with live search, test run interactive windows, and export production-ready V source code in seconds.

![SimpleGUI RAD Visual UI Designer Studio](screenshots/ui_designer.png)

### Launching the Designer Studio

Run the visual designer directly with the V compiler:

```bash
# Launch the Delphi/VB-inspired RAD Visual UI Designer
v run tools/ui_designer.v

# Or launch the interactive V Code Explorer & Live Previewer
v run tools/vlang_simple_gui_previewer.v
```

### Core Designer Features & Capabilities

- **🎨 Drag & Drop WYSIWYG Canvas**: Place and position 25+ native macOS controls directly on the canvas — buttons, inputs, password fields, textareas, checkboxes, switches, sliders, progress dials, color wells, date pickers, editable data grids, tree views, code views, rating stars, steppers, token fields, and file drop zones.
- **⚡ Object Inspector & Instant Property Search**:
  - **Component Selector Dropdown**: Top dropdown selector (`id: ControlType ("Caption")`) for instant selection and canvas highlighting.
  - **Property Filter Bar**: Live keyword search to filter properties (`color`, `width`, `text`, `hover`, `font`, etc.).
  - **Component Tree Inspector**: Visual z-index layer ordering (`Move Up`/`Move Down`) and locking (`Lock`/`Unlock`).
- **🔲 Multi-Selection & Simultaneous Move/Resize**: Marquee drag selection box, `Shift`/`Cmd`-click selection, and `Cmd+A` Select All with simultaneous multi-control drag moving, resize handles, and batch property edits.
- **📐 Layout & Distribution Toolbar**: 1-click alignment (`Align Left`, `Center`, `Right`, `Top`, `Middle`, `Bottom`, `Center H Form`, `Center V Form`, `Distribute Horizontally/Vertically`, `Equal Width/Height`, `Fit Text Size`) with smart snap alignment guide lines.
- **⚡ 1-Click RAD Event Generator & V Code Stubs**: Automatically generates clean event handlers (`on_<id>_click`, `on_<id>_change`, `on_<id>_hover`, `on_<id>_hover_exit`) and idiomatic `simplegui` V code ready to copy to the clipboard or save into a `.v` file.
- **▶️ Live Interactive Test Run**: Click **"Test Run Form"** (`tb_run`) to instantly spin up a live native Cocoa window executing your form layout and callbacks in real time.
- **📋 10+ Pre-Loaded Layout Presets**: Instant starter templates for Customer Registration, Auth Login, KPI Dashboard, Settings Studio, Checkout, Data Grid CRUD Manager, Support Ticket Reporter, REST API Client Tester, Audio Player, and User Profile.
- **↺ Undo / Redo History**: Full `Cmd+Z` and `Cmd+Shift+Z` state history engine.
- **🌐 Standalone HTML5/CSS Web Export**: 1-click export of form layouts to clean HTML5 and CSS for multi-platform preview and web embedding.

---

## Platform Support Matrix

| OS / Architecture | Status | Backend / Renderer | Notes |
|:---|:---|:---|:---|
| **macOS (Apple Silicon arm64)** | Tier 1 (Full Support) | Native Cocoa / Objective-C Bridge | Zero external dependencies; uses system Cocoa framework. |
| **macOS (Intel x86_64)** | Tier 1 (Full Support) | Native Cocoa / Objective-C Bridge | Full feature parity across all macOS releases (10.13+). |
| **Linux (x86_64 / arm64)** | Roadmap / Planned | GTK4 / WebKit Bridge | Architecture abstraction layer under development. |
| **Windows (x64)** | Roadmap / Planned | Win32 / Direct2D | Architecture abstraction layer under development. |

## Installation

### Requirements

- macOS
- [V](https://github.com/vlang/v) installed and available on your `PATH`
- Xcode Command Line Tools (`xcode-select --install`)

### Setup

#### Option A: VPM Package Manager (Recommended)

Install directly via `vpm`:
```bash
v install simplegui
# Or install directly from the repository:
v install https://github.com/codecaine-zz/vlang_simplegui
```

#### Option B: Clone & Local Project Embedding

```bash
git clone https://github.com/codecaine-zz/vlang_simplegui.git
cd vlang_simplegui

# Verify everything works — launches the main demo window
v run .

# Run the test suite
v test .
```

To use SimpleGUI in your own project, install via `v install` (`v install --git https://github.com/codecaine-zz/vlang_simplegui`) and `import simplegui`.

## Quick start

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Starter', 640, 420)
    win.add_input('name', 'Ada')
    win.add_button('save', 'Save')
    win.on_click('save', fn (mut win &simplegui.SimpleWindow) {
        println("saved: ${win.get_text('name')}")
    })
    win.run()
}
```

Event handlers can also be free functions:

```v
import simplegui

fn main() {
    mut gui := simplegui.new_simple_window('My App', 700, 500)

    gui.add_input('name', 'Ada')
    gui.add_button('run', 'Run')

    gui.on_change('name', on_name_changed)
    gui.on_click('run', on_run_clicked)

    gui.run()
}

fn on_name_changed(mut win &simplegui.SimpleWindow, value string) {
    println('name changed: ${value}')
}

fn on_run_clicked(mut win &simplegui.SimpleWindow) {
    println('run clicked')
}
```

## Faster form building

For common forms, a few high-level helpers keep the code short:

```v
mut win := simplegui.new_simple_window('Profile', 640, 420)
win.configure(fn (mut cfg simplegui.WindowConfig) {
    cfg.title = 'Profile'
    cfg.padding = 18
    cfg.spacing = 10
})
win.form('Account', fn (mut w &simplegui.SimpleWindow) {
    w.add_input('email', 'ada@example.com')
    w.add_checkbox('newsletter', 'Subscribe', true)
})
win.section('Preferences', fn (mut w &simplegui.SimpleWindow) {
    w.add_number('experience', 8)
})
win.add_action('save', 'Save', fn (mut win &simplegui.SimpleWindow) {
    println(win.validate_controls({
        'email': simplegui.validate_not_empty
    }))
})
```

## Best and easiest ways to create GUIs and events

These patterns are the fastest and least error-prone way to build native macOS apps with this project.

### 1. Start simple: window → controls → events

For the majority of apps, the easiest flow is:

1. Create a window with `simplegui.new_simple_window(...)`.
2. Add controls with clear names such as `name`, `email`, `save`, or `status`.
3. Connect a small event callback with `add_action(...)`, `on_click(...)`, or `on_change(...)`.
4. Read and update state with helpers like `get_text()`, `get_checked()`, `get_value_int()`, `set_text()`, and `set_status()`.

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Quick Start', 640, 420)

    win.add_input('name', 'Ada')
    win.add_action('save', 'Save', fn (mut win &simplegui.SimpleWindow) {
        name := win.get_text('name')
        win.alert('Saved', 'Hello ${name}')
    })

    win.run()
}
```

### 2. Prefer the high-level helpers for common UI patterns

Use the builder helpers when you want to move quickly:

- `.configure(...)` for window spacing and padding.
- `.form(...)` and `.section(...)` for grouped content.
- `.row(...)` for side-by-side controls.
- `add_action(...)` for buttons that need a single handler.

These helpers usually make the code shorter and easier to read than manually stitching together low-level layout calls.

### 3. High-Level Reactive Control Bindings (`bind_*`)

SimpleGUI provides built-in `bind_*` convenience methods to handle common reactive UI patterns automatically without writing manual event listeners:

- **`bind_checkbox_enables`**: Keeps controls enabled while checked, disabled when unchecked.
  ```v
  win.bind_checkbox_enables('enable_sec', ['sec_pin', 'sec_phone'])
  ```

- **`bind_checkbox_disables`**: Keeps controls disabled while checked, enabled when unchecked.
  ```v
  win.bind_checkbox_disables('use_system_theme', ['custom_theme_select'])
  ```

- **`bind_checkbox_shows` / `bind_checkbox_hides`**: Shows or hides controls dynamically based on checkbox state.
  ```v
  win.bind_checkbox_shows('enable_advanced', ['adv_panel_1', 'adv_panel_2'])
  ```

- **`bind_inputs_to_button`**: Keeps an action button enabled ONLY when all specified required input fields have non-empty text.
  ```v
  win.bind_inputs_to_button(['username', 'email'], 'submit_btn')
  ```

- **`bind_value_to_label`**: Dynamic control value mirroring (slider, stepper, input, dropdown) into a formatted text label.
  ```v
  win.bind_value_to_label('volume_slider', 'volume_label', 'Master Audio Volume: ', '%')
  // Renders label text as: "Master Audio Volume: 75%"
  ```

- **`bind_value_to_progress`**: Syncs slider/stepper/number values directly to a progress indicator bar.
  ```v
  win.bind_value_to_progress('volume_slider', 'vol_progress')
  ```

- **`bind_dropdown_to_label`**: Maps dropdown selection value to label text dynamically using a lookup map dictionary.
  ```v
  win.bind_dropdown_to_label('plan_select', 'price_label', {
      'Free': 'Price: $0/mo'
      'Pro':  'Price: $29/mo'
  })
  ```

- **`bind_two_way`**: Bi-directionally synchronizes two controls (e.g. slider <-> number input) without infinite feedback loops.
  ```v
  win.bind_two_way('input_a', 'input_b')
  ```

- **`bind_char_counter`**: Tracks character length of an input/textarea, updates `"used/max"` label, and triggers inline validation errors when limit is exceeded.
  ```v
  win.bind_char_counter('bio_input', 'bio_counter_lbl', 30)
  // Displays "12/30" and flags error "Maximum 30 characters" when exceeded.
  ```

- **`bind_search_to_list`**: Wires a search field to a list box so typing live-filters visible rows using case-insensitive substring matching.
  ```v
  win.bind_search_to_list('contact_search', 'contacts_list')
  ```

- **`bind_to_struct`**: Uses compile-time reflection (`$for field in T.fields`) to extract matching form values into a struct instance.
  ```v
  struct UserProfile {
      username  string
      bio_input string
  }
  mut profile := UserProfile{}
  win.bind_to_struct(mut profile)
  ```

### 4. Best event pattern for most apps

- Use `add_action(...)` for button presses.
- Use `on_change(...)` for text, dropdown, checkbox, or slider updates.
- Keep callbacks compact and read values from the window at the time of the event.
- Update the UI with `set_text(...)`, `set_checked(...)`, `set_value_int(...)`, or `set_status(...)`.

```v
mut win := simplegui.new_simple_window('Event Demo', 560, 420)
win.add_input('email', '')
win.on_change('email', fn (mut win &simplegui.SimpleWindow, value string) {
    win.set_status('Typing: ${value}')
})
win.run()
```

### 4. The Easiest Way to Build (The Golden Rules)

- **Always Configure Early**: Use `.configure(...)` right at the start to declare window geometry, paddings, and alignment spacing.
- **Keep Event Callbacks Small**: Avoid global mutability. Read current values with `win.get_text()`, `win.get_checked()`, or `win.get_value_int()` and update the UI directly.
- **Use Fluent Chaining**: Builder modifiers like `.placeholder()`, `.tooltip()`, `.width()`, and `.enabled()` can be chained immediately after creating the control, avoiding redundant `win.set_...` calls.

---

## Starter templates

Here are copy-pasteable beginner templates for **every layout container and control pattern** supported by SimpleGUI:

---

### 1. Vertical Stack Layout (Default Flow)

Best for simple forms, log views, setup wizards, and linear layouts where controls stack nicely top-to-bottom.

```v
module main

import simplegui

fn main() {
    simplegui.new_simple_window('Vertical Starter', 440, 520)
        .configure(fn (mut cfg simplegui.WindowConfig) {
            cfg.padding = 20
            cfg.spacing = 12
        })
        .add_heading('App Settings')

        .add_label('lbl_user', 'Username')
        .add_input('username', 'ada_lovelace')
            .placeholder('Enter username...')
            .tooltip('At least 3 characters')

        .add_toggle('newsletter', 'Subscribe to daily newsletter', true)

        .add_action('btn_submit', 'Submit Settings', on_submit)
        .run()
}

fn on_submit(mut win &simplegui.SimpleWindow) {
    username := win.get_text('username')
    subscribed := win.get_checked('newsletter')

    win.alert('Status Updated', 'Saved user ${username} (Newsletter: ${subscribed})')
}
```

---

### 2. Side-by-Side Horizontal Row Layout (`win.row` / `win.begin_row`)

Best for tabular layouts, search bars with inline action buttons, or database filter fields aligned horizontally.

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Row Grid Starter', 640, 320)
        .set_theme('dracula')
        .set_padding(20)

    win.add_heading('Database Lookup Filters')

    // Closure-based row helper aligns children side-by-side automatically
    win.row('filters_row', fn (mut w &simplegui.SimpleWindow) {
        w.add_label('', 'Category:')
        w.add_dropdown('category', ['Engineering', 'Marketing', 'Sales'], 'Engineering')
            .width(150)
            .onchange(on_filter_changed)

        w.add_label('', 'ID:')
        w.add_number('filter_id', 101)
            .width(80)
            .onchange(on_filter_changed)

        w.add_button('btn_search', 'Search')
            .onclick(on_search_clicked)
    })

    win.add_textarea('output', 'Search results will render here...')
        .height(120)

    win.run()
}

fn on_filter_changed(mut win &simplegui.SimpleWindow, value string) {
    category := win.get_text('category')
    id := win.get_value_int('filter_id')
    win.set_status('Active filter: Category=${category}, ID=${id}')
}

fn on_search_clicked(mut win &simplegui.SimpleWindow) {
    category := win.get_text('category')
    id := win.get_value_int('filter_id')

    win.set_text('output', 'Running database query...\nFetched records matching ${category} with minimum ID ${id}!')
    win.toast('Queries fetched successfully')
}
```

---

### 3. Multi-Column Grid Layout (`win.begin_grid` / `win.grid`)

Best for multi-column registration forms, survey cards, or dashboard grid boxes where controls automatically wrap across horizontal columns (e.g. 2-column forms or 3/4-column action grids) with custom element spacing.

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Multi-Column Grid Showcase', 680, 560)
        .set_padding(20)
        .set_spacing(12)

    win.add_heading('User Profile & Dashboard (Multi-Column Grid)')

    // 1. 2-Column Registration Form Grid with right-aligned labels (tight spacing to inputs)
    win.begin_grid('reg_grid', 2, 8)
        .add_label('lbl_fn', 'First Name:').align_right()
        .add_input('first_name', 'Ada')
        .add_label('lbl_ln', 'Last Name:').align_right()
        .add_input('last_name', 'Lovelace')
        .add_label('lbl_email', 'Email Address:').align_right()
        .add_input('email', 'ada@example.com')
        .add_label('lbl_role', 'Role:').align_right()
        .add_dropdown('role', ['Developer', 'Designer', 'Manager'], 'Developer')
        .end_grid()

    win.add_vertical_spacer(15)

    // 2. Closure-based 3-Column Metrics & Actions Grid (10px spacing)
    win.grid('actions_grid', 3, 10, fn (mut g simplegui.SimpleWindow) {
        g.add_button('btn_save', 'Save Changes')
        g.add_button('btn_reset', 'Reset Form')
        g.add_button('btn_export', 'Export CSV')
    })

    win.add_vertical_spacer(15)
    win.add_button('btn_submit', 'Submit Registration')
       .on_click('btn_submit', fn (mut w simplegui.SimpleWindow) {
           fn_str := w.get_text('first_name')
           ln_str := w.get_text('last_name')
           w.info('Welcome', 'Registered ${fn_str} ${ln_str}!')
       })

    win.run()
}
```

---

### 4. Flexbox Container Layout (`win.begin_flex_box` / `win.flex_box`)

Best for responsive toolbars, action footers, and flexible container cards with main-axis alignment (`start`, `center`, `end`, `space_between`, `fill`).

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Flexbox Starter', 600, 380)
        .set_padding(20)
        .add_heading('Flexbox Action Toolbar')

    // Flexbox row container with space_between distribution and centered alignment
    win.begin_flex_box('flex_bar', 'row', 'space_between', 'center')
        .add_label('lbl_status', 'Status: System Online')
        .add_button('btn_refresh', 'Refresh Data')
        .add_button('btn_settings', 'Settings')
        .end_flex_box()

    win.add_separator()
    win.add_textarea('log_view', 'Activity logs will appear here...')
        .height(180)

    win.run()
}
```

---

### 5. Group Box Container & Card Styling (`win.group` / `win.group_config` / `win.card`)

Best for visually grouping related form fields (e.g., Personal Info, Security Settings, Preferences) inside framed boxes or elevated card containers. Supports fine-grained styling: border stroke thickness (`border_width`), custom border colors (`border_color`), corner rounding (`corner_radius`), card fill colors (`bg_color`), inner padding (`padding`), drop shadows (`shadow`), and header title alignment/colors (`caption_alignment`, `caption_color`).

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Group Box & Card Studio', 600, 680)
        .configure(fn (mut cfg simplegui.WindowConfig) {
            cfg.padding = 18
            cfg.spacing = 12
        })
        .add_heading('Group Containers & Custom Cards')

    // Standard group box with default theme styling
    win.group('profile_group', 'Personal Details', fn (mut w simplegui.SimpleWindow) {
        w.add_input('name', 'Ada Lovelace')
        w.add_input('email', 'ada@example.com')
    })

    // Custom Pill Card with Accent Border & Tint Background Fill
    win.group_config('accent_card', simplegui.GroupConfig{
        title: 'Security Credentials'
        border: true
        border_width: 2.0
        border_color: '#3B82F6'
        corner_radius: 18.0
        bg_color: '#F0F6FF'
        padding: 16
        caption_color: '#1D4ED8'
    }, fn (mut w simplegui.SimpleWindow) {
        w.add_switch('2fa_switch', 'Enable Two-Factor Authentication', true)
        w.add_password('pass', 'secret_password')
    })

    // Borderless Elevated Card with Drop Shadow
    win.card('shadow_card', fn (mut w simplegui.SimpleWindow) {
        w.add_label('lbl_card', 'Elevated card container with drop shadow & custom padding.')
        w.add_button('btn_action', 'Perform Action')
    })

    // Dynamic runtime style update
    win.set_group_style('accent_card', simplegui.GroupConfig{
        border: true
        border_width: 2.5
        border_color: '#10B981'
        corner_radius: 24.0
        bg_color: '#ECFDF5'
    })

    win.run()
}
```

---

### 6. Tabbed Multi-Pane Layout (`win.add_tabs`)

Best for multi-view settings windows, preference panels, or dashboards where users switch between views using top tab buttons.

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Tabs Starter', 640, 420)
        .set_padding(15)

    // Native macOS tabbed container view
    win.add_tabs('main_tabs', ['General', 'Appearance', 'Notifications'])
        .add_heading('General Preferences')
        .add_input('app_name', 'My Custom Mac App')
        .add_checkbox('auto_start', 'Launch application at system login', true)
        .add_button('btn_save', 'Save Preferences')
        .on_click('btn_save', fn (mut w simplegui.SimpleWindow) {
            w.toast('Preferences saved successfully!')
        })

    win.run()
}
```

---

### 7. Scroll View Container Layout (`win.add_scroll_view`)

Best for displaying long text documents, privacy policies, terms of service, or log feeds that exceed the window height.

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Scroll View Starter', 520, 440)
        .set_padding(20)
        .add_heading('Terms of Service')

    win.add_label('lbl_sub', 'Please review the license terms below:')
    win.set_control_font_size('lbl_sub', 12)

    win.add_vertical_spacer(8)

    // Scrollable text area container with 200px viewport height
    win.add_textarea('terms_text', '1. License Agreement\nBy using this software, you agree to all terms and conditions set forth herein.\n\n2. Privacy Policy\nYour data remains private and stored strictly on your local machine.\n\n3. User Responsibilities\nDo not use this software for unauthorized network scanning or malicious activities.\n\n4. Warranty Disclaimer\nThis software is provided AS-IS without warranty of any kind, express or implied.\n\n5. Termination\nYour right to use this software terminates automatically upon violation of these terms.')
        .height(200)

    win.add_vertical_spacer(12)
    win.add_checkbox('accept_chk', 'I have read and agree to the terms', false)

    win.add_vertical_spacer(10)
    win.add_button('btn_continue', 'Continue')
       .on_click('btn_continue', fn (mut w simplegui.SimpleWindow) {
           if w.get_checked('accept_chk') {
               w.info('Success', 'Terms accepted!')
           } else {
               w.warn('Notice', 'Please check the agreement box before continuing.')
           }
       })

    win.run()
}
```

---

### 8. Multi-Column Data Table Layout (`win.add_table`)

Best for displaying structured records, employee lists, inventory catalogs, or database results in a multi-column table view.

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Data Table Starter', 640, 420)
        .set_padding(18)
        .add_heading('Employee Directory Table')

    // Create a multi-column table with 3 headers
    win.add_table('employees_table', ['ID', 'Name', 'Department'])
       .set_table_rows('employees_table', [
           ['101', 'Ada Lovelace', 'Engineering'],
           ['102', 'Alan Turing', 'Research'],
           ['103', 'Grace Hopper', 'Systems'],
       ])

    win.on_table_select('employees_table', fn (mut w simplegui.SimpleWindow, row_idx string) {
        w.set_status('Selected row index: ${row_idx}')
    })

    win.run()
}
```

---

### 9. Hierarchical Tree View Layout (`win.add_tree_view`)

Best for file browsers, folder navigation, category hierarchies, or organization charts with expandable parent-child nodes.

```v
module main

import simplegui

fn main() {
    mut win := simplegui.new_simple_window('Tree View Starter', 520, 400)
        .set_padding(18)
        .add_heading('Project File Explorer Tree')

    // Define tree nodes with parent-child relationships
    nodes := [
        simplegui.tree_root('root', 'My Project Workspace'),
        simplegui.tree_child('src', 'root', 'src/'),
        simplegui.tree_child('main', 'src', 'main.v'),
        simplegui.tree_child('gui', 'src', 'simplegui.v'),
        simplegui.tree_child('docs', 'root', 'README.md'),
    ]

    win.add_tree_view('file_tree', 250)
       .set_tree_nodes('file_tree', nodes)

    win.run()
}
```

---

### 10. State Controller & Reactive Event Binding Pattern

Use this template to see how to programmatically manipulate labels, checkboxes, text values, sliders, and progress loaders dynamically on click or change.

```v
module main

import simplegui

fn main() {
    simplegui.new_simple_window('State Controller', 520, 480)
        .set_theme('nord')
        .set_padding(18)
        .add_heading('Controller Panel')

        .add_slider('volume_slider', 50)
            .onchange(on_volume_changed)

        .add_label('lbl_volume', 'Current Volume: 50%')

        .add_separator()

        .add_checkbox('mute_toggle', 'Mute output entirely', false)
            .onchange(on_mute_toggled)

        .add_progress_indicator('prog_bar', 50)

        .add_action('reset_btn', 'Reset Back to Factory Defaults', on_reset_clicked)
        .run()
}

fn on_volume_changed(mut win &simplegui.SimpleWindow, value string) {
    // 1. Get mutated value
    vol := win.get_value_int('volume_slider')

    // 2. Set companion label text and progress indicator
    win.set_text('lbl_volume', 'Current Volume: ${vol}%')
    win.set_value_int('prog_bar', vol)
}

fn on_mute_toggled(mut win &simplegui.SimpleWindow, value string) {
    muted := win.get_checked('mute_toggle')

    if muted {
        // Backup volume slider, set visual feedback or disable
        win.set_text('lbl_volume', 'Current Volume: MUTED')
        win.set_value_int('prog_bar', 0)
        win.set_control_enabled('volume_slider', false)
    } else {
        // Re-enable and restore companion status
        vol := win.get_value_int('volume_slider')
        win.set_text('lbl_volume', 'Current Volume: ${vol}%')
        win.set_value_int('prog_bar', vol)
        win.set_control_enabled('volume_slider', true)
    }
}

fn on_reset_clicked(mut win &simplegui.SimpleWindow) {
    // Programmatically overwrite and set states on control handles
    win.set_value_int('volume_slider', 50)
    win.set_text('lbl_volume', 'Current Volume: 50%')
    win.set_value_int('prog_bar', 50)
    win.set_checked('mute_toggle', false)
    win.set_control_enabled('volume_slider', true)

    win.toast('Defaults restored')
}
```

---

### 11. High-Level Reactive Control Bindings (`demos/binding_demo.v`)

Demonstrates all 5 high-level `bind_*` convenience methods in a single interactive application without writing manual event listeners.

```v
module main

import simplegui

struct UserProfile {
	username      string
	bio_input     string
	sec_pin       string
	enable_sec    bool
	volume_slider int
}

fn main() {
	mut win := simplegui.new_simple_window('SimpleGUI Reactive Bindings Showcase', 680, 750)
		.set_theme('dracula')
		.set_padding(18)
		.set_spacing(12)

	win.add_heading('SimpleGUI High-Level Control Bindings Demo')

	// 1. bind_checkbox_enables: Keeps controls enabled when checked, disabled when unchecked.
	win.group('sec_group', '1. Checkbox / Switch Conditional Enabling (bind_checkbox_enables)', fn (mut w simplegui.SimpleWindow) {
		w.add_switch('enable_sec', 'Enable Two-Factor & PIN Security', false)
		w.add_input('username', 'ada_lovelace')
		w.add_input('sec_pin', '9482')
			.placeholder('Enter Security PIN...')
	})
	win.bind_checkbox_enables('enable_sec', ['sec_pin'])

	win.add_vertical_spacer(8)

	// 2. bind_value_to_label: Dynamic control value mirroring formatted with prefix/suffix.
	win.group('slider_group', '2. Dynamic Control Value Mirroring (bind_value_to_label)', fn (mut w simplegui.SimpleWindow) {
		w.add_label('volume_label', 'Volume: 75%')
		w.add_slider('volume_slider', 75)
	})
	win.bind_value_to_label('volume_slider', 'volume_label', 'Master Audio Volume: ', '%')

	win.add_vertical_spacer(8)

	// 3. bind_char_counter: Tracks text length, updates label, and flags inline error on limit overflow.
	win.group('counter_group', '3. Character Counter & Validation (bind_char_counter)', fn (mut w simplegui.SimpleWindow) {
		w.add_label('bio_counter_lbl', '0/30')
		w.add_input('bio_input', 'SimpleGUI reactive UI!')
			.placeholder('Write a short bio (max 30 chars)...')
	})
	win.bind_char_counter('bio_input', 'bio_counter_lbl', 30)

	win.add_vertical_spacer(8)

	// 4. bind_search_to_list: Live-filters list box rows using case-insensitive substring matching.
	win.group('search_group', '4. Live Search Box Filtering (bind_search_to_list)', fn (mut w simplegui.SimpleWindow) {
		w.add_search_field('contact_search', 'Filter contacts...')
		w.add_list_box('contacts_list', [
			'Ada Lovelace (Engineer)',
			'Alan Turing (Cryptographer)',
			'Grace Hopper (Computer Scientist)',
			'John von Neumann (Mathematician)',
			'Claude Shannon (Information Theory)',
			'Margaret Hamilton (Software Lead)',
			'Linus Torvalds (Kernel Architect)',
			'Guido van Rossum (Python Creator)',
		])
	})
	win.bind_search_to_list('contact_search', 'contacts_list')

	win.add_vertical_spacer(8)

	// 5. bind_to_struct: Extracts matching form values into struct via reflection.
	win.group('struct_group', '5. Automatic Form Extraction via Struct Reflection (bind_to_struct)', fn (mut w simplegui.SimpleWindow) {
		w.add_action_row({
			'Export Struct Data': on_export_struct
		})
	})

	win.set_status('Interactive Bindings Demo Ready.')
	win.run()
}

fn on_export_struct(mut win simplegui.SimpleWindow) {
	mut profile := UserProfile{}
	win.bind_to_struct(mut profile)

	sec_status := if profile.enable_sec { 'Enabled' } else { 'Disabled' }
	msg := 'Extracted UserProfile Struct:\n' +
		'• Username: ${profile.username}\n' +
		'• Bio: ${profile.bio_input}\n' +
		'• Security PIN: ${profile.sec_pin}\n' +
		'• Security Switch: ${sec_status}\n' +
		'• Volume Setting: ${profile.volume_slider}%'

	win.alert('Struct Reflection Export', msg)
	win.set_status('Extracted form state into struct via reflection.')
}
```

## Developer tips

- Use the built-in control discovery helpers: `has_control`, `list_controls`, and `get_control_kind`.
- Calling `get_value`, `set_value`, `get_checked`, or similar on a missing control now raises a clear panic to make mistakes visible early.
- The API is intentionally lightweight; start with the named control helpers and add layout helpers only when needed.

## Compiling & Packaging as a macOS App

To build and package your V project into a standalone, native macOS application bundle (`.app`) with custom icons, use the pure V tool script `build.vsh`. The builder uses macOS native utilities (`sips` and `iconutil`) with zero JavaScript runtime dependencies.

### 1. Default Build

To compile `main.v` with release optimization (`-prod`) and bundle it as a macOS application using the default app name:

```bash
v run scripts/build.vsh
```

This compiles your V code and creates:

```text
dist/Vlang Macos Native Window.app
```

### 2. Custom App Packaging

You can build the app with a custom entry point, a custom name, a custom icon PNG, and a custom bundle ID:

```bash
v run scripts/build.vsh [entry_file.v] --name "My Custom App" --icon icon.png --identifier "com.example.myapp"
```

### 3. Compilation Examples with Premium Icons

This project comes packaged with **101 premium, futuristic Apple-style obsidian/glassmorphism high-fidelity icons** in the `resources/` folder. Below are practical examples showcasing how to compile existing demos as standalone native `.app` bundles styled with their corresponding theme-matched icons:

```bash
# 1. Compile the Calculator Demo with the native Calculator Tile icon
v run scripts/build.vsh demos/calculator.v --name "Interactive Calculator" --icon resources/calculator.png

# 2. Compile the Markdown Editor with the native Markdown Editor Tile icon
v run scripts/build.vsh demos/markdown_editor.v --name "Markdown Studio" --icon resources/markdown_editor.png

# 3. Compile the System Monitor Demo with the native System Monitor Tile icon
v run scripts/build.vsh demos/timer_demo.v --name "Task Timer" --icon resources/clock.png

# 4. Compile the Data Viewer / Database Catalog with the Database Admin Tile icon
v run scripts/build.vsh demos/data_viewer.v --name "DB Browser" --icon resources/database_admin.png

# 5. Compile the Settings Configuration Editor with the Password manager / Security Tile icon
v run scripts/build.vsh demos/settings_editor.v --name "Preferences Panel" --icon resources/password_manager.png

# 6. Compile the Web Studio Demo with the high-fidelity Browser / DOM Explorer Tile icon
v run scripts/build.vsh demos/web_studio_demo.v --name "Web BI Studio" --icon resources/browser.png

# 7. Compile the RAD Code Explorer & Live Previewer into a standalone macOS .app bundle with icon
v run scripts/build_previewer.vsh
```

### 4. Batch Compile All Demos

To compile and package all available demos concurrently in a single command, run:

```bash
v run scripts/build_demos.vsh
```

This script:

1. **Pre-compiles** `build.vsh` to a temporary `./build_app` binary to bypass redundant compilations and prevent C/V compiler concurrency conflicts.
2. **Scans** `demos/` and maps each application to its corresponding premium icon in `resources/` (e.g., `calculator.png` for the Calculator, `database.png` for SQLite CRUD, etc.).
3. **Runs** the packaging pipeline concurrently in parallel batches (6 tasks by default), significantly speeding up the build process.
4. **Cleans up** the temporary compiler helper when finished.

#### CLI Options

- `-i, --icon <path>`: Path to a PNG icon. Defaults to `resources/icon.png` or `icon.png`. (If no icon is found, the builder will gracefully assemble the `.app` bundle using a default macOS application icon).
- `-n, --name <name>`: Custom display name for the `.app` bundle.
- `-d, --identifier <id>`: CFBundleIdentifier (e.g., `com.example.myapp`).
- `-v, --version <version>`: App version (defaults to version in `v.mod`, or `1.0.0`).
- `-o, --out <dir>`: Output folder (defaults to `dist`).
- `-h, --help`: Show help message.

## Capturing Demo Screenshots

To automatically launch every demo, capture a screenshot of its window, and save the result to the `screenshots/` folder, use the pure V shell script `capture_demos.vsh` in `scripts/`. It requires no Python or external runtime — only V and the macOS Clang toolchain.

### Capture All Demos

```bash
v run scripts/capture_demos.vsh
```

### Capture a Single Demo

Pass the demo filename (with or without the `.v` extension) as an argument:

```bash
v run scripts/capture_demos.vsh beginner_demo
```

### How It Works

1. **Compiles** the `list_windows.m` Objective-C helper using `clang` to enumerate on-screen windows and their coordinates.
2. **Iterates** over all `.v` files in the `demos/` folder (or the single specified demo).
3. **Compiles and launches** each demo, then waits 2 seconds for the window to appear.
4. **Locates the window** by matching the process PID or binary name against the live window list.
5. **Captures a cropped screenshot** of the window rect using macOS `screencapture -R`.
6. **Saves** the PNG to `screenshots/<demo_name>.png`, then terminates the demo and cleans up the compiled binary.

## Demos

The [demos/](demos/) folder contains 70+ runnable examples covering every control, layout style, and stdlib integration. Run any demo with:

```bash
v run demos/<demo_name>.v
```

Run the main demo, which combines the vertical stack and grid layout styles:

```bash
v run .
```

### Getting started & starter templates

| Demo                                                           | Description                                                                 |
| -------------------------------------------------------------- | --------------------------------------------------------------------------- |
| [ui_designer.v](tools/ui_designer.v)                           | Delphi & VB-inspired Visual RAD Designer studio & V code generator          |
| [vlang_simple_gui_previewer.v](tools/vlang_simple_gui_previewer.v) | Interactive V Code Explorer & Live Window Preview Studio                 |
| [starter_template.v](demos/starter_template.v)                 | Minimal starter app for new developers                                      |
| [beginner_demo.v](demos/beginner_demo.v)                       | Beginner-friendly signup form and profile builder                           |
| [vertical_stack_starter.v](demos/vertical_stack_starter.v)     | Best-practice template for vertical stack forms                             |
| [grid_column_starter.v](demos/grid_column_starter.v)           | Best-practice template for horizontal row layouts and events                |
| [state_controller_pattern.v](demos/state_controller_pattern.v) | State manipulation and reactive controls pattern                            |
| [stack_style.v](demos/stack_style.v)                           | Clean, vertical form stacking                                               |
| [grid_style.v](demos/grid_style.v)                             | Side-by-side row-based grids                                                |
| [guessing_game.v](demos/guessing_game.v)                       | Guess-the-number game with level indicators, rating stars, and history logs |

### Layout system

| Demo                                                                       | Description                                                                                  |
| -------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------- |
| [layout_vertical_stack.v](demos/layout_vertical_stack.v)                   | Linear vertical stacking, padding, spacing, dividers, and spacers                            |
| [layout_horizontal_rows.v](demos/layout_horizontal_rows.v)                 | Side-by-side rows, spacers, action rows, and field rows                                      |
| [layout_form_sections.v](demos/layout_form_sections.v)                     | Semantic forms, section blocks, and form control validation                                  |
| [layout_group_boxes.v](demos/layout_group_boxes.v)                         | Visual panel containment boxes                                                               |
| [group_box_card_styling_demo.v](demos/group_box_card_styling_demo.v)       | Custom group box borders, stroke thickness, corner radii, background fills, and drop shadows  |
| [layout_tabs.v](demos/layout_tabs.v)                                       | Interactive native tabs switching between multi-view panels                                  |
| [layout_scroll_view.v](demos/layout_scroll_view.v)                         | Scrollable panel constraints                                                                 |
| [layout_struct_reflection.v](demos/layout_struct_reflection.v)             | Auto-generating forms from structs using compile-time reflection                             |
| [layout_responsive_constraints.v](demos/layout_responsive_constraints.v)   | Responsive auto-layout scaling vs fixed constraints                                          |
| [layout_events_mini_demo.v](demos/layout_events_mini_demo.v)               | Compact showcase combining sections, rows, groups, and event bindings                        |
| [layout_advanced_grid_flex_demo.v](demos/layout_advanced_grid_flex_demo.v) | Multi-column grid forms, flexbox directions & distribution, alignment, and nested containers |

### Controls & widgets

| Demo                                                                 | Description                                                                                                                                                                    |
| -------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| [all_controls_demo.v](demos/all_controls_demo.v)                     | Comprehensive 20-section showcase of every `win.add_*` control in [docs/API.md](docs/API.md) — text, buttons, sliders, pickers, charts, grids, badges, stat cards, code editor, timeline, and more |
| [cursor_demo.v](demos/cursor_demo.v)                                 | Window-wide and per-control cursor icon/size customization, mouse warping, and live mouse tracking                                                                             |
| [developer_controls_demo.v](demos/developer_controls_demo.v)         | Breadcrumbs, shortcut recorders, charts, gauges, property grids, and log consoles                                                                                              |
| [editable_grid_showcase_demo.v](demos/editable_grid_showcase_demo.v) | Editable grid workflow: selection, filtering, sorting, and programmatic cell access                                                                                            |
| [spy_plus_plus_demo.v](demos/spy_plus_plus_demo.v)                   | Production Spy++-style inspector: target table + control tree, strict selector mode, value watch mode, health checks, action history, and JSON snapshot export                 |
| [ext_spy_calc_check.v](demos/ext_spy_calc_check.v)                   | External Calculator accessibility probe: auto-opens Calculator, drives the UI with AXUIElement, and verifies button presses plus readback results                      |
| [tree_view_demo.v](demos/tree_view_demo.v)                           | Hierarchical tree view                                                                                                                                                         |
| [advanced_features_demo.v](demos/advanced_features_demo.v)           | Advanced typography and macOS APIs                                                                                                                                             |
| [menu_demo.v](demos/menu_demo.v)                                     | Standard macOS application menus and text editing shortcuts                                                                                                                    |
| [colors_demo.v](demos/colors_demo.v)                                 | Live custom colors styling sandbox for typography and backgrounds                                                                                                              |
| [animation_demo.v](demos/animation_demo.v)                           | Animated control and window effects                                                                                                                                            |
| [lorem_and_html_demo.v](demos/lorem_and_html_demo.v)                 | Placeholder text generation and HTML rendering                                                                                                                                 |

### Complete example apps

| Demo                                                       | Description                                                             |
| ---------------------------------------------------------- | ----------------------------------------------------------------------- |
| [calculator.v](demos/calculator.v)                         | Interactive math calculator with nested grid rows                       |
| [settings_editor.v](demos/settings_editor.v)               | Advanced dashboard with date picker, color wells, and modes             |
| [data_viewer.v](demos/data_viewer.v)                       | Mock database user records lookup table with filters                    |
| [markdown_editor.v](demos/markdown_editor.v)               | Live Markdown Studio with full WebKit render updates                    |
| [web_studio_demo.v](demos/web_studio_demo.v)               | BI KPI & fintech analytics board mixing native widgets with HTML/CSS/JS |
| [grid_data_editor.v](demos/grid_data_editor.v)             | Reactive inventory-catalog CRUD grid editing dashboard                  |
| [sqlite_crud_demo.v](demos/sqlite_crud_demo.v)             | SQLite dashboard performing CREATE, READ, UPDATE, DELETE actions        |
| [todo_list_demo.v](demos/todo_list_demo.v)                 | Todo list built on the list box item-management helpers                 |
| [table_manager_demo.v](demos/table_manager_demo.v)         | Inventory manager built on the table row-management helpers             |
| [pomodoro_timer_demo.v](demos/pomodoro_timer_demo.v)       | Focus clock with live progress timer, toasts, and session settings      |
| [password_dashboard.v](demos/password_dashboard.v)         | Lockbox security dashboard with credentials table and detail forms      |
| [rest_client_demo.v](demos/rest_client_demo.v)             | REST client API studio with methods, params, headers, and JSON viewer   |
| [timer_demo.v](demos/timer_demo.v)                         | Background timer tasks updating progress indicators periodically        |
| [list_image_demo.v](demos/list_image_demo.v)               | Interactive list selector previewing images in real time                |
| [clipboard_demo.v](demos/clipboard_demo.v)                 | Clipboard monitor with history log                                      |
| [overlay_widget_demo.v](demos/overlay_widget_demo.v)       | Sticky floating yellow notepad overlay widget                           |
| [worker_pool_visualizer.v](demos/worker_pool_visualizer.v) | Concurrent task queue monitor with progress bars and worker states      |
| [grid_beginner_demo.v](demos/grid_beginner_demo.v)         | Interactive 2D painting grid with presets and native color wells        |

### Developer experience & ergonomics

| Demo                                                         | Description                                                                        |
| ------------------------------------------------------------ | ---------------------------------------------------------------------------------- |
| [list_table_toolkit_demo.v](demos/list_table_toolkit_demo.v) | Live search filtering, sorting, reordering, CSV export/import, and validators      |
| [features_demo.v](demos/features_demo.v)                     | Compile-time struct binding and automatic multi-column tables                      |
| [configuration_demo.v](demos/configuration_demo.v)           | Fluent configuration using `WindowConfig`                                          |
| [dirty_form_demo.v](demos/dirty_form_demo.v)                 | Live form dirty-tracking state and control validation callbacks                    |
| [save_restore_demo.v](demos/save_restore_demo.v)             | One-call JSON settings persistence with unsaved-changes prompts                    |
| [delphi_inspired_demo.v](demos/delphi_inspired_demo.v)       | Classic Delphi RAD tool look and event bindings                                    |
| [events_demo.v](demos/events_demo.v)                         | Hover, focus, blur, and window resize event listeners                              |
| [always_on_top_demo.v](demos/always_on_top_demo.v)           | Window z-axis float levels and the always-on-top API                               |

### Networking & security

| Demo                                                     | Description                                      |
| -------------------------------------------------------- | ------------------------------------------------ |
| [tcp_socket_demo.v](demos/tcp_socket_demo.v)             | Client-server TCP networking with message logs   |
| [udp_socket_demo.v](demos/udp_socket_demo.v)             | Datagram packet transfer over UDP sockets        |
| [unix_socket_demo.v](demos/unix_socket_demo.v)           | Local IPC using Unix domain sockets              |
| [secure_socket_demo.v](demos/secure_socket_demo.v)       | TLS-encrypted client-server socket communication |
| [secure_udp_demo.v](demos/secure_udp_demo.v)             | DTLS-encrypted datagram transfers                |
| [secure_unix_demo.v](demos/secure_unix_demo.v)           | TLS-encrypted Unix domain sockets                |
| [secure_websocket_demo.v](demos/secure_websocket_demo.v) | Secure WebSocket client-server networking        |
| [wrapped_sockets_demo.v](demos/wrapped_sockets_demo.v)   | Simplified wrapper APIs for TCP socket streams   |

### System, data & performance

| Demo                                                                         | Description                                                                                                    |
| ---------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------- |
| [deflate_demo.v](demos/deflate_demo.v)                                       | Compress and decompress strings using zlib deflate                                                             |
| [zstd_demo.v](demos/zstd_demo.v)                                             | Zstandard compression and decompression                                                                        |
| [encoding_and_system_info_demo.v](demos/encoding_and_system_info_demo.v)     | Hex/Base64 encoder-decoder with environment details viewer                                                     |
| [sys_demo.v](demos/sys_demo.v)                                               | Neutralino-inspired system call extensions, OS diagnostics, hardware specs, network tools, and shell utilities |
| [macos_power_controls_demo.v](demos/macos_power_controls_demo.v)             | Global macOS theme/power/session controls with keep-awake guard status and safe confirmations                  |
| [system_calls_demo.v](demos/system_calls_demo.v)                             | Shell process invocation, stdout routing, and folder monitoring                                                |
| [benchmark_demo.v](demos/benchmark_demo.v)                                   | Wrapper operation latency benchmarks                                                                           |

### Run the Stack Style demo

```bash
v run demos/stack_style.v
```

### Run the Grid Style demo

```bash
v run demos/grid_style.v
```

### Run the starter template

```bash
v run demos/starter_template.v
```

### Run the Best Practices: Vertical Stack Starter demo

```bash
v run demos/vertical_stack_starter.v
```

### Run the Best Practices: Grid Column Starter demo

```bash
v run demos/grid_column_starter.v
```

### Run the Best Practices: State Controller Pattern demo

```bash
v run demos/state_controller_pattern.v
```

### Run the Calculator demo

```bash
v run demos/calculator.v
```

### Run the Settings Editor demo

```bash
v run demos/settings_editor.v
```

### Run the Data Viewer/Filter demo

```bash
v run demos/data_viewer.v
```

### Run the Timer & Progress loader demo

```bash
v run demos/timer_demo.v
```

### Run the List & Image Preview Selector demo

```bash
v run demos/list_image_demo.v
```

### Run the Interactive Events & States demo

```bash
v run demos/events_demo.v
```

### Run the single-window all-controls demo

```bash
v run demos/all_controls_demo.v
```

### Run the always-on-top demo

```bash
v run demos/always_on_top_demo.v
```

### Run the Hierarchical Tree View demo

```bash
v run demos/tree_view_demo.v
```

### Run the Spy++ External PID Inspector demo

```bash
v run demos/spy_plus_plus_demo.v
```

### Run the external Calculator accessibility probe demo

```bash
v run demos/ext_spy_calc_check.v
```

This demo opens Calculator automatically if needed, inspects its AXUIElement controls, presses buttons, and reads back the display so the workflow can be verified end to end.

### Run the Ergonomic Helpers demo

```bash
```

### Run the Delphi & C# Inspired RAD Showcase demo

```bash
v run demos/delphi_inspired_demo.v
```

### Run the developer experience (DX) showcase demo

```bash
```

### Run the High-Level Helpers demo

```bash
```

### Run the Native Menu Bar & Text Shortcuts demo

```bash
v run demos/menu_demo.v
```

### Run the Advanced Typography & Features demo

```bash
v run demos/advanced_features_demo.v
```

### Run the Beginner Friendly signup demo

```bash
v run demos/beginner_demo.v
```

### Run the Window Configuration demo

```bash
v run demos/configuration_demo.v
```

### Run the Change Tracking & Dirty Form demo

```bash
v run demos/dirty_form_demo.v
```

### Run the Developer Experience DX Showcase

```bash
```

### Run the QoL Bulk Binding Features demo

```bash
v run demos/features_demo.v
```

### Run the Product Catalog CRUD Grid Editor demo

```bash
v run demos/grid_data_editor.v
```

### Run the Studio Markdown Live Editor demo

```bash
v run demos/markdown_editor.v
```

### Run the Interactive Web HTML Studio demo

```bash
v run demos/web_studio_demo.v
```

### Run the Native Switch & Custom Controls Showcase

```bash
```

### Run the Sticky Floating Yellow Pad Overlay widget

```bash
v run demos/overlay_widget_demo.v
```

### Run the Interactive Window controller demo

```bash
```

### Run the 2D Grid Beginner Painter demo

```bash
v run demos/grid_beginner_demo.v
```

### Run the SQLite CRUD Catalog demo

```bash
v run demos/sqlite_crud_demo.v
```

### Run the Number Guessing Game demo

```bash
v run demos/guessing_game.v
```

### Run the Pomodoro Focus Clock demo

```bash
v run demos/pomodoro_timer_demo.v
```

### Run the Lockbox Security Dashboard demo

```bash
v run demos/password_dashboard.v
```

### Run the Custom Colors Live Editor demo

```bash
v run demos/colors_demo.v
```

### Run the REST client API Studio demo

```bash
v run demos/rest_client_demo.v
```

### Run the Worker Pool Concurrency Visualizer demo

```bash
v run demos/worker_pool_visualizer.v
```

### Run the TCP Sockets demo

```bash
v run demos/tcp_socket_demo.v
```

### Run the UDP Sockets demo

```bash
v run demos/udp_socket_demo.v
```

### Run the Unix Domain Sockets demo

```bash
v run demos/unix_socket_demo.v
```

### Run the Secure TLS Sockets demo

```bash
v run demos/secure_socket_demo.v
```

### Run the Secure UDP DTLS Sockets demo

```bash
v run demos/secure_udp_demo.v
```

### Run the Secure Unix Sockets demo

```bash
v run demos/secure_unix_demo.v
```

### Run the Secure WebSockets demo

```bash
v run demos/secure_websocket_demo.v
```

### Run the Wrapped Sockets helper demo

```bash
v run demos/wrapped_sockets_demo.v
```

### Run the Deflate Compression demo

```bash
v run demos/deflate_demo.v
```

### Run the Zstandard Compression demo

```bash
v run demos/zstd_demo.v
```

### Run the System Info & Encodings demo

```bash
v run demos/encoding_and_system_info_demo.v
```

### Run the System Calls Info Viewer demo

```bash
v run demos/system_calls_demo.v
```

### Run the System & Stdlib Features demo

```bash
```

### Run the Clipboard Manager demo

```bash
v run demos/clipboard_demo.v
```

### Run the Performance Benchmark demo

```bash
v run demos/benchmark_demo.v
```

## Testing

```bash
v test .
```

## Project structure

- [scripts/build.vsh](scripts/build.vsh) — macOS binary compilation and packaging script for `.app` bundle with icons
- [scripts/build_demos.vsh](scripts/build_demos.vsh) — batch compilation script to package all demos concurrently with their premium icons
- [demos/main.v](demos/main.v) — example app and demo entry point
- [demos/starter_template.v](demos/starter_template.v) — minimal starter app for new developers
- [window.v](window.v) — core window API and Cocoa integration
- [window.m](window.m) — native macOS bridge implementation
- [window.h](window.h) — bridge declarations used by V
- [tests/](tests/) — regression and unit test suite for controls, windows, layout, theming, and security
- [demos/stack_style.v](demos/stack_style.v) — demo of clean, vertical form stacking
- [demos/grid_style.v](demos/grid_style.v) — demo of side-by-side row-based grids
- [demos/calculator.v](demos/calculator.v) — interactive math calculator showing nested grid rows
- [demos/settings_editor.v](demos/settings_editor.v) — advanced dashboard containing date picker, color wells, modes
- [demos/data_viewer.v](demos/data_viewer.v) — mock database user records lookup table with filters
- [demos/timer_demo.v](demos/timer_demo.v) — background timer tasks updating progress indicators periodically
- [demos/list_image_demo.v](demos/list_image_demo.v) — interactive list selector previewing mockup screenshots in real time
- [demos/events_demo.v](demos/events_demo.v) — advanced event listener triggers for hover, focus, lose focus (blur), and window resizes
- [demos/all_controls_demo.v](demos/all_controls_demo.v) — single-window demo that exercises many controls in one place
- [demos/menu_demo.v](demos/menu_demo.v) — demo of standard macOS application menus and text editing shortcuts
- [demos/advanced_features_demo.v](demos/advanced_features_demo.v) — showcases advanced typography and macOS APIs
- [demos/beginner_demo.v](demos/beginner_demo.v) — beginner-friendly signup form and profile builder
- [demos/configuration_demo.v](demos/configuration_demo.v) — fluent configurations using WindowConfig
- [demos/dirty_form_demo.v](demos/dirty_form_demo.v) — illustrates live form dirty-tracking state and control validation callbacks
- [demos/features_demo.v](demos/features_demo.v) — showcases compiling-time struct binding and automatic multi-column tables
- [demos/grid_data_editor.v](demos/grid_data_editor.v) — a fully-functional reactive database inventory-catalog CRUD grid editing dashboard
- [demos/markdown_editor.v](demos/markdown_editor.v) — a complete live Markdown Studio layout using full WebKit render updates
- [demos/overlay_widget_demo.v](demos/overlay_widget_demo.v) — sticky window overlay yellow notepad memo widget with custom styling
- [demos/always_on_top_demo.v](demos/always_on_top_demo.v) — a utility app indicating how window z-axis float levels behaves
- [demos/list_table_toolkit_demo.v](demos/list_table_toolkit_demo.v) — focused showcase of the list/table toolkit add-ons: live search filtering (bind_search_to_list), case-insensitive list sorting and Move Up/Down reordering, numeric-aware table column sorting, table row reordering, table column mapping & row filtering, one-call CSV export/import, ready-made validators (email/number/min-length/IP/phone/range), and batch clear_fields / clear_all_fields
- [demos/todo_list_demo.v](demos/todo_list_demo.v) — a todo list app built on the list box item-management helpers (add/remove/clear items, Cmd/Shift multi-selection, double-click to complete, bulk remove of selected rows)
- [demos/table_manager_demo.v](demos/table_manager_demo.v) — an inventory manager built on the table row-management helpers (add/insert/update/remove rows, cell editing, find_table_row, Cmd/Shift multi-selection, double-click to increment, bulk remove of selected rows)
- [demos/save_restore_demo.v](demos/save_restore_demo.v) — one-call JSON settings persistence with save_values_to_file/load_values_from_file and unsaved-changes prompts
- [demos/delphi_inspired_demo.v](demos/delphi_inspired_demo.v) — showcases standard Delphi RAD tool look and event bindings
- [demos/vertical_stack_starter.v](demos/vertical_stack_starter.v) — starter template demonstrating best practices for vertical stack forms
- [demos/grid_column_starter.v](demos/grid_column_starter.v) — starter template demonstrating horizontal row layouts and event handling
- [demos/state_controller_pattern.v](demos/state_controller_pattern.v) — interactive design pattern illustrating state manipulation and reactive controls
- [demos/web_studio_demo.v](demos/web_studio_demo.v) — real-world business intelligence (BI) KPI & Fintech analytics board integrating native V widgets, responsive HTML structures, CSS custom layout properties, and Javascript currency loaders
- [demos/layout_vertical_stack.v](demos/layout_vertical_stack.v) — demonstrates linear vertical stacking, padding, spacing, dividers, and spacers
- [demos/layout_horizontal_rows.v](demos/layout_horizontal_rows.v) — demonstrates side-by-side rows, spacers, action rows, and field rows
- [demos/layout_form_sections.v](demos/layout_form_sections.v) — demonstrates semantic forms, section blocks, and form control validation
- [demos/layout_group_boxes.v](demos/layout_group_boxes.v) — demonstrates visual panel containment boxes
- [demos/layout_tabs.v](demos/layout_tabs.v) — demonstrates interactive native tabs switching between multi-view panels
- [demos/layout_scroll_view.v](demos/layout_scroll_view.v) — demonstrates scrollable panel constraints
- [demos/layout_events_mini_demo.v](demos/layout_events_mini_demo.v) — compact showcase combining sections, rows, groups, and event bindings
- [demos/layout_struct_reflection.v](demos/layout_struct_reflection.v) — demonstrates auto-generating forms from structs using V reflection
- [demos/layout_responsive_constraints.v](demos/layout_responsive_constraints.v) — demonstrates responsive auto-layout scaling, fixed dimensions, and a background color picker well
- [demos/layout_advanced_grid_flex_demo.v](demos/layout_advanced_grid_flex_demo.v) — demonstrates multi-column grid forms, flexbox directions & distribution modes, explicit control alignment & fill expansion modifiers, and container nesting
- [demos/grid_beginner_demo.v](demos/grid_beginner_demo.v) — beginner-friendly interactive 2D painting grid/canvas illustrating row/column structures, pointer-shared mutable state, presets, and native color well integrations
- [demos/sqlite_crud_demo.v](demos/sqlite_crud_demo.v) — SQLite database dashboard performing CREATE, READ, UPDATE, DELETE actions on database tables
- [demos/guessing_game.v](demos/guessing_game.v) — guess-the-number game showcasing native level indicators, ratings stars, color wells, and guess history logs
- [demos/pomodoro_timer_demo.v](demos/pomodoro_timer_demo.v) — focus clock dashboard with a live system progress timer, notification toasts, and custom sessions settings
- [demos/password_dashboard.v](demos/password_dashboard.v) — secure Lockbox security dashboard showing credentials lookup table and detail forms
- [demos/colors_demo.v](demos/colors_demo.v) — dynamic workspace custom colors styling sandbox to tweak typography, background, active selections, and font colors at runtime
- [demos/rest_client_demo.v](demos/rest_client_demo.v) — REST client API studio supporting methods selection (GET, POST), request parameters, headers dictionary, and JSON viewer
- [demos/worker_pool_visualizer.v](demos/worker_pool_visualizer.v) — visual pipeline queue monitor displaying parallel concurrent task progress bars, worker states, and thread activity
- [demos/tcp_socket_demo.v](demos/tcp_socket_demo.v) — simple client-server TCP networking message logs
- [demos/udp_socket_demo.v](demos/udp_socket_demo.v) — datagram packet transfer over UDP sockets
- [demos/unix_socket_demo.v](demos/unix_socket_demo.v) — local inter-process communication using Unix Domain sockets
- [demos/secure_socket_demo.v](demos/secure_socket_demo.v) — TLS encrypted client-server socket communication
- [demos/secure_udp_demo.v](demos/secure_udp_demo.v) — DTLS encrypted datagram network socket transfers
- [demos/secure_unix_demo.v](demos/secure_unix_demo.v) — secure TLS encrypted Unix domain sockets
- [demos/secure_websocket_demo.v](demos/secure_websocket_demo.v) — secure web sockets client-server networking
- [demos/wrapped_sockets_demo.v](demos/wrapped_sockets_demo.v) — simplified wrapper APIs for TCP socket stream operations
- [demos/deflate_demo.v](demos/deflate_demo.v) — compress and decompress strings using raw zlib deflate compression
- [demos/zstd_demo.v](demos/zstd_demo.v) — Zstandard compression/decompression operations
- [demos/encoding_and_system_info_demo.v](demos/encoding_and_system_info_demo.v) — text encoder/decoder (Hex, Base64) with native environment details viewer
- [demos/system_calls_demo.v](demos/system_calls_demo.v) — native shell processes invocation, stdout routing, and folder monitoring
- [demos/clipboard_demo.v](demos/clipboard_demo.v) — monitors clipboard entries and displays history log
- [demos/benchmark_demo.v](demos/benchmark_demo.v) — measures wrapper operation latency benchmarks

## Documentation

Full API documentation and detailed signature references are maintained in [API.md](docs/API.md). Below is an architectural overview of SimpleGUI's API surface:

### 1. Window Operations & Themes

- **Lifecycle**: `new_simple_window(title, w, h)`, `win.run()`, `win.close()`, `win.hide()`, `win.show_window()`
- **Geometry & Alignment**: `win.center()`, `win.align(pos)`, `win.set_size(w, h)`, `win.set_position(x, y)`, `win.set_min_size()`, `win.set_max_size()`, `win.set_resizable()`, `win.set_aspect_ratio()`
- **Appearance & Opacity**: `win.set_title(t)`, `win.set_subtitle(s)`, `win.set_opacity(alpha)`, `win.set_background_color(hex)`, `win.set_font_color(color)`, `win.set_titlebar_visible()`, `win.set_titlebar_appears_transparent()`, `win.set_full_size_content_view()`
- **17 Theme Presets**: `win.set_theme(name)` applies any built-in theme (`Apple Light`, `Apple Dark`, `Midnight Space Gray`, `Apple Sunset`, `Sonoma Emerald`, `Ventura Amber`, `Soft Pastel`, `Catppuccin`, `Nord`, `Dracula`, `Cyberpunk`, `Solarized Light`, `Solarized Dark`, `GitHub Dark`, `GitHub Light`, `Navy Blue`, `Forest Green`). List all themes with `simplegui.list_themes()`, inspect one with `simplegui.get_theme(name)` (returns a `Theme` struct with `background_color`, `font_color`, `accent_color`, `is_dark`), or apply a custom `Theme` with `win.apply_theme(t)`. Applying a theme restyles all controls (buttons, dropdowns, inputs, date pickers) to match the theme's light/dark background regardless of the macOS system appearance; per-control overrides via `set_control_background_color`/`set_control_font_color` can be layered on top afterwards.
- **Window Stacking & Dock**: `win.set_always_on_top(bool)`, `win.set_window_level(level)`, `win.toggle_fullscreen()`, `win.bounce_dock(critical)`, `win.set_dock_badge(count)`, `win.set_movable_by_window_background()`
- **Production Window State**: `win.set_represented_filename(path)`, `win.set_document_edited(bool)`, `win.set_frame_autosave_name(name)`, `win.save_frame()`, `win.restore_frame()`, `win.capture_screenshot(path)`

### 2. Control Layout & Containers

- **Horizontal Stacking**: `win.begin_row(name)`, `win.end_row()`, `win.row(name, callback)`
- **Multi-Column Grid Containers**: `win.begin_grid(name, columns, spacing)`, `win.end_grid()`, `win.grid(name, columns, spacing, callback)`
- **Flexbox Containers**: `win.begin_flex_box(name, direction, justify, align)`, `win.end_flex_box()`, `win.flex_box(name, direction, justify, align, callback)`
- **Bulk Rows**: `win.add_action_row(map)`, `win.add_fields_row(map)`, `win.add_labeled_*`
- **Group Containers**: `win.add_group_box(name, title)` / `win.group(...)`, `win.add_tabs(name, titles)`, `win.add_scroll_view(name, height)`
- **Layout Spacers**: `win.add_vertical_spacer(h)`, `win.add_horizontal_spacer(w)`, `win.add_separator()`

### 3. Controls & Widgets (40+ Native Controls)

- **Standard Controls**: `add_input`, `add_password`, `add_textarea`, `add_checkbox`, `add_button`, `add_number`, `add_slider`, `add_dropdown`, `add_segmented_control`, `add_radio_group`, `add_switch`, `add_search_field`
- **Rich Cocoa Controls**: `add_combo_box`, `add_level_indicator`, `add_rating`, `add_spinner`, `add_path_control`, `add_token_field`, `add_stepper`, `add_knob`, `add_pull_down`, `add_image_button`
- **Dashboard & Developer Widgets**: `add_breadcrumbs`, `add_shortcut_recorder`, `add_chart`, `add_circular_progress`, `add_property_grid`, `add_color_grid`, `add_console`, `add_code_editor`, `add_timeline_view`, `add_stat_card`, `add_banner`, `add_star_rating`, `add_range_slider`, `add_split_button`, `add_tag_cloud`, `add_wizard_stepper`, `add_section_header`, `add_vertical_slider`, `add_chip_group`, `add_badge`, `add_icon_segments`, `add_status_indicator`, `add_metric_meter`, `add_avatar_card`, `add_time_picker`, `add_tray_icon`, `add_collapsible_section`, `add_toolbar_item`, `add_html_view`, `add_drop_zone`
- **Reflection & Struct Validation**: `win.add_form_from_struct[T](default_data)` auto-builds forms from V structs; `win.validate_struct[T]()` validates struct field attributes (`@[required]`, `@[min_len]`, `@[max_len]`, `@[email]`, `@[url]`, `@[alphanumeric]`, `@[min]`, `@[max]`).

### 4. Sizing, Styling, Alignment & Fluent Chaining

- `set_control_width`, `set_control_height`, `set_control_font_size`, `set_control_font_bold`, `set_control_font_name`, `set_control_background_color`, `set_control_font_color`, `set_control_visible`, `set_control_enabled`, `set_control_alignment`, `set_control_expand_fill`, `set_placeholder`, `set_error`, `set_tooltip`
- **Fluent Modifiers**: Chain directly on creation: `.width(w)`, `.height(h)`, `.font_size(s)`, `.bold(b)`, `.font_name(f)`, `.color(hex)`, `.font_color(hex)`, `.align_left()`, `.align_center()`, `.align_right()`, `.align_top()`, `.align_bottom()`, `.expand_fill()`, `.placeholder(t)`, `.error(err)`, `.tooltip(t)`, `.visible(b)`, `.enabled(b)`, `.onclick(cb)`, `.onchange(cb)`, `.onenter(cb)`, `.onfocus(cb)`, `.onblur(cb)`, `.onhover(cb)`

### 5. Dialogs, Popups & File Pickers

- `win.alert(title, msg)`, `win.alert_with_style(title, msg, style)`, `win.confirm(title, msg)`, `win.prompt(title, msg, default)`, `win.choice_dialog(title, msg, choices)`
- `win.info()`, `win.warn()`, `win.error_dialog()`, `win.ask()`, `win.choose()`, `win.ask_text()`
- `win.select_file()`, `win.select_file_with_extensions(exts)`, `win.select_folder()`, `win.save_file_picker()`, `win.choose_file()`, `win.choose_folder()`, `win.choose_save_file()`

### 6. System & Platform APIs (`NL_OS`, `NL_COMPUTER`, `NL_FILESYSTEM`)

- **Process Execution**: `win.exec(cmd)`, `win.exec_bg(cmd)`, `win.exec_timeout(cmd, ms)`, `win.spawn_process(path, args, env)`
- **Environment & System Paths**: `win.get_env(key)`, `win.set_env(key, val)`, `win.get_system_path(name)` (`home`, `temp`, `desktop`, `documents`, `downloads`, `cache`, `app_data`)
- **Hardware & Computer Specs**: `win.get_hostname()`, `win.get_username()`, `win.get_pid()`, `win.get_cpu_info()`, `win.get_cpu_cores()`, `win.get_memory_info()`, `win.get_disk_usage(path)`, `win.get_file_metadata(path)`
- **Network Tools**: `win.get_local_ip()`, `win.get_external_ip()`, `win.ping(host, count)`, `win.dns_lookup(host)`, `win.get_wifi_ssid()`, `win.get_listening_ports()`, `win.is_internet_connected()`
- **Resource Monitoring**: `win.get_cpu_usage_percent()`, `win.get_process_memory_mb(pid)`, `win.get_load_average()`, `win.get_memory_pressure()`
- **macOS System Actions**: `win.show_system_notification()`, `win.say()`, `win.speak_with_voice()`, `win.toggle_dark_mode()`, `win.get_battery_percent()`, `win.get_volume()`, `win.take_screenshot()`, `win.trash_file()`, `win.defaults_read/write`
- **Cross-Window & External App Control (`simplegui.sys_*`)**: `sys_list_app_windows()`, `sys_get_window(title)`, `sys_order_app_window_front/back(...)`, `sys_set_app_window_visible(...)`, `sys_list_external_apps()`, `sys_spy_external_app(pid)`, `sys_set_external_control_*`, `sys_set_external_app_frontmost(pid)`, `sys_set_external_app_visible(pid, ...)`
- **Clipboard & Finder**: `win.copy_to_clipboard(text)`, `win.get_clipboard_text()`, `simplegui.clipboard_text()`, `simplegui.reveal_in_finder(path)`

#### Production Automation Quick Start

```v
module main

import simplegui

fn main() {
  mut win := simplegui.new_simple_window('Automation Starter', 560, 320)

  win.add_button('btn_refresh', 'List Targets')
  win.add_button('btn_focus', 'Bring First Target Front')
  win.add_button('btn_list_controls', 'List External Controls')
  win.add_console('log', 180)

  win.on_click('btn_refresh', fn (mut w simplegui.SimpleWindow) {
    titles := simplegui.sys_list_app_windows()
    apps := simplegui.sys_list_external_apps()
    w.append_console('log', 'Internal windows: ${titles.len} | External apps: ${apps.len}', 0)
  })

  win.on_click('btn_focus', fn (mut w simplegui.SimpleWindow) {
    titles := simplegui.sys_list_app_windows()
    if titles.len == 0 {
      w.append_console('log', 'No registered internal windows found.', 0)
      return
    }
    ok := simplegui.sys_order_app_window_front(titles[0])
    w.append_console('log', 'Bring front "${titles[0]}" => ${ok}', 0)
  })

  win.on_click('btn_list_controls', fn (mut w simplegui.SimpleWindow) {
    apps := simplegui.sys_list_external_apps()
    if apps.len == 0 {
      w.append_console('log', 'No external desktop apps found.', 0)
      return
    }
    pid := apps[0].pid
    controls := simplegui.sys_spy_external_app(pid)
    w.append_console('log', 'PID ${pid} controls discovered: ${controls.len}', 0)
  })

  win.run()
}
```

### 7. V Standard Library High-Level Wrappers

- **HTTP & WebSockets**: `win.http_get(url)`, `win.http_post(url, data)`, `win.websocket_client(url, callback)`
- **Crypto & Hashing**: `win.crypto_sha256()`, `win.crypto_sha512()`, `win.crypto_md5()`, `win.crypto_bcrypt_hash()`, `win.crypto_bcrypt_verify()`, `win.crypto_hmac_sha256()`, `win.crypto_wyhash()`, `win.crypto_encrypt_aes()`, `win.crypto_decrypt_aes()`
- **Encodings & Compression**: `win.hex_encode()`, `win.hex_decode()`, `win.base64_encode()`, `win.base64_decode()`, `win.compress_gzip()`, `win.decompress_gzip()`, `win.compress_zlib()`, `win.compress_deflate()`, `win.compress_zstd()`
- **Random & Time**: `win.rand_int()`, `win.rand_string()`, `win.crypto_rand_uuid()`, `win.start_stopwatch()`, `win.start_benchmark()`
- **Collections**: `simplegui.new_stack[T]()`, `simplegui.new_queue[T]()`, `simplegui.new_set[T]()`, `simplegui.new_ringbuffer[T]()`, `simplegui.new_min_heap[T]()`
- **Math & Stats**: `win.complex_new(re, im)` ($e^z$, conjugate, phase), `win.math_sin()`, `win.math_cos()`, `win.math_sqrt()`, `win.math_clamp()`, `win.math_remap()`, `win.math_smoothstep()`, `win.stats_mean()`, `win.stats_median()`, `win.stats_rms()`

### 8. Event Handling & Custom Menus

- **Control Events**: `win.on_click(name, cb)`, `win.on_change(name, cb)`, `win.on_hover(name, cb)`, `win.on_focus(name, cb)`, `win.on_blur(name, cb)`, `win.on_enter(name, cb)`, `win.on_key(key, cb)`
- **Window Events**: `win.on_resize(cb)`, `win.on_window_focus(cb)`, `win.on_window_blur(cb)`, `win.on_window_minimize(cb)`, `win.on_window_restore(cb)`, `win.on_close(cb)`, `win.on_file_drop(cb)`
- **Application Menus**: `win.add_menu_item(menu, title, shortcut, cb)`, `win.add_menu(menu, items)`
- **Context Menus**: `win.add_context_menu_item(control, title, cb)`, `win.add_context_menu(control, items)`

### 9. Multi-Column Tables, Data Grids & Tree Views

- **Tables**: `win.add_table(name, columns)`, `win.set_table_rows(name, rows)`, `win.load_table_from_structs[T](name, items)`, `win.add_table_row()`, `win.get_table_selected()`, `win.on_table_select()`, `win.on_table_double_click()`, `win.save_table_to_csv()`, `win.load_table_from_csv()`
- **Editable Data Grids**: `win.add_grid(name, headers, initial_rows)`, `win.grid_add_row()`, `win.grid_delete_row()`, `win.grid_get_cell()`, `win.grid_set_cell()`, `win.grid_get_selected_cell()`, `win.grid_set_filter()`, `win.grid_sort_by_column()`, `win.grid_set_column_type()`
- **Tree Views**: `TreeNode` struct (`id`, `parent_id`, `text`), `win.add_tree_view(name, height)`, `win.set_tree_nodes(name, nodes)`, `win.get_tree_selected()`, `win.set_tree_selected()`

### 10. Ergonomics, Settings Persistence & Async Execution

- **Settings & App State Persistence**: `win.save_app_state("preset_1")!`, `win.load_app_state("preset_1")`, `win.enable_app_autosave()`, `win.save_values_to_file("settings.json")!`, `win.load_values_from_file("settings.json")!` (automatically persists to the recommended macOS user application storage `~/Library/Application Support/<app_name>/`, ensuring full functionality when moved to `/Applications`)
- **Storage Path Resolvers**: `win.get_app_storage_dir()`, `win.get_app_storage_path(filename)`, `win.resolve_storage_path(path)`
- **Form Dirty Tracking**: `win.is_dirty()`, `win.is_control_dirty(name)`, `win.commit_changes()`, `win.confirm_discard_changes()`
- **Live Search Filtering**: `win.bind_search_to_list(search_name, list_name)`
- **Async Execution**: `win.run_async(bg_task_fn, on_complete_cb)`, `win.run_on_main_thread(cb)`, `win.run_on_main_thread_sync(cb)`
- **System Tray Mode**: `win.enable_status_bar(icon_path)`

For complete method details, arguments, and full code examples, view [API.md](docs/API.md).

## Notes

The goal of this project is to provide a simple, high-abstraction GUI layer that feels familiar to people who are used to event-driven environments like Delphi, VBA, or Python-based UI toolkits.

---

## Screenshots

The following native macOS windows were captured dynamically by building and running each live V demo.
Screenshots are auto-generated using `v run scripts/capture_demos.vsh`.

### High-Fidelity App & Web Studios

- **RAD Visual UI Designer Studio**: `v run tools/ui_designer.v`
  ![SimpleGUI RAD Visual UI Designer Studio](screenshots/ui_designer.png)
- **RAD Code Explorer & Live Previewer Studio**: `v run tools/vlang_simple_gui_previewer.v`
  ![SimpleGUI RAD Code Explorer & Live Previewer Studio](screenshots/vlang_simple_gui_previewer.png)
- **Web HTML Studio**: `v run demos/web_studio_demo.v`
  ![Web HTML Studio](screenshots/web_studio_demo.png)
- **Markdown Live Editor**: `v run demos/markdown_editor.v`
  ![Markdown Live Editor](screenshots/markdown_editor.png)
- **Product Catalog CRUD Grid**: `v run demos/grid_data_editor.v`
  ![Product Catalog CRUD](screenshots/grid_data_editor.png)
- **SQLite CRUD Showcase**: `v run demos/sqlite_crud_demo.v`
  ![SQLite CRUD Showcase](screenshots/sqlite_crud_demo.png)

### Additional Major App Demos

- **REST Client API Studio**: `v run demos/rest_client_demo.v`
  ![REST Client API Studio](screenshots/rest_client_demo.png)
- **Password Security Dashboard**: `v run demos/password_dashboard.v`
  ![Password Security Dashboard](screenshots/password_dashboard.png)
- **Pomodoro Focus Clock**: `v run demos/pomodoro_timer_demo.v`
  ![Pomodoro Focus Clock](screenshots/pomodoro_timer_demo.png)
- **Concurrent Worker Pool Visualizer**: `v run demos/worker_pool_visualizer.v`
  ![Concurrent Worker Pool Visualizer](screenshots/worker_pool_visualizer.png)
- **Developer Controls Console**: `v run demos/developer_controls_demo.v`
  ![Developer Controls Console](screenshots/developer_controls_demo.png)
- **Spy++ External PID Inspector**: `v run demos/spy_plus_plus_demo.v`
  ![Spy++ External PID Inspector](screenshots/spy_plus_plus_demo.png)
- **List/Table Toolkit Operations**: `v run demos/list_table_toolkit_demo.v`
  ![List/Table Toolkit Operations](screenshots/list_table_toolkit_demo.png)
- **Settings Save/Restore Workflow**: `v run demos/save_restore_demo.v`
  ![Settings Save/Restore Workflow](screenshots/save_restore_demo.png)
  ![API Coverage Dashboard](screenshots/api_coverage_demo.png)

### Layout & Component Showcases

- **Vertical Stack Style (Default Layout)**: `v run demos/stack_style.v`
  ![Vertical Stack Style](screenshots/stack_style.png)
- **Grid Column Layout**: `v run demos/grid_style.v`
  ![Grid Layout](screenshots/grid_style.png)
- **Advanced Preferences Settings**: `v run demos/settings_editor.v`
  ![Settings Editor](screenshots/settings_editor.png)
- **All Controls — Complete API Showcase** (20 sections, every `win.add_*` control): `v run demos/all_controls_demo.v`
  ![All Controls Demo — 20 sections, every win.add_* control](screenshots/all_controls_demo.png)
- **Hierarchical Tree View**: `v run demos/tree_view_demo.v`
  ![Tree View Demo](screenshots/tree_view_demo.png)
  ![Rich Widgets Demo](screenshots/modern_widgets_demo.png)
  ![Advanced Controls Suite](screenshots/rich_widgets_demo.png)
  ![Native Switch & Custom Controls](screenshots/new_controls_demo.png)

### Built-in Interactive Utilities

- **Interactive Calculator**: `v run demos/calculator.v`
  ![Calculator](screenshots/calculator.png)
- **Database Query Viewer**: `v run demos/data_viewer.v`
  ![Data Viewer](screenshots/data_viewer.png)
- **Task Timer Loader**: `v run demos/timer_demo.v`
  ![Timer Demo](screenshots/timer_demo.png)
- **List & Image Preview Selector**: `v run demos/list_image_demo.v`
  ![List & Image Demo](screenshots/list_image_demo.png)
- **Events & State Controller**: `v run demos/events_demo.v`
  ![Events Demo](screenshots/events_demo.png)
- **Lorem & HTML Render**: `v run demos/lorem_and_html_demo.v`
  ![Lorem & HTML Render](screenshots/lorem_and_html_demo.png)
- **Menu Bar**: `v run demos/menu_demo.v`
  ![Menu Bar](screenshots/menu_demo.png)

### Custom & Architectural Patterns

- **Delphi & RAD Inspired Showcase**: `v run demos/delphi_inspired_demo.v`
  ![Delphi Demo](screenshots/delphi_inspired_demo.png)
  ![Ergonomic Demo](screenshots/ergonomic_demo.png)
- **Sticky Yellow Overlay Notepad**: `v run demos/overlay_widget_demo.v`
  ![Overlay Widget Demo](screenshots/overlay_widget_demo.png)
- **Always On Top Window**: `v run demos/always_on_top_demo.v`
  ![Always On Top Demo](screenshots/always_on_top_demo.png)
  ![DX Showcase Demo](screenshots/dx_showcase.png)
  ![Developer DX Features](screenshots/dx_features_demo.png)
- **Fluent Window Configurations**: `v run demos/configuration_demo.v`
  ![Window Config Demo](screenshots/configuration_demo.png)
- **Dirty Form Change Tracking**: `v run demos/dirty_form_demo.v`
  ![Dirty Form Demo](screenshots/dirty_form_demo.png)
  ![Window Controller](screenshots/window_controller_demo.png)
- **State Controller Pattern**: `v run demos/state_controller_pattern.v`
  ![State Controller Pattern](screenshots/state_controller_pattern.png)
- **QoL Bulk Binding Features**: `v run demos/features_demo.v`
  ![QoL Bulk Binding](screenshots/features_demo.png)
- **Advanced Features & Hooks**: `v run demos/advanced_features_demo.v`
  ![Advanced Features & Hooks](screenshots/advanced_features_demo.png)

### System & Standard Library Integrations

  ![System and Standard Library Features](screenshots/system_and_stdlib_features_demo.png)
- **System Calls Info Viewer**: `v run demos/system_calls_demo.v`
  ![System Calls Info Viewer](screenshots/system_calls_demo.png)
- **System Info & Encodings (Hex/Base64)**: `v run demos/encoding_and_system_info_demo.v`
  ![System Info & Encodings](screenshots/encoding_and_system_info_demo.png)
- **Clipboard Monitor**: `v run demos/clipboard_demo.v`
  ![Clipboard Monitor](screenshots/clipboard_demo.png)
- **Performance Benchmark**: `v run demos/benchmark_demo.v`
  ![Performance Benchmark](screenshots/benchmark_demo.png)

### Network Sockets & Security Suite

- **TCP Socket**: `v run demos/tcp_socket_demo.v`
  ![TCP Socket](screenshots/tcp_socket_demo.png)
- **UDP Socket**: `v run demos/udp_socket_demo.v`
  ![UDP Socket](screenshots/udp_socket_demo.png)
- **Unix Domain Socket**: `v run demos/unix_socket_demo.v`
  ![Unix Domain Socket](screenshots/unix_socket_demo.png)
- **Secure TLS Socket**: `v run demos/secure_socket_demo.v`
  ![Secure TLS Socket](screenshots/secure_socket_demo.png)
- **Secure UDP DTLS Socket**: `v run demos/secure_udp_demo.v`
  ![Secure UDP DTLS Socket](screenshots/secure_udp_demo.png)
- **Secure Unix TLS Socket**: `v run demos/secure_unix_demo.v`
  ![Secure Unix TLS Socket](screenshots/secure_unix_demo.png)
- **Secure WebSockets**: `v run demos/secure_websocket_demo.v`
  ![Secure WebSockets](screenshots/secure_websocket_demo.png)
- **High-Level Wrapped Sockets**: `v run demos/wrapped_sockets_demo.v`
  ![High-Level Wrapped Sockets](screenshots/wrapped_sockets_demo.png)

### Data Utilities & Compression

- **Deflate Compression**: `v run demos/deflate_demo.v`
  ![Deflate Compression](screenshots/deflate_demo.png)
- **Zstandard Compression**: `v run demos/zstd_demo.v`
  ![Zstandard Compression](screenshots/zstd_demo.png)

### Starter Templates & Basics

- **Starter Template**: `v run demos/starter_template.v`
  ![Starter Template](screenshots/starter_template.png)
- **Beginner Signup Form**: `v run demos/beginner_demo.v`
  ![Beginner Signup Form](screenshots/beginner_demo.png)
- **Vertical Stack Starter**: `v run demos/vertical_stack_starter.v`
  ![Vertical Stack Starter](screenshots/vertical_stack_starter.png)
- **Grid Column Starter**: `v run demos/grid_column_starter.v`
  ![Grid Column Starter](screenshots/grid_column_starter.png)
  ![High-Level Form Builder](screenshots/high_level_demo.png)

### Modern Super Controls & simple_gg Ideals Suite

  - **Donut / Radial Progress Gauges**: `win.add_donut_chart` / `win.donut` with live dynamic percentage updates (`win.set_donut_percentage`).
  - **macOS Code Studio**: `win.add_code_studio` / `win.code_box` featuring macOS traffic light buttons, line numbers, and live code updating.
  - **Review Scorecards**: `win.add_score_card` / `win.score_card` with 5-tier review distributions and ratings.
  - **Floating Action Toolbars**: `win.add_floating_toolbar` / `win.floating_toolbar` with capsule action buttons.
  - **User Profile Cards**: `win.add_user_profile_card` / `win.user_profile` with avatar, active presence dot, and bio.
  - **Product Showcase Cards**: `win.add_product_card` / `win.product_card` with pricing, badge tags, and CTA buttons.
  - **Carousel Image Gallery**: `win.add_image_gallery` / `win.gallery` with slide thumbnails, captions, and navigation arrows.
  - **3D App Launcher Tiles**: `win.add_app_launcher_tile` / `win.app_tile` with status pills and app icons.
  - **Hi-Fi Media Players**: `win.add_media_player` / `win.media_player` with track info, scrubber progress, and play/pause controls.
  - **Contribution Heatmaps**: `win.add_activity_heatmap` / `win.heatmap` with 7xN GitHub-style activity matrices.
  - **Masked Input Fields**: `win.add_masked_input` / `win.masked_input` with automated formatting rules.
  - **Inline Editable Labels**: `win.add_inline_editable_label` / `win.editable_label` with click-to-edit interactions.
  - **Vertical Navigation Rails**: `win.add_nav_rail` / `win.nav_rail` with icons and notification badges.

---

## Production Workstations & Studio Applications

SimpleGUI includes 47 complete, native desktop workstation applications in [`applications/`](applications/) designed for engineering workflows, low-level binary & bitwise register engineering, 2D function plotting & network graph topology, symbolic math, physical dimensional analysis, calculus, statistics & data science, network intelligence, text & code editing, universal archiving & compression, stream editing, system monitoring, filesystem discovery, data analysis, speech synthesis, and media transformation:

### 📦 Prerequisites & Homebrew Installation

You can install all dependencies via standard Homebrew bundle, automated shell script, or the interactive V dependency inspector:

```bash
# 1. Automated Homebrew Bundle Installation (Recommended)
./install_homebrew_dependencies.sh

# Or directly with Homebrew bundle:
brew bundle --file ./Brewfile

# 2. Interactive / Non-Interactive V Dependency Inspector & Auto-Installer
./install_dependencies.vsh -y

# 3. Check health and missing dependencies without installing:
./install_dependencies.vsh --check

# 4. Include optional cross-compilers (zig, mingw-w64):
./install_homebrew_dependencies.sh --optional
# or
./install_dependencies.vsh --all -y
```

### 🚀 Complete Workstations Catalog (47 Applications):

| Application | Source File | Description |
| :--- | :--- | :--- |
| **📦 App Bundler Studio Pro** | [`applications/app_bundler_studio.v`](applications/app_bundler_studio.v) | Visual macOS `.app` bundle packager & `.icns` generator: turn any Mach-O binary or CLI tool into a native macOS `.app` bundle with Retina icons, Info.plist config, ad-hoc codesigning, and quarantine scrubber. |
| **🧩 JQ Studio Pro** | [`applications/jq_studio.v`](applications/jq_studio.v) | Interactive JSON query, formatting & filter workbench powered by `jq`: live query evaluations, 12 built-in transformation recipes, key/path inspection, minifier/prettifier, and error diagnostics. |
| **🚀 API Studio Pro** | [`applications/api_studio.v`](applications/api_studio.v) | Full-featured REST API testing client powered by `curl`: HTTP method selector (GET, POST, PUT, PATCH, DELETE, HEAD), request headers/body editors, latency telemetry (DNS/TLS/TTFB), and 1-click `curl` command exporter. |
| **🛡️ Nmap Studio Pro** | [`applications/nmap_studio.v`](applications/nmap_studio.v) | High-speed port scanner & network discovery workbench powered by `nmap`: quick scan (-F), service versioning (-sV), OS detection (-O), aggressive timing (-T4), vulnerability scripts, and open port reports. |
| **🌐 DNS & SSL Studio Pro** | [`applications/dns_studio.v`](applications/dns_studio.v) | Dual DNS resolution & TLS certificate analyzer powered by `dig` and `openssl`: multi-record lookups (A, AAAA, CNAME, MX, TXT, NS, SOA, CAA), certificate chain & expiry inspector, and SPF/DKIM/DMARC email security auditor. |
| **🕵️ Recon Studio Pro** | [`applications/recon_studio.v`](applications/recon_studio.v) | OSINT footprinting & asset mapping workbench powered by `whois`, public CT logs (crt.sh), IPInfo geolocation/ASN lookup, security headers inspector, and robots.txt crawler. |
| **🔄 Format Converter Pro** | [`applications/dataconvert_studio.v`](applications/dataconvert_studio.v) | Universal data interchange transformer: bidirectional live translation across **JSON ⇄ YAML ⇄ TOML ⇄ CSV ⇄ XML ⇄ SQLite**, schema validation, field extractors, and file export. |
| **🗄️ SQLite Studio Pro** | [`applications/sqlite_studio.v`](applications/sqlite_studio.v) | Embedded SQL database workbench powered by `sqlite3`: database browser, table/column/index schema explorer, SQL query scratchpad, formatted grid table/JSON/CSV views, and query plan analyzer. |
| **🎯 Regex Studio Pro** | [`applications/regex_studio.v`](applications/regex_studio.v) | Interactive regular expression workbench: live match highlighter, capture group breakdown table (`$1, $2`), flags (case-insensitive, multiline, dotall), find-and-replace engine, and 9 built-in regex recipes. |
| **🔍 ExifTool Studio Pro** | [`applications/exif_studio.v`](applications/exif_studio.v) | Image & video metadata investigator powered by `exiftool`: camera/lens specs, EXIF/IPTC/XMP tag explorer, 1-click GPS launch in Apple Maps, batch privacy PII metadata stripper, and tag writer. |
| **👁️ Tesseract OCR Pro** | [`applications/ocr_studio.v`](applications/ocr_studio.v) | Optical character recognition & document scanner powered by `tesseract`: multi-language packs (eng, spa, fra, deu, chi, jpn...), page segmentation modes (PSM), text post-processing, and searchable PDF generator. |
| **🎵 Audio Tag Studio Pro** | [`applications/audiotag_studio.v`](applications/audiotag_studio.v) | Audio metadata & lossless tagging studio powered by `ffmpeg` and `ffprobe`: track title, artist, album, genre, year, track #, comment, cover art extractor, tag stripper, and live macOS `afplay` playback. |
| **📊 Graphviz Studio Pro** | [`applications/dot_studio.v`](applications/dot_studio.v) | Code-to-diagram visual workbench powered by `graphviz` (`dot`): DOT source editor, live SVG/PNG compilation, 7 architecture/state/tree diagram templates, and layout engine selector (`dot`, `neato`, `fdp`, `circo`, `twopi`). |
| **🍺 Homebrew Studio Pro** | [`applications/brew_studio.v`](applications/brew_studio.v) | Visual package manager & service controller for macOS powered by `brew`: formula & cask search, package info inspector, 1-click bulk updates/upgrades, background services manager (`brew services`), and disk cache cleaner. |
| **🐳 Docker Studio Pro** | [`applications/docker_studio.v`](applications/docker_studio.v) | Container & microservice workbench powered by `docker`/`podman`: active container status table, start/stop/restart/logs lifecycle controls, local image repository manager, volume/network explorer, and system prune. |
| **💾 Disk Space Studio Pro** | [`applications/disk_studio.v`](applications/disk_studio.v) | macOS storage analyzer & developer junk cleaner powered by `du` and `df`: directory size breakdown, top 30 largest files finder, APFS volume monitor, and developer cache scrubber (`node_modules`, Xcode `DerivedData`, `.cache`). |
| **⏰ Launchd & Cron Pro** | [`applications/launchd_studio.v`](applications/launchd_studio.v) | macOS daemon & task scheduler workbench powered by `launchctl` and `crontab`: active system/user daemon explorer, user LaunchAgents inspector, visual cron expression generator, and `.plist` builder. |
| **🔐 Crypto & Hash Studio** | [`applications/crypto_studio.v`](applications/crypto_studio.v) | Cryptographic & checksum verification utility: multi-algorithm hash generator (MD5, SHA-1, SHA-224, SHA-256, SHA-384, SHA-512), target hash verifier, HMAC generator, JWT token claims decoder, and password entropy generator. |
| **🔄 TR Studio Pro** | [`applications/tr_studio.v`](applications/tr_studio.v) | Character translation & stream cleansing workbench powered by `tr`: character mapping, deletion (`-d`), squeeze repeats (`-s`), delete & squeeze (`-ds`), complement inversion (`-c`), 10 built-in recipes, and dual-pane editor. |
| **✂️ Cut Studio Pro** | [`applications/cut_studio.v`](applications/cut_studio.v) | Fast stream & column slicing workbench powered by `cut`: field extraction (`-f`), delimiter modes, character columns (`-c`), byte slices (`-b`), only delimited lines (`-s`), 9 built-in recipes, and file exporters. |
| **🔍 RG Studio Pro** | [`applications/rg_studio.v`](applications/rg_studio.v) | High-speed code & content search workbench powered by `ripgrep` (`rg`): regex search, fixed-strings (`-F`), whole-word matching (`-w`), case-modes (`-s`/`-S`), inverted match (`-v`), file-type selectors, glob filters, and context lines. |
| **⚡ FD Studio Pro** | [`applications/fd_studio.v`](applications/fd_studio.v) | Ultra-fast file finder & filesystem workbench powered by `fd`: regex/glob search, multi-extension filters, large file detection (>100MB), recent modification filters, and type filters. |
| **🔍 SD Studio Pro** | [`applications/sd_studio.v`](applications/sd_studio.v) | Ultra-fast regex search and replace workbench powered by `sd`: dual-pane editor, captured group transforms (`$1, $2`), code refactoring, text cleansing, PII redaction, and in-place multi-file batch processor. |
| **⚡ GAWK Studio Pro** | [`applications/gawk_studio.v`](applications/gawk_studio.v) | Interactive data stream & log processing workbench: real-time dual-pane editor, CSV/TSV/Log parser, built-in library of **40+ classic & modern AWK one-liners**, and direct multi-gigabyte disk file streamer. |
| **📄 Pandoc Studio Pro** | [`applications/pandoc_studio.v`](applications/pandoc_studio.v) | Universal document converter & publishing studio: Markdown, HTML5, LaTeX, Typst, MS Word (.docx), EPUB eBooks, Slide decks (PPTX, Reveal.js, Beamer), syntax themes, math rendering (MathJax), and direct PDF compiler. |
| **⚡ Wget2 Studio Pro** | [`applications/wget2_studio.v`](applications/wget2_studio.v) | High-speed multi-threaded download accelerator & website mirror powered by GNU `wget2`: parallel chunking (up to 16 threads), offline site crawling (`--mirror`), extension scrapers, automatic resume (`-c`), and browser emulation. |
| **🎬 yt-dlp Studio Pro** | [`applications/yt_dlp_studio.v`](applications/yt_dlp_studio.v) | High-performance media downloader & stream archiver: 4K UHD / 1080p / 720p presets, audio extractors (MP3 320k, FLAC, AAC, Opus, WAV), subtitle & metadata embedding, browser cookie support, and stream inspector (`-F`). |
| **🎬 FFmpeg Studio Pro** | [`applications/ffmpeg_studio.v`](applications/ffmpeg_studio.v) | Full-featured video & audio engineering studio: transcode engine, social media & Discord limits, EBU R128 audio loudnorm & denoise, lossless trimmer, 9:16 vertical crop, frame extraction, and 2-pass HD GIF maker. |
| **🎨 ImageMagick Studio Pro** | [`applications/imagemagick_studio.v`](applications/imagemagick_studio.v) | Complete graphic manipulation workstation: modern WebP/AVIF compression, multi-size favicon generator, magic background color removal (transparency), social presets, floating drop shadows, and bulk processing. |
| **🌐 Subfinder Studio Pro** | [`applications/subfinder_studio.v`](applications/subfinder_studio.v) | High-speed passive subdomain discovery & asset mapping workbench powered by `subfinder`: multi-source passive OSINT enumeration, active DNS validation, rate-limiting, and custom resolvers. |
| **🗣️ Say Studio Pro** | [`applications/say_studio.v`](applications/say_studio.v) | Native macOS speech synthesizer & voiceover generator powered by `say`: real-time text-to-speech, system voice browser (Samantha, Alex, Daniel, Fred, Victoria, Zarvox...), rate tuner, and audio exporter (.m4a, .aiff, .wav). |
| **📂 Find Studio Pro** | [`applications/find_studio.v`](applications/find_studio.v) | Advanced filesystem explorer & inode search workbench powered by POSIX/BSD `find`: glob/regex matching, multi-pattern names, entry type filters, size filters, age filters, and permission auditors. |
| **📝 Text Editor Pro** | [`applications/text_editor.v`](applications/text_editor.v) | Ultimate native code editor & workspace: multi-buffer scratchpads, live WebKit Markdown HTML preview, integrated code runner (V, Python, Node, Bash, Ruby), unified diff comparison, regex search/replace, and telemetry. |
| **⚡ Task Manager Pro** | [`applications/task_manager.v`](applications/task_manager.v) | macOS process manager & hardware telemetry monitor: real-time process data grid (PID, Name, CPU %, Memory RSS, State), hardware stats cards, filtering scopes, process signals (SIGKILL, SIGTERM), and `lsof` socket inspector. |
| **📦 Ouch Studio Pro** | [`applications/ouch_studio.v`](applications/ouch_studio.v) | Ultra-fast universal archive & compression workbench powered by `ouch`: lossless/high-density packaging across `.tar.zst`, `.tar.gz`, `.zip`, `.7z`, `.tar.xz`, `.tar.bz2`, compression tuning, and tree hierarchy explorer. |
| **📝 Sed Studio Pro** | [`applications/sed_studio.v`](applications/sed_studio.v) | POSIX/BSD `sed` stream editor & regex transformation workbench: dual-pane live scratchpad, in-place disk file editing (`-i ''`), character/line counters, backup preservation, and 15 built-in recipes. |
| **🌐 IFConfig Studio Pro** | [`applications/ifconfig_studio.v`](applications/ifconfig_studio.v) | Comprehensive native macOS IP intelligence & network diagnostics studio: Public IPv4 & IPv6 detection, rich geolocation (City, Country, GPS, ASN, ISP), 1-click Maps launcher, local interface scanner, and DNS latency ping. |
| **🧮 Qalc Studio Pro** | [`applications/qalc_studio.v`](applications/qalc_studio.v) | Advanced symbolic mathematics & universal unit converter powered by `qalc` (`libqalculate`): arbitrary precision (up to 100 digits), symbolic equation solver, calculus derivatives/integrals, and 30+ formula presets. |
| **⚡ Numbat Studio Pro** | [`applications/numbat_studio.v`](applications/numbat_studio.v) | Scientific & physical dimensional analysis studio powered by `numbat`: statically-typed physical expressions, automatic dimension validation, multi-line physics IDE, and fundamental physical constants database. |
| **📐 Kalker Studio Pro** | [`applications/kalker_studio.v`](applications/kalker_studio.v) | Pure mathematics, natural calculus syntax & complex analysis studio powered by `kalker`: natural calculus syntax (∫, √, f'(x)), complex arithmetic, polar conversions, and matrix/vector algebra. |
| **📊 Statistics Studio Pro** | [`applications/statistics_studio.v`](applications/statistics_studio.v) | Comprehensive scientific data science workbench in pure V: descriptive statistics, normality tests, hypothesis testing (Student's t-test, ANOVA), OLS linear regression, and ASCII histograms. |
| **📈 Graph Studio Pro** | [`applications/graph_studio.v`](applications/graph_studio.v) | High-precision scientific plotting & visualization studio in pure V: 2D continuous function grapher, multi-series data visualizer, bar charts, scatter plots, and network topology graph visualizer. |
| **💻 Programmer Calculator** | [`applications/programmer_calculator.v`](applications/programmer_calculator.v) | Advanced multi-radix computer science calculator in pure V: simultaneous Hex, Dec, Oct, Bin displays, interactive 64-bit grid, IEEE-754 floating point inspector, Endianness converters, and bitwise logic. |
| **🚀 Media & Data Studio Hub** | [`applications/media_studio_hub.v`](applications/media_studio_hub.v) | Master workstation with system environment diagnostics, instant one-click quick tools (Discord <10MB, TikTok 9:16, Loudnorm, Favicon, Remove White BG, 2-Pass GIF, WebP), and sub-app launchers. |

All applications support dynamic runtime theme switching across all 18 curated palettes with automatic state persistence to `~/.config/simplegui/theme.txt`.

### 📸 Workstations Showcase & Screenshots

All 44 applications are fully equipped with native screenshots. See [`applications/README.md`](applications/README.md) for the complete visual catalog.

#### 🧮 Computing, Math & Science

- **💻 Programmer Calculator**: `v run applications/programmer_calculator.v`
  ![💻 Programmer Calculator](screenshots/programmer_calculator.png)

- **📈 Graph Studio Pro**: `v run applications/graph_studio.v`
  ![📈 Graph Studio Pro](screenshots/graph_studio.png)

- **📊 Statistics Studio Pro**: `v run applications/statistics_studio.v`
  ![📊 Statistics Studio Pro](screenshots/statistics_studio.png)

- **🧮 Qalc Studio Pro**: `v run applications/qalc_studio.v`
  ![🧮 Qalc Studio Pro](screenshots/qalc_studio.png)

- **⚡ Numbat Studio Pro**: `v run applications/numbat_studio.v`
  ![⚡ Numbat Studio Pro](screenshots/numbat_studio.png)

- **📐 Kalker Studio Pro**: `v run applications/kalker_studio.v`
  ![📐 Kalker Studio Pro](screenshots/kalker_studio.png)

#### 🌐 Security, Network & OSINT

- **🚀 API Studio Pro**: `v run applications/api_studio.v`
  ![🚀 API Studio Pro](screenshots/api_studio.png)

- **🛡️ Nmap Studio Pro**: `v run applications/nmap_studio.v`
  ![🛡️ Nmap Studio Pro](screenshots/nmap_studio.png)

- **🌐 DNS & SSL Studio Pro**: `v run applications/dns_studio.v`
  ![🌐 DNS & SSL Studio Pro](screenshots/dns_studio.png)

- **🕵️ Recon Studio Pro**: `v run applications/recon_studio.v`
  ![🕵️ Recon Studio Pro](screenshots/recon_studio.png)

- **🌐 IFConfig Studio Pro**: `v run applications/ifconfig_studio.v`

- **🔐 Crypto & Hash Studio**: `v run applications/crypto_studio.v`
  ![🔐 Crypto & Hash Studio](screenshots/crypto_studio.png)

- **🌐 Subfinder Studio Pro**: `v run applications/subfinder_studio.v`
  ![🌐 Subfinder Studio Pro](screenshots/subfinder_studio.png)

#### 🎬 Media, Graphics & Publishing

- **🚀 Media & Data Studio Hub**: `v run applications/media_studio_hub.v`
  ![🚀 Media & Data Studio Hub](screenshots/media_studio_hub.png)

- **🎬 FFmpeg Studio Pro**: `v run applications/ffmpeg_studio.v`
  ![🎬 FFmpeg Studio Pro](screenshots/ffmpeg_studio.png)

- **🎨 ImageMagick Studio Pro**: `v run applications/imagemagick_studio.v`
  ![🎨 ImageMagick Studio Pro](screenshots/imagemagick_studio.png)

- **🎬 yt-dlp Studio Pro**: `v run applications/yt_dlp_studio.v`
  ![🎬 yt-dlp Studio Pro](screenshots/yt_dlp_studio.png)

- **📊 Graphviz Studio Pro**: `v run applications/dot_studio.v`
  ![📊 Graphviz Studio Pro](screenshots/dot_studio.png)

- **👁️ Tesseract OCR Pro**: `v run applications/ocr_studio.v`
  ![👁️ Tesseract OCR Pro](screenshots/ocr_studio.png)

- **🎵 Audio Tag Studio Pro**: `v run applications/audiotag_studio.v`
  ![🎵 Audio Tag Studio Pro](screenshots/audiotag_studio.png)

- **📄 Pandoc Studio Pro**: `v run applications/pandoc_studio.v`
  ![📄 Pandoc Studio Pro](screenshots/pandoc_studio.png)

- **🗣️ Say Studio Pro**: `v run applications/say_studio.v`
  ![🗣️ Say Studio Pro](screenshots/say_studio.png)

#### ⚡ Data Engineering & Stream Processing

- **🧩 JQ Studio Pro**: `v run applications/jq_studio.v`
  ![🧩 JQ Studio Pro](screenshots/jq_studio.png)

- **🔄 Format Converter Pro**: `v run applications/dataconvert_studio.v`
  ![🔄 Format Converter Pro](screenshots/dataconvert_studio.png)

- **🗄️ SQLite Studio Pro**: `v run applications/sqlite_studio.v`
  ![🗄️ SQLite Studio Pro](screenshots/sqlite_studio.png)

- **⚡ GAWK Studio Pro**: `v run applications/gawk_studio.v`
  ![⚡ GAWK Studio Pro](screenshots/gawk_studio.png)

- **📝 Sed Studio Pro**: `v run applications/sed_studio.v`
  ![📝 Sed Studio Pro](screenshots/sed_studio.png)

- **🎯 Regex Studio Pro**: `v run applications/regex_studio.v`
  ![🎯 Regex Studio Pro](screenshots/regex_studio.png)

- **✂️ Cut Studio Pro**: `v run applications/cut_studio.v`
  ![✂️ Cut Studio Pro](screenshots/cut_studio.png)

- **🔄 TR Studio Pro**: `v run applications/tr_studio.v`
  ![🔄 TR Studio Pro](screenshots/tr_studio.png)

#### 🛠️ System, DevOps & Filesystem Workstations

- **📦 App Bundler Studio Pro**: `v run applications/app_bundler_studio.v`
  ![📦 App Bundler Studio Pro](screenshots/app_bundler_studio.png)

- **⚡ Task Manager Pro**: `v run applications/task_manager.v`
  ![⚡ Task Manager Pro](screenshots/task_manager.png)

- **📝 Text Editor Pro**: `v run applications/text_editor.v`
  ![📝 Text Editor Pro](screenshots/text_editor.png)

- **🍺 Homebrew Studio Pro**: `v run applications/brew_studio.v`
  ![🍺 Homebrew Studio Pro](screenshots/brew_studio.png)

- **🐳 Docker Studio Pro**: `v run applications/docker_studio.v`
  ![🐳 Docker Studio Pro](screenshots/docker_studio.png)

- **💾 Disk Space Studio Pro**: `v run applications/disk_studio.v`
  ![💾 Disk Space Studio Pro](screenshots/disk_studio.png)

- **⏰ Launchd & Cron Pro**: `v run applications/launchd_studio.v`
  ![⏰ Launchd & Cron Pro](screenshots/launchd_studio.png)

- **🔍 RG Studio Pro**: `v run applications/rg_studio.v`
  ![🔍 RG Studio Pro](screenshots/rg_studio.png)

- **⚡ FD Studio Pro**: `v run applications/fd_studio.v`
  ![⚡ FD Studio Pro](screenshots/fd_studio.png)

- **📂 Find Studio Pro**: `v run applications/find_studio.v`
  ![📂 Find Studio Pro](screenshots/find_studio.png)

- **🔍 SD Studio Pro**: `v run applications/sd_studio.v`
  ![🔍 SD Studio Pro](screenshots/sd_studio.png)

- **📦 Ouch Studio Pro**: `v run applications/ouch_studio.v`
  ![📦 Ouch Studio Pro](screenshots/ouch_studio.png)

- **⚡ Wget2 Studio Pro**: `v run applications/wget2_studio.v`
  ![⚡ Wget2 Studio Pro](screenshots/wget2_studio.png)

---
👉 **Explore the complete detailed documentation and all screenshots in the [SimpleGUI Applications Suite README](applications/README.md).**

---

## SimpleCLI: Headless Console & RAD Toolkit

For headless console scripts, backend daemons, automation tools, and CI/CD pipelines without graphical window backends, `simplegui` includes **`simplecli`** ([`simplecli/`](simplecli/)):

- **Zero GUI Dependencies**: Pure terminal RAD toolkit (ANSI colors, banners, panels, data tables, spinners, progress bars, interactive prompts).
- **Subprocess & Telemetry**: Safe process execution (`exec_safe`, `exec_timeout`, `parallel_exec`), argument quoting, and hardware resource monitoring (CPU, RAM, swap, load, battery).
- **Stdlib & Cryptography**: Native HTTP client, SHA-256/512/MD5, AES-256-CTR, Bcrypt, GZIP, TOML, JSON, and generic data collections (`SimpleStack`, `SimpleQueue`, `SimpleRingBuffer`, `SimpleMinHeap`).
- **Production CLI Suite**: 49 ready-to-use tools in [`cli_apps/`](cli_apps/), all with generated `--help`/`--version`, strict typed argument validation, stderr diagnostics, and nonzero invalid-usage exit codes.

👉 **Read the comprehensive 20-chapter [SimpleCLI Reference Manual (CLI_API.md)](CLI_API.md) or browse the [complete CLI application catalog](cli_apps/README.md).**

---

## Developer Utility Suite (40 Modules)

`vlang_simplegui` bundles the complete **`vlang_utils`** developer utility suite — 40 standalone, zero-dependency, production-grade utility modules designed for rapid application development:

| Module | Description |
| :--- | :--- |
| [`fileutils`](UTILS_API.md#fileutils-api) | JSON serialization, text operations, CSV parsing, copy/move, recursive listing, human-readable file sizes. |
| [`sqliteutils`](UTILS_API.md#sqliteutils-api) | Ergonomic SQLite persistence, KV store, JSON document store, SQL injection defense, parameterized CRUD, secure PRAGMAs, DDL migrations. |
| [`strutils`](UTILS_API.md#strutils-api) | Case conversions (snake, kebab, camel, pascal, title), slugify, masking, padding, Levenshtein distance, word wrap. |
| [`sliceutils`](UTILS_API.md#sliceutils-api) | Generic collection operations: unique, chunk, flatten, partition, intersection, difference, shuffle, sampling, stats. |
| [`envutils`](UTILS_API.md#envutils-api) | Type-safe environment variable retrieval (`get_str`, `get_int`, `get_bool`), setters (`set`, `set_int`, `set_bool`), `.env` file loader, variable expansion. |
| [`cryptoutils`](UTILS_API.md#cryptoutils-api) | SHA-256, SHA-512, MD5, HMAC-SHA256, Base64 / Base64URL encode/decode, UUID v4, secure tokens. |
| [`timeutils`](UTILS_API.md#timeutils-api) | Human relative time ("2 hours ago", "in 3 days"), ISO 8601 formatting/parsing, calendar boundaries, `Stopwatch`. |
| [`httputils`](UTILS_API.md#httputils-api) | Ergonomic HTTP client (`get_json[T]`, `post_json[T, R]`, `get_text`), query string builder/parser, retry with backoff. |
| [`cliutils`](UTILS_API.md#cliutils-api) | Terminal ANSI styling, FlagParser, interactive prompts, progress bar, sparkline, bar chart, gauge, tree, diff, tables. |
| [`sysutils`](UTILS_API.md#sysutils-api) | System telemetry (CPU usage/cores, RAM, swap, disk, uptime, load averages), safe exec (`exec_safe`, `quote_arg`), paths, clipboard. |
| [`netutils`](UTILS_API.md#netutils-api) | Network discovery (local/public IP, MAC, Wi-Fi SSID, DNS servers, gateway, listening ports), connectivity check & TCP ping. |
| [`validutils`](UTILS_API.md#validutils-api) | High-speed validation for email, URL, IPv4/IPv6, phone numbers, alphanumeric strings, numeric ranges, UUID, JSON. |
| [`structutils`](UTILS_API.md#structutils-api) | Generic RAD data structures: `SimpleStack[T]`, `SimpleQueue[T]`, circular `SimpleRingBuffer[T]`, and `SimpleMinHeap`. |
| [`statutils`](UTILS_API.md#statutils-api) | Statistical analysis, regression & modeling: mean, median, mode, sample/pop variance & std dev, SEM, quartiles, IQR, skewness, kurtosis, covariance, Pearson/Spearman correlation, OLS linear regression, normal PDF/CDF, Z-scores, outlier detection, moving averages, and summary profiles. |
| [`stateutils`](UTILS_API.md#stateutils-api) | Managed app state persistence (`AppStateStore[T]`, `KeyValueState`) in OS-recommended paths with atomic writes, auto-save, and rollback. |
| [`cacheutils`](UTILS_API.md#cacheutils-api) | In-memory caching: fixed-capacity `LRUCache[T]` with O(1) recency eviction and entry-level `TTLCache[T]` with auto-cleanup and `get_or_set`. |
| [`semverutils`](UTILS_API.md#semverutils-api) | Semantic Versioning 2.0.0 parsing, precedence comparison, range matching (`^`, `~`, `>=`, `<=`), and version bumping. |
| [`flowutils`](UTILS_API.md#flowutils-api) | Traffic control & resilience: Token Bucket `RateLimiter`, 3-state `CircuitBreaker`, exponential backoff `retry[T]`, and event `Debouncer`. |
| [`templateutils`](UTILS_API.md#templateutils-api) | Fast string templating with fallback defaults (`{{key | default}}`), dynamic resolvers, and styled ANSI markdown terminal rendering. |
| [`colorutils`](UTILS_API.md#colorutils-api) | HEX/RGB/HSL color conversions, color theory transforms, WCAG 2.1 accessibility & contrast auditing, and 24-bit Truecolor terminal formatting. |
| [`archiveutils`](UTILS_API.md#archiveutils-api) | Ergonomic Zip archive creation, extraction, recursive directory compression, and in-memory inspection via V's native `compress.szip`. |
| [`asyncutils`](UTILS_API.md#asyncutils-api) | Bounded concurrency primitives: order-preserving `parallel_map[T, R]`, `parallel_filter[T]`, `parallel_each[T]`, `WaitGroup`, and `WorkerPool`. |
| [`regexutils`](UTILS_API.md#regexutils-api) | High-level regular expression helpers: `is_match`, `find_first`, `find_all`, `replace`, `split`, and `find_matches`. |
| [`mockutils`](UTILS_API.md#mockutils-api) | Synthetic testing & prototyping data generation: `lorem_text`, `lorem_words`, `mock_user`, `mock_email`, `mock_phone`, `mock_ipv4`, `mock_url`. |
| [`logutils`](UTILS_API.md#logutils-api) | Leveled structured logging (`LogLevel`, `Logger`, file/stdout targets, ANSI color highlighting). |
| [`tomlutils`](UTILS_API.md#tomlutils-api) | TOML configuration file and string parsing with typed accessors (`get_string`, `get_int`, `get_bool`, `get_strings`). |
| [`htmlutils`](UTILS_API.md#htmlutils-api) | HTML parsing, DOM navigation (`get_element_by_id`, `get_elements_by_tag`), entity escaping, and tag stripping. |
| [`bitutils`](UTILS_API.md#bitutils-api) | Dynamic bitsets (`BitSet`), bitwise operations, popcount, binary string conversions, and bitmask flag manipulation. |
| [`compressutils`](UTILS_API.md#compressutils-api) | Fast compression & decompression for Gzip, Zlib, Deflate, and Zstandard strings and byte buffers. |
| [`tarutils`](UTILS_API.md#tarutils-api) | In-memory and on-disk TAR archive creation, unpacking, directory archiving, and tarball inspection. |
| [`mathutils`](UTILS_API.md#mathutils-api) | 2D Math, spatial geometry, and number theory: `lerp`, `remap`, `clamp`, `round_to_step`, `Point2D`, `Rect` intersection, `gcd`, `lcm`, and power-of-two checks. |
| [`cronutils`](UTILS_API.md#cronutils-api) | Standard 5-field cron parsing, next execution calculation (`next_after`), matching, and human-readable English summaries (`cron_to_human`). |
| [`urlutils`](UTILS_API.md#urlutils-api) | RFC 3986 URL parsing (`parse_url`), component inspection, path segment joining (`join_path`), and credential redaction (`redact_credentials`). |
| [`jwtutils`](UTILS_API.md#jwtutils-api) | Lightweight, zero-dependency HS256 JSON Web Token signing (`sign_jwt`) and verification (`verify_jwt`) with expiration checks. |
| [`eventutils`](UTILS_API.md#eventutils-api) | In-memory publish-subscribe event dispatcher (`EventEmitter`, `on`, `once`, `off`, `emit`). |
| [`diffutils`](UTILS_API.md#diffutils-api) | Line-level text diffing (`diff_lines`) and standard Git-style unified diff generation (`unified_diff`). |
| [`graphutils`](UTILS_API.md#graphutils-api) | Generic Directed Acyclic Graphs (`Graph[T]`), cycle detection, Kahn's topological sort (`topological_sort`), BFS, and DFS. |
| [`jsonutils`](UTILS_API.md#jsonutils-api) | RFC 6901 JSON Pointer get/set, RFC 7386 Merge Patch, canonical encoding, deep equality, structural diff, and pretty/minify. |
| [`markdownutils`](UTILS_API.md#markdownutils-api) | Safe Markdown → HTML converter (GFM tables, task lists, code blocks), heading anchors, TOC generation, and plain-text extraction. |
| [`webutils`](UTILS_API.md#webutils-api) | Express-style web framework with a secure EJS-style template engine and batteries included (sessions, CSRF, CORS, rate limiting, security headers, static files, multipart, gzip, signed cookies, in-process testing). |

👉 **Read the comprehensive 6,000+ line [Developer Utility Suite Reference Manual (UTILS_API.md)](UTILS_API.md).**

---

# Related GUI & RAD Desktop Projects

Explore sister projects and complementary GUI frameworks, templates, and RAD visual design suites:

| Project | Primary Stack | Architecture & Description |
| :--- | :--- | :--- |
| **[simple_gg](https://github.com/codecaine-zz/simple_gg)** | V (vlang) + Sokol / gg | Lightweight, cross-platform hardware-accelerated GUI framework with zero C/Obj-C dependencies. Delivers 47 desktop studio workstations, 49 companion CLIs, 34 themes, and reactive state persistence. |
| **[Vlang Webview RAD Studio](https://github.com/codecaine-zz/vlang_webview_rad_studio)** | V (vlang) + Native OS Webview | Cross-platform visual Rapid Application Development (RAD) IDE & enterprise desktop suite. Borland Delphi/VB-inspired form designer with 70+ controls, 42 desktop themes, 16 enterprise studio applications, 16 companion CLIs, and standalone `.app`/`.exe`/ELF packager. |
| **[Bun RAD Studio](https://github.com/codecaine-zz/bun_rad_studio)** | Bun + TypeScript / Webview-Bun | High-performance visual RAD IDE for Bun and Webview-Bun, featuring instant hot-reload, drag-and-drop form canvas, full TypeScript type safety, and 42 desktop themes. |
| **[Vlang macOS Webview App Template](https://github.com/codecaine-zz/vlang_macos_webview_app_template)** | V (vlang) + Cocoa Webview | Standalone native macOS `.app` desktop application template for V using `ttytm.webview`, Cocoa Objective-C window helper integration (`window_helper.m`), 9-point screen placement geometry, stay-on-top pinning, and two-way IPC. |
| **[vlang_simplegui](https://github.com/codecaine-zz/vlang_simplegui)** | V (vlang) + Native GUI | Declarative macOS-native GUI starter framework written in V, featuring fluent builder syntax, reactive two-way value synchronization, KPI dashboards, sortable tables, and dynamic live theme switching across 42 themes. |

### Notable Open-Source Webview Frameworks

- **[pywebview](https://github.com/r0x0r/pywebview)** - Lightweight, cross-platform native GUI window wrapper around OS webviews for Python applications by Roman Sirokov (`r0x0r`) and community.
- **[Neutralinojs](https://github.com/neutralinojs/neutralinojs)** - Portable, lightweight cross-platform desktop application development framework using web technologies and native webview by the Neutralinojs organization.

---

## Security & Command Injection Prevention

SimpleGUI features a centralized security module (`security.v`) providing POSIX-compliant argument quoting and safe subshell execution primitives to eliminate command breakout vulnerabilities:

```v
import simplegui

// 1. Strictly quote arguments for safe subshell execution
safe_arg := simplegui.quote_arg('user_input; rm -rf /')
// Output: '\'user_input; rm -rf /\''

// 2. Safely execute CLI binaries without shell breakout risk
res := simplegui.exec_safe('rg', ['-n', '-e', user_pattern, target_dir])

// 3. Safely pipe stdin from a file
stream_res := simplegui.exec_safe_stdin('tr', ['-d', '\r'], input_file)

// 4. Sanitize user-provided filenames to prevent path traversal
clean_name := simplegui.sanitize_filename('../../secret.txt')
```

---

## Contributing

Contributions are welcome! If you find a bug, have a feature request, or want to contribute new controls or themes:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/my-feature`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Run tests (`VJOBS=1 v test .`) and the application wiring audit (`v test tests/application_audit_test.v`)
5. Push to your branch (`git push origin feature/my-feature`)
6. Open a Pull Request

## License

This project is licensed under the MIT License.
