add_executable(example_calculator main.cpp)
target_link_libraries(example_calculator PRIVATE simplegui::simplegui)

# Set macOS application bundle properties
set_target_properties(example_calculator PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.calculator
    MACOSX_BUNDLE_BUNDLE_NAME "Calculator"
)
