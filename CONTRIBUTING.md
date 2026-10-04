# Contributing to EasyQt6

Thanks for helping! Contributions of every size are welcome. Please be kind and patient with
everyone in the community.

## Prerequisites

- A C++17 compiler (Clang, GCC, or MSVC 2022)
- CMake 3.16+ (Ninja recommended)
- Qt 6 with the **Widgets** and **WebEngineWidgets** modules

## Repository layout

| Path | Contents |
|---|---|
| `cpp/include/simplegui/` | Public headers — the API users see |
| `cpp/src/` | Implementations (`.cpp`) and private helpers in `cpp/src/detail/` |
| `cpp/examples_shared/` | Cross-platform examples |
| `cpp/examples_macos/`, `cpp/examples_linux/`, `cpp/examples_windows/` | Platform-styled examples |
| `cpp/tests/` | Headless smoke tests run by `ctest` |
| `cpp/tools/screenshot_generator/` | Renders the images in `screenshots/` |
| `docs/API_REFERENCE.md` | The complete user guide |

## Build and test

```bash
cmake -S cpp -B cpp/build -G Ninja -DCMAKE_CXX_FLAGS="-Wall -Wextra -Wpedantic -Wshadow"
cmake --build cpp/build
ctest --test-dir cpp/build --output-on-failure
```

New code must build **without warnings** and all tests must pass.

To refresh screenshots:

```bash
./cpp/build/tools/screenshot_generator/screenshot_generator screenshots
```

## Design rules

1. **No Qt types in public headers.** Use the PIMPL pattern: declare `struct Impl;` in the
   header and keep `QPointer`/`QWidget` members in the `.cpp` file. The only exception is the
   existing `get_qwidget()` escape hatch.
2. **Events return `EventConnection`.** Use the helpers in `cpp/src/detail/common.h`
   (`detail::wrap`, `detail::make_event`, `detail::add_handler`, `detail::fire`).
3. **Never capture `this` in Qt callbacks.** Capture a `std::weak_ptr<Impl>` instead, so a
   callback can't run after the control is destroyed.
4. **Plain text and safe colors.** Show user text with `Qt::PlainText`, and parse colors with
   `detail::parse_color` before putting them in a style sheet.
5. **Bounds-check everything.** Out-of-range indexes and unknown ids should be ignored (or
   return `false`), never crash.
6. **Beginner-friendly names.** Prefer `set_x` / `get_x` / `on_event`, add a short comment to
   every public function, and document it in `docs/API_REFERENCE.md`.
7. **Portable code.** Target macOS, Linux, Windows x64 and Windows ARM64. Avoid compiler
   extensions such as `M_PI`, and use `QChar`/`QString::fromUcs4` for non-ASCII characters
   in string literals.

## Pull requests

- Keep each PR focused on one change.
- Update `docs/API_REFERENCE.md` and `CHANGELOG.md` for any user-visible change.
- Add or extend a test in `cpp/tests/` when fixing a bug.
- Don't commit build output (`cpp/build/`, `*_autogen/`, binaries).
- Use LF line endings (enforced by `.gitattributes`).
