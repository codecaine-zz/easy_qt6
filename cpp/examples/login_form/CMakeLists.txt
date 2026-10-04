add_executable(example_login_form main.cpp)
target_link_libraries(example_login_form PRIVATE simplegui::simplegui)

# Set macOS application bundle properties
set_target_properties(example_login_form PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.loginform
    MACOSX_BUNDLE_BUNDLE_NAME "Login Form"
)
