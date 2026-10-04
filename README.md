# EasyQt6

EasyQt6 is a modern, lightweight, and zero-boilerplate C++ wrapper around Qt6. It is designed to be as straightforward to use as Visual Basic or Delphi - allowing rapid UI development without touching raw Qt classes, macros, or memory management.

## Features

- **Zero Boilerplate**: No `Q_OBJECT` macros, no `MOC` headaches, and no need to subclass `QMainWindow` just to put a button on the screen.
- **PIMPL Architecture**: Your application code doesn't `#include` Qt headers. This keeps compile times blazingly fast and your namespace clean.
- **Modern C++**: Uses `std::shared_ptr` for memory-safe UI construction and `std::function` lambdas for inline, readable event handling.
- **Cross-Platform**: Powered by Qt6, runs seamlessly on macOS, Linux, and Windows.

## Quick Start

See the `cpp/examples` folder for copy-paste-ready RAD templates:

- `hello_world`: The absolute bare minimum to get a window on screen.
- `login_form`: Demonstrates layouts, checkboxes, and input masking.
- `calculator`: Demonstrates dynamic UI building with loops and grids.
- `web_browser`: A fully functional mini browser using `QtWebEngine` in under 40 lines.

### Example

```cpp
#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/button.h"
#include "simplegui/label.h"
#include "simplegui/vbox.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    simplegui::Window window("My App", 400, 300);

    auto layout = std::make_shared<simplegui::VBox>();
    auto label = std::make_shared<simplegui::Label>("Ready.");
    auto btn = std::make_shared<simplegui::Button>("Click Me!");

    btn->on_click([label]() {
        label->set_text("You clicked the button!");
    });

    layout->add_child(label);
    layout->add_child(btn);
    
    window.set_content(layout);
    window.show();
    
    return app.run();
}
```

## Documentation

Check the full [API Reference](docs/API_REFERENCE.md) to see all supported layouts, controls, and advanced web views.

## Building

### Dependencies (macOS)

Before building, you must install the Qt6 framework and the required build tools using Homebrew:

```bash
brew install qt cmake ninja
```

### Compile

```bash
# Generate the build system
cmake -S cpp -B cpp/build -G Ninja

# Compile the library and examples
cmake --build cpp/build
```
