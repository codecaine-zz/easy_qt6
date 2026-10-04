add_executable(example_system_monitor main.cpp)
target_link_libraries(example_system_monitor PRIVATE simplegui::simplegui)

set_target_properties(example_system_monitor PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.systemmonitor
    MACOSX_BUNDLE_BUNDLE_NAME "System Monitor"
)
