add_executable(example_web_browser main.cpp)
target_link_libraries(example_web_browser PRIVATE simplegui::simplegui)

# Set macOS application bundle properties
set_target_properties(example_web_browser PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.webbrowser
    MACOSX_BUNDLE_BUNDLE_NAME "Web Browser"
)
