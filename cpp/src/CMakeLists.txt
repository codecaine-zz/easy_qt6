add_library(simplegui
    application.cpp
    window.cpp
    control.cpp
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
    sparkline.cpp
    vfd_meter.cpp
    stat_card.cpp
    composition_bar.cpp
    status_pill.cpp
    activity_heatmap.cpp
    stat_grid.cpp
    user_profile_card.cpp
    product_card.cpp
    donut_chart.cpp
    kanban_board.cpp
    feedback_mood.cpp
    date_range_picker.cpp
    masked_input.cpp
    token_field.cpp
    nav_rail.cpp
    media_player.cpp
    bar_chart.cpp
    line_chart.cpp
    pie_chart.cpp
    canvas.cpp
    radar_chart.cpp
    candlestick_chart.cpp
    timer.cpp
    dialogs.cpp
    list_box.cpp
    toggle_switch.cpp
    radial_gauge.cpp
    neon_button.cpp
    led_indicator.cpp
    radar_scope.cpp
    terminal_view.cpp
    segmented_control.cpp
    glass_panel.cpp
    detail/common.h
    detail/box_core.h
)

target_include_directories(simplegui PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

# Private helpers in src/detail/ are only visible to the library itself.
target_include_directories(simplegui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(simplegui PUBLIC Qt6::Widgets Qt6::WebEngineWidgets)

add_library(simplegui::simplegui ALIAS simplegui)
