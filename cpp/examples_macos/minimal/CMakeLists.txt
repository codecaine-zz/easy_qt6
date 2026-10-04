add_executable(example_minimal main.cpp)
target_link_libraries(example_minimal PRIVATE simplegui::simplegui)

# Ensure GUI app behavior on macOS/Windows
set_target_properties(example_minimal PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
