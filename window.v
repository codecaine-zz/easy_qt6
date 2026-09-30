module simplegui

#flag -I @VMODROOT

#include <Cocoa/Cocoa.h>

#include "window.h"

#flag -framework Cocoa
#flag -framework WebKit
#flag -framework QuartzCore
#flag -framework ApplicationServices
#flag -framework IOKit
#flag -framework AudioToolbox
#flag -framework PDFKit
#flag -framework AVKit
#flag -framework AVFoundation
#flag -framework MapKit
#flag -framework CoreLocation
#flag -framework MetalKit
#flag -framework Metal

#flag @VMODROOT/window.m

fn C.window_app_init(&WindowParams) &WindowInfo

fn C.window_app_run(&WindowInfo)

fn C.window_app_exit(&WindowInfo)

fn C.window_set_title_text(&WindowInfo, &u8)

fn C.window_set_status_text(&WindowInfo, &u8)

fn C.window_set_always_on_top(&WindowInfo, int)

fn C.window_get_always_on_top(&WindowInfo) int

fn C.window_set_background_color(&WindowInfo, &u8)

fn C.window_set_padding(&WindowInfo, int)

fn C.window_set_spacing(&WindowInfo, int)

fn C.window_set_responsive_layout(&WindowInfo, int)

fn C.window_add_group_box_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_group_box_control_with_options(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_add_group_box_control_with_config(&WindowInfo, &u8, &u8, int, f32, &u8, f32, &u8, int, int, int, &u8, &u8) voidptr

fn C.window_begin_group_box(&WindowInfo, &u8, &u8, int)

fn C.window_begin_group_box_with_config(&WindowInfo, &u8, &u8, int, f32, &u8, f32, &u8, int, int, int, &u8, &u8)

fn C.window_end_group_box(&WindowInfo)

fn C.window_set_group_border(&WindowInfo, &u8, int)

fn C.window_set_group_caption(&WindowInfo, &u8, &u8)

fn C.window_set_group_style(&WindowInfo, &u8, int, f32, &u8, f32, &u8, int)

fn C.window_add_tabs_control(&WindowInfo, &u8, &&u8, int) voidptr

fn C.window_add_scroll_view_control(&WindowInfo, &u8, int) voidptr

fn C.window_focus_control(&WindowInfo, &u8)

fn C.window_set_placeholder_by_name(&WindowInfo, &u8, &u8)

fn C.window_set_error_by_name(&WindowInfo, &u8, &u8)

fn C.window_set_tooltip_by_name(&WindowInfo, &u8, &u8)

fn C.window_set_default_button_by_name(&WindowInfo, &u8)

fn C.window_run_after(&WindowInfo, int, &u8)

fn C.window_show_toast(&WindowInfo, &u8)

fn C.window_open_url(&WindowInfo, &u8)

fn C.window_copy_to_clipboard(&WindowInfo, &u8)

fn C.window_get_clipboard_text() &u8

fn C.window_reveal_in_finder(&u8) int

fn C.window_set_font_color(&WindowInfo, &u8)

fn C.window_set_control_background_color_by_name(&WindowInfo, &u8, &u8)

fn C.window_set_control_font_color_by_name(&WindowInfo, &u8, &u8)

fn C.window_set_control_width_by_name(&WindowInfo, &u8, int)

fn C.window_set_control_height_by_name(&WindowInfo, &u8, int)

fn C.window_set_control_font_size_by_name(&WindowInfo, &u8, int)

fn C.window_set_control_font_bold_by_name(&WindowInfo, &u8, int)

fn C.window_set_control_font_name_by_name(&WindowInfo, &u8, &u8)

fn C.window_show_choice_dialog(&WindowInfo, &u8, &u8, &&u8, int) int

fn C.window_add_context_menu_item(&WindowInfo, &u8, &u8, &u8)

// Dialogs and Message boxes
fn C.window_show_alert(&WindowInfo, &u8, &u8)

fn C.window_show_alert_with_style(&WindowInfo, &u8, &u8, &u8)

fn C.window_show_confirm(&WindowInfo, &u8, &u8) int

fn C.window_show_prompt(&WindowInfo, &u8, &u8, &u8) &u8

// File Panels
fn C.window_select_file(&WindowInfo) &u8

fn C.window_select_file_with_extensions(&WindowInfo, &u8) &u8

fn C.window_select_folder(&WindowInfo) &u8

fn C.window_save_file_picker(&WindowInfo) &u8

// Visibility & Enabled
fn C.window_set_control_visible_by_name(&WindowInfo, &u8, int)

fn C.window_get_control_visible_by_name(&WindowInfo, &u8) int

fn C.window_set_control_enabled_by_name(&WindowInfo, &u8, int)

fn C.window_get_control_enabled_by_name(&WindowInfo, &u8) int

fn C.window_highlight_control_by_name(&WindowInfo, &u8, int)

fn C.window_flash_control_by_name(&WindowInfo, &u8)

fn C.window_list_external_apps() &u8

fn C.window_spy_external_app(int) &u8

fn C.window_set_external_control_value(int, &u8, &u8) int

fn C.window_press_external_control(int, &u8) int

fn C.window_set_external_control_enabled(int, &u8, int) int

fn C.window_set_external_control_visible(int, &u8, int) int

fn C.window_flash_external_control(int, &u8) int

// Timers
fn C.window_set_interval(&WindowInfo, int, &u8)

fn C.window_stop_interval(&WindowInfo, &u8)

// List Box and Image View Controls
fn C.window_add_list_box_control(&WindowInfo, &u8, &&u8, int) voidptr

fn C.window_update_list_items(&WindowInfo, &u8, &&u8, int)

fn C.window_set_list_selected(&WindowInfo, &u8, int)

fn C.window_get_list_selected(&WindowInfo, &u8) int

fn C.window_set_list_multi_select(&WindowInfo, &u8, int)

fn C.window_get_list_selected_indexes(&WindowInfo, &u8) &u8

fn C.window_set_list_selected_indexes(&WindowInfo, &u8, &u8)

fn C.window_select_all_list_items(&WindowInfo, &u8)

fn C.window_clear_list_selection(&WindowInfo, &u8)

fn C.window_add_image_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_set_image_path(&WindowInfo, &u8, &u8)

// Hover tracking
fn C.window_enable_hover_events(&WindowInfo, &u8)

// Menu Customization
fn C.window_add_menu_item(&WindowInfo, &u8, &u8, &u8, &u8)

// Name-based generic control accessors
fn C.window_set_control_text_by_name(&WindowInfo, &u8, &u8)

fn C.window_get_control_text_by_name(&WindowInfo, &u8) &u8

fn C.window_set_control_bool_by_name(&WindowInfo, &u8, int)

fn C.window_get_control_bool_by_name(&WindowInfo, &u8) int

fn C.window_set_control_int_by_name(&WindowInfo, &u8, int)

fn C.window_get_control_int_by_name(&WindowInfo, &u8) int

// Dynamic control creation bridges
fn C.window_add_label_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_input_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_password_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_textarea_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_textarea_goto_line(&WindowInfo, &u8, int, int)

fn C.window_add_html_view_control(&WindowInfo, &u8, &u8) voidptr
fn C.window_add_pdf_view_control(&WindowInfo, &u8, &u8) voidptr
fn C.window_add_avplayer_view_control(&WindowInfo, &u8, &u8) voidptr
fn C.window_add_mtk_view_control(&WindowInfo, &u8) voidptr
fn C.window_add_map_view_control(&WindowInfo, &u8) voidptr
fn C.window_add_column_browser_control(&WindowInfo, &u8) voidptr
fn C.window_show_color_sampler(&WindowInfo) &char

fn C.window_add_drop_zone_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_checkbox_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_add_radio_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_add_button_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_number_control(&WindowInfo, &u8, int) voidptr

fn C.window_add_slider_control(&WindowInfo, &u8, int) voidptr

fn C.window_add_theme_menu_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_color_well_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_date_picker_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_date_time_picker_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_mode_control_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_progress_indicator_control(&WindowInfo, &u8, int) voidptr

// Layout row & container groupings
fn C.window_begin_row(&WindowInfo, &u8)

fn C.window_end_row(&WindowInfo)

fn C.window_begin_grid(&WindowInfo, &u8, int, int)

fn C.window_end_grid(&WindowInfo)

fn C.window_begin_flex_box(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_end_flex_box(&WindowInfo)

fn C.window_set_control_alignment_by_name(&WindowInfo, &u8, &u8)

fn C.window_set_control_expand_fill_by_name(&WindowInfo, &u8, int)

// Spacers and Separators
fn C.window_add_vertical_spacer(&WindowInfo, int)

fn C.window_add_horizontal_spacer(&WindowInfo, int)

fn C.window_add_separator(&WindowInfo)

// Multi-Column Table Controls
fn C.window_add_table_control(&WindowInfo, &u8, &&u8, int) voidptr

fn C.window_set_table_rows(&WindowInfo, &u8, &&u8, int, int)

fn C.window_table_set_column_selection(&WindowInfo, &u8, int)

fn C.window_table_get_selected_column(&WindowInfo, &u8) int

fn C.window_table_set_selected_column(&WindowInfo, &u8, int)

fn C.window_table_delete_column(&WindowInfo, &u8, int)

// Tree View Controls
fn C.window_add_tree_view_control(&WindowInfo, &u8, int) voidptr

fn C.window_set_tree_nodes(&WindowInfo, &u8, &&u8, int)

fn C.window_get_tree_selected(&WindowInfo, &u8) &u8

fn C.window_set_tree_selected(&WindowInfo, &u8, &u8)

fn C.window_tree_expand_all(&WindowInfo, &u8)

fn C.window_tree_collapse_all(&WindowInfo, &u8)

fn C.window_tree_expand_node(&WindowInfo, &u8, &u8, int)

fn C.window_tree_collapse_node(&WindowInfo, &u8, &u8, int)

// System Menu Bar/Tray App Mode
fn C.window_enable_status_bar(&WindowInfo, &u8)

fn C.window_show(&WindowInfo)

// Thread Safety Runner
fn C.window_run_on_main_thread(voidptr, voidptr)

fn C.window_run_on_main_thread_sync(voidptr, voidptr)

// New general-purpose controls
fn C.window_add_dropdown_control(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_add_segmented_control_custom(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_add_radio_group_control(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_add_switch_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_add_search_field_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_combo_box_control(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_add_level_indicator_control(&WindowInfo, &u8, int, int, int, int) voidptr

fn C.window_add_spinner_control(&WindowInfo, &u8, int) voidptr

fn C.window_add_path_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_token_field_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_console_control(&WindowInfo, &u8, int) voidptr

fn C.window_append_console_text(&WindowInfo, &u8, &u8, int)

fn C.window_clear_console(&WindowInfo, &u8)

fn C.window_add_chart_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_set_chart_data(&WindowInfo, &u8, &f64, int)

fn C.window_add_shortcut_recorder_control(&WindowInfo, &u8) voidptr

fn C.window_add_circular_progress_control(&WindowInfo, &u8, f64, f64, f64) voidptr

fn C.window_set_circular_progress_value(&WindowInfo, &u8, f64)

fn C.window_add_breadcrumbs_control(&WindowInfo, &u8, &&u8, int) voidptr

fn C.window_set_breadcrumbs(&WindowInfo, &u8, &&u8, int)

fn C.window_add_property_grid_control(&WindowInfo, &u8, &&u8, &&u8, int) voidptr

fn C.window_set_property_grid_value(&WindowInfo, &u8, &u8, &u8)

fn C.window_add_color_grid_control(&WindowInfo, &u8, &&u8, int) voidptr

fn C.window_set_color_grid_selected(&WindowInfo, &u8, &u8)

fn C.window_add_grid_control(&WindowInfo, &u8, &&u8, int) voidptr

fn C.window_grid_add_row(&WindowInfo, &u8, &&u8, int)

fn C.window_grid_delete_row(&WindowInfo, &u8, int)

fn C.window_grid_add_column(&WindowInfo, &u8, &u8)

fn C.window_grid_delete_column(&WindowInfo, &u8, int)

fn C.window_grid_set_cell(&WindowInfo, &u8, int, int, &u8)

fn C.window_grid_get_cell(&WindowInfo, &u8, int, int) &u8

fn C.window_grid_get_selected_row(&WindowInfo, &u8) int

fn C.window_grid_get_column_editable(&WindowInfo, &u8, int) int

fn C.window_grid_get_row_editable(&WindowInfo, &u8, int) int

fn C.window_grid_get_cell_editable(&WindowInfo, &u8, int, int) int

fn C.window_grid_get_column_enabled(&WindowInfo, &u8, int) int

fn C.window_grid_get_row_enabled(&WindowInfo, &u8, int) int

fn C.window_grid_get_cell_enabled(&WindowInfo, &u8, int, int) int

fn C.window_grid_get_filter(&WindowInfo, &u8) &u8

fn C.window_grid_get_row_count(&WindowInfo, &u8) int

fn C.window_grid_get_column_count(&WindowInfo, &u8) int

fn C.window_grid_set_column_type(&WindowInfo, &u8, int, &u8)

fn C.window_grid_set_column_width(&WindowInfo, &u8, int, int)

fn C.window_grid_set_row_height(&WindowInfo, &u8, int)

fn C.window_grid_sort_by_column(&WindowInfo, &u8, int, int)

fn C.window_grid_get_selected_column(&WindowInfo, &u8) int

fn C.window_grid_set_selected_column(&WindowInfo, &u8, int)

fn C.window_grid_set_selected_cell(&WindowInfo, &u8, int, int)

fn C.window_grid_set_filter(&WindowInfo, &u8, &u8)

fn C.window_grid_clear_filter(&WindowInfo, &u8)

fn C.window_grid_autosize_columns(&WindowInfo, &u8)

fn C.window_grid_set_selected_row(&WindowInfo, &u8, int)

fn C.window_grid_clear(&WindowInfo, &u8)

fn C.window_grid_set_column_editable(&WindowInfo, &u8, int, int)

fn C.window_grid_set_row_editable(&WindowInfo, &u8, int, int)

fn C.window_grid_set_cell_editable(&WindowInfo, &u8, int, int, int)

fn C.window_grid_set_column_enabled(&WindowInfo, &u8, int, int)

fn C.window_grid_set_row_enabled(&WindowInfo, &u8, int, int)

fn C.window_grid_set_cell_enabled(&WindowInfo, &u8, int, int, int)

// Window constraints and behavior options
fn C.window_set_min_size(&WindowInfo, int, int)

fn C.window_set_max_size(&WindowInfo, int, int)

fn C.window_set_resizable(&WindowInfo, int)

fn C.window_set_minimizable(&WindowInfo, int)

fn C.window_set_maximizable(&WindowInfo, int)

fn C.window_set_closable(&WindowInfo, int)

fn C.window_get_closable(&WindowInfo) int

fn C.window_set_has_shadow(&WindowInfo, int)

fn C.window_get_has_shadow(&WindowInfo) int

fn C.window_set_movable_by_window_background(&WindowInfo, int)

fn C.window_get_movable_by_window_background(&WindowInfo) int

fn C.window_is_visible(&WindowInfo) int

fn C.window_set_title_visible(&WindowInfo, int)

fn C.window_get_title_visible(&WindowInfo) int

fn C.window_get_titlebar_visible(&WindowInfo) int

// Additional Window Operations
fn C.window_close(&WindowInfo)

fn C.window_hide(&WindowInfo)

fn C.window_center(&WindowInfo)

fn C.window_align(&WindowInfo, &u8)

fn C.window_set_size(&WindowInfo, int, int)

fn C.window_get_width(&WindowInfo) int

fn C.window_get_height(&WindowInfo) int

fn C.window_set_position(&WindowInfo, int, int)

fn C.window_get_x(&WindowInfo) int

fn C.window_get_y(&WindowInfo) int

fn C.window_set_opacity(&WindowInfo, f64)

fn C.window_get_opacity(&WindowInfo) f64

fn C.window_toggle_fullscreen(&WindowInfo)

fn C.window_minimize(&WindowInfo)

fn C.window_deminimize(&WindowInfo)

fn C.window_maximize(&WindowInfo)

fn C.window_is_minimized(&WindowInfo) int

fn C.window_is_maximized(&WindowInfo) int

fn C.window_is_fullscreen(&WindowInfo) int

fn C.window_is_active(&WindowInfo) int

fn C.window_set_titlebar_visible(&WindowInfo, int)

fn C.window_request_attention(&WindowInfo, int)

fn C.window_deliver_notification(&u8, &u8)

fn C.window_set_dock_badge(&u8)

fn C.window_set_slider_range(&WindowInfo, &u8, f64, f64)

fn C.window_add_link_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_beep()

fn C.window_add_disclosure_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_enable_search_history(&WindowInfo, &u8, &u8)

fn C.window_add_stepper_control(&WindowInfo, &u8, f64, f64, f64, f64) voidptr

fn C.window_add_help_button_control(&WindowInfo, &u8) voidptr

fn C.window_add_knob_control(&WindowInfo, &u8, f64, f64, f64) voidptr

fn C.window_add_pull_down_control(&WindowInfo, &u8, &u8, &&u8, int) voidptr

fn C.window_add_image_button_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_set_status_bar_icon(&WindowInfo, &u8)

fn C.window_set_status_bar_title(&WindowInfo, &u8)

fn C.window_set_dock_icon(&u8)

fn C.window_play_system_sound(&u8)

// Animations and Transition Helpers C declarations
fn C.window_animate_control_opacity(&WindowInfo, &u8, f64, int)

fn C.window_animate_opacity(&WindowInfo, f64, int)

fn C.window_animate_control_shake(&WindowInfo, &u8)

fn C.window_shake(&WindowInfo)

fn C.window_animate_control_width(&WindowInfo, &u8, int, int)

fn C.window_animate_control_height(&WindowInfo, &u8, int, int)

fn C.window_animate_control_size(&WindowInfo, &u8, int, int, int)

fn C.window_animate_size(&WindowInfo, int, int, int)

fn C.window_animate_position(&WindowInfo, int, int, int)

fn C.window_animate_bounds(&WindowInfo, int, int, int, int, int)

fn C.window_add_toolbar_item(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_add_toolbar_space(&WindowInfo)

fn C.window_add_toolbar_flexible_space(&WindowInfo)

fn C.window_set_toolbar_style(&WindowInfo, &u8)

fn C.window_show_sheet_alert(&WindowInfo, &u8, &u8, &u8)

fn C.window_add_dock_menu_item(&WindowInfo, &u8, &u8)

// New controls C declarations
fn C.window_begin_split_view(&WindowInfo, &u8, int)

fn C.window_split_view_next_pane(&WindowInfo)

fn C.window_end_split_view(&WindowInfo)

fn C.window_add_collection_view_control(&WindowInfo, &u8, int, int) voidptr

fn C.window_set_collection_items(&WindowInfo, &u8, &&u8, int)

fn C.window_show_popover(&WindowInfo, &u8, &u8, &u8)

fn C.window_add_calendar_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_add_canvas_control(&WindowInfo, &u8, int) voidptr

fn C.window_draw_line(&WindowInfo, &u8, f64, f64, f64, f64, &u8, f64)

fn C.window_draw_rect(&WindowInfo, &u8, f64, f64, f64, f64, &u8, int, f64)

fn C.window_draw_circle(&WindowInfo, &u8, f64, f64, f64, &u8, int, f64)

fn C.window_clear_canvas(&WindowInfo, &u8)

// Glass, Badge, Icon Segment C declarations
fn C.window_begin_glass_box(&WindowInfo, &u8, &u8)

fn C.window_end_glass_box(&WindowInfo)

fn C.window_add_badge_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_add_icon_segments_control(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_add_stat_card_control(&WindowInfo, &u8, &u8, &u8, &u8, &u8) voidptr

fn C.window_set_stat_card_value(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_add_banner_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_add_section_header_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_add_vertical_slider_control(&WindowInfo, &u8, int, int, int, int) voidptr

fn C.window_add_chip_group_control(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_set_banner_value(&WindowInfo, &u8, &u8)

fn C.window_set_vertical_slider_value(&WindowInfo, &u8, int)

fn C.window_set_chip_group_selected(&WindowInfo, &u8, &u8)

fn C.window_set_badge_value(&WindowInfo, &u8, &u8, &u8)

fn C.window_add_status_indicator_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_set_status_indicator_value(&WindowInfo, &u8, &u8)

fn C.window_add_metric_meter_control(&WindowInfo, &u8, &u8, int, int, int, &u8) voidptr

fn C.window_set_metric_meter_value(&WindowInfo, &u8, int)

fn C.window_add_avatar_card_control(&WindowInfo, &u8, &u8, &u8, &u8) voidptr

fn C.window_set_avatar_card_value(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_add_time_picker_control(&WindowInfo, &u8, &u8) voidptr

fn C.window_set_time_picker_value(&WindowInfo, &u8, &u8)

fn C.window_get_time_picker_value(&WindowInfo, &u8) &u8

fn C.window_add_tray_icon_control(&WindowInfo, &u8, &u8, &u8)

fn C.window_set_tray_icon_value(&WindowInfo, &u8, &u8, &u8)

fn C.window_add_collapsible_section_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_set_collapsible_section_expanded(&WindowInfo, &u8, int)

fn C.window_add_code_editor_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_set_code_editor_value(&WindowInfo, &u8, &u8)

fn C.window_get_code_editor_value(&WindowInfo, &u8) &u8

fn C.window_add_timeline_view_control(&WindowInfo, &u8, int) voidptr

fn C.window_add_timeline_entry(&WindowInfo, &u8, &u8, &u8, &u8, &u8)

fn C.window_clear_timeline(&WindowInfo, &u8)

fn C.window_add_toolbar_button(&WindowInfo, &u8, &u8, &u8)

fn C.window_set_toolbar_visible(&WindowInfo, int)

fn C.window_grid_get_row_values(&WindowInfo, &u8, int, &&u8, int) int

fn C.window_set_checkbox_state(&WindowInfo, int)

fn C.window_set_input_text(&WindowInfo, &u8)

fn C.window_set_number_value(&WindowInfo, int)

fn C.window_set_text_area(&WindowInfo, &u8)

fn C.window_set_subtitle(&WindowInfo, &u8)

fn C.window_get_subtitle(&WindowInfo) &u8

fn C.window_set_titlebar_appears_transparent(&WindowInfo, int)

fn C.window_get_titlebar_appears_transparent(&WindowInfo) int

fn C.window_set_full_size_content_view(&WindowInfo, int)

fn C.window_get_full_size_content_view(&WindowInfo) int

fn C.window_set_movable(&WindowInfo, int)

fn C.window_get_movable(&WindowInfo) int

fn C.window_set_window_level(&WindowInfo, &u8)

fn C.window_set_aspect_ratio(&WindowInfo, f64, f64)

fn C.window_reset_aspect_ratio(&WindowInfo)

fn C.window_bounce_dock_icon(int)

fn C.window_set_vibrancy(&WindowInfo, &u8)

fn C.window_set_corner_radius(&WindowInfo, f64)

fn C.window_get_corner_radius(&WindowInfo) f64

fn C.window_set_background_blur(&WindowInfo, int)

fn C.window_flash_frame(&WindowInfo, int)

fn C.window_center_on_active_screen(&WindowInfo)

fn C.window_set_level_type(&WindowInfo, &u8)

fn C.window_get_window_level(&WindowInfo) &u8

fn C.window_set_fullscreen(&WindowInfo, int)

fn C.window_snap_to_edge(&WindowInfo, &u8)

fn C.window_set_bounds(&WindowInfo, int, int, int, int)

fn C.window_get_bounds(&WindowInfo, &int, &int, &int, &int)

fn C.window_has_aspect_ratio(&WindowInfo) int

fn C.window_set_ignores_mouse_events(&WindowInfo, int)

fn C.window_get_ignores_mouse_events(&WindowInfo) int

fn C.window_set_hides_on_deactivate(&WindowInfo, int)

fn C.window_get_hides_on_deactivate(&WindowInfo) int

fn C.window_set_prevents_app_termination(&WindowInfo, int)

fn C.window_get_prevents_app_termination(&WindowInfo) int

fn C.window_set_represented_filename(&WindowInfo, &u8)

fn C.window_get_represented_filename(&WindowInfo) &u8

fn C.window_set_frame_autosave_name(&WindowInfo, &u8)

fn C.window_get_frame_autosave_name(&WindowInfo) &u8

fn C.window_save_frame(&WindowInfo) int

fn C.window_restore_frame(&WindowInfo) int

fn C.window_save_geometry(&WindowInfo, &u8) int

fn C.window_restore_geometry(&WindowInfo, &u8) int

fn C.window_capture_screenshot(&WindowInfo, &u8) int

fn C.window_set_document_edited(&WindowInfo, int)

fn C.window_is_document_edited(&WindowInfo) int

fn C.window_fade_in(&WindowInfo, int)

fn C.window_fade_out(&WindowInfo, int)

fn C.window_order_front(&WindowInfo)

fn C.window_order_back(&WindowInfo)

fn C.window_add_rating_control(&WindowInfo, &u8, int, int) voidptr

fn C.window_set_rating_value(&WindowInfo, &u8, int)

fn C.window_get_rating_value(&WindowInfo, &u8) int

fn C.window_add_range_slider_control(&WindowInfo, &u8, int, int, int, int) voidptr

fn C.window_set_range_slider_values(&WindowInfo, &u8, int, int)

fn C.window_get_range_slider_low(&WindowInfo, &u8) int

fn C.window_get_range_slider_high(&WindowInfo, &u8) int

fn C.window_add_split_button_control(&WindowInfo, &u8, &u8, &&u8, int) voidptr

fn C.window_add_tag_cloud_control(&WindowInfo, &u8, &&u8, int) voidptr

fn C.window_set_tag_cloud_tags(&WindowInfo, &u8, &&u8, int)

fn C.window_add_wizard_stepper_control(&WindowInfo, &u8, &&u8, int, int) voidptr

fn C.window_set_wizard_stepper_step(&WindowInfo, &u8, int)

fn C.window_add_gauge_control(&WindowInfo, &u8, &u8, int, int, int, &u8) voidptr

fn C.window_set_gauge_value(&WindowInfo, &u8, int)

fn C.window_get_gauge_value(&WindowInfo, &u8) int

fn C.window_add_pagination_control(&WindowInfo, &u8, int, int) voidptr

fn C.window_set_pagination_page(&WindowInfo, &u8, int, int)

fn C.window_get_pagination_page(&WindowInfo, &u8) int

fn C.window_add_activity_feed_control(&WindowInfo, &u8, int) voidptr

fn C.window_add_activity_feed_item(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_clear_activity_feed(&WindowInfo, &u8)

fn C.window_add_markdown_view_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_set_markdown_view_text(&WindowInfo, &u8, &u8)

fn C.window_get_markdown_view_text(&WindowInfo, &u8) &u8

fn C.window_add_sparkline_control(&WindowInfo, &u8, &f64, int, int) voidptr

fn C.window_set_sparkline_data(&WindowInfo, &u8, &f64, int)

fn C.window_add_pin_code_control(&WindowInfo, &u8, int) voidptr

fn C.window_set_pin_code_value(&WindowInfo, &u8, &u8)

fn C.window_get_pin_code_value(&WindowInfo, &u8) &u8

fn C.window_add_color_palette_control(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_set_color_palette_selected(&WindowInfo, &u8, &u8)

fn C.window_get_color_palette_selected(&WindowInfo, &u8) &u8

fn C.window_add_timeline_control(&WindowInfo, &u8, int) voidptr

fn C.window_add_timeline_item(&WindowInfo, &u8, &u8, &u8, &u8, &u8)

fn C.window_add_metric_card_control(&WindowInfo, &u8, &u8, &u8, &u8, &u8) voidptr

fn C.window_set_metric_card_value(&WindowInfo, &u8, &u8, &u8)

fn C.window_add_tab_pills_control(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_set_tab_pills_active(&WindowInfo, &u8, &u8)

fn C.window_get_tab_pills_active(&WindowInfo, &u8) &u8

fn C.window_add_transfer_list_control(&WindowInfo, &u8, &&u8, int, &&u8, int, bool) voidptr

fn C.window_add_audio_waveform_control(&WindowInfo, &u8, &f64, int, int) voidptr

fn C.window_set_audio_waveform_data(&WindowInfo, &u8, &f64, int)

fn C.window_add_rating_breakdown_control(&WindowInfo, &u8, f64, int, &f64, int) voidptr

fn C.window_set_rating_breakdown_data(&WindowInfo, &u8, f64, int, &f64, int)

fn C.window_add_code_view_control(&WindowInfo, &u8, &u8, &u8, int) voidptr

fn C.window_set_code_view_text(&WindowInfo, &u8, &u8)

fn C.window_get_code_view_text(&WindowInfo, &u8) &u8

fn C.window_add_alert_banner_control(&WindowInfo, &u8, &u8, &u8, &u8) voidptr

fn C.window_set_alert_banner_value(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_add_step_tracker_control(&WindowInfo, &u8, &&u8, int, int) voidptr

fn C.window_set_step_tracker_step(&WindowInfo, &u8, int)

fn C.window_get_step_tracker_step(&WindowInfo, &u8) int

fn C.window_add_filter_chips_control(&WindowInfo, &u8, &&u8, int, &&u8, int, bool) voidptr

fn C.window_set_filter_chips_selected(&WindowInfo, &u8, &&u8, int)

fn C.window_get_filter_chips_selected(&WindowInfo, &u8) &u8

fn C.window_add_file_picker_field_control(&WindowInfo, &u8, &u8, &u8, bool) voidptr

fn C.window_set_file_picker_path(&WindowInfo, &u8, &u8)

fn C.window_get_file_picker_path(&WindowInfo, &u8) &u8

fn C.window_add_radial_gauge_control(&WindowInfo, &u8, &u8, f64, f64, f64, &u8) voidptr

fn C.window_set_radial_gauge_value(&WindowInfo, &u8, f64)

fn C.window_get_radial_gauge_value(&WindowInfo, &u8) f64

fn C.window_add_key_value_card_control(&WindowInfo, &u8, &u8, &&u8, &&u8, int) voidptr

fn C.window_set_key_value_card_data(&WindowInfo, &u8, &&u8, &&u8, int)

fn C.window_add_diff_view_control(&WindowInfo, &u8, &u8, &u8, int) voidptr

fn C.window_set_diff_view_text(&WindowInfo, &u8, &u8, &u8)

fn C.window_add_json_tree_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_set_json_tree_data(&WindowInfo, &u8, &u8)

fn C.window_add_http_request_card_control(&WindowInfo, &u8, &u8, &u8, int, int) voidptr

fn C.window_set_http_request_card_data(&WindowInfo, &u8, &u8, &u8, int, int)

fn C.window_add_terminal_view_control(&WindowInfo, &u8, &u8, int) voidptr

fn C.window_append_terminal_line(&WindowInfo, &u8, &u8, int)

fn C.window_clear_terminal(&WindowInfo, &u8)

fn C.window_add_resource_monitor_control(&WindowInfo, &u8, int, int, int, int) voidptr

fn C.window_set_resource_monitor_metrics(&WindowInfo, &u8, int, int, int, int)

fn C.window_add_env_vars_control(&WindowInfo, &u8, &u8, &&u8, &&u8, int) voidptr

fn C.window_set_env_vars_data(&WindowInfo, &u8, &&u8, &&u8, int)

fn C.window_add_badge_button_control(&WindowInfo, &u8, &u8, int, &u8) voidptr

fn C.window_set_badge_button_count(&WindowInfo, &u8, int)

fn C.window_add_command_palette_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_set_command_palette_text(&WindowInfo, &u8, &u8)

fn C.window_add_status_banner_control(&WindowInfo, &u8, &u8, &u8, &u8) voidptr

fn C.window_set_status_banner_text(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_add_pill_toggle_control(&WindowInfo, &u8, &&u8, int, int) voidptr

fn C.window_set_pill_toggle_selected(&WindowInfo, &u8, int)

fn C.window_add_color_swatch_panel_control(&WindowInfo, &u8, &&u8, int, &u8) voidptr

fn C.window_set_color_swatch_selected(&WindowInfo, &u8, &u8)

fn C.window_add_hotkey_badge_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_set_hotkey_badge_shortcut(&WindowInfo, &u8, &u8, &u8)

// 6 New UI Controls C declarations
fn C.window_add_quick_action_bar_control(&WindowInfo, &u8, &&u8, &&u8, int) voidptr

fn C.window_set_quick_action_enabled(&WindowInfo, &u8, int, int)

fn C.window_add_accordion_group_control(&WindowInfo, &u8, &&u8, int, int) voidptr

fn C.window_set_accordion_expanded(&WindowInfo, &u8, int, int)

fn C.window_add_segment_distribution_bar_control(&WindowInfo, &u8, &&u8, &f64, &&u8, int, int) voidptr

fn C.window_set_segment_distribution_values(&WindowInfo, &u8, &f64, int)

fn C.window_add_tag_input_field_control(&WindowInfo, &u8, &&u8, int) voidptr

fn C.window_set_tag_input_tags(&WindowInfo, &u8, &&u8, int)

fn C.window_get_tag_input_tags(&WindowInfo, &u8) &u8

fn C.window_add_status_dock_control(&WindowInfo, &u8, &u8, &u8, &u8) voidptr

fn C.window_set_status_dock_info(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_add_info_callout_control(&WindowInfo, &u8, &u8, &u8, &u8, &u8) voidptr

fn C.window_set_info_callout_text(&WindowInfo, &u8, &u8, &u8)

fn C.window_show_share_sheet(&WindowInfo, &&u8, int, &u8)

fn C.window_show_font_picker(&WindowInfo, &u8)

fn C.window_preview_file(&WindowInfo, &u8)

fn C.window_add_browser_control(&WindowInfo, &u8, int) voidptr

fn C.window_set_browser_column_items(&WindowInfo, &u8, int, &&u8, int)

fn C.window_get_browser_selected_row(&WindowInfo, &u8, int) int

fn C.window_get_browser_path(&WindowInfo, &u8) &u8

fn C.window_add_hero_banner_control(&WindowInfo, &u8, &u8, &u8, &u8, &u8) voidptr

fn C.window_add_activity_rings_control(&WindowInfo, &u8, &f64, &&u8, int, int) voidptr

fn C.window_set_activity_rings_values(&WindowInfo, &u8, &f64, int)

fn C.window_add_segmented_progress_control(&WindowInfo, &u8, &&u8, &f64, &&u8, int, int) voidptr

fn C.window_add_feedback_mood_control(&WindowInfo, &u8, int) voidptr

fn C.window_set_feedback_mood(&WindowInfo, &u8, int)

fn C.window_get_feedback_mood(&WindowInfo, &u8) int

fn C.window_add_kanban_board_control(&WindowInfo, &u8, &&u8, int, int) voidptr

fn C.window_kanban_add_card(&WindowInfo, &u8, int, &u8, &u8, &u8)

fn C.window_clear_kanban_board(&WindowInfo, &u8)

fn C.window_add_date_range_picker_control(&WindowInfo, &u8, &u8, &u8) voidptr

fn C.window_get_date_range_start(&WindowInfo, &u8) &u8

fn C.window_get_date_range_end(&WindowInfo, &u8) &u8

fn C.window_set_date_range(&WindowInfo, &u8, &u8, &u8)

fn C.window_add_stat_grid_control(&WindowInfo, &u8, &&u8, &&u8, &&u8, &&u8, int) voidptr

fn C.window_add_donut_chart_control(&WindowInfo, &u8, &u8, f64) voidptr
fn C.window_set_donut_percentage(&WindowInfo, &u8, f64)

fn C.window_add_code_studio_control(&WindowInfo, &u8, &u8, &u8, &u8) voidptr
fn C.window_set_code_studio(&WindowInfo, &u8, &u8, &u8, &u8)

fn C.window_add_score_card_control(&WindowInfo, &u8, &u8, f64, int, &f64, int) voidptr

fn C.window_add_floating_toolbar_control(&WindowInfo, &u8, &u8, &&u8, int) voidptr

fn C.window_add_user_profile_card_control(&WindowInfo, &u8, &u8, &u8, &u8, &u8, &u8, int, &u8) voidptr
fn C.window_set_user_online_status(&WindowInfo, &u8, int)

fn C.window_add_product_card_control(&WindowInfo, &u8, &u8, &u8, &u8, &u8, &u8, &u8) voidptr

fn C.window_add_image_gallery_control(&WindowInfo, &u8, &&u8, &&u8, int, int) voidptr
fn C.window_next_gallery_image(&WindowInfo, &u8)
fn C.window_prev_gallery_image(&WindowInfo, &u8)
fn C.window_set_gallery_index(&WindowInfo, &u8, int)
fn C.window_get_gallery_index(&WindowInfo, &u8) int

fn C.window_add_app_launcher_tile_control(&WindowInfo, &u8, &u8, &u8, &u8, &u8) voidptr

fn C.window_add_media_player_control(&WindowInfo, &u8, &u8, &u8, &u8, int, int, int) voidptr
fn C.window_set_media_player_audio_file(&WindowInfo, &u8, &u8)
fn C.window_toggle_media_player(&WindowInfo, &u8)
fn C.window_set_media_player_progress(&WindowInfo, &u8, int)
fn C.window_get_media_player_playing(&WindowInfo, &u8) int

fn C.window_add_activity_heatmap_control(&WindowInfo, &u8, &u8, int, &int, int, int) voidptr

fn C.window_add_masked_input_control(&WindowInfo, &u8, &u8, &u8) voidptr
fn C.window_get_masked_input_value(&WindowInfo, &u8) &u8
fn C.window_set_masked_input_value(&WindowInfo, &u8, &u8)

fn C.window_add_inline_editable_label_control(&WindowInfo, &u8, &u8) voidptr
fn C.window_get_inline_editable_label(&WindowInfo, &u8) &u8
fn C.window_set_inline_editable_label(&WindowInfo, &u8, &u8)

fn C.window_add_nav_rail_control(&WindowInfo, &u8, &&u8, &&u8, &&u8, &&u8, &int, int) voidptr
fn C.window_get_nav_rail_selected(&WindowInfo, &u8) &u8
fn C.window_set_nav_rail_selected(&WindowInfo, &u8, &u8)


fn C.window_set_alpha(&WindowInfo, f64)

fn C.window_get_alpha(&WindowInfo) f64

fn C.window_get_min_size(&WindowInfo, &int, &int)

fn C.window_get_max_size(&WindowInfo, &int, &int)

fn C.window_set_collection_behavior(&WindowInfo, &u8)

fn C.window_set_close_button_enabled(&WindowInfo, int)

fn C.window_set_minimize_button_enabled(&WindowInfo, int)

fn C.window_set_zoom_button_enabled(&WindowInfo, int)

fn C.window_set_content_insets(&WindowInfo, int, int, int, int)

fn C.window_set_tabbing_mode(&WindowInfo, &u8)

fn C.window_get_tabbing_mode(&WindowInfo) &char

fn C.window_set_tabbing_identifier(&WindowInfo, &u8)

fn C.window_get_tabbing_identifier(&WindowInfo) &char

fn C.window_toggle_tab_bar(&WindowInfo)

fn C.window_select_next_tab(&WindowInfo)

fn C.window_select_previous_tab(&WindowInfo)

fn C.window_set_sharing_type(&WindowInfo, &u8)
fn C.window_get_sharing_type(&WindowInfo) int

// Appearance Override
fn C.window_set_window_appearance(&WindowInfo, &u8)

fn C.window_get_window_appearance(&WindowInfo) &char

fn C.window_is_system_dark_mode(&WindowInfo) int

// Screen Info
fn C.window_get_screen_frame(&WindowInfo, &int, &int, &int, &int)

fn C.window_get_screen_full_frame(&WindowInfo, &int, &int, &int, &int)

fn C.window_get_screen_scale_factor(&WindowInfo) f64

// Cursor Control
fn C.window_set_cursor_hidden(&WindowInfo, int)

fn C.window_set_cursor(&WindowInfo, &u8)

fn C.window_get_cursor(&WindowInfo) &u8

fn C.window_set_cursor_scale(&WindowInfo, f64)

fn C.window_get_cursor_scale(&WindowInfo) f64

fn C.window_reset_cursor(&WindowInfo)

fn C.window_push_cursor(&WindowInfo, &u8)

fn C.window_pop_cursor(&WindowInfo)

fn C.window_set_control_cursor_by_name(&WindowInfo, &u8, &u8)

fn C.window_get_mouse_location(&WindowInfo, &int, &int)

fn C.window_move_cursor_to(&WindowInfo, int, int)

// Resize Indicator
fn C.window_set_shows_resize_indicator(&WindowInfo, int)

fn C.window_get_shows_resize_indicator(&WindowInfo) int

// Content Size Constraints
fn C.window_set_content_min_size(&WindowInfo, int, int)

fn C.window_set_content_max_size(&WindowInfo, int, int)

fn C.window_get_content_min_size(&WindowInfo, &int, &int)

fn C.window_get_content_max_size(&WindowInfo, &int, &int)

fn C.window_get_tab_count(&WindowInfo) int

fn C.window_set_split_position(&WindowInfo, &u8, int, f64)
fn C.window_set_split_divider_style(&WindowInfo, &u8, &u8)
fn C.window_set_slider_tick_marks(&WindowInfo, &u8, int, int)
fn C.window_set_progress_indeterminate(&WindowInfo, &u8, int)
fn C.window_start_progress_animation(&WindowInfo, &u8)
fn C.window_stop_progress_animation(&WindowInfo, &u8)
fn C.window_select_tab(&WindowInfo, &u8, int)
fn C.window_select_tab_by_title(&WindowInfo, &u8, &u8)
fn C.window_get_tab_index(&WindowInfo, &u8) int
fn C.window_set_search_recent_searches_key(&WindowInfo, &u8, &u8)
fn C.window_clear_search_history(&WindowInfo, &u8)
fn C.window_set_path_control_path(&WindowInfo, &u8, &u8)
fn C.window_get_path_control_path(&WindowInfo, &u8) &char
fn C.window_set_path_control_style(&WindowInfo, &u8, &u8)
fn C.window_set_token_field_tokens(&WindowInfo, &u8, &&u8, int)
fn C.window_get_token_field_tokens(&WindowInfo, &u8) &char
fn C.window_textarea_insert_text(&WindowInfo, &u8, &u8)
fn C.window_textarea_get_selected_text(&WindowInfo, &u8) &char
fn C.window_textarea_set_selected_range(&WindowInfo, &u8, int, int)
fn C.window_textarea_scroll_to_end(&WindowInfo, &u8)
fn C.window_textarea_clear(&WindowInfo, &u8)
fn C.window_scroll_to_top(&WindowInfo, &u8)
fn C.window_scroll_to_bottom(&WindowInfo, &u8)
fn C.window_show_color_picker(&WindowInfo, &u8) &char
fn C.window_select_multiple_files(&WindowInfo, &u8) &char
fn C.window_save_file_picker_with_name(&WindowInfo, &u8, &u8) &char
fn C.window_speech_speak(&u8, &u8)
fn C.window_speech_stop()
fn C.window_speech_is_speaking() int
fn C.window_speech_get_voices() &char
fn C.window_speech_get_default_voice() &char
fn C.window_perform_haptic_feedback(&u8)
fn C.window_clipboard_copy_image(&u8) int
fn C.window_clipboard_get_image(&u8) int
fn C.window_clipboard_copy_files(&&u8, int) int
fn C.window_clipboard_get_files() &char
fn C.window_clipboard_clear()
fn C.window_clipboard_get_change_count() int
fn C.window_recycle_to_trash(&u8) int
fn C.window_get_running_apps() &char
fn C.window_activate_app(&u8) int
fn C.window_terminate_app(int) int
fn C.window_hide_other_apps()
fn C.window_unhide_all_apps()
fn C.window_get_frontmost_app() &char
fn C.window_open_with_app(&u8, &u8) int
fn C.window_get_default_app_for_extension(&u8) &char
fn C.window_play_sound_file(&u8) int
fn C.window_stop_sound_file()
fn C.window_is_sound_file_playing() int
fn C.window_prevent_sleep(&u8) u32
fn C.window_allow_sleep(u32)
fn C.window_get_battery_percentage() f64
fn C.window_is_battery_charging() int
fn C.window_is_on_battery_power() int
fn C.window_get_battery_time_remaining() int
fn C.window_defaults_set_string(&u8, &u8)
fn C.window_defaults_get_string(&u8) &char
fn C.window_defaults_set_bool(&u8, int)
fn C.window_defaults_get_bool(&u8) int
fn C.window_defaults_set_int(&u8, int)
fn C.window_defaults_get_int(&u8) int
fn C.window_defaults_remove(&u8)
fn C.window_defaults_has(&u8) int
fn C.window_get_macos_version_str() &char
fn C.window_get_macos_version_numbers(&int, &int, &int)
fn C.window_get_computer_name() &char
fn C.window_get_user_full_name() &char
fn C.window_is_apple_silicon() int
fn C.window_get_system_uptime() f64
fn C.window_is_low_power_mode() int
fn C.window_get_screens_info() &char
fn C.window_move_to_screen(&WindowInfo, int)
fn C.window_get_dock_badge() &char
fn C.window_cancel_dock_bounce()
fn C.window_set_activation_policy(&u8)
fn C.window_get_activation_policy() &char
fn C.window_get_app_bundle_path() &char

// new_simple_window creates and initializes a new native SimpleWindow instance with the specified title, width, and height.
pub fn new_simple_window(title string, width int, height int) &SimpleWindow {
	mut win := &SimpleWindow{
		width:                        width
		height:                       height
		title:                        title
		responsive_layout:            true
		resizable:                    true
		minimizable:                  true
		maximizable:                  true
		closable:                     true
		has_shadow:                   true
		movable_by_window_background: false
		titlebar_visible:             true
		title_visible:                true
		movable:                      true
		prevents_app_termination:     true
	}
	win.placeholders = map[string]string{}
	win.tooltips = map[string]string{}
	win.errors = map[string]string{}
	win.tree_nodes = map[string][]TreeNode{}
	win.table_rows = map[string][][]string{}
	win.table_columns = map[string][]string{}
	win.table_selected_columns = map[string]int{}
	win.table_column_selection = map[string]bool{}
	win.grid_rows = map[string][][]string{}
	win.grid_headers = map[string][]string{}
	win.state_store = map[string]string{}
	win.state_listeners = map[string][]StringEventCallback{}
	win.auto_save_state = true
	win.theme = get_theme(get_saved_theme())
	win.ensure_window()
	sys_register_window(win)
	return win
}

// ensure_window initializes the native Cocoa window backend if not already created.
fn (win &SimpleWindow) ensure_window() {
	if win.window_info == unsafe { nil } {
		params := WindowParams{
			title:                        win.title.str
			width:                        win.width
			height:                       win.height
			win_ptr:                      win
			padding:                      win.padding
			spacing:                      win.spacing
			always_on_top:                if win.always_on_top { 1 } else { 0 }
			responsive_layout:            if win.responsive_layout { 1 } else { 0 }
			resizable:                    if win.resizable { 1 } else { 0 }
			minimizable:                  if win.minimizable { 1 } else { 0 }
			maximizable:                  if win.maximizable { 1 } else { 0 }
			closable:                     if win.closable { 1 } else { 0 }
			has_shadow:                   if win.has_shadow { 1 } else { 0 }
			movable_by_window_background: if win.movable_by_window_background { 1 } else { 0 }
			titlebar_visible:             if win.titlebar_visible { 1 } else { 0 }
			title_visible:                if win.title_visible { 1 } else { 0 }
		}
		unsafe {
			mut w := &SimpleWindow(win)
			w.window_info = C.window_app_init(&params)
		}
	}
}

// normalize_key_shortcut converts key shortcut strings into canonical form ("cmd+shift+p").
pub fn normalize_key_shortcut(input string) string {
	if input == '' {
		return ''
	}
	mut s := input.to_lower()
	s = s.replace('⌘', 'cmd+')
	s = s.replace('⇧', 'shift+')
	s = s.replace('⌥', 'opt+')
	s = s.replace('⌃', 'ctrl+')
	s = s.replace('command', 'cmd')
	s = s.replace('control', 'ctrl')
	s = s.replace('option', 'opt')
	s = s.replace('alt', 'opt')
	s = s.replace(' ', '')

	parts := s.split('+')
	mut has_cmd := false
	mut has_ctrl := false
	mut has_opt := false
	mut has_shift := false
	mut key_parts := []string{}

	for part in parts {
		match part {
			'cmd', 'meta' { has_cmd = true }
			'ctrl' { has_ctrl = true }
			'opt' { has_opt = true }
			'shift' { has_shift = true }
			'' {}
			else { key_parts << part }
		}
	}

	mut res := []string{}
	if has_cmd {
		res << 'cmd'
	}
	if has_ctrl {
		res << 'ctrl'
	}
	if has_opt {
		res << 'opt'
	}
	if has_shift {
		res << 'shift'
	}
	if key_parts.len > 0 {
		res << key_parts.join('+')
	}

	return res.join('+')
}

// textarea_goto_line scrolls the specified textarea control to the given line number and optionally focuses it.
pub fn (win &SimpleWindow) textarea_goto_line(name string, line_number int, focus bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		do_focus := if focus { 1 } else { 0 }
		C.window_textarea_goto_line(win.window_info, name.str, line_number, do_focus)
	}
	return win
}

// append_console appends formatted text log messages to a developer console widget.
pub fn (win &SimpleWindow) append_console(name string, text string, level int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_append_console_text(win.window_info, name.str, text.str, level)
	}
	return win
}

// clear_console clears all text in the console control.
pub fn (win &SimpleWindow) clear_console(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_clear_console(win.window_info, name.str)
	}
	return win
}

// add_chart adds a beautiful native line or area chart control.
// chart_type: "line" or "area"
pub fn (win &SimpleWindow) set_chart_data(name string, values []f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		if values.len > 0 {
			C.window_set_chart_data(win.window_info, name.str, values.data, values.len)
		}
	}
	return win
}

// add_shortcut_recorder adds a key combination recording control.
// When focused, the user can press a shortcut which triggers the "change" callback.
pub fn (win &SimpleWindow) set_circular_progress(name string, value int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_circular_progress_value(win.window_info, name.str, f64(value))
	}
	return win
}

// add_breadcrumbs adds a breadcrumb / path navigation control.
// The list of segments contains the path elements (e.g. ['Home', 'Projects', 'simplegui']).
// You can handle segment click events by registering an `on_click(name, callback)` event.
pub fn (win &SimpleWindow) set_breadcrumbs(name string, segments []string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		mut c_segs := []&u8{}
		for seg in segments {
			c_segs << seg.str
		}
		C.window_set_breadcrumbs(win.window_info, name.str, c_segs.data, c_segs.len)
	}
	return win
}

// add_property_grid adds a property inspector grid containing key-value rows.
pub fn (win &SimpleWindow) set_property_grid_value(name string, key string, value string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_property_grid_value(win.window_info, name.str, key.str, value.str)
	}
	return win
}

// add_color_grid adds an interactive grid of color swatches.
pub fn (win &SimpleWindow) set_color_grid_selected(name string, color string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_color_grid_selected(win.window_info, name.str, color.str)
	}
	return win
}

// add_grid adds a grid control with Excel-like editability and CRUD support.
pub fn (mut win SimpleWindow) grid_delete_row(name string, row_idx int) &SimpleWindow {
	mut rows := win.grid_rows[name]
	if row_idx >= 0 && row_idx < rows.len {
		rows.delete(row_idx)
		win.grid_rows[name] = rows
	}
	if win.window_info != unsafe { nil } {
		C.window_grid_delete_row(win.window_info, name.str, row_idx)
	}
	return &win
}

// grid_add_column appends a new column header.
pub fn (mut win SimpleWindow) grid_delete_column(name string, col_idx int) &SimpleWindow {
	mut rows := win.grid_rows[name]
	for i in 0 .. rows.len {
		if col_idx >= 0 && col_idx < rows[i].len {
			rows[i].delete(col_idx)
		}
	}
	win.grid_rows[name] = rows
	mut headers := win.grid_headers[name]
	if col_idx >= 0 && col_idx < headers.len {
		headers.delete(col_idx)
		win.grid_headers[name] = headers
	}
	if win.window_info != unsafe { nil } {
		C.window_grid_delete_column(win.window_info, name.str, col_idx)
	}
	return &win
}

// grid_set_cell sets the value of cell at row, col.
pub fn (mut win SimpleWindow) grid_set_cell(name string, row int, col int, val string) &SimpleWindow {
	mut rows := win.grid_rows[name]
	if row >= 0 && row < rows.len {
		mut cols := rows[row].clone()
		if col >= 0 && col < cols.len {
			cols[col] = val
			rows[row] = cols
			win.grid_rows[name] = rows
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_grid_set_cell(win.window_info, name.str, row, col, val.str)
	}
	return &win
}

// grid_get_cell returns the string value of cell at row, col.
pub fn (win &SimpleWindow) grid_get_cell(name string, row int, col int) string {
	if win.window_info != unsafe { nil } {
		res := C.window_grid_get_cell(win.window_info, name.str, row, col)
		if res != unsafe { nil } {
			return unsafe { tos_clone(res) }
		}
	}
	rows := win.grid_rows[name]
	if row >= 0 && row < rows.len {
		cols := rows[row]
		if col >= 0 && col < cols.len {
			return cols[col]
		}
	}
	return ''
}

// grid_get_selected_row returns the 0-indexed selected row index, or -1 if none is selected.
pub fn (win &SimpleWindow) grid_get_selected_row(name string) int {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_selected_row(win.window_info, name.str)
	}
	return -1
}

// grid_get_selected_column returns the 0-indexed selected column index, or -1 if none is selected.
pub fn (win &SimpleWindow) grid_get_selected_column(name string) int {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_selected_column(win.window_info, name.str)
	}
	return -1
}

// grid_get_selected_cell returns the currently selected row/column coordinates.
pub fn (win &SimpleWindow) grid_get_selected_cell(name string) (int, int) {
	return win.grid_get_selected_row(name), win.grid_get_selected_column(name)
}

// grid_get_rows returns the current grid data as a [][]string.
pub fn (win &SimpleWindow) grid_get_rows(name string) [][]string {
	return win.grid_rows[name]
}

// grid_set_rows replaces the entire grid data set.
pub fn (mut win SimpleWindow) grid_set_rows(name string, rows [][]string) &SimpleWindow {
	win.grid_rows[name] = rows.clone()
	if win.window_info != unsafe { nil } {
		C.window_grid_clear(win.window_info, name.str)
		for row in rows {
			mut c_vals := []&u8{}
			for val in row {
				c_vals << val.str
			}
			C.window_grid_add_row(win.window_info, name.str, c_vals.data, row.len)
		}
	}
	return &win
}

// grid_get_row returns the current values for a specific row.
pub fn (win &SimpleWindow) grid_get_row(name string, row_idx int) []string {
	rows := win.grid_rows[name]
	if row_idx >= 0 && row_idx < rows.len {
		return rows[row_idx]
	}
	return []string{}
}

// grid_set_row replaces the values for a specific row.
pub fn (mut win SimpleWindow) grid_set_row(name string, row_idx int, values []string) &SimpleWindow {
	mut rows := win.grid_rows[name]
	if row_idx >= 0 && row_idx < rows.len {
		rows[row_idx] = values.clone()
		win.grid_rows[name] = rows
		if win.window_info != unsafe { nil } {
			for idx, value in values {
				win.grid_set_cell(name, row_idx, idx, value)
			}
		}
	}
	return &win
}

// grid_get_column returns the current values for a specific column.
pub fn (win &SimpleWindow) grid_get_column(name string, col_idx int) []string {
	mut values := []string{}
	rows := win.grid_rows[name]
	for row in rows {
		if col_idx >= 0 && col_idx < row.len {
			values << row[col_idx]
		} else {
			values << ''
		}
	}
	return values
}

// grid_set_column replaces the values for a specific column.
pub fn (mut win SimpleWindow) grid_set_column(name string, col_idx int, values []string) &SimpleWindow {
	mut rows := win.grid_rows[name]
	for idx, value in values {
		if idx >= 0 && idx < rows.len {
			mut row_values := rows[idx].clone()
			if col_idx >= 0 && col_idx < row_values.len {
				row_values[col_idx] = value
				rows[idx] = row_values
			}
		}
	}
	win.grid_rows[name] = rows
	if win.window_info != unsafe { nil } {
		for idx, value in values {
			win.grid_set_cell(name, idx, col_idx, value)
		}
	}
	return &win
}

// grid_set_selected_column selects the given column programmatically.
pub fn (mut win SimpleWindow) grid_set_selected_column(name string, col_idx int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_selected_column(win.window_info, name.str, col_idx)
	}
	return &win
}

// grid_set_selected_cell selects the given row/column cell programmatically.
pub fn (mut win SimpleWindow) grid_set_selected_cell(name string, row_idx int, col_idx int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_selected_cell(win.window_info, name.str, row_idx, col_idx)
	}
	return &win
}

// grid_get_column_editable returns whether a column is editable.
pub fn (win &SimpleWindow) grid_get_column_editable(name string, col_idx int) bool {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_column_editable(win.window_info, name.str, col_idx) == 1
	}
	return false
}

// grid_get_columns_editable returns editability for a batch of columns.
pub fn (win &SimpleWindow) grid_get_columns_editable(name string, col_idxs []int) map[int]bool {
	mut result := map[int]bool{}
	for col_idx in col_idxs {
		result[col_idx] = win.grid_get_column_editable(name, col_idx)
	}
	return result
}

// grid_set_columns_editable updates editability for a batch of columns.
pub fn (win &SimpleWindow) grid_set_columns_editable(name string, col_idxs []int, editable bool) &SimpleWindow {
	for col_idx in col_idxs {
		win.grid_set_column_editable(name, col_idx, editable)
	}
	return win
}

// grid_get_row_editable returns whether a row is editable.
pub fn (win &SimpleWindow) grid_get_row_editable(name string, row_idx int) bool {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_row_editable(win.window_info, name.str, row_idx) == 1
	}
	return false
}

// grid_get_rows_editable returns editability for a batch of rows.
pub fn (win &SimpleWindow) grid_get_rows_editable(name string, row_idxs []int) map[int]bool {
	mut result := map[int]bool{}
	for row_idx in row_idxs {
		result[row_idx] = win.grid_get_row_editable(name, row_idx)
	}
	return result
}

// grid_set_rows_editable updates editability for a batch of rows.
pub fn (win &SimpleWindow) grid_set_rows_editable(name string, row_idxs []int, editable bool) &SimpleWindow {
	for row_idx in row_idxs {
		win.grid_set_row_editable(name, row_idx, editable)
	}
	return win
}

// grid_get_cell_editable returns whether a cell is editable.
pub fn (win &SimpleWindow) grid_get_cell_editable(name string, row int, col int) bool {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_cell_editable(win.window_info, name.str, row, col) == 1
	}
	return false
}

// grid_get_cells_editable returns editability for a batch of cells.
pub fn (win &SimpleWindow) grid_get_cells_editable(name string, cells []string) map[string]bool {
	mut result := map[string]bool{}
	for coord in cells {
		parts := coord.split('_')
		if parts.len == 2 {
			row_idx := parts[0].int()
			col_idx := parts[1].int()
			result[coord] = win.grid_get_cell_editable(name, row_idx, col_idx)
		}
	}
	return result
}

// grid_set_cells_editable updates editability for a batch of cells.
pub fn (win &SimpleWindow) grid_set_cells_editable(name string, cells []string, editable bool) &SimpleWindow {
	for coord in cells {
		parts := coord.split('_')
		if parts.len == 2 {
			row_idx := parts[0].int()
			col_idx := parts[1].int()
			win.grid_set_cell_editable(name, row_idx, col_idx, editable)
		}
	}
	return win
}

// grid_get_column_enabled returns whether a column is enabled.
pub fn (win &SimpleWindow) grid_get_column_enabled(name string, col_idx int) bool {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_column_enabled(win.window_info, name.str, col_idx) == 1
	}
	return false
}

// grid_get_columns_enabled returns enabled state for a batch of columns.
pub fn (win &SimpleWindow) grid_get_columns_enabled(name string, col_idxs []int) map[int]bool {
	mut result := map[int]bool{}
	for col_idx in col_idxs {
		result[col_idx] = win.grid_get_column_enabled(name, col_idx)
	}
	return result
}

// grid_set_columns_enabled updates enabled state for a batch of columns.
pub fn (win &SimpleWindow) grid_set_columns_enabled(name string, col_idxs []int, enabled bool) &SimpleWindow {
	for col_idx in col_idxs {
		win.grid_set_column_enabled(name, col_idx, enabled)
	}
	return win
}

// grid_get_row_enabled returns whether a row is enabled.
pub fn (win &SimpleWindow) grid_get_row_enabled(name string, row_idx int) bool {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_row_enabled(win.window_info, name.str, row_idx) == 1
	}
	return false
}

// grid_get_rows_enabled returns enabled state for a batch of rows.
pub fn (win &SimpleWindow) grid_get_rows_enabled(name string, row_idxs []int) map[int]bool {
	mut result := map[int]bool{}
	for row_idx in row_idxs {
		result[row_idx] = win.grid_get_row_enabled(name, row_idx)
	}
	return result
}

// grid_set_rows_enabled updates enabled state for a batch of rows.
pub fn (win &SimpleWindow) grid_set_rows_enabled(name string, row_idxs []int, enabled bool) &SimpleWindow {
	for row_idx in row_idxs {
		win.grid_set_row_enabled(name, row_idx, enabled)
	}
	return win
}

// grid_get_cell_enabled returns whether a cell is enabled.
pub fn (win &SimpleWindow) grid_get_cell_enabled(name string, row int, col int) bool {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_cell_enabled(win.window_info, name.str, row, col) == 1
	}
	return false
}

// grid_get_cells_enabled returns enabled state for a batch of cells.
pub fn (win &SimpleWindow) grid_get_cells_enabled(name string, cells []string) map[string]bool {
	mut result := map[string]bool{}
	for coord in cells {
		parts := coord.split('_')
		if parts.len == 2 {
			row_idx := parts[0].int()
			col_idx := parts[1].int()
			result[coord] = win.grid_get_cell_enabled(name, row_idx, col_idx)
		}
	}
	return result
}

// grid_set_cells_enabled updates enabled state for a batch of cells.
pub fn (win &SimpleWindow) grid_set_cells_enabled(name string, cells []string, enabled bool) &SimpleWindow {
	for coord in cells {
		parts := coord.split('_')
		if parts.len == 2 {
			row_idx := parts[0].int()
			col_idx := parts[1].int()
			win.grid_set_cell_enabled(name, row_idx, col_idx, enabled)
		}
	}
	return win
}

// grid_get_filter returns the active filter text for a grid.
pub fn (win &SimpleWindow) grid_get_filter(name string) string {
	if win.window_info != unsafe { nil } {
		res := C.window_grid_get_filter(win.window_info, name.str)
		if res != unsafe { nil } {
			return unsafe { tos_clone(res) }
		}
	}
	return ''
}

// grid_get_row_count returns the current number of rows in a grid.
pub fn (win &SimpleWindow) grid_get_row_count(name string) int {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_row_count(win.window_info, name.str)
	}
	return (win.grid_rows[name] or { [][]string{} }).len
}

// grid_get_column_count returns the current number of columns in a grid.
pub fn (win &SimpleWindow) grid_get_column_count(name string) int {
	if win.window_info != unsafe { nil } {
		return C.window_grid_get_column_count(win.window_info, name.str)
	}
	if headers := win.grid_headers[name] {
		if headers.len > 0 {
			return headers.len
		}
	}
	mut max_len := 0
	for row in win.grid_rows[name] or { [][]string{} } {
		if row.len > max_len {
			max_len = row.len
		}
	}
	return max_len
}

// grid_get_row_values returns the current values for a row as a []string.
pub fn (win &SimpleWindow) grid_get_row_values(name string, row_idx int) []string {
	mut values := []string{}
	if row_idx >= 0 {
		col_count := win.grid_get_column_count(name)
		for col_idx in 0 .. col_count {
			values << win.grid_get_cell(name, row_idx, col_idx)
		}
	}
	return values
}

// grid_get_column_values returns the current values for a column as a []string.
pub fn (win &SimpleWindow) grid_get_column_values(name string, col_idx int) []string {
	mut values := []string{}
	if col_idx >= 0 {
		row_count := win.grid_get_row_count(name)
		for row_idx in 0 .. row_count {
			values << win.grid_get_cell(name, row_idx, col_idx)
		}
	}
	return values
}

// grid_set_row_values updates every cell in a row from a []string.
pub fn (mut win SimpleWindow) grid_set_row_values(name string, row_idx int, values []string) &SimpleWindow {
	if row_idx >= 0 {
		for idx, value in values {
			win.grid_set_cell(name, row_idx, idx, value)
		}
	}
	return &win
}

// grid_set_column_values updates every cell in a column from a []string.
pub fn (mut win SimpleWindow) grid_set_column_values(name string, col_idx int, values []string) &SimpleWindow {
	if col_idx >= 0 {
		for idx, value in values {
			win.grid_set_cell(name, idx, col_idx, value)
		}
	}
	return &win
}

// grid_set_column_type sets the type of a column (e.g. 'text' or 'checkbox').
pub fn (win &SimpleWindow) grid_set_column_width(name string, col_idx int, width int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_column_width(win.window_info, name.str, col_idx, width)
	}
	return win
}

// grid_set_row_height resizes all rows to a fixed height.
pub fn (win &SimpleWindow) grid_set_row_height(name string, height int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_row_height(win.window_info, name.str, height)
	}
	return win
}

// grid_sort_by_column sorts the grid rows by the given column using the current sort direction.
pub fn (win &SimpleWindow) grid_sort_by_column(name string, col_idx int, ascending bool) &SimpleWindow {
	mut rows := win.grid_rows[name]
	if col_idx >= 0 && rows.len > 1 {
		for i in 0 .. rows.len {
			for j in i + 1 .. rows.len {
				left := if col_idx < rows[i].len { rows[i][col_idx].to_lower() } else { '' }
				right := if col_idx < rows[j].len { rows[j][col_idx].to_lower() } else { '' }
				if (ascending && left > right) || (!ascending && left < right) {
					tmp := rows[i]
					rows[i] = rows[j]
					rows[j] = tmp
				}
			}
		}
		unsafe {
			mut w := &SimpleWindow(win)
			w.grid_rows[name] = rows
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_grid_sort_by_column(win.window_info, name.str, col_idx, if ascending {
			1
		} else {
			0
		})
	}
	return win
}

// grid_set_filter filters visible rows by matching cell contents.
pub fn (win &SimpleWindow) grid_set_filter(name string, query string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_filter(win.window_info, name.str, query.str)
	}
	return win
}

// grid_clear_filter removes any active row filter.
pub fn (win &SimpleWindow) grid_clear_filter(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_clear_filter(win.window_info, name.str)
	}
	return win
}

// grid_autosize_columns auto-sizes all columns to fit content.
pub fn (win &SimpleWindow) grid_autosize_columns(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_autosize_columns(win.window_info, name.str)
	}
	return win
}

// grid_set_selected_row sets the selected row index programmatically.
pub fn (win &SimpleWindow) grid_set_selected_row(name string, row_idx int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_selected_row(win.window_info, name.str, row_idx)
	}
	return win
}

// grid_clear removes all rows from the grid.
pub fn (win &SimpleWindow) grid_clear(name string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.grid_rows[name] = [][]string{}
	}
	if win.window_info != unsafe { nil } {
		C.window_grid_clear(win.window_info, name.str)
	}
	return win
}

// grid_set_column_editable enables or disables editing for a column.
pub fn (win &SimpleWindow) grid_set_column_editable(name string, col_idx int, editable bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_column_editable(win.window_info, name.str, col_idx, if editable {
			1
		} else {
			0
		})
	}
	return win
}

// grid_set_row_editable enables or disables editing for a row.
pub fn (win &SimpleWindow) grid_set_row_editable(name string, row_idx int, editable bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_row_editable(win.window_info, name.str, row_idx, if editable {
			1
		} else {
			0
		})
	}
	return win
}

// grid_set_cell_editable enables or disables editing for a cell.
pub fn (win &SimpleWindow) grid_set_cell_editable(name string, row int, col int, editable bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_cell_editable(win.window_info, name.str, row, col, if editable {
			1
		} else {
			0
		})
	}
	return win
}

// grid_set_column_enabled enables or disables a column.
pub fn (win &SimpleWindow) grid_set_column_enabled(name string, col_idx int, enabled bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_column_enabled(win.window_info, name.str, col_idx, if enabled {
			1
		} else {
			0
		})
	}
	return win
}

// grid_set_row_enabled enables or disables a row.
pub fn (win &SimpleWindow) grid_set_row_enabled(name string, row_idx int, enabled bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_row_enabled(win.window_info, name.str, row_idx, if enabled { 1 } else { 0 })
	}
	return win
}

// grid_set_cell_enabled enables or disables a cell.
pub fn (win &SimpleWindow) grid_set_cell_enabled(name string, row int, col int, enabled bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_cell_enabled(win.window_info, name.str, row, col, if enabled {
			1
		} else {
			0
		})
	}
	return win
}

// add_stepper inserts a standalone native NSStepper (up/down arrows) with a live value label.
// Use get_value_int/set_value_int to read or write the current value.
pub fn (win &SimpleWindow) configure(callback fn (mut cfg WindowConfig)) &SimpleWindow {
	mut cfg := WindowConfig{
		title:                        win.title
		width:                        win.width
		height:                       win.height
		padding:                      win.padding
		spacing:                      win.spacing
		background_color:             win.background_color
		font_color:                   win.font_color
		always_on_top:                win.always_on_top
		responsive_layout:            win.responsive_layout
		resizable:                    win.resizable
		minimizable:                  win.minimizable
		maximizable:                  win.maximizable
		closable:                     win.closable
		has_shadow:                   win.has_shadow
		movable_by_window_background: win.movable_by_window_background
		titlebar_visible:             win.titlebar_visible
		title_visible:                win.title_visible
	}
	callback(mut cfg)
	win.set_title(cfg.title)
	win.set_padding(cfg.padding)
	win.set_spacing(cfg.spacing)
	win.set_background_color(cfg.background_color)
	win.set_font_color(cfg.font_color)
	win.set_always_on_top(cfg.always_on_top)
	win.set_responsive_layout(cfg.responsive_layout)
	win.set_resizable(cfg.resizable)
	win.set_minimizable(cfg.minimizable)
	win.set_maximizable(cfg.maximizable)
	win.set_closable(cfg.closable)
	win.set_has_shadow(cfg.has_shadow)
	win.set_movable_by_window_background(cfg.movable_by_window_background)
	win.set_titlebar_visible(cfg.titlebar_visible)
	win.set_title_visible(cfg.title_visible)
	unsafe {
		mut w := &SimpleWindow(win)
		w.width = cfg.width
		w.height = cfg.height
	}
	return win
}

// validate_not_empty validates that a string value is not empty, returning "Required" if empty.
pub fn validate_not_empty(value string) string {
	if value.trim_space() == '' {
		return 'Required'
	}
	return ''
}

// set_debug_mode enables or disables verbose debug logging for window event dispatches.
pub fn (win &SimpleWindow) set_debug_mode(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.debug_mode = enabled
	}
	return win
}

// set_checkbox sets the checked state for the default checkbox control.
pub fn (win &SimpleWindow) set_checkbox(checked bool) &SimpleWindow {
	win.set_bool('default_checkbox', checked)
	return win
}

// get_checkbox returns the checked state of the default checkbox control.
pub fn (win &SimpleWindow) get_checkbox() bool {
	return win.get_bool('default_checkbox')
}

// set_number sets the integer value of the default number control.
pub fn (win &SimpleWindow) set_number(value int) &SimpleWindow {
	win.set_number_value('default_number', value)
	return win
}

// get_number returns the integer value of the default number control.
pub fn (win &SimpleWindow) get_number() int {
	return win.get_number_value('default_number')
}

// set_responsive_layout enables or disables automatic responsive control resizing.
pub fn (win &SimpleWindow) set_responsive_layout(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.responsive_layout = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_responsive_layout(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// set_padding sets window edge content padding in pixels.
pub fn (win &SimpleWindow) set_padding(padding int) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.padding = padding
	}
	if win.window_info != unsafe { nil } {
		C.window_set_padding(win.window_info, padding)
	}
	return win
}

// set_spacing sets vertical spacing between stacked controls in pixels.
pub fn (win &SimpleWindow) set_spacing(spacing int) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.spacing = spacing
	}
	if win.window_info != unsafe { nil } {
		C.window_set_spacing(win.window_info, spacing)
	}
	return win
}

// set_default_button sets the primary action button triggered when pressing Enter.
pub fn (win &SimpleWindow) set_default_button(name string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.default_button = name
	}
	if win.window_info != unsafe { nil } {
		C.window_set_default_button_by_name(win.window_info, name.str)
	}
	return win
}

// on_shortcut registers a global keyboard shortcut event handler for the window.
pub fn (win &SimpleWindow) on_shortcut(shortcut string, callback VoidEventCallback) &SimpleWindow {
	norm_shortcut := normalize_key_shortcut(shortcut)
	unsafe {
		mut w := &SimpleWindow(win)
		w.handlers << ControlEventHandler{
			control_name: 'window'
			event_name:   'key'
			filter_value: norm_shortcut
			void_cb:      callback
		}
	}
	return win
}

// open_url opens a URL link in the user's default web browser.
pub fn (win &SimpleWindow) open_url(url string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_open_url(win.window_info, url.str)
	}
	return win
}

// copy_to_clipboard copies a text string to the system clipboard.
pub fn (win &SimpleWindow) copy_to_clipboard(text string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_copy_to_clipboard(win.window_info, text.str)
	}
	return win
}

// clipboard_text returns UTF-8 text from the system clipboard.
pub fn clipboard_text() string {
	res := C.window_get_clipboard_text()
	if res != unsafe { nil } {
		return unsafe { tos3(res) }
	}
	return ''
}

// reveal_in_finder asks Finder to reveal an existing path.
pub fn reveal_in_finder(path string) bool {
	if path == '' {
		return false
	}
	return C.window_reveal_in_finder(path.str) == 1
}

// get_clipboard_text returns UTF-8 text from the system clipboard.
pub fn (win &SimpleWindow) get_title() string {
	return win.title
}

// set_title sets the title of the window or target control.
pub fn (win &SimpleWindow) set_title(text string) &SimpleWindow {
	old_title := win.title
	unsafe {
		mut w := &SimpleWindow(win)
		w.title = text
	}
	if win.window_info != unsafe { nil } {
		C.window_set_title_text(win.window_info, text.str)
	}
	// Keep cross-window registry keys in sync when title changes.
	if old_title != text {
		sys_unregister_window(old_title)
		sys_register_window(win)
	}
	return win
}

// set_always_on_top sets the always on top of the window or target control.
pub fn (win &SimpleWindow) set_always_on_top(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.always_on_top = enabled
	}
	if win.window_info != unsafe { nil } {
		val := if enabled { 1 } else { 0 }
		C.window_set_always_on_top(win.window_info, val)
	}
	return win
}

// get_always_on_top retrieves the always on top of the window or target control.
pub fn (win &SimpleWindow) get_always_on_top() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_always_on_top(win.window_info) == 1
	}
	return win.always_on_top
}

// set_min_size sets the min size of the window or target control.
pub fn (win &SimpleWindow) set_min_size(width int, height int) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.min_width = width
		w.min_height = height
	}
	if win.window_info != unsafe { nil } {
		C.window_set_min_size(win.window_info, width, height)
	}
	return win
}

// set_max_size sets the max size of the window or target control.
pub fn (win &SimpleWindow) set_max_size(width int, height int) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.max_width = width
		w.max_height = height
	}
	if win.window_info != unsafe { nil } {
		C.window_set_max_size(win.window_info, width, height)
	}
	return win
}

// get_min_size retrieves the minimum allowed window dimensions (w, h).
pub fn (win &SimpleWindow) get_min_size() (int, int) {
	if win.window_info != unsafe { nil } {
		mut w := 0
		mut h := 0
		C.window_get_min_size(win.window_info, &w, &h)
		if w > 0 && h > 0 {
			return w, h
		}
	}
	return win.min_width, win.min_height
}

// get_max_size retrieves the maximum allowed window dimensions (w, h).
pub fn (win &SimpleWindow) get_max_size() (int, int) {
	if win.window_info != unsafe { nil } {
		mut w := 0
		mut h := 0
		C.window_get_max_size(win.window_info, &w, &h)
		if w > 0 && h > 0 {
			return w, h
		}
	}
	return win.max_width, win.max_height
}

// set_resizable sets the resizable of the window or target control.
pub fn (win &SimpleWindow) set_resizable(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.resizable = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_resizable(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// set_minimizable sets the minimizable of the window or target control.
pub fn (win &SimpleWindow) set_minimizable(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.minimizable = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_minimizable(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// set_maximizable sets the maximizable of the window or target control.
pub fn (win &SimpleWindow) set_maximizable(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.maximizable = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_maximizable(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_resizable retrieves the resizable of the window or target control.
pub fn (win &SimpleWindow) get_resizable() bool {
	return win.resizable
}

// get_minimizable retrieves the minimizable of the window or target control.
pub fn (win &SimpleWindow) get_minimizable() bool {
	return win.minimizable
}

// get_maximizable retrieves the maximizable of the window or target control.
pub fn (win &SimpleWindow) get_maximizable() bool {
	return win.maximizable
}

// close closes the window.
pub fn (win &SimpleWindow) close() &SimpleWindow {
	if win.auto_save_state {
		win.save_app_form_state_or()
	}
	if win.window_info != unsafe { nil } {
		C.window_close(win.window_info)
	}
	return win
}

// close_window closes the window.
pub fn (win &SimpleWindow) close_window() &SimpleWindow {
	return win.close()
}

// hide hides the window from view.
pub fn (win &SimpleWindow) hide() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_hide(win.window_info)
	}
	return win
}

// hide_window hides the window from view.
pub fn (win &SimpleWindow) hide_window() &SimpleWindow {
	return win.hide()
}

// center centers the window on the active screen.
pub fn (win &SimpleWindow) center() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_center(win.window_info)
	}
	return win
}

// center_window centers the window on the active screen.
pub fn (win &SimpleWindow) center_window() &SimpleWindow {
	return win.center()
}

// align aligns the window to screen edges or positions ('top_left', 'top_right', 'bottom_left', 'bottom_right', 'center').
pub fn (win &SimpleWindow) align(position string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_align(win.window_info, position.str)
	}
	return win
}

// align_window aligns the window to screen edges or positions ('top_left', 'top_right', 'bottom_left', 'bottom_right', 'center').
pub fn (win &SimpleWindow) align_window(position string) &SimpleWindow {
	return win.align(position)
}

// set_size sets the width and height dimensions of the window in pixels.
pub fn (win &SimpleWindow) set_size(width int, height int) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.width = width
		w.height = height
	}
	if win.window_info != unsafe { nil } {
		C.window_set_size(win.window_info, width, height)
	}
	return win
}

// set_width sets the window width in pixels while maintaining the current height.
pub fn (win &SimpleWindow) set_width(width int) &SimpleWindow {
	return win.set_size(width, win.get_height())
}

// set_height sets the window height in pixels while maintaining the current width.
pub fn (win &SimpleWindow) set_height(height int) &SimpleWindow {
	return win.set_size(win.get_width(), height)
}

// get_width returns the current window width in pixels.
pub fn (win &SimpleWindow) get_width() int {
	if win.window_info != unsafe { nil } {
		return C.window_get_width(win.window_info)
	}
	return win.width
}

// get_height returns the current window height in pixels.
pub fn (win &SimpleWindow) get_height() int {
	if win.window_info != unsafe { nil } {
		return C.window_get_height(win.window_info)
	}
	return win.height
}

// set_position sets the (x, y) screen coordinates of the window in pixels.
pub fn (win &SimpleWindow) set_position(x int, y int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_position(win.window_info, x, y)
	}
	return win
}

// get_x returns the current x screen coordinate of the window.
pub fn (win &SimpleWindow) get_x() int {
	if win.window_info != unsafe { nil } {
		return C.window_get_x(win.window_info)
	}
	return 0
}

// get_y returns the current y screen coordinate of the window.
pub fn (win &SimpleWindow) get_y() int {
	if win.window_info != unsafe { nil } {
		return C.window_get_y(win.window_info)
	}
	return 0
}

// set_opacity sets the window opacity / transparency level (0.0 to 1.0).
pub fn (win &SimpleWindow) set_opacity(opacity f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_opacity(win.window_info, opacity)
	}
	return win
}

// get_opacity returns the current window opacity level (0.0 to 1.0).
pub fn (win &SimpleWindow) get_opacity() f64 {
	if win.window_info != unsafe { nil } {
		return C.window_get_opacity(win.window_info)
	}
	return 1.0
}

// toggle_fullscreen toggles the window fullscreen state.
pub fn (win &SimpleWindow) toggle_fullscreen() &SimpleWindow {
	next := !win.is_fullscreen()
	return win.set_fullscreen(next)
}

// minimize minimizes the window to the Dock.
pub fn (win &SimpleWindow) minimize() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_minimize(win.window_info)
	}
	return win
}

// deminimize restores the window from the Dock.
pub fn (win &SimpleWindow) deminimize() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_deminimize(win.window_info)
	}
	return win
}

// maximize expands the window to occupy the visible screen.
pub fn (win &SimpleWindow) maximize() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_maximize(win.window_info)
	}
	return win
}

// is_minimized returns true if the window is currently minimized.
pub fn (win &SimpleWindow) is_minimized() bool {
	if win.window_info != unsafe { nil } {
		return C.window_is_minimized(win.window_info) == 1
	}
	return false
}

// is_maximized checks if the window or control is maximized.
pub fn (win &SimpleWindow) is_maximized() bool {
	if win.window_info != unsafe { nil } {
		return C.window_is_maximized(win.window_info) == 1
	}
	return false
}

// is_fullscreen checks if the window or control is fullscreen.
pub fn (win &SimpleWindow) is_fullscreen() bool {
	if win.window_info != unsafe { nil } {
		return C.window_is_fullscreen(win.window_info) == 1
	}
	return win.fullscreen
}

// is_active checks if the window or control is active.
pub fn (win &SimpleWindow) is_active() bool {
	if win.window_info != unsafe { nil } {
		return C.window_is_active(win.window_info) == 1
	}
	return false
}

// set_titlebar_visible sets the titlebar visible of the window or target control.
pub fn (win &SimpleWindow) set_titlebar_visible(visible bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.titlebar_visible = visible
	}
	if win.window_info != unsafe { nil } {
		C.window_set_titlebar_visible(win.window_info, if visible { 1 } else { 0 })
	}
	return win
}

// request_attention requests user attention by bouncing the app icon in the macOS Dock.
pub fn (win &SimpleWindow) request_attention(critical bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_request_attention(win.window_info, if critical { 1 } else { 0 })
	}
	return win
}

// bounce_dock requests user attention by bouncing the app icon in the macOS Dock.
pub fn (win &SimpleWindow) bounce_dock(critical bool) &SimpleWindow {
	return win.request_attention(critical)
}

// set_closable sets the closable of the window or target control.
pub fn (win &SimpleWindow) set_closable(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.closable = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_closable(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_closable retrieves the closable of the window or target control.
pub fn (win &SimpleWindow) get_closable() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_closable(win.window_info) == 1
	}
	return win.closable
}

// set_has_shadow sets the has shadow of the window or target control.
pub fn (win &SimpleWindow) set_has_shadow(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.has_shadow = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_has_shadow(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_has_shadow retrieves the has shadow of the window or target control.
pub fn (win &SimpleWindow) get_has_shadow() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_has_shadow(win.window_info) == 1
	}
	return win.has_shadow
}

// set_movable_by_window_background sets the movable by window background of the window or target control.
pub fn (win &SimpleWindow) set_movable_by_window_background(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.movable_by_window_background = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_movable_by_window_background(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_movable_by_window_background retrieves the movable by window background of the window or target control.
pub fn (win &SimpleWindow) get_movable_by_window_background() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_movable_by_window_background(win.window_info) == 1
	}
	return win.movable_by_window_background
}

// is_visible checks if the window or control is visible.
pub fn (win &SimpleWindow) is_visible() bool {
	if win.window_info != unsafe { nil } {
		return C.window_is_visible(win.window_info) == 1
	}
	return false
}

// set_title_visible sets the title visible of the window or target control.
pub fn (win &SimpleWindow) set_title_visible(visible bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.title_visible = visible
	}
	if win.window_info != unsafe { nil } {
		C.window_set_title_visible(win.window_info, if visible { 1 } else { 0 })
	}
	return win
}

// get_title_visible retrieves the title visible of the window or target control.
pub fn (win &SimpleWindow) get_title_visible() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_title_visible(win.window_info) == 1
	}
	return win.title_visible
}

// is_title_visible checks if the window title is visible.
pub fn (win &SimpleWindow) is_title_visible() bool {
	return win.get_title_visible()
}

// get_titlebar_visible retrieves whether the titlebar is visible.
pub fn (win &SimpleWindow) get_titlebar_visible() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_titlebar_visible(win.window_info) == 1
	}
	return win.titlebar_visible
}

// is_titlebar_visible checks if the window or control is titlebar visible.
pub fn (win &SimpleWindow) is_titlebar_visible() bool {
	return win.get_titlebar_visible()
}

// set_subtitle sets the subtitle text displayed in the window titlebar.
pub fn (win &SimpleWindow) set_subtitle(text string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.subtitle = text
	}
	if win.window_info != unsafe { nil } {
		C.window_set_subtitle(win.window_info, text.str)
	}
	return win
}

// get_subtitle retrieves the subtitle text of the window.
pub fn (win &SimpleWindow) get_subtitle() string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_subtitle(win.window_info)
		if res != unsafe { nil } {
			s := unsafe { tos3(res) }
			if s.len > 0 {
				return s
			}
		}
	}
	return win.subtitle
}

// set_titlebar_appears_transparent sets whether the titlebar appears transparent.
pub fn (win &SimpleWindow) set_titlebar_appears_transparent(transparent bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.titlebar_appears_transparent = transparent
	}
	if win.window_info != unsafe { nil } {
		C.window_set_titlebar_appears_transparent(win.window_info, if transparent { 1 } else { 0 })
	}
	return win
}

// get_titlebar_appears_transparent retrieves whether the titlebar is transparent.
pub fn (win &SimpleWindow) get_titlebar_appears_transparent() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_titlebar_appears_transparent(win.window_info) == 1
	}
	return win.titlebar_appears_transparent
}

// set_full_size_content_view sets whether content extends under the titlebar.
pub fn (win &SimpleWindow) set_full_size_content_view(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.full_size_content_view = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_full_size_content_view(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_full_size_content_view retrieves whether content extends under the titlebar.
pub fn (win &SimpleWindow) get_full_size_content_view() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_full_size_content_view(win.window_info) == 1
	}
	return win.full_size_content_view
}

// set_vibrancy sets the NSVisualEffectView material (e.g. "hud", "popover", "sidebar", "header", "titlebar", "menu").
pub fn (win &SimpleWindow) set_vibrancy(material string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.vibrancy_material = material
	}
	if win.window_info != unsafe { nil } {
		C.window_set_vibrancy(win.window_info, material.str)
	}
	return win
}

// set_corner_radius sets the window corner rounding radius.
pub fn (win &SimpleWindow) set_corner_radius(radius f64) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.corner_radius = radius
	}
	if win.window_info != unsafe { nil } {
		C.window_set_corner_radius(win.window_info, radius)
	}
	return win
}

// get_corner_radius retrieves the window corner rounding radius.
pub fn (win &SimpleWindow) get_corner_radius() f64 {
	if win.window_info != unsafe { nil } {
		return C.window_get_corner_radius(win.window_info)
	}
	return win.corner_radius
}

// set_background_blur enables or disables window background blur.
pub fn (win &SimpleWindow) set_background_blur(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.background_blur = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_background_blur(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// set_window_level sets the window z-level ("normal", "floating", "modal", "mainMenu", "statusBar", "screenSaver").
pub fn (win &SimpleWindow) set_window_level(level string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.window_level = level
	}
	if win.window_info != unsafe { nil } {
		C.window_set_window_level(win.window_info, level.str)
	}
	return win
}

// set_level_type sets the window level type ("normal", "floating", "modal", etc.).
pub fn (win &SimpleWindow) get_window_level() string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_window_level(win.window_info)
		if res != unsafe { nil } {
			s := unsafe { tos3(res) }
			if s.len > 0 {
				return s
			}
		}
	}
	return win.window_level
}

// set_fullscreen toggles full screen mode on or off.
pub fn (win &SimpleWindow) set_fullscreen(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.fullscreen = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_fullscreen(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// center_on_active_screen centers the window on the active display containing the mouse cursor.
pub fn (win &SimpleWindow) center_on_active_screen() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_center_on_active_screen(win.window_info)
	}
	return win
}

// snap_to_edge snaps the window to screen edges ("top_left", "top_right", "bottom_left", "bottom_right", "top", "bottom", "left", "right", "center").
pub fn (win &SimpleWindow) snap_to_edge(edge string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_snap_to_edge(win.window_info, edge.str)
	}
	return win
}

// set_bounds sets the window x, y position and width, height bounds in screen coordinates.
pub fn (win &SimpleWindow) set_bounds(x int, y int, width int, height int) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.width = width
		w.height = height
	}
	if win.window_info != unsafe { nil } {
		C.window_set_bounds(win.window_info, x, y, width, height)
	}
	return win
}

// get_bounds retrieves the window x, y position and width, height bounds as a tuple (x, y, w, h).
pub fn (win &SimpleWindow) get_bounds() (int, int, int, int) {
	if win.window_info != unsafe { nil } {
		mut x := 0
		mut y := 0
		mut w := 0
		mut h := 0
		C.window_get_bounds(win.window_info, &x, &y, &w, &h)
		if w > 0 && h > 0 {
			return x, y, w, h
		}
	}
	return 0, 0, win.width, win.height
}

// move_by shifts window position by delta values in screen coordinates.
pub fn (win &SimpleWindow) move_by(dx int, dy int) &SimpleWindow {
	x, y, _, _ := win.get_bounds()
	return win.set_position(x + dx, y + dy)
}

// resize_by adjusts current window size by delta width and height.
pub fn (win &SimpleWindow) resize_by(dw int, dh int) &SimpleWindow {
	_, _, current_w, current_h := win.get_bounds()
	mut new_w := current_w + dw
	mut new_h := current_h + dh
	if new_w < 1 {
		new_w = 1
	}
	if new_h < 1 {
		new_h = 1
	}
	return win.set_size(new_w, new_h)
}

// get_center returns the window center point as (x, y) in screen coordinates.
pub fn (win &SimpleWindow) get_center() (int, int) {
	x, y, w, h := win.get_bounds()
	return x + (w / 2), y + (h / 2)
}

// set_center positions the window so its center matches the target screen point.
pub fn (win &SimpleWindow) set_center(center_x int, center_y int) &SimpleWindow {
	_, _, w, h := win.get_bounds()
	target_x := center_x - (w / 2)
	target_y := center_y - (h / 2)
	return win.set_position(target_x, target_y)
}

// center_horizontally centers the window on the current screen horizontally, preserving y.
pub fn (win &SimpleWindow) center_horizontally() &SimpleWindow {
	sx, _, sw, _ := win.get_screen_frame()
	_, y, w, _ := win.get_bounds()
	target_x := sx + ((sw - w) / 2)
	return win.set_position(target_x, y)
}

// center_vertically centers the window on the current screen vertically, preserving x.
pub fn (win &SimpleWindow) center_vertically() &SimpleWindow {
	_, sy, _, sh := win.get_screen_frame()
	x, _, _, h := win.get_bounds()
	target_y := sy + ((sh - h) / 2)
	return win.set_position(x, target_y)
}

// fit_to_screen resizes and positions window to the visible screen frame.
pub fn (win &SimpleWindow) fit_to_screen() &SimpleWindow {
	sx, sy, sw, sh := win.get_screen_frame()
	return win.set_bounds(sx, sy, sw, sh)
}

// constrain_to_screen keeps the window fully inside the visible screen frame.
pub fn (win &SimpleWindow) constrain_to_screen() &SimpleWindow {
	sx, sy, sw, sh := win.get_screen_frame()
	x, y, w, h := win.get_bounds()
	mut target_x := x
	mut target_y := y
	mut target_w := w
	mut target_h := h

	if target_w > sw {
		target_w = sw
	}
	if target_h > sh {
		target_h = sh
	}

	if target_x < sx {
		target_x = sx
	}
	if target_y < sy {
		target_y = sy
	}

	max_x := sx + sw - target_w
	max_y := sy + sh - target_h
	if target_x > max_x {
		target_x = max_x
	}
	if target_y > max_y {
		target_y = max_y
	}

	return win.set_bounds(target_x, target_y, target_w, target_h)
}

// set_aspect_ratio constrains the window resizing aspect ratio.
pub fn (win &SimpleWindow) set_aspect_ratio(width_ratio f64, height_ratio f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_aspect_ratio(win.window_info, width_ratio, height_ratio)
	}
	return win
}

// reset_aspect_ratio clears any enforced window aspect ratio.
pub fn (win &SimpleWindow) reset_aspect_ratio() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_reset_aspect_ratio(win.window_info)
	}
	return win
}

// has_aspect_ratio checks if an aspect ratio constraint is active.
pub fn (win &SimpleWindow) has_aspect_ratio() bool {
	if win.window_info != unsafe { nil } {
		return C.window_has_aspect_ratio(win.window_info) == 1
	}
	return false
}

// set_movable enables or disables whether the window can be moved by dragging.
pub fn (win &SimpleWindow) set_movable(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.movable = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_movable(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_movable retrieves whether the window can be moved by dragging.
pub fn (win &SimpleWindow) get_movable() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_movable(win.window_info) == 1
	}
	return win.movable
}

// is_movable returns true if the window can be dragged by the user. Alias for get_movable.
pub fn (win &SimpleWindow) is_movable() bool {
	return win.get_movable()
}

// set_ignores_mouse_events sets whether mouse clicks pass through the window (click-through overlay).
pub fn (win &SimpleWindow) set_ignores_mouse_events(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.ignores_mouse_events = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_ignores_mouse_events(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_ignores_mouse_events checks if mouse events pass through the window.
pub fn (win &SimpleWindow) get_ignores_mouse_events() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_ignores_mouse_events(win.window_info) == 1
	}
	return win.ignores_mouse_events
}

// set_hides_on_deactivate sets whether the window automatically hides when the app loses focus.
pub fn (win &SimpleWindow) set_hides_on_deactivate(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.hides_on_deactivate = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_hides_on_deactivate(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_hides_on_deactivate checks if the window hides when app loses focus.
pub fn (win &SimpleWindow) get_hides_on_deactivate() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_hides_on_deactivate(win.window_info) == 1
	}
	return win.hides_on_deactivate
}

// set_prevents_app_termination sets whether closing this window prevents app termination.
pub fn (win &SimpleWindow) set_prevents_app_termination(enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.prevents_app_termination = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_set_prevents_app_termination(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// get_prevents_app_termination checks if closing this window prevents app termination.
pub fn (win &SimpleWindow) get_prevents_app_termination() bool {
	if win.window_info != unsafe { nil } {
		native_val := C.window_get_prevents_app_termination(win.window_info) == 1
		if native_val != win.prevents_app_termination {
			return win.prevents_app_termination
		}
		return native_val
	}
	return win.prevents_app_termination
}

// set_represented_filename sets a file path to show document icon in window titlebar.
pub fn (win &SimpleWindow) set_represented_filename(filepath string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.represented_filename = filepath
	}
	if win.window_info != unsafe { nil } {
		C.window_set_represented_filename(win.window_info, filepath.str)
	}
	return win
}

// get_represented_filename retrieves the represented file path.
pub fn (win &SimpleWindow) get_represented_filename() string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_represented_filename(win.window_info)
		if res != unsafe { nil } {
			s := unsafe { tos3(res) }
			if s.len > 0 {
				return s
			}
		}
	}
	return win.represented_filename
}

// set_frame_autosave_name enables frame persistence under a stable autosave key.
pub fn (win &SimpleWindow) set_frame_autosave_name(autosave_name string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.frame_autosave_name = autosave_name
	}
	if win.window_info != unsafe { nil } {
		C.window_set_frame_autosave_name(win.window_info, autosave_name.str)
	}
	return win
}

// get_frame_autosave_name returns the active autosave key used for frame persistence.
pub fn (win &SimpleWindow) get_frame_autosave_name() string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_frame_autosave_name(win.window_info)
		if res != unsafe { nil } {
			s := unsafe { tos3(res) }
			if s.len > 0 {
				return s
			}
		}
	}
	return win.frame_autosave_name
}

// save_frame persists current window bounds using the configured autosave key.
pub fn (win &SimpleWindow) save_frame() bool {
	if win.window_info != unsafe { nil } {
		return C.window_save_frame(win.window_info) == 1
	}
	return false
}

// restore_frame restores window bounds previously saved with autosave.
pub fn (win &SimpleWindow) restore_frame() bool {
	if win.window_info != unsafe { nil } {
		return C.window_restore_frame(win.window_info) == 1
	}
	return false
}

// capture_screenshot writes a PNG screenshot of the current window to file_path.
pub fn (win &SimpleWindow) capture_screenshot(file_path string) bool {
	if file_path == '' {
		return false
	}
	if win.window_info != unsafe { nil } {
		return C.window_capture_screenshot(win.window_info, file_path.str) == 1
	}
	return false
}

// set_document_edited sets the unsaved changes dirty indicator in window titlebar close button.
pub fn (win &SimpleWindow) set_document_edited(edited bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.document_edited = edited
	}
	if win.window_info != unsafe { nil } {
		C.window_set_document_edited(win.window_info, if edited { 1 } else { 0 })
	}
	return win
}

// is_document_edited checks if window has unsaved changes dirty indicator.
pub fn (win &SimpleWindow) is_document_edited() bool {
	if win.window_info != unsafe { nil } {
		return C.window_is_document_edited(win.window_info) == 1
	}
	return win.document_edited
}

// flash_frame flashes the window frame to request user attention.
pub fn (win &SimpleWindow) flash_frame(critical bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_flash_frame(win.window_info, if critical { 1 } else { 0 })
	}
	return win
}

// bounce_dock_icon bounces the application dock icon to request attention.
pub fn (win &SimpleWindow) bounce_dock_icon(critical bool) &SimpleWindow {
	C.window_bounce_dock_icon(if critical { 1 } else { 0 })
	return win
}

// order_front brings the window to the front of the window stack.
pub fn (win &SimpleWindow) order_front() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_order_front(win.window_info)
	}
	return win
}

// bring_to_front is an alias for order_front.
pub fn (win &SimpleWindow) bring_to_front() &SimpleWindow {
	return win.order_front()
}

// order_back sends the window behind all other windows.
pub fn (win &SimpleWindow) order_back() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_order_back(win.window_info)
	}
	return win
}

// send_to_back is an alias for order_back.
pub fn (win &SimpleWindow) send_to_back() &SimpleWindow {
	return win.order_back()
}

// toggle_minimize toggles the window minimized state.
pub fn (win &SimpleWindow) toggle_minimize() &SimpleWindow {
	if win.is_minimized() {
		return win.deminimize()
	} else {
		return win.minimize()
	}
}

// toggle_maximize toggles the window maximized state.
pub fn (win &SimpleWindow) toggle_maximize() &SimpleWindow {
	return win.maximize()
}

// toggle_visibility toggles window visibility between shown and hidden.
pub fn (win &SimpleWindow) toggle_visibility() &SimpleWindow {
	if win.is_visible() {
		return win.hide()
	} else {
		if win.window_info != unsafe { nil } {
			C.window_show(win.window_info)
		}
		return win
	}
}

// set_background_color sets the background color of the window or target control.
pub fn (win &SimpleWindow) set_status(text string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.status_text = text
	}
	if !win.has_control('status') {
		win.add_label('status', text)
	}
	if win.window_info != unsafe { nil } {
		C.window_set_status_text(win.window_info, text.str)
	}
	return win
}

// get_status retrieves the status of the window or target control.
pub fn (win &SimpleWindow) get_status() string {
	return win.status_text
}

// status sets the status bar text of the window.
pub fn (win &SimpleWindow) status(text string) &SimpleWindow {
	win.set_status(text)
	return win
}

// run starts the application event loop and displays the main Cocoa window.
pub fn (win &SimpleWindow) run() &SimpleWindow {
	if win.auto_save_state {
		win.restore_app_form_state()
	}
	if win.window_info == unsafe { nil } {
		unsafe {
			mut w := &SimpleWindow(win)
			w.ensure_window()
		}
	}
	C.window_app_run(win.window_info)
	return win
}

// start starts the application event loop and displays the main Cocoa window (alias for run).
pub fn (win &SimpleWindow) start() &SimpleWindow {
	return win.run()
}

// show_control shows the named control (fluent builder).
pub fn (win &SimpleWindow) show_control(name string) &SimpleWindow {
	return win.set_control_visible(name, true)
}

// hide_control hides the named control (fluent builder).
pub fn (win &SimpleWindow) hide_control(name string) &SimpleWindow {
	return win.set_control_visible(name, false)
}

// update_list_items updates the list box items by name.
pub fn (win &SimpleWindow) update_list_items(name string, items []string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.list_items[name] = items.clone()
	}
	if win.window_info != unsafe { nil } {
		mut c_items := []&u8{}
		for item in items {
			c_items << item.str
		}
		C.window_update_list_items(win.window_info, name.str, c_items.data, items.len)
	}
	return win
}

// set_list_selected sets the list selected of the window or target control.
pub fn tree_node(id string, parent_id string, text string) TreeNode {
	return TreeNode{
		id:        id
		parent_id: parent_id
		text:      text
	}
}

// tree_root creates a root-level TreeNode (without a parent).
pub fn tree_root(id string, text string) TreeNode {
	return tree_node(id, '', text)
}

// tree_child creates a child TreeNode under the provided parent id.
pub fn tree_child(id string, parent_id string, text string) TreeNode {
	return tree_node(id, parent_id, text)
}

// tree_nodes_from_paths builds flat TreeNode entries from hierarchical path strings.
// Example path: "Company/Engineering/Backend".
pub fn tree_nodes_from_paths(paths []string, separator string) []TreeNode {
	sep := if separator == '' { '/' } else { separator }
	mut nodes := []TreeNode{}
	mut seen := map[string]bool{}

	for raw_path in paths {
		trimmed_path := raw_path.trim_space()
		if trimmed_path == '' {
			continue
		}

		mut parts := []string{}
		for part in trimmed_path.split(sep) {
			trimmed_part := part.trim_space()
			if trimmed_part != '' {
				parts << trimmed_part
			}
		}

		if parts.len == 0 {
			continue
		}

		mut parent_id := ''
		mut path_parts := []string{}
		for part in parts {
			path_parts << part
			node_id := path_parts.join('/')
			if node_id !in seen {
				nodes << TreeNode{
					id:        node_id
					parent_id: parent_id
					text:      part
				}
				seen[node_id] = true
			}
			parent_id = node_id
		}
	}

	return nodes
}

fn clone_tree_nodes(nodes []TreeNode) []TreeNode {
	mut copied := []TreeNode{cap: nodes.len}
	for node in nodes {
		copied << TreeNode{
			id:        node.id
			parent_id: node.parent_id
			text:      node.text
		}
	}
	return copied
}

// add_tree_view adds a tree view control to the window layout.
pub fn (win &SimpleWindow) set_tree_nodes(name string, nodes []TreeNode) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.tree_nodes[name] = clone_tree_nodes(nodes)
	}
	if win.window_info != unsafe { nil } {
		if nodes.len == 0 {
			C.window_set_tree_nodes(win.window_info, name.str, unsafe { nil }, 0)
			return win
		}
		mut flat := []&u8{}
		for node in nodes {
			flat << node.id.str
			flat << node.parent_id.str
			flat << node.text.str
		}
		C.window_set_tree_nodes(win.window_info, name.str, flat.data, flat.len)
	}
	return win
}

// get_tree_selected retrieves the tree selected of the window or target control.
pub fn (win &SimpleWindow) expand_tree(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_tree_expand_all(win.window_info, name.str)
	}
	return win
}

// open_tree is an alias for expand_tree.
pub fn (win &SimpleWindow) open_tree(name string) &SimpleWindow {
	return win.expand_tree(name)
}

// collapse_tree collapses all nodes in the target tree view.
pub fn (win &SimpleWindow) collapse_tree(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_tree_collapse_all(win.window_info, name.str)
	}
	return win
}

// close_tree is an alias for collapse_tree.
pub fn (win &SimpleWindow) close_tree(name string) &SimpleWindow {
	return win.collapse_tree(name)
}

// expand_tree_node expands a single node; optionally expands its descendants.
pub fn (win &SimpleWindow) expand_tree_node(name string, node_id string, expand_children bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		expand_val := if expand_children { 1 } else { 0 }
		C.window_tree_expand_node(win.window_info, name.str, node_id.str, expand_val)
	}
	return win
}

// collapse_tree_node collapses a single node; optionally collapses descendants.
pub fn (win &SimpleWindow) collapse_tree_node(name string, node_id string, collapse_children bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		collapse_val := if collapse_children { 1 } else { 0 }
		C.window_tree_collapse_node(win.window_info, name.str, node_id.str, collapse_val)
	}
	return win
}

// set_tree is an alias for set_tree_nodes.
pub fn (win &SimpleWindow) set_tree(name string, nodes []TreeNode) &SimpleWindow {
	return win.set_tree_nodes(name, nodes)
}

// clear_tree removes all tree nodes and clears the current selection.
pub fn (win &SimpleWindow) clear_tree(name string) &SimpleWindow {
	win.set_tree_selected(name, '')
	return win.set_tree_nodes(name, []TreeNode{})
}

// clear_tree_selection clears selection for the target tree control.
pub fn (win &SimpleWindow) get_tree_nodes(name string) []TreeNode {
	if name in win.tree_nodes {
		return clone_tree_nodes(win.tree_nodes[name])
	}
	return []TreeNode{}
}

// has_tree_node checks whether a node id exists in the target tree.
pub fn (win &SimpleWindow) has_tree_node(name string, node_id string) bool {
	for node in win.get_tree_nodes(name) {
		if node.id == node_id {
			return true
		}
	}
	return false
}

// get_tree_node returns a node by id when present.
pub fn (win &SimpleWindow) get_tree_node(name string, node_id string) ?TreeNode {
	for node in win.get_tree_nodes(name) {
		if node.id == node_id {
			return node
		}
	}
	return none
}

// add_tree_node inserts or updates one node and refreshes the control.
pub fn (win &SimpleWindow) remove_tree_node(name string, node_id string, remove_children bool) &SimpleWindow {
	if node_id == '' {
		return win
	}
	mut nodes := win.get_tree_nodes(name)
	if nodes.len == 0 {
		return win
	}

	mut target_parent := ''
	mut found := false
	for node in nodes {
		if node.id == node_id {
			target_parent = node.parent_id
			found = true
			break
		}
	}
	if !found {
		return win
	}

	mut filtered := []TreeNode{}
	if remove_children {
		mut children_by_parent := map[string][]string{}
		for node in nodes {
			if node.parent_id == '' {
				continue
			}
			if node.parent_id !in children_by_parent {
				children_by_parent[node.parent_id] = []string{}
			}
			mut existing := children_by_parent[node.parent_id]
			existing << node.id
			children_by_parent[node.parent_id] = existing
		}

		mut to_remove := map[string]bool{}
		mut queue := [node_id]
		to_remove[node_id] = true
		for queue.len > 0 {
			current := queue[0]
			queue.delete(0)
			for child_id in children_by_parent[current] or { []string{} } {
				if child_id in to_remove {
					continue
				}
				to_remove[child_id] = true
				queue << child_id
			}
		}

		for node in nodes {
			if node.id in to_remove {
				continue
			}
			filtered << node
		}
	} else {
		for mut node in nodes {
			if node.id == node_id {
				continue
			}
			if node.parent_id == node_id {
				node.parent_id = target_parent
			}
			filtered << node
		}
	}

	if win.get_tree_selected(name) == node_id {
		win.set_tree_selected(name, '')
	}
	return win.set_tree_nodes(name, filtered)
}

// set_tree_node_text updates the display text of a single tree node.
pub fn (win &SimpleWindow) set_tree_node_text(name string, node_id string, text string) &SimpleWindow {
	mut nodes := win.get_tree_nodes(name)
	mut changed := false
	for i, node in nodes {
		if node.id == node_id {
			nodes[i] = TreeNode{
				id:        node.id
				parent_id: node.parent_id
				text:      text
			}
			changed = true
			break
		}
	}
	if !changed {
		return win
	}
	return win.set_tree_nodes(name, nodes)
}

// set_tree_paths builds a tree from slash-delimited path strings.
pub fn (win &SimpleWindow) set_tree_paths(name string, paths []string) &SimpleWindow {
	return win.set_tree_paths_with_separator(name, paths, '/')
}

// set_tree_paths_with_separator builds a tree from path strings using a custom separator.
pub fn (win &SimpleWindow) set_tree_paths_with_separator(name string, paths []string, separator string) &SimpleWindow {
	nodes := tree_nodes_from_paths(paths, separator)
	return win.set_tree_nodes(name, nodes)
}

// add_table adds a table control to the window layout.
pub fn (win &SimpleWindow) set_table_rows(name string, rows [][]string) &SimpleWindow {
	win.set_table_rows_strict(name, rows) or {
		// Backward-compatible no-op on invalid targets in non-strict API.
	}
	return win
}

// set_table_column_selection enables/disables whole-column selection for a table.
pub fn (win &SimpleWindow) set_table_column_selection(name string, enabled bool) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.table_column_selection[name] = enabled
	}
	if win.window_info != unsafe { nil } {
		C.window_table_set_column_selection(win.window_info, name.str, if enabled { 1 } else { 0 })
	}
	return win
}

// get_table_column_selection returns whether whole-column selection is enabled.
pub fn (win &SimpleWindow) get_table_column_selection(name string) bool {
	return win.table_column_selection[name] or { false }
}

// set_table_selected_column selects an entire table column (0-based).
// Pass -1 to clear selection.
pub fn (win &SimpleWindow) set_table_selected_column(name string, column int) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.table_selected_columns[name] = column
	}
	if win.window_info != unsafe { nil } {
		C.window_table_set_selected_column(win.window_info, name.str, column)
	}
	return win
}

// get_table_selected_column returns the 0-based selected column index, or -1.
pub fn (win &SimpleWindow) get_table_selected_column(name string) int {
	if win.window_info != unsafe { nil } {
		native := C.window_table_get_selected_column(win.window_info, name.str)
		if native >= 0 {
			return native
		}
	}
	return win.table_selected_columns[name] or { -1 }
}

// get_table_selected_column_values returns values from the selected column.
pub fn (win &SimpleWindow) get_table_selected_column_values(name string) []string {
	selected := win.get_table_selected_column(name)
	if selected < 0 {
		return []string{}
	}
	rows := win.table_rows[name] or { [][]string{} }
	mut values := []string{cap: rows.len}
	for row in rows {
		if selected < row.len {
			values << row[selected]
		} else {
			values << ''
		}
	}
	return values
}

// remove_table_column_strict removes a table column at a 0-based index.
// Returns all removed cell values in row order.
pub fn (win &SimpleWindow) remove_selected_table_column_strict(name string) !(int, []string) {
	selected := win.get_table_selected_column(name)
	if selected < 0 {
		return error('remove_selected_table_column_strict: no column selected')
	}
	removed := win.remove_table_column_strict(name, selected)!
	return selected, removed
}

// load_table_from_structs loads an array of structs into a table control by mapping fields to columns.
pub fn (win &SimpleWindow) load_table_from_structs[T](name string, items []T) &SimpleWindow {
	mut rows := [][]string{}
	for item in items {
		mut row := []string{}
		$for field in T.fields {
			$if field.typ is string {
				row << item.$(field.name)
			} $else $if field.typ is int {
				row << item.$(field.name).str()
			} $else $if field.typ is bool {
				row << item.$(field.name).str()
			}
		}
		rows << row
	}
	win.set_table_rows(name, rows)
	return win
}

// get_values retrieves the values of the window or target control.
pub fn (win &SimpleWindow) validate_struct[T]() bool {
	mut all_valid := true
	$for field in T.fields {
		name := field.name
		mut val := ''
		$if field.typ is string {
			val = win.get_text(name)
		} $else $if field.typ is int {
			val = win.get_value_int(name).str()
		} $else $if field.typ is bool {
			val = win.get_checked(name).str()
		} $else {
			val = win.get_text(name)
		}

		mut err_msg := ''
		for attr in field.attrs {
			if attr == 'required' {
				if val.trim_space() == '' {
					err_msg = 'This field is required'
					break
				}
			} else if attr.starts_with('min_len:') {
				min_len := attr.all_after('min_len:').trim_space().trim('\'"').int()
				if val.len < min_len {
					err_msg = 'Must be at least ${min_len} characters'
					break
				}
			} else if attr.starts_with('max_len:') {
				max_len := attr.all_after('max_len:').trim_space().trim('\'"').int()
				if val.len > max_len {
					err_msg = 'Must be at most ${max_len} characters'
					break
				}
			} else if attr == 'email' {
				if val.trim_space() != '' {
					email_err := validate_email(val)
					if email_err != '' {
						err_msg = email_err
						break
					}
				}
			} else if attr == 'url' {
				if val.trim_space() != '' {
					url_err := validate_url(val)
					if url_err != '' {
						err_msg = url_err
						break
					}
				}
			} else if attr == 'alphanumeric' {
				if val.trim_space() != '' {
					alpha_err := validate_alphanumeric(val)
					if alpha_err != '' {
						err_msg = alpha_err
						break
					}
				}
			} else if attr.starts_with('min:') {
				$if field.typ is int {
					min_val := attr.all_after('min:').trim_space().trim('\'"').int()
					int_val := win.get_value_int(name)
					if int_val < min_val {
						err_msg = 'Must be at least ${min_val}'
						break
					}
				}
			} else if attr.starts_with('max:') {
				$if field.typ is int {
					max_val := attr.all_after('max:').trim_space().trim('\'"').int()
					int_val := win.get_value_int(name)
					if int_val > max_val {
						err_msg = 'Must be at most ${max_val}'
						break
					}
				}
			}
		}

		if err_msg != '' {
			win.set_error(name, err_msg)
			all_valid = false
		} else {
			win.clear_error(name)
		}
	}
	return all_valid
}

fn normalize_table_rows(rows [][]string, cols_count int) [][]string {
	if cols_count < 0 {
		return rows.map(it.clone())
	}
	mut normalized := [][]string{cap: rows.len}
	for row in rows {
		mut next := row.clone()
		if cols_count == 0 {
			normalized << []string{}
			continue
		}
		if next.len > cols_count {
			next = next[..cols_count].clone()
		} else if next.len < cols_count {
			next << []string{len: cols_count - next.len, init: ''}
		}
		normalized << next
	}
	return normalized
}

fn table_infer_column_count(rows [][]string) int {
	mut cols_count := 0
	for row in rows {
		if row.len > cols_count {
			cols_count = row.len
		}
	}
	return cols_count
}

fn (win &SimpleWindow) table_column_count_for(name string, rows [][]string) int {
	if cols := win.table_columns[name] {
		if cols.len > 0 {
			return cols.len
		}
	}
	return table_infer_column_count(rows)
}

// get_table_column_count returns the configured table column count.
// If no explicit columns were registered, count is inferred from the widest row.
pub fn (win &SimpleWindow) get_table_column_count(name string) int {
	if cols := win.table_columns[name] {
		if cols.len > 0 {
			return cols.len
		}
	}
	return table_infer_column_count(win.table_rows[name] or { [][]string{} })
}

// set_table_rows_strict validates the target control and normalizes row width.
pub fn (win &SimpleWindow) insert_table_row_strict(name string, index int, row []string) ! {
	mut rows := win.table_rows[name] or { [][]string{} }
	if index < 0 || index > rows.len {
		return error('insert_table_row_strict: index ${index} out of range 0..${rows.len}')
	}
	rows.insert(index, row.clone())
	win.set_table_rows_strict(name, rows)!
}

// update_table_row_strict replaces a row at a 0-based index.
pub fn (win &SimpleWindow) update_table_row_strict(name string, index int, row []string) ! {
	mut rows := win.table_rows[name] or { [][]string{} }
	if index < 0 || index >= rows.len {
		return error('update_table_row_strict: index ${index} out of range 0..${rows.len - 1}')
	}
	rows[index] = row.clone()
	win.set_table_rows_strict(name, rows)!
}

// remove_table_row_strict removes a row at a 0-based index.
pub fn (win &SimpleWindow) remove_table_row_strict(name string, index int) ! {
	mut rows := win.table_rows[name] or { [][]string{} }
	if index < 0 || index >= rows.len {
		return error('remove_table_row_strict: index ${index} out of range 0..${rows.len - 1}')
	}
	rows.delete(index)
	win.set_table_rows_strict(name, rows)!
}

// set_table_cell_strict updates a single table cell with bounds checks.
pub fn (win &SimpleWindow) set_table_cell_strict(name string, row int, col int, value string) ! {
	rows := win.table_rows[name] or { [][]string{} }
	if row < 0 || row >= rows.len {
		return error('set_table_cell_strict: row ${row} out of range 0..${rows.len - 1}')
	}
	if col < 0 || col >= rows[row].len {
		return error('set_table_cell_strict: column ${col} out of range 0..${rows[row].len - 1}')
	}
	mut next := rows.map(it.clone())
	next[row][col] = value
	win.set_table_rows_strict(name, next)!
}

// find_table_row_strict returns the first row index whose column value matches.
pub fn (win &SimpleWindow) find_table_row_strict(name string, column int, value string) !int {
	rows := win.table_rows[name] or { [][]string{} }
	cols_count := win.get_table_column_count(name)
	if column < 0 || column >= cols_count {
		return error('find_table_row_strict: column ${column} out of range 0..${cols_count - 1}')
	}
	for i, row in rows {
		if column < row.len && row[column] == value {
			return i
		}
	}
	return error('find_table_row_strict: value not found')
}

// Layout Rows and Form Generation Helpers
pub fn (win &SimpleWindow) enable_status_bar(icon_path string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_enable_status_bar(win.window_info, icon_path.str)
	}
	return win
}

// add_toolbar_item adds a toolbar item control to the window layout.
pub fn (win &SimpleWindow) set_toolbar_style(style string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_toolbar_style(win.window_info, style.str)
	}
	return win
}

// on_toolbar_click registers an event handler for on toolbar click events.
pub fn (win &SimpleWindow) show_window() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_show(win.window_info)
	}
	return win
}

fn vlang_main_thread_dispatcher(ctx voidptr) {
	mut data := unsafe { &MainThreadCallback(ctx) }
	cb := data.cb
	mut win := data.win
	cb(mut win)
	unsafe {
		free(data)
	}
}

// run_on_main_thread dispatches a callback execution asynchronously onto the macOS main UI thread.
pub fn (win &SimpleWindow) run_on_main_thread(callback VoidEventCallback) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		data := &MainThreadCallback{
			win: win
			cb:  callback
		}
		C.window_run_on_main_thread(vlang_main_thread_dispatcher, data)
	}
	return win
}

// run_on_main_thread_sync dispatches a callback execution onto the macOS main UI thread and waits synchronously for completion.
pub fn (win &SimpleWindow) run_on_main_thread_sync(callback VoidEventCallback) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		data := &MainThreadCallback{
			win: win
			cb:  callback
		}
		C.window_run_on_main_thread_sync(vlang_main_thread_dispatcher, data)
	}
	return win
}

// run_async executes a background task on a separate thread and invokes on_complete on the main UI thread.
pub fn (win &SimpleWindow) run_async(bg_task fn (), on_complete VoidEventCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		spawn fn [mut w, bg_task, on_complete] () {
			bg_task()
			w.run_on_main_thread(on_complete)
		}()
	}
	return win
}

// clear_error clears any validation error message displayed on a control.
pub fn (win &SimpleWindow) clear_error(name string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.errors.delete(name)
	}
	if win.window_info != unsafe { nil } {
		C.window_set_error_by_name(win.window_info, name.str, c'')
	}
	return win
}

// clear_errors clears all active validation error messages across all controls.
pub fn (win &SimpleWindow) clear_errors() &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		for name, _ in w.errors {
			if win.window_info != nil {
				C.window_set_error_by_name(win.window_info, name.str, c'')
			}
		}
		w.errors = map[string]string{}
	}
	return win
}

// set_status_temp temporarily displays a status message for a specified duration in milliseconds.
pub fn (win &SimpleWindow) set_status_temp(message string, ms int) &SimpleWindow {
	current_status := win.get_status()
	win.set_status(message)
	win.after(ms, fn [current_status] (mut w SimpleWindow) {
		w.set_status(current_status)
	})
	return win
}

// notify delivers a macOS system notification banner.
pub fn (win &SimpleWindow) notify(title string, message string) &SimpleWindow {
	C.window_deliver_notification(title.str, message.str)
	return win
}

// badge sets the badge count or text on the macOS Dock application icon.
pub fn (win &SimpleWindow) badge(text string) &SimpleWindow {
	C.window_set_dock_badge(text.str)
	return win
}

// set_slider_range sets the slider range of the window or target control.
pub fn (win &SimpleWindow) set_slider_range(name string, min_val f64, max_val f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_slider_range(win.window_info, name.str, min_val, max_val)
	}
	return win
}

// beep plays the system alert beep sound.
pub fn beep() {
	C.window_beep()
}

// enable_search_history enables search history autocompletion for a search field control.
pub fn (win &SimpleWindow) enable_search_history(name string, autosave_name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_enable_search_history(win.window_info, name.str, autosave_name.str)
	}
	return win
}

// set_status_bar_icon sets the status bar icon of the window or target control.
pub fn (win &SimpleWindow) set_status_bar_icon(icon_path string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_status_bar_icon(win.window_info, icon_path.str)
	}
	return win
}

// set_status_bar_title sets the status bar title of the window or target control.
pub fn (win &SimpleWindow) set_status_bar_title(title string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_status_bar_title(win.window_info, title.str)
	}
	return win
}

// set_dock_icon sets the dock icon of the window or target control.
pub fn (win &SimpleWindow) set_dock_icon(image_path string) &SimpleWindow {
	C.window_set_dock_icon(image_path.str)
	return win
}

// clear_dock_icon clears the content of dock icon.
pub fn (win &SimpleWindow) clear_dock_icon() &SimpleWindow {
	C.window_set_dock_icon(c'')
	return win
}

// split_view_next_pane moves insertion to the next pane in a split view layout container.
pub fn (win &SimpleWindow) split_view_next_pane() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_split_view_next_pane(win.window_info)
	}
	return win
}

// set_collection_items populates items in a collection view.
pub fn (win &SimpleWindow) set_collection_items(name string, items []string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		mut c_items := []&u8{}
		for it in items {
			c_items << it.str
		}
		C.window_set_collection_items(win.window_info, name.str, c_items.data, items.len)
	}
	return win
}

// show_popover displays a popover callout attached to an anchor control.
pub fn (win &SimpleWindow) show_popover(anchor_name string, title string, message string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_show_popover(win.window_info, anchor_name.str, title.str, message.str)
	}
	return win
}

// add_calendar adds a calendar control to the window layout.
pub fn (win &SimpleWindow) draw_line(canvas_name string, x1 f64, y1 f64, x2 f64, y2 f64, color string, stroke_width f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_draw_line(win.window_info, canvas_name.str, x1, y1, x2, y2, color.str,
			stroke_width)
	}
	return win
}

// draw_rect draws a rect on the specified canvas control.
pub fn (win &SimpleWindow) draw_rect(canvas_name string, x f64, y f64, w f64, h f64, color string, fill bool, stroke_width f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		fill_val := if fill { 1 } else { 0 }
		C.window_draw_rect(win.window_info, canvas_name.str, x, y, w, h, color.str, fill_val,
			stroke_width)
	}
	return win
}

// draw_circle draws a circle on the specified canvas control.
pub fn (win &SimpleWindow) draw_circle(canvas_name string, x f64, y f64, r f64, color string, fill bool, stroke_width f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		fill_val := if fill { 1 } else { 0 }
		C.window_draw_circle(win.window_info, canvas_name.str, x, y, r, color.str, fill_val,
			stroke_width)
	}
	return win
}

// clear_canvas clears the content of canvas.
pub fn (win &SimpleWindow) clear_canvas(canvas_name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_clear_canvas(win.window_info, canvas_name.str)
	}
	return win
}

// begin_glass_box begins a glass box container in the layout.
pub fn (win &SimpleWindow) banner(text string, style string) &SimpleWindow {
	return win.add_banner('', text, style)
}

// add_section_header adds a styled layout divider with title and optional subtitle.
pub fn (win &SimpleWindow) add_section_header(name string, title string, subtitle string) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('section_header')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.controls << ControlEntry{
			name:  real_name
			kind:  'section_header'
			value: title
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_add_section_header_control(win.window_info, real_name.str, title.str,
			subtitle.str)
	}
	return win
}

// section_header inserts an auto-named section header.
pub fn (win &SimpleWindow) section_header(title string, subtitle string) &SimpleWindow {
	return win.add_section_header('', title, subtitle)
}

// add_vertical_slider adds a vertical slider to the layout.
pub fn (win &SimpleWindow) metric_meter(title string, value int, min_val int, max_val int, unit string) &SimpleWindow {
	return win.add_metric_meter('', title, value, min_val, max_val, unit)
}

// add_avatar_card adds a user profile avatar tile with round initial icon, title, subtitle, and status pill.
pub fn (win &SimpleWindow) add_avatar_card(name string, title string, subtitle string, status string) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('avatar_card')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.controls << ControlEntry{
			name:  real_name
			kind:  'avatar_card'
			value: title
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_add_avatar_card_control(win.window_info, real_name.str, title.str, subtitle.str,
			status.str)
	}
	return win
}

// avatar_card inserts an auto-named avatar card.
pub fn (win &SimpleWindow) avatar_card(title string, subtitle string, status string) &SimpleWindow {
	return win.add_avatar_card('', title, subtitle, status)
}

// add_time_picker adds a standalone time picker control (clock/time selector) to the layout.
pub fn (win &SimpleWindow) set_star_rating_value(name string, value int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_rating_value(win.window_info, name.str, value)
	}
	return win
}

// get_star_rating_value gets selected rating value for a control.
pub fn (win &SimpleWindow) get_star_rating_value(name string) int {
	if win.window_info != unsafe { nil } {
		return C.window_get_rating_value(win.window_info, name.str)
	}
	return 0
}

// add_range_slider adds a dual-thumb range selector slider widget.
pub fn (win &SimpleWindow) set_range_slider_values(name string, low_val int, high_val int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_range_slider_values(win.window_info, name.str, low_val, high_val)
	}
	return win
}

// get_range_slider_low gets low range boundary.
pub fn (win &SimpleWindow) get_range_slider_low(name string) int {
	if win.window_info != unsafe { nil } {
		return C.window_get_range_slider_low(win.window_info, name.str)
	}
	return 0
}

// get_range_slider_high gets high range boundary.
pub fn (win &SimpleWindow) get_range_slider_high(name string) int {
	if win.window_info != unsafe { nil } {
		return C.window_get_range_slider_high(win.window_info, name.str)
	}
	return 0
}

// add_split_button adds a primary action button paired with a secondary drop-down popup menu.
pub fn (win &SimpleWindow) set_tag_cloud_tags(name string, tags []string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		c_tags := tags.map(it.str)
		C.window_set_tag_cloud_tags(win.window_info, name.str, c_tags.data, c_tags.len)
	}
	return win
}

// add_wizard_stepper adds a multi-step process flow indicator bar.
pub fn (win &SimpleWindow) set_wizard_stepper_step(name string, step int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_wizard_stepper_step(win.window_info, name.str, step)
	}
	return win
}

// add_gauge adds a progress/level gauge indicator widget.
pub fn (win &SimpleWindow) clear_activity_feed(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_clear_activity_feed(win.window_info, name.str)
	}
	return win
}

// add_markdown_view adds a formatted Markdown text viewer widget.
pub fn (win &SimpleWindow) set_sparkline_data(name string, values []f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_sparkline_data(win.window_info, name.str, values.data, values.len)
	}
	return win
}

// add_pin_code adds a digit verification PIN/OTP code input widget.
pub fn (win &SimpleWindow) add_timeline_item(name string, title string, subtitle string, time_str string, status string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_add_timeline_item(win.window_info, name.str, title.str, subtitle.str,
			time_str.str, status.str)
	}
	return win
}

// add_metric_card adds a KPI metric stats card widget.
pub fn (win &SimpleWindow) add_metric_card(name string, title string, value string, change_badge string, subtitle string) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('metric_card')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.controls << ControlEntry{
			name:  real_name
			kind:  'metric_card'
			value: value
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_add_metric_card_control(win.window_info, real_name.str, title.str, value.str,
			change_badge.str, subtitle.str)
	}
	return win
}

// metric_card inserts an auto-named metric card widget.
pub fn (win &SimpleWindow) metric_card(title string, value string, change_badge string, subtitle string) &SimpleWindow {
	return win.add_metric_card('', title, value, change_badge, subtitle)
}

// set_rating_breakdown_data updates rating breakdown chart data.
pub fn (win &SimpleWindow) set_rating_breakdown_data(name string, avg_score f64, total_reviews int, star_percentages []f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_rating_breakdown_data(win.window_info, name.str, avg_score, total_reviews,
			star_percentages.data, star_percentages.len)
	}
	return win
}

// add_code_view adds a dark monospaced code snippet viewer with language header.
pub fn (win &SimpleWindow) set_alert_banner_value(name string, title string, message string, style string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_alert_banner_value(win.window_info, name.str, title.str, message.str,
			style.str)
	}
	return win
}

// add_step_tracker adds a horizontal process step progress bar with interactive step nodes.
pub fn (win &SimpleWindow) set_key_value_card_data(name string, keys []string, values []string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		mut c_keys := []&u8{cap: keys.len}
		for k in keys {
			c_keys << k.str
		}
		mut c_vals := []&u8{cap: values.len}
		for v in values {
			c_vals << v.str
		}
		count := if keys.len < values.len { keys.len } else { values.len }
		C.window_set_key_value_card_data(win.window_info, name.str, c_keys.data, c_vals.data,
			count)
	}
	return win
}

// add_diff_view adds a side-by-side / unified code diff comparison view widget.
pub fn (win &SimpleWindow) set_diff_view(name string, old_text string, new_text string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_diff_view_text(win.window_info, name.str, old_text.str, new_text.str)
	}
	return win
}

// add_json_tree adds a JSON / structured data syntax-highlighted inspector control.
pub fn (win &SimpleWindow) set_json_tree(name string, json_str string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_json_tree_data(win.window_info, name.str, json_str.str)
	}
	return win
}

// add_http_request_card adds an API / HTTP request status & metric inspector card.
pub fn (win &SimpleWindow) set_http_request_card(name string, method string, url string, status_code int, response_time_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_http_request_card_data(win.window_info, name.str, method.str, url.str,
			status_code, response_time_ms)
	}
	return win
}

// add_terminal_view adds a shell / terminal command output view widget.
pub fn (win &SimpleWindow) clear_terminal(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_clear_terminal(win.window_info, name.str)
	}
	return win
}

// add_resource_monitor adds a system resource & telemetry monitor dashboard control.
pub fn (win &SimpleWindow) set_resource_monitor(name string, cpu_pct int, mem_pct int, disk_pct int, net_kbps int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_resource_monitor_metrics(win.window_info, name.str, cpu_pct, mem_pct,
			disk_pct, net_kbps)
	}
	return win
}

// add_env_vars adds an environment & config variables viewer/editor card.
pub fn (win &SimpleWindow) set_env_vars(name string, keys []string, values []string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		mut c_keys := []&u8{cap: keys.len}
		for k in keys {
			c_keys << k.str
		}
		mut c_vals := []&u8{cap: values.len}
		for v in values {
			c_vals << v.str
		}
		count := if keys.len < values.len { keys.len } else { values.len }
		C.window_set_env_vars_data(win.window_info, name.str, c_keys.data, c_vals.data,
			count)
	}
	return win
}

// add_badge_button adds an action button with an attached notification counter badge.
pub fn (win &SimpleWindow) set_badge_button_count(name string, count int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_badge_button_count(win.window_info, name.str, count)
	}
	return win
}

// add_command_palette adds a search / command palette bar with search icon & shortcut hint.
pub fn (win &SimpleWindow) set_command_palette_text(name string, text string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_command_palette_text(win.window_info, name.str, text.str)
	}
	return win
}

// add_status_banner adds an alert message strip banner with icon and accent border.
pub fn (win &SimpleWindow) set_pill_toggle_selected(name string, selected_index int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_pill_toggle_selected(win.window_info, name.str, selected_index)
	}
	return win
}

// add_color_swatch_panel adds a palette panel with circular color swatches.
pub fn (win &SimpleWindow) set_color_swatch_selected(name string, hex_color string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_color_swatch_selected(win.window_info, name.str, hex_color.str)
	}
	return win
}

// add_hotkey_badge adds a macOS metallic keycap hotkey display badge with description.
pub fn (win &SimpleWindow) set_hotkey_badge_shortcut(name string, shortcut_str string, description string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_hotkey_badge_shortcut(win.window_info, name.str, shortcut_str.str,
			description.str)
	}
	return win
}

// add_quick_action_bar adds a quick action bar control with interactive action buttons.
pub fn (win &SimpleWindow) set_quick_action_enabled(name string, index int, enabled bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_quick_action_enabled(win.window_info, name.str, index, if enabled {
			1
		} else {
			0
		})
	}
	return win
}

// add_accordion_group adds a multi-section expandable accordion group widget.
pub fn (win &SimpleWindow) set_accordion_expanded(name string, index int, expanded bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_accordion_expanded(win.window_info, name.str, index, if expanded {
			1
		} else {
			0
		})
	}
	return win
}

// add_segment_distribution_bar adds a proportional segment distribution bar displaying breakdown ratios.
pub fn (win &SimpleWindow) set_segment_distribution_values(name string, values []f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_segment_distribution_values(win.window_info, name.str, values.data,
			values.len)
	}
	return win
}

// add_tag_input_field adds an interactive tag pill field with add/remove capability.
pub fn (win &SimpleWindow) set_tag_input_tags(name string, tags []string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		mut c_tags := []&u8{cap: tags.len}
		for t in tags {
			c_tags << t.str
		}
		C.window_set_tag_input_tags(win.window_info, name.str, c_tags.data, tags.len)
	}
	return win
}

// get_tag_input_tags returns comma-separated tags from a tag input field.
pub fn (win &SimpleWindow) get_tag_input_tags(name string) string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_tag_input_tags(win.window_info, name.str)
		if res != unsafe { nil } {
			return unsafe { tos3(res) }
		}
	}
	return ''
}

// add_status_dock adds a window footer status dock control with status dot, status text, and count badge.
pub fn (win &SimpleWindow) set_status_dock_info(name string, status_text string, dot_color string, count_text string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_status_dock_info(win.window_info, name.str, status_text.str, dot_color.str,
			count_text.str)
	}
	return win
}

// add_info_callout adds a styled info/alert callout card with accent border and optional action button.
pub fn (win &SimpleWindow) set_info_callout_text(name string, title string, message string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_info_callout_text(win.window_info, name.str, title.str, message.str)
	}
	return win
}

// set_alpha sets the window transparency level (0.0 transparent to 1.0 opaque).
pub fn (win &SimpleWindow) set_alpha(alpha f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_alpha(win.window_info, alpha)
	}
	return win
}

// get_alpha retrieves the current window transparency level.
pub fn (win &SimpleWindow) get_alpha() f64 {
	if win.window_info != unsafe { nil } {
		return C.window_get_alpha(win.window_info)
	}
	return 1.0
}

// set_collection_behavior configures macOS virtual desktop / Spaces behavior ("can_join_all_spaces", "move_to_active_space", "transient", "full_screen_primary", "full_screen_auxiliary").
pub fn (win &SimpleWindow) set_collection_behavior(behavior string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_collection_behavior(win.window_info, behavior.str)
	}
	return win
}

// set_close_button_enabled enables or disables the titlebar close button.
pub fn (win &SimpleWindow) set_close_button_enabled(enabled bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_close_button_enabled(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// set_minimize_button_enabled enables or disables the titlebar minimize button.
pub fn (win &SimpleWindow) set_minimize_button_enabled(enabled bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_minimize_button_enabled(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// set_zoom_button_enabled enables or disables the titlebar zoom / maximize button.
pub fn (win &SimpleWindow) set_zoom_button_enabled(enabled bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_zoom_button_enabled(win.window_info, if enabled { 1 } else { 0 })
	}
	return win
}

// set_content_insets sets safe area margins/padding (top, left, bottom, right) on the window content view.
pub fn (win &SimpleWindow) set_content_insets(top int, left int, bottom int, right int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_content_insets(win.window_info, top, left, bottom, right)
	}
	return win
}

// set_tabbing_mode configures macOS native window tabbing mode ("automatic", "preferred", "disallowed").
pub fn (win &SimpleWindow) set_tabbing_mode(mode string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_tabbing_mode(win.window_info, mode.str)
	}
	return win
}

// get_tabbing_mode retrieves current macOS native tabbing mode.
pub fn (win &SimpleWindow) get_tabbing_mode() string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_tabbing_mode(win.window_info)
		if res != unsafe { nil } {
			return unsafe { tos3(res) }
		}
	}
	return 'automatic'
}

// set_tabbing_identifier groups windows together under the same macOS tab bar identifier.
pub fn (win &SimpleWindow) set_tabbing_identifier(identifier string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_tabbing_identifier(win.window_info, identifier.str)
	}
	return win
}

// get_tabbing_identifier retrieves the macOS tabbing group identifier.
pub fn (win &SimpleWindow) get_tabbing_identifier() string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_tabbing_identifier(win.window_info)
		if res != unsafe { nil } {
			return unsafe { tos3(res) }
		}
	}
	return ''
}

// toggle_tab_bar toggles the native macOS window tab bar.
pub fn (win &SimpleWindow) toggle_tab_bar() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_toggle_tab_bar(win.window_info)
	}
	return win
}

// select_next_tab switches focus to the next window tab.
pub fn (win &SimpleWindow) select_next_tab() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_select_next_tab(win.window_info)
	}
	return win
}

// select_previous_tab switches focus to the previous window tab.
pub fn (win &SimpleWindow) select_previous_tab() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_select_previous_tab(win.window_info)
	}
	return win
}

// set_sharing_type configures window screen capture sharing access ("none", "read_only", "read_write").
pub fn (win &SimpleWindow) set_sharing_type(sharing string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.sharing_type = sharing
	}
	if win.window_info != unsafe { nil } {
		C.window_set_sharing_type(win.window_info, sharing.str)
	}
	return win
}

// set_content_protection prevents window contents from being captured by screen recordings or screen sharing when enabled.
pub fn (win &SimpleWindow) set_content_protection(enabled bool) &SimpleWindow {
	return win.set_sharing_type(if enabled { 'none' } else { 'read_write' })
}

// get_content_protection checks if content protection (screen sharing restriction) is currently active.
pub fn (win &SimpleWindow) get_content_protection() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_sharing_type(win.window_info) == 1
	}
	return win.sharing_type == 'none'
}

// unminimize restores a minimized window back from the Dock.
pub fn (win &SimpleWindow) unminimize() &SimpleWindow {
	return win.deminimize()
}

// unmaximize restores a maximized window back to normal size.
pub fn (win &SimpleWindow) unmaximize() &SimpleWindow {
	if win.is_maximized() {
		return win.maximize()
	}
	return win
}

// save_geometry persists window position, width, height, and display frame under key.
pub fn (win &SimpleWindow) save_geometry(key string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_save_geometry(win.window_info, key.str)
	}
	return win
}

// restore_geometry restores window position and size previously saved under key.
pub fn (win &SimpleWindow) restore_geometry(key string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_restore_geometry(win.window_info, key.str)
	}
	return win
}

// request_user_attention alerts the user by bouncing the Dock icon on macOS or flashing the taskbar.
pub fn (win &SimpleWindow) request_user_attention(critical bool) &SimpleWindow {
	return win.bounce_dock(critical)
}

// save_screenshot captures current window contents and saves to PNG at filepath.
pub fn (win &SimpleWindow) save_screenshot(filepath string) &SimpleWindow {
	win.capture_screenshot(filepath)
	return win
}

// on_close_requested registers a callback function that can veto or confirm window close by returning bool.
pub fn (win &SimpleWindow) on_close_requested(callback CloseRequestedCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.on_close_requested_fn = callback
	}
	return win
}

// can_close evaluates whether the window is permitted to close based on on_close_requested callback.
pub fn (win &SimpleWindow) can_close() bool {
	if win.on_close_requested_fn != unsafe { nil } {
		unsafe {
			mut w := &SimpleWindow(win)
			return w.on_close_requested_fn(mut w)
		}
	}
	return true
}

// ── Appearance Override ──────────────────────────────────────────────────────

// set_window_appearance overrides the window appearance to "dark", "light", or "auto" (system default).
// This lets you force dark/light mode on a per-window basis regardless of system preference.
pub fn (win &SimpleWindow) set_window_appearance(appearance string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_window_appearance(win.window_info, appearance.str)
	}
	return win
}

// get_window_appearance returns the current window appearance override: "dark", "light", or "auto".
pub fn (win &SimpleWindow) get_window_appearance() string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_window_appearance(win.window_info)
		if res != unsafe { nil } {
			return unsafe { tos3(res) }
		}
	}
	return 'auto'
}

// ── Screen Info ──────────────────────────────────────────────────────────────

// get_screen_full_frame returns the full physical frame of the screen containing this window.
// Returns (x, y, width, height).
pub fn (win &SimpleWindow) get_screen_full_frame() (int, int, int, int) {
	if win.window_info != unsafe { nil } {
		mut x, mut y, mut w, mut h := 0, 0, 0, 0
		C.window_get_screen_full_frame(win.window_info, &x, &y, &w, &h)
		return x, y, w, h
	}
	return 0, 0, 0, 0
}

// get_screen_scale_factor returns the Retina display scale factor (1.0 for standard, 2.0+ for Retina).
pub fn (win &SimpleWindow) get_screen_scale_factor() f64 {
	if win.window_info != unsafe { nil } {
		return C.window_get_screen_scale_factor(win.window_info)
	}
	return 1.0
}

// ── Cursor Control ────────────────────────────────────────────────────────────

// set_cursor_hidden hides or shows the macOS system cursor.
// Note: This is application-wide — be sure to restore visibility when done.
pub fn (win &SimpleWindow) set_cursor_hidden(hidden bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_cursor_hidden(win.window_info, if hidden { 1 } else { 0 })
	}
	return win
}

// set_cursor changes the cursor icon shown over the window content.
// Supported names: 'arrow', 'ibeam'/'text', 'crosshair', 'pointing_hand'/'hand',
// 'open_hand', 'closed_hand', 'resize_left', 'resize_right', 'resize_left_right',
// 'resize_up', 'resize_down', 'resize_up_down', 'drag_copy', 'drag_link',
// 'operation_not_allowed'/'not_allowed', 'context_menu', 'disappearing_item'/'poof',
// 'ibeam_vertical'. The cursor persists while the mouse is over the window.
pub fn (win &SimpleWindow) set_cursor(cursor_name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_cursor(win.window_info, cursor_name.str)
	}
	return win
}

// get_cursor returns the name of the currently active cursor set via set_cursor,
// or 'arrow' if no custom cursor is active.
pub fn (win &SimpleWindow) get_cursor() string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_cursor(win.window_info)
		if res != unsafe { nil } {
			return unsafe { tos3(res) }
		}
	}
	return 'arrow'
}

// set_cursor_size scales the cursor icon (1.0 = system size, 2.0 = double, etc.).
// The scale is clamped to 0.25–8.0 and applies to cursors set via set_cursor,
// push_cursor, and set_control_cursor.
pub fn (win &SimpleWindow) set_cursor_size(scale f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_cursor_scale(win.window_info, scale)
	}
	return win
}

// get_cursor_size returns the current cursor scale factor (1.0 = system size).
pub fn (win &SimpleWindow) get_cursor_size() f64 {
	if win.window_info != unsafe { nil } {
		return C.window_get_cursor_scale(win.window_info)
	}
	return 1.0
}

// reset_cursor restores the default arrow cursor at system size and clears
// any cursor icon and scale set via set_cursor / set_cursor_size.
pub fn (win &SimpleWindow) reset_cursor() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_reset_cursor(win.window_info)
	}
	return win
}

// push_cursor temporarily pushes a cursor onto the system cursor stack.
// Use pop_cursor to restore the previous cursor.
pub fn (win &SimpleWindow) push_cursor(cursor_name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_push_cursor(win.window_info, cursor_name.str)
	}
	return win
}

// pop_cursor restores the cursor that was active before the last push_cursor.
pub fn (win &SimpleWindow) pop_cursor() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_pop_cursor(win.window_info)
	}
	return win
}

// set_control_cursor assigns a cursor icon shown while hovering a specific control.
// Pass '' or 'default' as cursor_name to remove the assignment.
pub fn (win &SimpleWindow) set_control_cursor(name string, cursor_name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_control_cursor_by_name(win.window_info, name.str, cursor_name.str)
	}
	return win
}

// get_mouse_location returns the current mouse location in global screen coordinates.
pub fn (win &SimpleWindow) get_mouse_location() (int, int) {
	if win.window_info != unsafe { nil } {
		mut x, mut y := 0, 0
		C.window_get_mouse_location(win.window_info, &x, &y)
		return x, y
	}
	return 0, 0
}

// move_cursor_to warps the mouse cursor to the given global screen coordinates
// (bottom-left origin, same coordinate space as get_mouse_location).
pub fn (win &SimpleWindow) move_cursor_to(x int, y int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_move_cursor_to(win.window_info, x, y)
	}
	return win
}

// ── Resize Indicator ──────────────────────────────────────────────────────────

// set_shows_resize_indicator shows or hides the bottom-right resize grip on the window.
pub fn (win &SimpleWindow) set_shows_resize_indicator(show bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_shows_resize_indicator(win.window_info, if show { 1 } else { 0 })
	}
	return win
}

// get_shows_resize_indicator returns true if the resize indicator (grip) is visible.
pub fn (win &SimpleWindow) get_shows_resize_indicator() bool {
	if win.window_info != unsafe { nil } {
		return C.window_get_shows_resize_indicator(win.window_info) == 1
	}
	return true
}

// ── Content Size Constraints ──────────────────────────────────────────────────

// set_content_min_size sets the minimum content area size (excluding titlebar height).
// This is more precise than set_min_size which includes the full window frame.
pub fn (win &SimpleWindow) set_content_min_size(width int, height int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_content_min_size(win.window_info, width, height)
	}
	return win
}

// set_content_max_size sets the maximum content area size. Use 0 for unconstrained.
pub fn (win &SimpleWindow) set_content_max_size(width int, height int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_content_max_size(win.window_info, width, height)
	}
	return win
}

// get_content_min_size returns the current content area minimum size as (width, height).
pub fn (win &SimpleWindow) get_content_min_size() (int, int) {
	if win.window_info != unsafe { nil } {
		mut w, mut h := 0, 0
		C.window_get_content_min_size(win.window_info, &w, &h)
		return w, h
	}
	return 0, 0
}

// get_content_max_size returns the current content area maximum size as (width, height).
// Returns (0, 0) if unconstrained.
pub fn (win &SimpleWindow) get_content_max_size() (int, int) {
	if win.window_info != unsafe { nil } {
		mut w, mut h := 0, 0
		C.window_get_content_max_size(win.window_info, &w, &h)
		return w, h
	}
	return 0, 0
}

// ── Tab Count ─────────────────────────────────────────────────────────────────

// get_tab_count returns the number of tabs in the current window tab group.
// Returns 1 if the window has no tab group or tabbing is not available.
pub fn (win &SimpleWindow) get_tab_count() int {
	if win.window_info != unsafe { nil } {
		return C.window_get_tab_count(win.window_info)
	}
	return 1
}
