add_executable(example_hello_world main.cpp)
target_link_libraries(example_hello_world PRIVATE simplegui::simplegui)

# Set macOS application bundle properties
set_target_properties(example_hello_world PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.helloworld
    MACOSX_BUNDLE_BUNDLE_NAME "Hello World"
)
