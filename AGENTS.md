# EasyQt6 AI Agent Instructions

## Project Direction

- **Primary Goal**: Maintain and extend a C++ wrapper library for Qt 6 Widgets designed to be as simple as Visual Basic.
- **Cross-Platform**: Target macOS, Linux, Windows x64, and Windows ARM64.
- **Build System**: Use CMake and prefer Ninja when available.
- **Dependencies**: Support standard Qt6 and QtWebEngineWidgets installations.

## Working Rules

- Inspect the actual source before making implementation decisions.
- **Architecture**: Do not introduce raw Qt classes into the public headers. Always use the PIMPL pattern (Pointer to Implementation) to hide `QPointer` and `QWidget` implementations inside the `.cpp` files.
- **Memory Safety**: Event handlers must return an `EventConnection` token that securely wraps `QMetaObject::Connection`.
- Do not install packages or change system settings without permission.
- Do not add generated build artifacts to source control.
- Do not commit, push, publish, or use signing credentials without permission.
- Keep explanations clear and understandable.