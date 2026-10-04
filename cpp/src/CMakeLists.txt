add_library(simplegui
    application.cpp
    window.cpp
    label.cpp
    text_input.cpp
    button.cpp
    vbox.cpp
    hbox.cpp
    checkbox.cpp
    slider.cpp
    radio.cpp
    dropdown.cpp
    textarea.cpp
    progress_indicator.cpp
    number_input.cpp
    knob.cpp
    date_picker.cpp
    group_box.cpp
    tab_view.cpp
    color_well.cpp
    combo_box.cpp
    grid.cpp
    link.cpp
    password_input.cpp
    search_field.cpp
    image_button.cpp
    image.cpp
    split_view.cpp
    scroll_view.cpp
    circular_progress.cpp
    rating.cpp
    breadcrumbs.cpp
    web_view.cpp
)

target_include_directories(simplegui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(simplegui PUBLIC Qt6::Widgets Qt6::WebEngineWidgets)

add_library(simplegui::simplegui ALIAS simplegui)
