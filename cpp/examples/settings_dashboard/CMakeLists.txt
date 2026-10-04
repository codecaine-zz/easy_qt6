add_executable(example_settings_dashboard main.cpp)
target_link_libraries(example_settings_dashboard PRIVATE simplegui::simplegui)

set_target_properties(example_settings_dashboard PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.settingsdashboard
    MACOSX_BUNDLE_BUNDLE_NAME "Settings Dashboard"
)
