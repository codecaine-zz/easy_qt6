add_executable(screenshot_generator main.cpp)
target_link_libraries(screenshot_generator PRIVATE simplegui::simplegui Qt6::Widgets Qt6::WebEngineWidgets)
