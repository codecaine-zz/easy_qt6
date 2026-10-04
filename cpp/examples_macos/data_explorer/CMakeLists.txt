add_executable(example_data_explorer main.cpp)
target_link_libraries(example_data_explorer PRIVATE simplegui::simplegui)

set_target_properties(example_data_explorer PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.dataexplorer
    MACOSX_BUNDLE_BUNDLE_NAME "Data Explorer"
)
