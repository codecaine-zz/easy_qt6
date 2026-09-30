#ifndef WINDOW_H
#define WINDOW_H
#include <stdint.h>

typedef struct main__WindowParams {
  const char *title;
  int64_t width;
  int64_t height;
  void *win_ptr;
  int64_t padding;
  int64_t spacing;
  int64_t always_on_top;
  int64_t responsive_layout;
  int64_t resizable;
  int64_t minimizable;
  int64_t maximizable;
  int64_t closable;
  int64_t has_shadow;
  int64_t movable_by_window_background;
  int64_t titlebar_visible;
  int64_t title_visible;
} main__WindowParams;

typedef struct main__WindowInfo {
  void *app;
  void *app_delegate;
} main__WindowInfo;

main__WindowInfo *window_app_init(void *params);
void window_app_run(main__WindowInfo *info);
void window_app_exit(main__WindowInfo *info);
void window_set_input_text(main__WindowInfo *info, const char *text);
void window_set_text_area(main__WindowInfo *info, const char *text);
void window_set_status_text(main__WindowInfo *info, const char *text);
void window_set_title_text(main__WindowInfo *info, const char *text);
void window_set_always_on_top(main__WindowInfo *info, int enabled);
int window_get_always_on_top(main__WindowInfo *info);
void window_set_checkbox_state(main__WindowInfo *info, int checked);
void window_set_number_value(main__WindowInfo *info, int value);
void window_set_background_color(main__WindowInfo *info, const char *color);
void window_set_font_color(main__WindowInfo *info, const char *color);
void window_set_control_background_color_by_name(main__WindowInfo *info, const char *name, const char *color);
void window_set_control_font_color_by_name(main__WindowInfo *info, const char *name, const char *color);
void window_set_control_width_by_name(main__WindowInfo *info, const char *name, int width);
void window_set_control_height_by_name(main__WindowInfo *info, const char *name, int height);
void window_set_control_font_size_by_name(main__WindowInfo *info, const char *name, int size);
void window_set_control_font_bold_by_name(main__WindowInfo *info, const char *name, int bold);
void window_set_control_font_name_by_name(main__WindowInfo *info, const char *name, const char *font_name);
int window_show_choice_dialog(main__WindowInfo *info, const char *title, const char *message, const char **choices, int choices_count);
void window_add_context_menu_item(main__WindowInfo *info, const char *control_name, const char *item_title, const char *handler_name);
void window_set_padding(main__WindowInfo *info, int padding);
void window_set_spacing(main__WindowInfo *info, int spacing);
void window_set_responsive_layout(main__WindowInfo *info, int enabled);
void *window_add_group_box_control(main__WindowInfo *info, const char *name, const char *title);
void *window_add_group_box_control_with_options(main__WindowInfo *info, const char *name, const char *title, int show_border);
void *window_add_group_box_control_with_config(main__WindowInfo *info, const char *name, const char *title, int show_border, float border_width, const char *border_color, float corner_radius, const char *bg_color, int padding, int shadow, int show_caption, const char *caption_color, const char *caption_alignment);
void window_begin_group_box(main__WindowInfo *info, const char *name, const char *title, int show_border);
void window_begin_group_box_with_config(main__WindowInfo *info, const char *name, const char *title, int show_border, float border_width, const char *border_color, float corner_radius, const char *bg_color, int padding, int shadow, int show_caption, const char *caption_color, const char *caption_alignment);
void window_end_group_box(main__WindowInfo *info);
void window_set_group_border(main__WindowInfo *info, const char *name, int show_border);
void window_set_group_caption(main__WindowInfo *info, const char *name, const char *caption);
void window_set_group_style(main__WindowInfo *info, const char *name, int show_border, float border_width, const char *border_color, float corner_radius, const char *bg_color, int shadow);
void *window_add_tabs_control(main__WindowInfo *info, const char *name, const char **titles, int titles_count);
void *window_add_scroll_view_control(main__WindowInfo *info, const char *name, int height);
void window_focus_control(main__WindowInfo *info, const char *name);
void window_set_placeholder_by_name(main__WindowInfo *info, const char *name, const char *text);
void window_set_error_by_name(main__WindowInfo *info, const char *name, const char *text);
void window_set_tooltip_by_name(main__WindowInfo *info, const char *name, const char *text);
void window_set_default_button_by_name(main__WindowInfo *info, const char *name);
void window_run_after(main__WindowInfo *info, int ms, const char *handler_name);
void window_show_toast(main__WindowInfo *info, const char *message);
void window_open_url(main__WindowInfo *info, const char *url);
void window_copy_to_clipboard(main__WindowInfo *info, const char *text);
char *window_get_clipboard_text(void);
int window_reveal_in_finder(const char *path);

// Name-based generic control accessors
void window_set_control_text_by_name(main__WindowInfo *info, const char *name, const char *text);
char *window_get_control_text_by_name(main__WindowInfo *info, const char *name);
void window_set_control_bool_by_name(main__WindowInfo *info, const char *name, int checked);
int window_get_control_bool_by_name(main__WindowInfo *info, const char *name);
void window_set_control_int_by_name(main__WindowInfo *info, const char *name, int value);
int window_get_control_int_by_name(main__WindowInfo *info, const char *name);

// Dynamic control creation bridges
void *window_add_label_control(main__WindowInfo *info, const char *name, const char *text);
void *window_add_input_control(main__WindowInfo *info, const char *name, const char *value);
void *window_add_password_control(main__WindowInfo *info, const char *name, const char *value);
void *window_add_textarea_control(main__WindowInfo *info, const char *name, const char *value);
void window_textarea_goto_line(main__WindowInfo *info, const char *name, int line_number, int focus);
void *window_add_html_view_control(main__WindowInfo *info, const char *name, const char *html);
void *window_add_drop_zone_control(main__WindowInfo *info, const char *name, const char *label);
void *window_add_checkbox_control(main__WindowInfo *info, const char *name, const char *text, int checked);
void *window_add_radio_control(main__WindowInfo *info, const char *name, const char *text, int checked);
void *window_add_button_control(main__WindowInfo *info, const char *name, const char *text);
void *window_add_number_control(main__WindowInfo *info, const char *name, int value);
void *window_add_slider_control(main__WindowInfo *info, const char *name, int value);
void *window_add_theme_menu_control(main__WindowInfo *info, const char *name, const char *selected);
void *window_add_color_well_control(main__WindowInfo *info, const char *name, const char *color);
void *window_add_date_picker_control(main__WindowInfo *info, const char *name, const char *date);
void *window_add_date_time_picker_control(main__WindowInfo *info, const char *name, const char *datetime);
void *window_add_mode_control_control(main__WindowInfo *info, const char *name, const char *selected);
void *window_add_progress_indicator_control(main__WindowInfo *info, const char *name, int value);

// Layout row & container groupings
void window_begin_row(main__WindowInfo *info, const char *name);
void window_end_row(main__WindowInfo *info);
void window_begin_grid(main__WindowInfo *info, const char *name, int columns, int spacing);
void window_end_grid(main__WindowInfo *info);
void window_begin_flex_box(main__WindowInfo *info, const char *name, const char *direction, const char *justify, const char *align);
void window_end_flex_box(main__WindowInfo *info);
void window_set_control_alignment_by_name(main__WindowInfo *info, const char *name, const char *alignment);
void window_set_control_expand_fill_by_name(main__WindowInfo *info, const char *name, int expand);

// Popups and Dialogs
void window_show_alert(main__WindowInfo *info, const char *title, const char *message);
void window_show_alert_with_style(main__WindowInfo *info, const char *title, const char *message, const char *style);
int window_show_confirm(main__WindowInfo *info, const char *title, const char *message);
char *window_show_prompt(main__WindowInfo *info, const char *title, const char *message, const char *default_val);

// File and Folder Pickers
char *window_select_file(main__WindowInfo *info);
char *window_select_file_with_extensions(main__WindowInfo *info, const char *extensions);
char *window_select_folder(main__WindowInfo *info);
char *window_save_file_picker(main__WindowInfo *info);

// Visibility, Enabled, and Visual Highlight states
void window_set_control_visible_by_name(main__WindowInfo *info, const char *name, int visible);
int window_get_control_visible_by_name(main__WindowInfo *info, const char *name);
void window_set_control_enabled_by_name(main__WindowInfo *info, const char *name, int enabled);
int window_get_control_enabled_by_name(main__WindowInfo *info, const char *name);
void window_highlight_control_by_name(main__WindowInfo *info, const char *name, int duration_ms);
void window_flash_control_by_name(main__WindowInfo *info, const char *name);

// External Applications Accessibility Inspection (macOS AXUIElement)
char *window_list_external_apps(void);
char *window_spy_external_app(int pid);
int window_set_external_control_value(int pid, const char *title_or_role, const char *value);
int window_press_external_control(int pid, const char *title_or_role);
int window_set_external_control_enabled(int pid, const char *title_or_role, int enabled);
int window_set_external_control_visible(int pid, const char *title_or_role, int visible);
int window_flash_external_control(int pid, const char *title_or_role);

// Timers
void window_set_interval(main__WindowInfo *info, int ms, const char *timer_name);
void window_stop_interval(main__WindowInfo *info, const char *timer_name);

// List Box and Image View Controls
void *window_add_list_box_control(main__WindowInfo *info, const char *name, const char **items, int items_count);
void window_update_list_items(main__WindowInfo *info, const char *name, const char **items, int items_count);
void window_set_list_selected(main__WindowInfo *info, const char *name, int index);
int window_get_list_selected(main__WindowInfo *info, const char *name);
void window_set_list_multi_select(main__WindowInfo *info, const char *name, int enabled);
char *window_get_list_selected_indexes(main__WindowInfo *info, const char *name);
void window_set_list_selected_indexes(main__WindowInfo *info, const char *name, const char *csv_indexes);
void window_select_all_list_items(main__WindowInfo *info, const char *name);
void window_clear_list_selection(main__WindowInfo *info, const char *name);
void *window_add_image_control(main__WindowInfo *info, const char *name, const char *file_path);
void window_set_image_path(main__WindowInfo *info, const char *name, const char *file_path);

// Hover Event Tracking
void window_enable_hover_events(main__WindowInfo *info, const char *name);

// Menu Customization
void window_add_menu_item(main__WindowInfo *info, const char *menu_name, const char *item_title, const char *shortcut, const char *handler_name);

// Spacers and Separators
void window_add_vertical_spacer(main__WindowInfo *info, int height);
void window_add_horizontal_spacer(main__WindowInfo *info, int width);
void window_add_separator(main__WindowInfo *info);

// Multi-Column Table Controls
void *window_add_table_control(main__WindowInfo *info, const char *name, const char **columns, int columns_count);
void window_set_table_rows(main__WindowInfo *info, const char *name, const char **flat_items, int total_count, int columns_count);
void window_table_set_column_selection(main__WindowInfo *info, const char *name, int enabled);
int window_table_get_selected_column(main__WindowInfo *info, const char *name);
void window_table_set_selected_column(main__WindowInfo *info, const char *name, int col_idx);
void window_table_delete_column(main__WindowInfo *info, const char *name, int col_idx);

// Tree View Controls
void *window_add_tree_view_control(main__WindowInfo *info, const char *name, int height);
void window_set_tree_nodes(main__WindowInfo *info, const char *name, const char **flat_items, int total_count);
char *window_get_tree_selected(main__WindowInfo *info, const char *name);
void window_set_tree_selected(main__WindowInfo *info, const char *name, const char *node_id);
void window_tree_expand_all(main__WindowInfo *info, const char *name);
void window_tree_collapse_all(main__WindowInfo *info, const char *name);
void window_tree_expand_node(main__WindowInfo *info, const char *name, const char *node_id, int expand_children);
void window_tree_collapse_node(main__WindowInfo *info, const char *name, const char *node_id, int collapse_children);

// System Menu Bar/Tray App Mode
void window_enable_status_bar(main__WindowInfo *info, const char *icon_path);
void window_show(main__WindowInfo *info);

// Thread Safety Runner
void window_run_on_main_thread(void *callback_fn, void *context);
void window_run_on_main_thread_sync(void *callback_fn, void *context);

// New general-purpose controls
void *window_add_dropdown_control(main__WindowInfo *info, const char *name, const char **items, int items_count, const char *selected);
void *window_add_segmented_control_custom(main__WindowInfo *info, const char *name, const char **items, int items_count, const char *selected);
void *window_add_radio_group_control(main__WindowInfo *info, const char *name, const char **items, int items_count, const char *selected);
void *window_add_switch_control(main__WindowInfo *info, const char *name, const char *label, int checked);
void *window_add_search_field_control(main__WindowInfo *info, const char *name, const char *placeholder);
void *window_add_combo_box_control(main__WindowInfo *info, const char *name, const char **items, int items_count, const char *selected);
void *window_add_level_indicator_control(main__WindowInfo *info, const char *name, int style, int min_val, int max_val, int value);
void *window_add_spinner_control(main__WindowInfo *info, const char *name, int active);
void *window_add_path_control(main__WindowInfo *info, const char *name, const char *path);
void *window_add_token_field_control(main__WindowInfo *info, const char *name, const char *value);

// Window constraints and behavior options
void window_set_min_size(main__WindowInfo *info, int width, int height);
void window_set_max_size(main__WindowInfo *info, int width, int height);
void window_set_resizable(main__WindowInfo *info, int enabled);
void window_set_minimizable(main__WindowInfo *info, int enabled);
void window_set_maximizable(main__WindowInfo *info, int enabled);

// Additional Window Operations
void window_close(main__WindowInfo *info);
void window_hide(main__WindowInfo *info);
void window_center(main__WindowInfo *info);
void window_align(main__WindowInfo *info, const char *alignment);
void window_set_size(main__WindowInfo *info, int width, int height);
int window_get_width(main__WindowInfo *info);
int window_get_height(main__WindowInfo *info);
void window_set_position(main__WindowInfo *info, int x, int y);
int window_get_x(main__WindowInfo *info);
int window_get_y(main__WindowInfo *info);
void window_set_opacity(main__WindowInfo *info, double opacity);
double window_get_opacity(main__WindowInfo *info);
void window_toggle_fullscreen(main__WindowInfo *info);
void window_minimize(main__WindowInfo *info);
void window_deminimize(main__WindowInfo *info);
void window_maximize(main__WindowInfo *info);
int window_is_minimized(main__WindowInfo *info);
int window_is_maximized(main__WindowInfo *info);
int window_is_fullscreen(main__WindowInfo *info);
int window_is_active(main__WindowInfo *info);
void window_set_titlebar_visible(main__WindowInfo *info, int visible);
void window_request_attention(main__WindowInfo *info, int critical);

void window_set_closable(main__WindowInfo *info, int enabled);
int window_get_closable(main__WindowInfo *info);
void window_set_has_shadow(main__WindowInfo *info, int enabled);
int window_get_has_shadow(main__WindowInfo *info);
void window_set_movable_by_window_background(main__WindowInfo *info, int enabled);
int window_get_movable_by_window_background(main__WindowInfo *info);
int window_is_visible(main__WindowInfo *info);
void window_set_title_visible(main__WindowInfo *info, int visible);
int window_get_title_visible(main__WindowInfo *info);
int window_get_titlebar_visible(main__WindowInfo *info);

void window_deliver_notification(const char *title, const char *message);
void window_set_dock_badge(const char *text);
void window_set_slider_range(main__WindowInfo *info, const char *name, double min_val, double max_val);
void *window_add_link_control(main__WindowInfo *info, const char *name, const char *text, const char *url);
void window_beep();

void *window_add_disclosure_control(main__WindowInfo *info, const char *name, const char *title, int open);
void window_enable_search_history(main__WindowInfo *info, const char *name, const char *autosave_name);
void window_set_status_bar_icon(main__WindowInfo *info, const char *icon_path);
void window_set_status_bar_title(main__WindowInfo *info, const char *title);
void window_set_dock_icon(const char *image_path);
void window_play_system_sound(const char *sound_name);

void *window_add_stepper_control(main__WindowInfo *info, const char *name, double min_val, double max_val, double step, double value);
void *window_add_help_button_control(main__WindowInfo *info, const char *name);
void *window_add_knob_control(main__WindowInfo *info, const char *name, double min_val, double max_val, double value);
void *window_add_pull_down_control(main__WindowInfo *info, const char *name, const char *title, const char **items, int items_count);
void *window_add_image_button_control(main__WindowInfo *info, const char *name, const char *symbol, const char *title);

// Animations and Transition Helpers
void window_animate_control_opacity(main__WindowInfo *info, const char *name, double opacity, int duration_ms);
void window_animate_opacity(main__WindowInfo *info, double opacity, int duration_ms);
void window_animate_control_shake(main__WindowInfo *info, const char *name);
void window_shake(main__WindowInfo *info);
void window_animate_control_width(main__WindowInfo *info, const char *name, int width, int duration_ms);
void window_animate_control_height(main__WindowInfo *info, const char *name, int height, int duration_ms);
void window_animate_control_size(main__WindowInfo *info, const char *name, int width, int height, int duration_ms);
void window_animate_size(main__WindowInfo *info, int width, int height, int duration_ms);
void window_animate_position(main__WindowInfo *info, int x, int y, int duration_ms);
void window_animate_bounds(main__WindowInfo *info, int x, int y, int width, int height, int duration_ms);

// Native macOS UI extensions
void window_add_toolbar_item(main__WindowInfo *info, const char *name, const char *label, const char *tooltip, const char *symbol);
void window_add_toolbar_space(main__WindowInfo *info);
void window_add_toolbar_flexible_space(main__WindowInfo *info);
void window_set_toolbar_style(main__WindowInfo *info, const char *style);
void window_show_sheet_alert(main__WindowInfo *info, const char *title, const char *message, const char *style);
void window_add_dock_menu_item(main__WindowInfo *info, const char *title, const char *handler_name);

// New controls added in showcase
void window_begin_split_view(main__WindowInfo *info, const char *name, int vertical);
void window_split_view_next_pane(main__WindowInfo *info);
void window_end_split_view(main__WindowInfo *info);
void *window_add_collection_view_control(main__WindowInfo *info, const char *name, int item_width, int item_height);
void window_set_collection_items(main__WindowInfo *info, const char *name, const char **items, int items_count);
void window_show_popover(main__WindowInfo *info, const char *anchor_name, const char *title, const char *message);
void *window_add_calendar_control(main__WindowInfo *info, const char *name, const char *date);
void *window_add_canvas_control(main__WindowInfo *info, const char *name, int height);
void window_draw_line(main__WindowInfo *info, const char *canvas_name, double x1, double y1, double x2, double y2, const char *color_str, double stroke_width);
void window_draw_rect(main__WindowInfo *info, const char *canvas_name, double x, double y, double w, double h, const char *color_str, int fill, double stroke_width);
void window_draw_circle(main__WindowInfo *info, const char *canvas_name, double x, double y, double r, const char *color_str, int fill, double stroke_width);
void window_clear_canvas(main__WindowInfo *info, const char *canvas_name);

// Glass, Badge, Icon Segment controls
void window_begin_glass_box(main__WindowInfo *info, const char *name, const char *material);
void window_end_glass_box(main__WindowInfo *info);
void *window_add_badge_control(main__WindowInfo *info, const char *name, const char *text, const char *style);
void *window_add_icon_segments_control(main__WindowInfo *info, const char *name, const char **symbols, int symbols_count, const char *selected);

// Developer-oriented Custom controls
void *window_add_console_control(main__WindowInfo *info, const char *name, int height);
void window_append_console_text(main__WindowInfo *info, const char *name, const char *text, int level);
void window_clear_console(main__WindowInfo *info, const char *name);
void *window_add_chart_control(main__WindowInfo *info, const char *name, const char *chart_type, int height);
void window_set_chart_data(main__WindowInfo *info, const char *name, const double *values, int count);
void *window_add_shortcut_recorder_control(main__WindowInfo *info, const char *name);
void *window_add_circular_progress_control(main__WindowInfo *info, const char *name, double value, double min_val, double max_val);
void window_set_circular_progress_value(main__WindowInfo *info, const char *name, double value);
void *window_add_breadcrumbs_control(main__WindowInfo *info, const char *name, const char **segments, int count);
void window_set_breadcrumbs(main__WindowInfo *info, const char *name, const char **segments, int count);
void *window_add_property_grid_control(main__WindowInfo *info, const char *name, const char **keys, const char **values, int count);
void window_set_property_grid_value(main__WindowInfo *info, const char *name, const char *key, const char *value);
void *window_add_color_grid_control(main__WindowInfo *info, const char *name, const char **colors, int count);
void window_set_color_grid_selected(main__WindowInfo *info, const char *name, const char *color);
void *window_add_grid_control(main__WindowInfo *info, const char *name, const char **headers, int headers_count);
void window_grid_add_row(main__WindowInfo *info, const char *name, const char **values, int count);
void window_grid_delete_row(main__WindowInfo *info, const char *name, int row_idx);
void window_grid_add_column(main__WindowInfo *info, const char *name, const char *header);
void window_grid_delete_column(main__WindowInfo *info, const char *name, int col_idx);
void window_grid_set_cell(main__WindowInfo *info, const char *name, int row, int col, const char *value);
const char *window_grid_get_cell(main__WindowInfo *info, const char *name, int row, int col);
int window_grid_get_selected_row(main__WindowInfo *info, const char *name);
int window_grid_get_selected_column(main__WindowInfo *info, const char *name);
void window_grid_set_selected_column(main__WindowInfo *info, const char *name, int col_idx);
void window_grid_set_selected_cell(main__WindowInfo *info, const char *name, int row_idx, int col_idx);
int window_grid_get_column_editable(main__WindowInfo *info, const char *name, int col_idx);
int window_grid_get_row_editable(main__WindowInfo *info, const char *name, int row_idx);
int window_grid_get_cell_editable(main__WindowInfo *info, const char *name, int row, int col);
int window_grid_get_column_enabled(main__WindowInfo *info, const char *name, int col_idx);
int window_grid_get_row_enabled(main__WindowInfo *info, const char *name, int row_idx);
int window_grid_get_cell_enabled(main__WindowInfo *info, const char *name, int row, int col);
const char *window_grid_get_filter(main__WindowInfo *info, const char *name);
int window_grid_get_row_count(main__WindowInfo *info, const char *name);
int window_grid_get_column_count(main__WindowInfo *info, const char *name);
int window_grid_get_row_values(main__WindowInfo *info, const char *name, int row_idx, const char **values, int capacity);
void window_grid_set_column_type(main__WindowInfo *info, const char *name, int col_idx, const char *col_type);
void window_grid_set_column_width(main__WindowInfo *info, const char *name, int col_idx, int width);
void window_grid_set_row_height(main__WindowInfo *info, const char *name, int height);
void window_grid_sort_by_column(main__WindowInfo *info, const char *name, int col_idx, int ascending);
void window_grid_set_filter(main__WindowInfo *info, const char *name, const char *query);
void window_grid_clear_filter(main__WindowInfo *info, const char *name);
void window_grid_autosize_columns(main__WindowInfo *info, const char *name);
void window_grid_set_selected_row(main__WindowInfo *info, const char *name, int row_idx);
void window_grid_clear(main__WindowInfo *info, const char *name);
void window_grid_set_column_editable(main__WindowInfo *info, const char *name, int col_idx, int editable);
void window_grid_set_row_editable(main__WindowInfo *info, const char *name, int row_idx, int editable);
void window_grid_set_cell_editable(main__WindowInfo *info, const char *name, int row, int col, int editable);
void window_grid_set_column_enabled(main__WindowInfo *info, const char *name, int col_idx, int enabled);
void window_grid_set_row_enabled(main__WindowInfo *info, const char *name, int row_idx, int enabled);
void window_grid_set_cell_enabled(main__WindowInfo *info, const char *name, int row, int col, int enabled);

// New controls added
void *window_add_stat_card_control(main__WindowInfo *info, const char *name, const char *title, const char *value, const char *trend, const char *trend_style);
void window_set_stat_card_value(main__WindowInfo *info, const char *name, const char *value, const char *trend, const char *trend_style);
void *window_add_banner_control(main__WindowInfo *info, const char *name, const char *text, const char *style);
void window_set_banner_value(main__WindowInfo *info, const char *name, const char *text);
void *window_add_section_header_control(main__WindowInfo *info, const char *name, const char *title, const char *subtitle);
void *window_add_vertical_slider_control(main__WindowInfo *info, const char *name, int value, int min_val, int max_val, int height);
void window_set_vertical_slider_value(main__WindowInfo *info, const char *name, int value);
void *window_add_chip_group_control(main__WindowInfo *info, const char *name, const char **chips, int count, const char *selected);
void window_set_chip_group_selected(main__WindowInfo *info, const char *name, const char *selected);

void window_set_badge_value(main__WindowInfo *info, const char *name, const char *text, const char *style);

void *window_add_status_indicator_control(main__WindowInfo *info, const char *name, const char *label, const char *status);
void window_set_status_indicator_value(main__WindowInfo *info, const char *name, const char *status);

void *window_add_metric_meter_control(main__WindowInfo *info, const char *name, const char *title, int value, int min_val, int max_val, const char *unit);
void window_set_metric_meter_value(main__WindowInfo *info, const char *name, int value);

void *window_add_avatar_card_control(main__WindowInfo *info, const char *name, const char *title, const char *subtitle, const char *status);
void window_set_avatar_card_value(main__WindowInfo *info, const char *name, const char *title, const char *subtitle, const char *status);

void *window_add_time_picker_control(main__WindowInfo *info, const char *name, const char *time);
void window_set_time_picker_value(main__WindowInfo *info, const char *name, const char *time);
const char *window_get_time_picker_value(main__WindowInfo *info, const char *name);

void window_add_tray_icon_control(main__WindowInfo *info, const char *name, const char *symbol, const char *title);
void window_set_tray_icon_value(main__WindowInfo *info, const char *name, const char *symbol, const char *title);

void *window_add_collapsible_section_control(main__WindowInfo *info, const char *name, const char *title, int expanded);
void window_set_collapsible_section_expanded(main__WindowInfo *info, const char *name, int expanded);

void *window_add_code_editor_control(main__WindowInfo *info, const char *name, const char *code, int height);
void window_set_code_editor_value(main__WindowInfo *info, const char *name, const char *code);
const char *window_get_code_editor_value(main__WindowInfo *info, const char *name);

void *window_add_timeline_view_control(main__WindowInfo *info, const char *name, int height);
void window_add_timeline_entry(main__WindowInfo *info, const char *name, const char *time_str, const char *title, const char *detail, const char *style);
void window_clear_timeline(main__WindowInfo *info, const char *name);

void window_add_toolbar_button(main__WindowInfo *info, const char *id_str, const char *label, const char *symbol);
void window_set_toolbar_visible(main__WindowInfo *info, int visible);

// New Window Commands
void window_set_subtitle(main__WindowInfo *info, const char *subtitle);
const char *window_get_subtitle(main__WindowInfo *info);
void window_set_titlebar_appears_transparent(main__WindowInfo *info, int transparent);
int window_get_titlebar_appears_transparent(main__WindowInfo *info);
void window_set_full_size_content_view(main__WindowInfo *info, int enabled);
int window_get_full_size_content_view(main__WindowInfo *info);
void window_set_movable(main__WindowInfo *info, int enabled);
int window_get_movable(main__WindowInfo *info);
void window_set_window_level(main__WindowInfo *info, const char *level);
void window_set_aspect_ratio(main__WindowInfo *info, double width_ratio, double height_ratio);
void window_reset_aspect_ratio(main__WindowInfo *info);
void window_bounce_dock_icon(int critical);

// New Controls (Rating, Range Slider, Split Button, Tag Cloud, Wizard Stepper)
void *window_add_rating_control(main__WindowInfo *info, const char *name, int value, int max_stars);
void window_set_rating_value(main__WindowInfo *info, const char *name, int value);
int window_get_rating_value(main__WindowInfo *info, const char *name);

void *window_add_range_slider_control(main__WindowInfo *info, const char *name, int min_val, int max_val, int low_val, int high_val);
void window_set_range_slider_values(main__WindowInfo *info, const char *name, int low_val, int high_val);
int window_get_range_slider_low(main__WindowInfo *info, const char *name);
int window_get_range_slider_high(main__WindowInfo *info, const char *name);

void *window_add_split_button_control(main__WindowInfo *info, const char *name, const char *title, const char **menu_items, int count);

void *window_add_tag_cloud_control(main__WindowInfo *info, const char *name, const char **tags, int count);
void window_set_tag_cloud_tags(main__WindowInfo *info, const char *name, const char **tags, int count);

void *window_add_wizard_stepper_control(main__WindowInfo *info, const char *name, const char **steps, int count, int current_step);
void window_set_wizard_stepper_step(main__WindowInfo *info, const char *name, int step);

// Additional Controls (Gauge, Pagination, Activity Feed, Markdown View, Sparkline, PIN Code, Color Palette)
void *window_add_gauge_control(main__WindowInfo *info, const char *name, const char *title, int value, int min_val, int max_val, const char *unit);
void window_set_gauge_value(main__WindowInfo *info, const char *name, int value);
int window_get_gauge_value(main__WindowInfo *info, const char *name);

void *window_add_pagination_control(main__WindowInfo *info, const char *name, int total_pages, int current_page);
void window_set_pagination_page(main__WindowInfo *info, const char *name, int page, int total_pages);
int window_get_pagination_page(main__WindowInfo *info, const char *name);

void *window_add_activity_feed_control(main__WindowInfo *info, const char *name, int height);
void window_add_activity_feed_item(main__WindowInfo *info, const char *name, const char *timestamp, const char *message, const char *level);
void window_clear_activity_feed(main__WindowInfo *info, const char *name);

void *window_add_markdown_view_control(main__WindowInfo *info, const char *name, const char *markdown_text, int height);
void window_set_markdown_view_text(main__WindowInfo *info, const char *name, const char *markdown_text);
const char *window_get_markdown_view_text(main__WindowInfo *info, const char *name);

void *window_add_sparkline_control(main__WindowInfo *info, const char *name, const double *values, int count, int height);
void window_set_sparkline_data(main__WindowInfo *info, const char *name, const double *values, int count);

void *window_add_pin_code_control(main__WindowInfo *info, const char *name, int digits);
void window_set_pin_code_value(main__WindowInfo *info, const char *name, const char *code);
const char *window_get_pin_code_value(main__WindowInfo *info, const char *name);

void *window_add_color_palette_control(main__WindowInfo *info, const char *name, const char **hex_colors, int count, const char *selected);
void window_set_color_palette_selected(main__WindowInfo *info, const char *name, const char *hex_color);
const char *window_get_color_palette_selected(main__WindowInfo *info, const char *name);

// Additional Controls (Timeline, Metric Card, Tab Pills, Transfer List, Audio Waveform, Rating Breakdown, Code View)
void *window_add_timeline_control(main__WindowInfo *info, const char *name, int height);
void window_add_timeline_item(main__WindowInfo *info, const char *name, const char *title, const char *subtitle, const char *time_str, const char *status);

void *window_add_metric_card_control(main__WindowInfo *info, const char *name, const char *title, const char *value, const char *change_badge, const char *subtitle);

void window_set_metric_card_value(main__WindowInfo *info, const char *name, const char *value, const char *change_badge);

void *window_add_tab_pills_control(main__WindowInfo *info, const char *name, const char **items, int count, const char *selected);
void window_set_tab_pills_active(main__WindowInfo *info, const char *name, const char *selected);
const char *window_get_tab_pills_active(main__WindowInfo *info, const char *name);

void *window_add_transfer_list_control(main__WindowInfo *info, const char *name, const char **available, int avail_count, const char **selected, int sel_count, bool multi_select);

const char **window_get_transfer_list_selected(main__WindowInfo *info, const char *name, int *out_count);

void *window_add_audio_waveform_control(main__WindowInfo *info, const char *name, const double *amplitudes, int count, int height);
void window_set_audio_waveform_data(main__WindowInfo *info, const char *name, const double *amplitudes, int count);

void *window_add_rating_breakdown_control(main__WindowInfo *info, const char *name, double avg_score, int total_reviews, const double *star_percentages, int count);
void window_set_rating_breakdown_data(main__WindowInfo *info, const char *name, double avg_score, int total_reviews, const double *star_percentages, int count);

void *window_add_code_view_control(main__WindowInfo *info, const char *name, const char *lang, const char *code_text, int height);
void window_set_code_view_text(main__WindowInfo *info, const char *name, const char *code_text);
const char *window_get_code_view_text(main__WindowInfo *info, const char *name);

// 6 New Useful Controls (Alert Banner, Step Tracker, Filter Chips, File Picker Field, Radial Gauge, Key Value Card)
void *window_add_alert_banner_control(main__WindowInfo *info, const char *name, const char *title, const char *message, const char *style);
void window_set_alert_banner_value(main__WindowInfo *info, const char *name, const char *title, const char *message, const char *style);

void *window_add_step_tracker_control(main__WindowInfo *info, const char *name, const char **steps, int count, int current_step);
void window_set_step_tracker_step(main__WindowInfo *info, const char *name, int step);
int window_get_step_tracker_step(main__WindowInfo *info, const char *name);

void *window_add_filter_chips_control(main__WindowInfo *info, const char *name, const char **chips, int count, const char **selected, int sel_count, bool multi_select);
void window_set_filter_chips_selected(main__WindowInfo *info, const char *name, const char **selected, int count);
const char *window_get_filter_chips_selected(main__WindowInfo *info, const char *name);

void *window_add_file_picker_field_control(main__WindowInfo *info, const char *name, const char *initial_path, const char *button_title, bool folder_only);
void window_set_file_picker_path(main__WindowInfo *info, const char *name, const char *path);
const char *window_get_file_picker_path(main__WindowInfo *info, const char *name);

void *window_add_radial_gauge_control(main__WindowInfo *info, const char *name, const char *title, double value, double min_val, double max_val, const char *unit);
void window_set_radial_gauge_value(main__WindowInfo *info, const char *name, double value);
double window_get_radial_gauge_value(main__WindowInfo *info, const char *name);

void *window_add_key_value_card_control(main__WindowInfo *info, const char *name, const char *title, const char **keys, const char **values, int count);
void window_set_key_value_card_data(main__WindowInfo *info, const char *name, const char **keys, const char **values, int count);

// 6 Useful Developer Controls (Diff View, JSON Tree, HTTP Request Card, Terminal View, Resource Monitor, Env Vars Editor)

void *window_add_diff_view_control(main__WindowInfo *info, const char *name, const char *old_text, const char *new_text, int height);
void window_set_diff_view_text(main__WindowInfo *info, const char *name, const char *old_text, const char *new_text);

void *window_add_json_tree_control(main__WindowInfo *info, const char *name, const char *json_str, int height);
void window_set_json_tree_data(main__WindowInfo *info, const char *name, const char *json_str);

void *window_add_http_request_card_control(main__WindowInfo *info, const char *name, const char *method, const char *url, int status_code, int response_time_ms);
void window_set_http_request_card_data(main__WindowInfo *info, const char *name, const char *method, const char *url, int status_code, int response_time_ms);

void *window_add_terminal_view_control(main__WindowInfo *info, const char *name, const char *prompt_text, int height);
void window_append_terminal_line(main__WindowInfo *info, const char *name, const char *line_text, int line_type);
void window_clear_terminal(main__WindowInfo *info, const char *name);

void *window_add_resource_monitor_control(main__WindowInfo *info, const char *name, int cpu_pct, int mem_pct, int disk_pct, int net_kbps);
void window_set_resource_monitor_metrics(main__WindowInfo *info, const char *name, int cpu_pct, int mem_pct, int disk_pct, int net_kbps);

void *window_add_env_vars_control(main__WindowInfo *info, const char *name, const char *title, const char **keys, const char **values, int count);
void window_set_env_vars_data(main__WindowInfo *info, const char *name, const char **keys, const char **values, int count);

// 6 Additional Useful UI Controls (Badge Button, Command Palette, Status Banner, Pill Toggle, Color Swatch Panel, Hotkey Badge)
void *window_add_badge_button_control(main__WindowInfo *info, const char *name, const char *title, int count, const char *badge_color);
void window_set_badge_button_count(main__WindowInfo *info, const char *name, int count);

void *window_add_command_palette_control(main__WindowInfo *info, const char *name, const char *placeholder, const char *shortcut_hint);
void window_set_command_palette_text(main__WindowInfo *info, const char *name, const char *text);

void *window_add_status_banner_control(main__WindowInfo *info, const char *name, const char *title, const char *message, const char *style_type);
void window_set_status_banner_text(main__WindowInfo *info, const char *name, const char *title, const char *message, const char *style_type);

void *window_add_pill_toggle_control(main__WindowInfo *info, const char *name, const char **options, int count, int selected_index);
void window_set_pill_toggle_selected(main__WindowInfo *info, const char *name, int selected_index);

void *window_add_color_swatch_panel_control(main__WindowInfo *info, const char *name, const char **hex_colors, int count, const char *selected_color);
void window_set_color_swatch_selected(main__WindowInfo *info, const char *name, const char *hex_color);

void *window_add_hotkey_badge_control(main__WindowInfo *info, const char *name, const char *shortcut_str, const char *description);
void window_set_hotkey_badge_shortcut(main__WindowInfo *info, const char *name, const char *shortcut_str, const char *description);

// 6 New High-Utility UI Controls (Quick Action Bar, Accordion Group, Segment Distribution Bar, Tag Input Field, Status Dock, Info Callout Card)
void *window_add_quick_action_bar_control(main__WindowInfo *info, const char *name, const char **labels, const char **symbols, int count);
void window_set_quick_action_enabled(main__WindowInfo *info, const char *name, int index, int enabled);

void *window_add_accordion_group_control(main__WindowInfo *info, const char *name, const char **section_titles, int count, int expanded_index);
void window_set_accordion_expanded(main__WindowInfo *info, const char *name, int index, int expanded);

void *window_add_segment_distribution_bar_control(main__WindowInfo *info, const char *name, const char **labels, const double *values, const char **hex_colors, int count, int height);
void window_set_segment_distribution_values(main__WindowInfo *info, const char *name, const double *values, int count);

void *window_add_tag_input_field_control(main__WindowInfo *info, const char *name, const char **tags, int count);
void window_set_tag_input_tags(main__WindowInfo *info, const char *name, const char **tags, int count);
const char *window_get_tag_input_tags(main__WindowInfo *info, const char *name);

void *window_add_status_dock_control(main__WindowInfo *info, const char *name, const char *status_text, const char *dot_color, const char *count_text);
void window_set_status_dock_info(main__WindowInfo *info, const char *name, const char *status_text, const char *dot_color, const char *count_text);

void *window_add_info_callout_control(main__WindowInfo *info, const char *name, const char *title, const char *message, const char *style_type, const char *button_text);
void window_set_info_callout_text(main__WindowInfo *info, const char *name, const char *title, const char *message);

// Native macOS Sharing & System Panels
void window_show_share_sheet(main__WindowInfo *info, const char **items, int count, const char *anchor_control);
void window_show_font_picker(main__WindowInfo *info, const char *target_control);
void window_preview_file(main__WindowInfo *info, const char *file_path);

// Native Column Browser Control (NSBrowser)
void *window_add_browser_control(main__WindowInfo *info, const char *name, int height);
void window_set_browser_column_items(main__WindowInfo *info, const char *name, int column, const char **items, int count);
int window_get_browser_selected_row(main__WindowInfo *info, const char *name, int column);
const char *window_get_browser_path(main__WindowInfo *info, const char *name);

// Modern Productivity & Visual Analytics Controls
void *window_add_hero_banner_control(main__WindowInfo *info, const char *name, const char *title, const char *subtitle, const char *button_text, const char *gradient_style);
void *window_add_activity_rings_control(main__WindowInfo *info, const char *name, const double *percentages, const char **hex_colors, int count, int size);
void window_set_activity_rings_values(main__WindowInfo *info, const char *name, const double *percentages, int count);
void *window_add_segmented_progress_control(main__WindowInfo *info, const char *name, const char **labels, const double *values, const char **hex_colors, int count, int height);
void *window_add_feedback_mood_control(main__WindowInfo *info, const char *name, int selected_mood);
void window_set_feedback_mood(main__WindowInfo *info, const char *name, int selected_mood);
int window_get_feedback_mood(main__WindowInfo *info, const char *name);
void *window_add_kanban_board_control(main__WindowInfo *info, const char *name, const char **columns, int col_count, int height);
void window_kanban_add_card(main__WindowInfo *info, const char *name, int col_idx, const char *card_title, const char *card_subtitle, const char *tag);
void window_clear_kanban_board(main__WindowInfo *info, const char *name);
void *window_add_date_range_picker_control(main__WindowInfo *info, const char *name, const char *start_date, const char *end_date);
const char *window_get_date_range_start(main__WindowInfo *info, const char *name);
const char *window_get_date_range_end(main__WindowInfo *info, const char *name);
void window_set_date_range(main__WindowInfo *info, const char *name, const char *start_date, const char *end_date);
void *window_add_stat_grid_control(main__WindowInfo *info, const char *name, const char **titles, const char **values, const char **trends, const char **trend_styles, int count);

// Modern Super Controls & Image Controls (simple_gg suite)
void *window_add_donut_chart_control(main__WindowInfo *info, const char *name, const char *title, double percentage);
void window_set_donut_percentage(main__WindowInfo *info, const char *name, double percentage);

void *window_add_code_studio_control(main__WindowInfo *info, const char *name, const char *filename, const char *language, const char *code);
void window_set_code_studio(main__WindowInfo *info, const char *name, const char *filename, const char *language, const char *code);

void *window_add_score_card_control(main__WindowInfo *info, const char *name, const char *title, double score, int reviews, const double *breakdown, int breakdown_count);

void *window_add_floating_toolbar_control(main__WindowInfo *info, const char *name, const char *title, const char **actions, int action_count);

void *window_add_user_profile_card_control(main__WindowInfo *info, const char *name, const char *avatar_path, const char *name_text, const char *handle, const char *role, const char *bio, int is_online, const char *action_label);
void window_set_user_online_status(main__WindowInfo *info, const char *name, int is_online);

void *window_add_product_card_control(main__WindowInfo *info, const char *name, const char *image_path, const char *title, const char *description, const char *price, const char *badge, const char *action_label);

void *window_add_image_gallery_control(main__WindowInfo *info, const char *name, const char **images, const char **captions, int count, int initial_idx);
void window_next_gallery_image(main__WindowInfo *info, const char *name);
void window_prev_gallery_image(main__WindowInfo *info, const char *name);
void window_set_gallery_index(main__WindowInfo *info, const char *name, int index);
int window_get_gallery_index(main__WindowInfo *info, const char *name);

void *window_add_app_launcher_tile_control(main__WindowInfo *info, const char *name, const char *icon_path, const char *title, const char *subtitle, const char *status);

void *window_add_media_player_control(main__WindowInfo *info, const char *name, const char *cover_path, const char *title, const char *artist, int duration_sec, int elapsed_sec, int is_playing);
void window_set_media_player_audio_file(main__WindowInfo *info, const char *name, const char *audio_path);
void window_toggle_media_player(main__WindowInfo *info, const char *name);
void window_set_media_player_progress(main__WindowInfo *info, const char *name, int elapsed_sec);
int window_get_media_player_playing(main__WindowInfo *info, const char *name);

void *window_add_activity_heatmap_control(main__WindowInfo *info, const char *name, const char *title, int weeks, const int *matrix_flat, int rows, int cols);

void *window_add_masked_input_control(main__WindowInfo *info, const char *name, const char *mask, const char *value);
const char *window_get_masked_input_value(main__WindowInfo *info, const char *name);
void window_set_masked_input_value(main__WindowInfo *info, const char *name, const char *value);

void *window_add_inline_editable_label_control(main__WindowInfo *info, const char *name, const char *text);
const char *window_get_inline_editable_label(main__WindowInfo *info, const char *name);
void window_set_inline_editable_label(main__WindowInfo *info, const char *name, const char *text);

void *window_add_nav_rail_control(main__WindowInfo *info, const char *name, const char **ids, const char **titles, const char **icons, const char **badges, const int *is_active, int count);
const char *window_get_nav_rail_selected(main__WindowInfo *info, const char *name);
void window_set_nav_rail_selected(main__WindowInfo *info, const char *name, const char *item_id);


// Comprehensive Window Control APIs
void window_set_vibrancy(main__WindowInfo *info, const char *material);
void window_set_corner_radius(main__WindowInfo *info, double radius);
double window_get_corner_radius(main__WindowInfo *info);
void window_set_background_blur(main__WindowInfo *info, int enabled);
void window_flash_frame(main__WindowInfo *info, int critical);
void window_center_on_active_screen(main__WindowInfo *info);
void window_set_level_type(main__WindowInfo *info, const char *level_type);
const char *window_get_window_level(main__WindowInfo *info);
void window_set_fullscreen(main__WindowInfo *info, int enabled);
void window_snap_to_edge(main__WindowInfo *info, const char *edge);
void window_set_bounds(main__WindowInfo *info, int x, int y, int width, int height);
void window_get_bounds(main__WindowInfo *info, int *out_x, int *out_y, int *out_w, int *out_h);
int window_has_aspect_ratio(main__WindowInfo *info);
void window_set_ignores_mouse_events(main__WindowInfo *info, int enabled);
int window_get_ignores_mouse_events(main__WindowInfo *info);
void window_set_hides_on_deactivate(main__WindowInfo *info, int enabled);
int window_get_hides_on_deactivate(main__WindowInfo *info);
void window_set_prevents_app_termination(main__WindowInfo *info, int enabled);
int window_get_prevents_app_termination(main__WindowInfo *info);
void window_set_represented_filename(main__WindowInfo *info, const char *filepath);
const char *window_get_represented_filename(main__WindowInfo *info);
void window_set_frame_autosave_name(main__WindowInfo *info, const char *autosave_name);
const char *window_get_frame_autosave_name(main__WindowInfo *info);
int window_save_frame(main__WindowInfo *info);
int window_restore_frame(main__WindowInfo *info);
int window_save_geometry(main__WindowInfo *info, const char *key);
int window_restore_geometry(main__WindowInfo *info, const char *key);
int window_capture_screenshot(main__WindowInfo *info, const char *file_path);
void window_set_document_edited(main__WindowInfo *info, int edited);
int window_is_document_edited(main__WindowInfo *info);
void window_fade_in(main__WindowInfo *info, int duration_ms);
void window_fade_out(main__WindowInfo *info, int duration_ms);
void window_order_front(main__WindowInfo *info);
void window_order_back(main__WindowInfo *info);

// Additional Window Control APIs
void window_set_alpha(main__WindowInfo *info, double alpha);
double window_get_alpha(main__WindowInfo *info);
void window_set_min_size(main__WindowInfo *info, int width, int height);
void window_get_min_size(main__WindowInfo *info, int *out_w, int *out_h);
void window_set_max_size(main__WindowInfo *info, int width, int height);
void window_get_max_size(main__WindowInfo *info, int *out_w, int *out_h);
void window_set_collection_behavior(main__WindowInfo *info, const char *behavior);
void window_set_has_shadow(main__WindowInfo *info, int shadow);
int window_get_has_shadow(main__WindowInfo *info);
void window_set_close_button_enabled(main__WindowInfo *info, int enabled);
void window_set_minimize_button_enabled(main__WindowInfo *info, int enabled);
void window_set_zoom_button_enabled(main__WindowInfo *info, int enabled);
void window_set_title_visible(main__WindowInfo *info, int visible);
int window_get_title_visible(main__WindowInfo *info);
void window_shake(main__WindowInfo *info);
void window_set_content_insets(main__WindowInfo *info, int top, int left, int bottom, int right);
void window_set_tabbing_mode(main__WindowInfo *info, const char *mode);
const char *window_get_tabbing_mode(main__WindowInfo *info);
void window_set_tabbing_identifier(main__WindowInfo *info, const char *identifier);
const char *window_get_tabbing_identifier(main__WindowInfo *info);
void window_toggle_tab_bar(main__WindowInfo *info);
void window_select_next_tab(main__WindowInfo *info);
void window_select_previous_tab(main__WindowInfo *info);
void window_set_sharing_type(main__WindowInfo *info, const char *sharing);
int window_get_sharing_type(main__WindowInfo *info);

// Appearance Override
void window_set_window_appearance(main__WindowInfo *info, const char *appearance_name);
const char *window_get_window_appearance(main__WindowInfo *info);
int window_is_system_dark_mode(main__WindowInfo *info);

// Screen Info
void window_get_screen_frame(main__WindowInfo *info, int *out_x, int *out_y, int *out_w, int *out_h);
void window_get_screen_full_frame(main__WindowInfo *info, int *out_x, int *out_y, int *out_w, int *out_h);
double window_get_screen_scale_factor(main__WindowInfo *info);

// Cursor Control
void window_set_cursor_hidden(main__WindowInfo *info, int hidden);
void window_set_cursor(main__WindowInfo *info, const char *cursor_name);
const char *window_get_cursor(main__WindowInfo *info);
void window_set_cursor_scale(main__WindowInfo *info, double scale);
double window_get_cursor_scale(main__WindowInfo *info);
void window_reset_cursor(main__WindowInfo *info);
void window_push_cursor(main__WindowInfo *info, const char *cursor_name);
void window_pop_cursor(main__WindowInfo *info);
void window_set_control_cursor_by_name(main__WindowInfo *info, const char *name, const char *cursor_name);
void window_get_mouse_location(main__WindowInfo *info, int *out_x, int *out_y);
void window_move_cursor_to(main__WindowInfo *info, int x, int y);

// Resize Indicator
void window_set_shows_resize_indicator(main__WindowInfo *info, int show);
int window_get_shows_resize_indicator(main__WindowInfo *info);

// Content Size Constraints
void window_set_content_min_size(main__WindowInfo *info, int width, int height);
void window_set_content_max_size(main__WindowInfo *info, int width, int height);
void window_get_content_min_size(main__WindowInfo *info, int *out_w, int *out_h);
void window_get_content_max_size(main__WindowInfo *info, int *out_w, int *out_h);

// Window Movability
void window_set_movable(main__WindowInfo *info, int movable);
int window_get_movable(main__WindowInfo *info);

// Tab Count
int window_get_tab_count(main__WindowInfo *info);

// Additional Split View Control APIs
void window_set_split_position(main__WindowInfo *info, const char *name, int divider_index, double position);
void window_set_split_divider_style(main__WindowInfo *info, const char *name, const char *style);

// Slider Tick Marks & Snapping
void window_set_slider_tick_marks(main__WindowInfo *info, const char *name, int count, int stops_only);

// Progress Indicator Animation & Indeterminate
void window_set_progress_indeterminate(main__WindowInfo *info, const char *name, int indeterminate);
void window_start_progress_animation(main__WindowInfo *info, const char *name);
void window_stop_progress_animation(main__WindowInfo *info, const char *name);

// Tab Navigation APIs
void window_select_tab(main__WindowInfo *info, const char *name, int index);
void window_select_tab_by_title(main__WindowInfo *info, const char *name, const char *title);
int window_get_tab_index(main__WindowInfo *info, const char *name);

// Search Field Control
void window_set_search_recent_searches_key(main__WindowInfo *info, const char *name, const char *key);
void window_clear_search_history(main__WindowInfo *info, const char *name);

// Path Control Operations
void window_set_path_control_path(main__WindowInfo *info, const char *name, const char *path);
char *window_get_path_control_path(main__WindowInfo *info, const char *name);
void window_set_path_control_style(main__WindowInfo *info, const char *name, const char *style);

// Token Field Operations
void window_set_token_field_tokens(main__WindowInfo *info, const char *name, const char **tokens, int count);
char *window_get_token_field_tokens(main__WindowInfo *info, const char *name);

// Text Area Operations
void window_textarea_insert_text(main__WindowInfo *info, const char *name, const char *text);
char *window_textarea_get_selected_text(main__WindowInfo *info, const char *name);
void window_textarea_set_selected_range(main__WindowInfo *info, const char *name, int start, int length);
void window_textarea_scroll_to_end(main__WindowInfo *info, const char *name);
void window_textarea_clear(main__WindowInfo *info, const char *name);

// Scroll View Programmatic Scroll
void window_scroll_to_top(main__WindowInfo *info, const char *name);
void window_scroll_to_bottom(main__WindowInfo *info, const char *name);

// Native System Dialogs & Panels
char *window_show_color_picker(main__WindowInfo *info, const char *initial_hex);
char *window_select_multiple_files(main__WindowInfo *info, const char *extensions);
char *window_save_file_picker_with_name(main__WindowInfo *info, const char *default_filename, const char *allowed_extensions);

// Text-to-Speech (NSSpeechSynthesizer)
void window_speech_speak(const char *text, const char *voice_name);
void window_speech_stop(void);
int window_speech_is_speaking(void);
char *window_speech_get_voices(void);
char *window_speech_get_default_voice(void);

// Mac Trackpad Haptic Feedback (NSHapticFeedbackManager)
void window_perform_haptic_feedback(const char *pattern);

// Advanced Pasteboard / Clipboard APIs (NSPasteboard)
int window_clipboard_copy_image(const char *image_path);
int window_clipboard_get_image(const char *dest_png_path);
int window_clipboard_copy_files(const char **file_paths, int count);
char *window_clipboard_get_files(void);
void window_clipboard_clear(void);
int window_clipboard_get_change_count(void);

// macOS Workspace & Application Operations (NSWorkspace)
int window_recycle_to_trash(const char *file_path);
char *window_get_running_apps(void);
int window_activate_app(const char *bundle_id_or_name);
int window_terminate_app(int pid);
void window_hide_other_apps(void);
void window_unhide_all_apps(void);
char *window_get_frontmost_app(void);
int window_open_with_app(const char *file_path, const char *app_name);
char *window_get_default_app_for_extension(const char *ext);

// Sound & Audio Playback (NSSound)
int window_play_sound_file(const char *file_path);
void window_stop_sound_file(void);
int window_is_sound_file_playing(void);

// Power & Sleep Management (IOPMAssertion / IOKit)
unsigned int window_prevent_sleep(const char *reason);
void window_allow_sleep(unsigned int assertion_id);
double window_get_battery_percentage(void);
int window_is_battery_charging(void);
int window_is_on_battery_power(void);
int window_get_battery_time_remaining(void);

// User Defaults / Preferences (NSUserDefaults)
void window_defaults_set_string(const char *key, const char *val);
char *window_defaults_get_string(const char *key);
void window_defaults_set_bool(const char *key, int val);
int window_defaults_get_bool(const char *key);
void window_defaults_set_int(const char *key, int val);
int window_defaults_get_int(const char *key);
void window_defaults_remove(const char *key);
int window_defaults_has(const char *key);

// macOS System Information
char *window_get_macos_version_str(void);
void window_get_macos_version_numbers(int *out_major, int *out_minor, int *out_patch);
char *window_get_computer_name(void);
char *window_get_user_full_name(void);
int window_is_apple_silicon(void);
double window_get_system_uptime(void);
int window_is_low_power_mode(void);

// Multi-Screen / Display (NSScreen)
char *window_get_screens_info(void);
void window_move_to_screen(main__WindowInfo *info, int screen_index);

// Dock & App Activation Policy
char *window_get_dock_badge(void);
void window_cancel_dock_bounce(void);
void window_set_activation_policy(const char *policy);
char *window_get_activation_policy(void);
char *window_get_app_bundle_path(void);

#endif
