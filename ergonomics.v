module simplegui

import os
import json2
import strconv
import time
import encoding.csv

// ergonomics.v - Beginner-friendly ergonomic helpers for SimpleGUI
// High-level shortcuts, batch control operations, and convenience methods designed to
// make everyday desktop GUI programming in V intuitive, readable, and accessible for everyone.

// ==========================================
// 1. Dialog Shortcuts
// ==========================================

// info displays a native macOS informational alert pop-up box with an OK button.
pub fn (win &SimpleWindow) info(title string, message string) &SimpleWindow {
	return win.alert_with_style(title, message, 'info')
}

// warn displays a styled warning alert pop-up box with a warning icon.
pub fn (win &SimpleWindow) warn(title string, message string) &SimpleWindow {
	return win.alert_with_style(title, message, 'warning')
}

// error_dialog displays a critical error alert pop-up box with an error icon.
pub fn (win &SimpleWindow) error_dialog(title string, message string) &SimpleWindow {
	return win.alert_with_style(title, message, 'error')
}

// ask displays a native Yes/No confirmation dialog box and returns true if confirmed.
pub fn (win &SimpleWindow) ask(title string, question string) bool {
	return win.confirm(title, question)
}

// quit terminates the application event loop and closes the app window immediately.
pub fn (win &SimpleWindow) quit() {
	if win.window_info != unsafe { nil } {
		C.window_app_exit(win.window_info)
	}
}

// ==========================================
// 2. Batch Control Operations
// ==========================================

// show_controls makes every named control visible in one call.
pub fn (win &SimpleWindow) show_controls(names []string) &SimpleWindow {
	for name in names {
		win.set_control_visible(name, true)
	}
	return win
}

// hide_controls hides every named control in one call.
pub fn (win &SimpleWindow) hide_controls(names []string) &SimpleWindow {
	for name in names {
		win.set_control_visible(name, false)
	}
	return win
}

// enable_controls enables every named control in one call.
pub fn (win &SimpleWindow) enable_controls(names []string) &SimpleWindow {
	for name in names {
		win.set_control_enabled(name, true)
	}
	return win
}

// disable_controls disables (greys out) every named control in one call.
pub fn (win &SimpleWindow) disable_controls(names []string) &SimpleWindow {
	for name in names {
		win.set_control_enabled(name, false)
	}
	return win
}

// enable_all_controls enables every registered control (e.g. after a long task finishes).
pub fn (win &SimpleWindow) enable_all_controls() &SimpleWindow {
	for control in win.controls {
		win.set_control_enabled(control.name, true)
	}
	return win
}

// disable_all_controls disables every registered control (e.g. while processing).
pub fn (win &SimpleWindow) disable_all_controls() &SimpleWindow {
	for control in win.controls {
		win.set_control_enabled(control.name, false)
	}
	return win
}

// enable_all is a concise alias for enable_all_controls.
pub fn (win &SimpleWindow) enable_all() &SimpleWindow {
	return win.enable_all_controls()
}

// disable_all is a concise alias for disable_all_controls.
pub fn (win &SimpleWindow) disable_all() &SimpleWindow {
	return win.disable_all_controls()
}

// set_all sets multiple controls from a name->value map.
pub fn (win &SimpleWindow) set_all(values map[string]string) &SimpleWindow {
	for name, value in values {
		win.set_control_text(name, value)
	}
	return win
}

// get_all reads text values for multiple controls into a name->value map.
pub fn (win &SimpleWindow) get_all(names []string) map[string]string {
	mut values := map[string]string{}
	for name in names {
		values[name] = win.get_control_text(name)
	}
	return values
}

// export_form_json encodes field values for the specified control `names` into a JSON formatted string.
pub fn (win &SimpleWindow) export_form_json(names []string) string {
	m := win.get_all(names)
	return json2.encode[map[string]string](m)
}

// import_form_json populates control values from a JSON formatted string map.
pub fn (win &SimpleWindow) import_form_json(json_str string) &SimpleWindow {
	m := json2.decode[map[string]string](json_str) or { return win }
	win.set_all(m)
	return win
}

// batch_enable_controls is an alias for enable_controls.
pub fn (win &SimpleWindow) batch_enable_controls(names []string) &SimpleWindow {
	return win.enable_controls(names)
}

// batch_disable_controls is an alias for disable_controls.
pub fn (win &SimpleWindow) batch_disable_controls(names []string) &SimpleWindow {
	return win.disable_controls(names)
}

// batch_show_controls is an alias for show_controls.
pub fn (win &SimpleWindow) batch_show_controls(names []string) &SimpleWindow {
	return win.show_controls(names)
}

// batch_hide_controls is an alias for hide_controls.
pub fn (win &SimpleWindow) batch_hide_controls(names []string) &SimpleWindow {
	return win.hide_controls(names)
}

// toggle_controls_enabled toggles enabled state for multiple named controls.
pub fn (win &SimpleWindow) toggle_controls_enabled(names []string) &SimpleWindow {
	for name in names {
		win.toggle_control_enabled(name)
	}
	return win
}

// toggle_controls_visible toggles visible state for multiple named controls.
pub fn (win &SimpleWindow) toggle_controls_visible(names []string) &SimpleWindow {
	for name in names {
		win.toggle_control_visible(name)
	}
	return win
}

// flash_controls flashes outlines for multiple named controls.
pub fn (win &SimpleWindow) flash_controls(names []string) &SimpleWindow {
	for name in names {
		win.flash_control(name)
	}
	return win
}

// highlight_controls highlights outlines for multiple named controls for duration_ms.
pub fn (win &SimpleWindow) highlight_controls(names []string, duration_ms int) &SimpleWindow {
	for name in names {
		win.highlight_control(name, duration_ms)
	}
	return win
}

// toggle_visible flips the visibility of a control and returns the new visibility state.
pub fn (win &SimpleWindow) toggle_visible(name string) bool {
	new_state := !win.get_control_visible(name)
	win.set_control_visible(name, new_state)
	return new_state
}

// toggle_enabled flips the enabled state of a control and returns the new enabled state.
pub fn (win &SimpleWindow) toggle_enabled(name string) bool {
	new_state := !win.get_control_enabled(name)
	win.set_control_enabled(name, new_state)
	return new_state
}

// ==========================================
// 3. Value Convenience Accessors
// ==========================================

// get_int returns the numeric value of a number/slider/progress/text control.
pub fn (win &SimpleWindow) get_int(name string) int {
	kind := win.get_control_kind(name)
	if kind in ['input', 'password', 'textarea', 'label'] {
		return win.get_value(name).trim_space().int()
	}
	return win.get_number_value(name)
}

// set_int sets the numeric value of a number/slider/progress control.
pub fn (win &SimpleWindow) set_int(name string, value int) &SimpleWindow {
	return win.set_number_value(name, value)
}

// get_float parses the text of a control as a floating point number.
pub fn (win &SimpleWindow) get_float(name string) f64 {
	return win.get_value(name).f64()
}

// set_float writes a floating point number into a text-based control.
pub fn (win &SimpleWindow) set_float(name string, value f64) &SimpleWindow {
	return win.set_value(name, value.str())
}

// increment adds delta (may be negative) to a numeric control and returns the new value.
pub fn (win &SimpleWindow) increment(name string, delta int) int {
	new_value := win.get_number_value(name) + delta
	win.set_number_value(name, new_value)
	return new_value
}

// toggle_checked flips a checkbox/switch state and returns the new checked state.
pub fn (win &SimpleWindow) toggle_checked(name string) bool {
	new_state := !win.get_bool(name)
	win.set_bool(name, new_state)
	return new_state
}

// set_progress updates a progress indicator value (0 to 100).
pub fn (win &SimpleWindow) set_progress(name string, value int) &SimpleWindow {
	return win.set_number_value(name, value)
}

// get_progress reads the current progress indicator value.
pub fn (win &SimpleWindow) get_progress(name string) int {
	return win.get_number_value(name)
}

// append_text appends text to the end of a text-based control (input/textarea/label).
pub fn (win &SimpleWindow) append_text(name string, text string) &SimpleWindow {
	current := win.get_value(name)
	return win.set_value(name, current + text)
}

// append_line appends a new line of text to a textarea (great for activity logs).
pub fn (win &SimpleWindow) append_line(name string, line string) &SimpleWindow {
	current := win.get_value(name)
	if current == '' {
		return win.set_value(name, line)
	}
	return win.set_value(name, current + '\n' + line)
}

// set_many_texts updates several text-based controls in one call.
pub fn (win &SimpleWindow) set_many_texts(values map[string]string) &SimpleWindow {
	for name, value in values {
		win.set_text(name, value)
	}
	return win
}

// get_many_texts reads several text-based controls into a name/value map.
pub fn (win &SimpleWindow) get_many_texts(names []string) map[string]string {
	mut values := map[string]string{}
	for name in names {
		values[name] = win.get_text(name)
	}
	return values
}

// set_many_checked updates several checkbox/switch controls in one call.
pub fn (win &SimpleWindow) set_many_checked(values map[string]bool) &SimpleWindow {
	for name, value in values {
		win.set_checked(name, value)
	}
	return win
}

// get_many_checked reads several checkbox/switch controls into a name/value map.
pub fn (win &SimpleWindow) get_many_checked(names []string) map[string]bool {
	mut values := map[string]bool{}
	for name in names {
		values[name] = win.get_checked(name)
	}
	return values
}

// set_many_numbers updates several numeric controls in one call.
pub fn (win &SimpleWindow) set_many_numbers(values map[string]int) &SimpleWindow {
	for name, value in values {
		win.set_value_int(name, value)
	}
	return win
}

// get_many_numbers reads several numeric controls into a name/value map.
pub fn (win &SimpleWindow) get_many_numbers(names []string) map[string]int {
	mut values := map[string]int{}
	for name in names {
		values[name] = win.get_value_int(name)
	}
	return values
}

// set_many_visibility updates several controls' visibility in one call.
pub fn (win &SimpleWindow) set_many_visibility(values map[string]bool) &SimpleWindow {
	for name, value in values {
		win.set_control_visible(name, value)
	}
	return win
}

// get_many_visibility reads several controls' visibility into a name/value map.
pub fn (win &SimpleWindow) get_many_visibility(names []string) map[string]bool {
	mut values := map[string]bool{}
	for name in names {
		values[name] = win.get_control_visible(name)
	}
	return values
}

// set_many_enabled updates several controls' enabled state in one call.
pub fn (win &SimpleWindow) set_many_enabled(values map[string]bool) &SimpleWindow {
	for name, value in values {
		win.set_control_enabled(name, value)
	}
	return win
}

// get_many_enabled reads several controls' enabled state into a name/value map.
pub fn (win &SimpleWindow) get_many_enabled(names []string) map[string]bool {
	mut values := map[string]bool{}
	for name in names {
		values[name] = win.get_control_enabled(name)
	}
	return values
}

// set_many_errors updates several controls' inline error state in one call.
pub fn (win &SimpleWindow) set_many_errors(values map[string]string) &SimpleWindow {
	for name, value in values {
		win.set_error(name, value)
	}
	return win
}

// set_many_placeholders updates several controls' placeholder text in one call.
pub fn (win &SimpleWindow) set_many_placeholders(values map[string]string) &SimpleWindow {
	for name, value in values {
		win.set_placeholder(name, value)
	}
	return win
}

// set_many_tooltips updates several controls' tooltip text in one call.
pub fn (win &SimpleWindow) set_many_tooltips(values map[string]string) &SimpleWindow {
	for name, value in values {
		win.set_tooltip(name, value)
	}
	return win
}

// with_busy_state temporarily disables a set of controls, updates the status text,
// runs a callback, and then restores the previous enabled state.
pub fn (win &SimpleWindow) with_busy_state(names []string, status_text string, callback VoidEventCallback) &SimpleWindow {
	mut original_states := map[string]bool{}
	for name in names {
		original_states[name] = win.get_control_enabled(name)
		win.set_control_enabled(name, false)
	}
	win.set_status(status_text)
	unsafe {
		mut w := &SimpleWindow(win)
		callback(mut w)
	}
	for name, enabled in original_states {
		win.set_control_enabled(name, enabled)
	}
	return win
}

// clear_many clears a subset of controls to their empty/default state.
pub fn (win &SimpleWindow) clear_many(names []string) &SimpleWindow {
	for name in names {
		win.clear(name)
	}
	return win
}

// reset_many restores a subset of controls to their original values.
pub fn (win &SimpleWindow) reset_many(names []string) &SimpleWindow {
	for name in names {
		idx := win.find_control(name)
		if idx < 0 {
			continue
		}
		entry := win.controls[idx]
		if entry.kind in ['checkbox', 'switch', 'spinner'] {
			win.set_checked(name, entry.initial_checked)
		} else if entry.kind in ['number', 'slider', 'progress', 'levelindicator', 'stepper', 'knob'] {
			win.set_value_int(name, entry.initial_number)
		} else if entry.kind in ['input', 'password', 'textarea', 'date', 'mode', 'theme', 'listbox',
			'color', 'search', 'dropdown', 'segmented', 'radiogroup', 'combobox', 'pathcontrol',
			'tokenfield'] {
			win.set_text(name, entry.initial_value)
		}
	}
	return win
}

// focus moves keyboard focus to the named control (alias of set_focus).
pub fn (win &SimpleWindow) focus(name string) &SimpleWindow {
	return win.set_focus(name)
}

// ==========================================
// 4. List Box Item Management
// ==========================================

// get_list_items returns the current items of a list box.
pub fn (win &SimpleWindow) get_list_items(name string) []string {
	return win.list_items[name] or { []string{} }
}

// set_list_items replaces all items of a list box (alias of update_list_items).
pub fn (win &SimpleWindow) set_list_items(name string, items []string) &SimpleWindow {
	return win.update_list_items(name, items)
}

// add_list_item appends a single item to the end of a list box.
pub fn (win &SimpleWindow) add_list_item(name string, item string) &SimpleWindow {
	mut items := win.get_list_items(name)
	items << item
	return win.update_list_items(name, items)
}

// remove_list_item removes the item at the given 0-based index from a list box.
pub fn (win &SimpleWindow) remove_list_item(name string, index int) &SimpleWindow {
	mut items := win.get_list_items(name)
	if index >= 0 && index < items.len {
		items.delete(index)
		win.update_list_items(name, items)
	}
	return win
}

// remove_selected_list_item removes the currently selected row from a list box.
pub fn (win &SimpleWindow) remove_selected_list_item(name string) &SimpleWindow {
	return win.remove_list_item(name, win.get_list_selected(name))
}

// clear_list_items removes every item from a list box.
pub fn (win &SimpleWindow) clear_list_items(name string) &SimpleWindow {
	return win.update_list_items(name, []string{})
}

// get_list_count returns the number of items in a list box.
pub fn (win &SimpleWindow) get_list_count(name string) int {
	return win.get_list_items(name).len
}

// get_list_selected_text returns the text of the currently selected list box row,
// or an empty string when nothing is selected.
pub fn (win &SimpleWindow) get_list_selected_text(name string) string {
	idx := win.get_list_selected(name)
	items := win.get_list_items(name)
	if idx >= 0 && idx < items.len {
		return items[idx]
	}
	return ''
}

// set_list_multi_select enables or disables multiple row selection on a list box
// (users can then Cmd/Shift-click to select several rows).
pub fn (win &SimpleWindow) set_list_multi_select(name string, enabled bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_list_multi_select(win.window_info, name.str, int(enabled))
	}
	return win
}

// get_list_selected_indexes returns every selected row index of a list box (0-based, ascending).
pub fn (win &SimpleWindow) get_list_selected_indexes(name string) []int {
	if win.window_info == unsafe { nil } {
		return []int{}
	}
	res := C.window_get_list_selected_indexes(win.window_info, name.str)
	csv_text := unsafe { res.vstring() }
	mut indexes := []int{}
	for part in csv_text.split(',') {
		trimmed := part.trim_space()
		if trimmed != '' {
			indexes << trimmed.int()
		}
	}
	return indexes
}

// set_list_selected_indexes selects the given row indexes (requires multi-select
// for more than one row). Pass an empty array to clear the selection.
pub fn (win &SimpleWindow) set_list_selected_indexes(name string, indexes []int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		csv_text := indexes.map(it.str()).join(',')
		C.window_set_list_selected_indexes(win.window_info, name.str, csv_text.str)
	}
	return win
}

// get_list_selected_texts returns the text of every selected list box row.
pub fn (win &SimpleWindow) get_list_selected_texts(name string) []string {
	items := win.get_list_items(name)
	mut texts := []string{}
	for idx in win.get_list_selected_indexes(name) {
		if idx >= 0 && idx < items.len {
			texts << items[idx]
		}
	}
	return texts
}

// select_all_list_items selects every row of a list box (multi-select must be enabled).
pub fn (win &SimpleWindow) select_all_list_items(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_select_all_list_items(win.window_info, name.str)
	}
	return win
}

// clear_list_selection deselects every row of a list box.
pub fn (win &SimpleWindow) clear_list_selection(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_clear_list_selection(win.window_info, name.str)
	}
	return win
}

// remove_selected_list_items removes every selected row from a list box
// (works with both single and multi selection). Returns the removed items.
pub fn (win &SimpleWindow) remove_selected_list_items(name string) []string {
	selected := win.get_list_selected_indexes(name)
	if selected.len == 0 {
		return []string{}
	}
	items := win.get_list_items(name)
	mut removed := []string{}
	mut remaining := []string{}
	for i, item in items {
		if i in selected {
			removed << item
		} else {
			remaining << item
		}
	}
	win.update_list_items(name, remaining)
	return removed
}

// on_list_double_click registers a callback fired when a list box row is
// double-clicked. The callback value is the 0-based row index as a string.
pub fn (win &SimpleWindow) on_list_double_click(name string, callback StringEventCallback) &SimpleWindow {
	idx := win.find_handler(name, 'dblclick')
	mut handler := ControlEventHandler{
		control_name: name
		event_name:   'dblclick'
		string_cb:    callback
	}
	unsafe {
		mut w := &SimpleWindow(win)
		if idx >= 0 {
			w.handlers[idx] = handler
		} else {
			w.handlers << handler
		}
	}
	return win
}

// ==========================================
// 5. Settings Persistence
// ==========================================

// save_values_to_file saves every control value to a JSON file for later restoration.
// If a relative path is passed (e.g. "settings.json"), it automatically resolves to the
// recommended user storage location (~/Library/Application Support/<app>/ on macOS),
// ensuring the application works properly when moved to /Applications or installed globally.
pub fn (win &SimpleWindow) save_values_to_file(path string) ! {
	target_path := win.resolve_storage_path(path)
	values := win.dump_values()
	os.write_file(target_path, json2.encode(values))!
}

// load_values_from_file restores previously saved control values from a JSON file.
// Values for controls that no longer exist are silently skipped.
// If a relative path is passed, it checks both the local directory and the recommended
// user storage location (~/Library/Application Support/<app>/ on macOS).
pub fn (win &SimpleWindow) load_values_from_file(path string) ! {
	mut target_path := path
	if !path.starts_with('/') && !path.starts_with('~') {
		if !os.exists(path) {
			target_path = win.resolve_storage_path(path)
		}
	} else if path.starts_with('~') {
		target_path = path.replace('~', os.home_dir())
	}
	content := os.read_file(target_path)!
	values := json_decode_map(content)
	for name, val in values {
		if win.has_control(name) {
			kind := win.get_control_kind(name)
			if kind in ['checkbox', 'switch', 'spinner'] {
				win.set_checked(name, val == 'true')
			} else if kind in ['number', 'slider', 'progress', 'levelindicator', 'stepper', 'knob'] {
				win.set_value_int(name, val.int())
			} else {
				win.set_text(name, val)
			}
		}
	}
}

// enable_app_autosave periodically saves application state into the recommended application support directory.
// Default interval is 5000ms.
pub fn (win &SimpleWindow) enable_app_autosave(interval_ms ...int) &SimpleWindow {
	interval := if interval_ms.len > 0 && interval_ms[0] > 0 {
		interval_ms[0]
	} else {
		5000
	}
	return win.set_interval('autosave_app_state', interval, fn (mut w SimpleWindow) {
		w.save_app_state() or {}
	})
}

// is_checked returns true if a checkbox, switch, or toggle is checked. Alias for get_checked.
pub fn (win &SimpleWindow) is_checked(name string) bool {
	return win.get_checked(name)
}

// ==========================================
// 6. Labeled Control Rows
// ==========================================

// add_labeled_input creates a label and text field side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_input(label string, name string, value string) &SimpleWindow {
	return win.add_form_field(label, name, value)
}

// add_labeled_field is an alias for add_labeled_input.
pub fn (win &SimpleWindow) add_labeled_field(label string, name string, value string) &SimpleWindow {
	return win.add_form_field(label, name, value)
}

// add_labeled_textarea creates a label and textarea side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_textarea(label string, name string, value string) &SimpleWindow {
	return win.add_form_textarea(label, name, value)
}

// add_labeled_password creates a label and password field side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_password(label string, name string, value string) &SimpleWindow {
	return win.add_form_password(label, name, value)
}

// add_labeled_checkbox creates a label and checkbox side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_checkbox(label string, name string, chk_text string, checked bool) &SimpleWindow {
	win.begin_row('row_' + name)
	win.add_label(name + '_label', label)
	win.add_checkbox(name, chk_text, checked)
	win.end_row()
	return win
}

// add_labeled_switch creates a label and toggle switch side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_switch(label string, name string, switch_label string, checked bool) &SimpleWindow {
	return win.add_form_switch(label, name, switch_label, checked)
}

// add_labeled_slider creates a label and slider side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_slider(label string, name string, value int) &SimpleWindow {
	win.begin_row('row_' + name)
	win.add_label(name + '_label', label)
	win.add_slider(name, value)
	win.end_row()
	return win
}

// add_labeled_dropdown creates a label and dropdown side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_dropdown(label string, name string, items []string, selected string) &SimpleWindow {
	win.begin_row('row_' + name)
	win.add_label(name + '_label', label)
	win.add_dropdown(name, items, selected)
	win.end_row()
	return win
}

// add_labeled_number creates a label and number stepper side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_number(label string, name string, value int) &SimpleWindow {
	win.begin_row('row_' + name)
	win.add_label(name + '_label', label)
	win.add_number(name, value)
	win.end_row()
	return win
}

// add_labeled_date_picker creates a label and date picker side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_date_picker(label string, name string, date string) &SimpleWindow {
	win.begin_row('row_' + name)
	win.add_label(name + '_label', label)
	win.add_date_picker(name, date)
	win.end_row()
	return win
}

// add_labeled_progress creates a label and progress bar side-by-side in one row.
pub fn (win &SimpleWindow) add_labeled_progress(label string, name string, value int) &SimpleWindow {
	win.begin_row('row_' + name)
	win.add_label(name + '_label', label)
	win.add_progress_indicator(name, value)
	win.end_row()
	return win
}

// ==========================================
// 7. Timer & Event Sugar
// ==========================================

// every runs the callback repeatedly with the given interval, using an
// auto-generated timer name. Returns the window for chaining.
pub fn (win &SimpleWindow) every(ms int, callback VoidEventCallback) &SimpleWindow {
	timer_name := 'auto_timer_${win.handlers.len}'
	return win.set_interval(timer_name, ms, callback)
}

// after runs the callback once after the given delay (alias of run_after).
pub fn (win &SimpleWindow) after(ms int, callback VoidEventCallback) &SimpleWindow {
	return win.run_after(ms, callback)
}

// on_change_many registers the same change callback on multiple controls at once.
pub fn (win &SimpleWindow) on_change_many(names []string, callback StringEventCallback) &SimpleWindow {
	for name in names {
		win.on_change(name, callback)
	}
	return win
}

// on_click_many registers the same click callback on multiple controls at once.
pub fn (win &SimpleWindow) on_click_many(names []string, callback VoidEventCallback) &SimpleWindow {
	for name in names {
		win.on_click(name, callback)
	}
	return win
}

// ==========================================
// 8. Table Row Management
// ==========================================

// get_table_rows returns every row of a table (each row is a []string of cell values).
pub fn (win &SimpleWindow) get_table_rows(name string) [][]string {
	return win.table_rows[name] or { [][]string{} }
}

// get_table_row returns the row at the given 0-based index, or an empty array.
pub fn (win &SimpleWindow) get_table_row(name string, index int) []string {
	rows := win.get_table_rows(name)
	if index >= 0 && index < rows.len {
		return rows[index]
	}
	return []string{}
}

// get_table_row_count returns the number of rows in a table.
pub fn (win &SimpleWindow) get_table_row_count(name string) int {
	return win.get_table_rows(name).len
}

// get_table_cell returns a single cell value, or an empty string when out of range.
pub fn (win &SimpleWindow) get_table_cell(name string, row int, col int) string {
	cells := win.get_table_row(name, row)
	if col >= 0 && col < cells.len {
		return cells[col]
	}
	return ''
}

// set_table_cell updates a single cell of a table in place.
pub fn (win &SimpleWindow) set_table_cell(name string, row int, col int, value string) &SimpleWindow {
	mut rows := win.get_table_rows(name)
	if row >= 0 && row < rows.len && col >= 0 && col < rows[row].len {
		rows[row][col] = value
		win.set_table_rows(name, rows)
	}
	return win
}

// add_table_row appends a row to the end of a table.
pub fn (win &SimpleWindow) add_table_row(name string, row []string) &SimpleWindow {
	mut rows := win.get_table_rows(name)
	rows << row
	return win.set_table_rows(name, rows)
}

// insert_table_row inserts a row at the given 0-based index.
pub fn (win &SimpleWindow) insert_table_row(name string, index int, row []string) &SimpleWindow {
	mut rows := win.get_table_rows(name)
	if index >= 0 && index <= rows.len {
		rows.insert(index, row)
		win.set_table_rows(name, rows)
	}
	return win
}

// update_table_row replaces the row at the given 0-based index.
pub fn (win &SimpleWindow) update_table_row(name string, index int, row []string) &SimpleWindow {
	mut rows := win.get_table_rows(name)
	if index >= 0 && index < rows.len {
		rows[index] = row
		win.set_table_rows(name, rows)
	}
	return win
}

// remove_table_row removes the row at the given 0-based index.
pub fn (win &SimpleWindow) remove_table_row(name string, index int) &SimpleWindow {
	mut rows := win.get_table_rows(name)
	if index >= 0 && index < rows.len {
		rows.delete(index)
		win.set_table_rows(name, rows)
	}
	return win
}

// remove_table_column removes the column at a 0-based index and returns removed values.
pub fn (win &SimpleWindow) remove_table_column(name string, column int) []string {
	removed := win.remove_table_column_strict(name, column) or { return []string{} }
	return removed
}

// remove_selected_table_column removes the currently selected column and returns removed values.
pub fn (win &SimpleWindow) remove_selected_table_column(name string) []string {
	_, removed := win.remove_selected_table_column_strict(name) or { return []string{} }
	return removed
}

// clear_table removes every row from a table.
pub fn (win &SimpleWindow) clear_table(name string) &SimpleWindow {
	return win.set_table_rows(name, [][]string{})
}

// find_table_row returns the 0-based index of the first row whose cell in the
// given column equals value, or -1 when not found.
pub fn (win &SimpleWindow) find_table_row(name string, column int, value string) int {
	for i, row in win.get_table_rows(name) {
		if column >= 0 && column < row.len && row[column] == value {
			return i
		}
	}
	return -1
}

// get_table_selected returns the 0-based index of the selected table row (-1 when none).
pub fn (win &SimpleWindow) get_table_selected(name string) int {
	return win.get_list_selected(name)
}

// set_table_selected selects the table row at the given 0-based index
// (pass -1 to clear the selection).
pub fn (win &SimpleWindow) set_table_selected(name string, index int) &SimpleWindow {
	return win.set_list_selected(name, index)
}

// get_table_selected_row returns the cell values of the selected row, or an empty array.
pub fn (win &SimpleWindow) get_table_selected_row(name string) []string {
	return win.get_table_row(name, win.get_table_selected(name))
}

// set_table_multi_select enables or disables multiple row selection on a table
// (users can then Cmd/Shift-click to select several rows).
pub fn (win &SimpleWindow) set_table_multi_select(name string, enabled bool) &SimpleWindow {
	return win.set_list_multi_select(name, enabled)
}

// get_table_selected_indexes returns every selected table row index (0-based, ascending).
pub fn (win &SimpleWindow) get_table_selected_indexes(name string) []int {
	return win.get_list_selected_indexes(name)
}

// get_table_selected_rows returns the cell values of every selected table row.
pub fn (win &SimpleWindow) get_table_selected_rows(name string) [][]string {
	rows := win.get_table_rows(name)
	mut selected := [][]string{}
	for idx in win.get_table_selected_indexes(name) {
		if idx >= 0 && idx < rows.len {
			selected << rows[idx]
		}
	}
	return selected
}

// clear_table_selection deselects every row of a table.
pub fn (win &SimpleWindow) clear_table_selection(name string) &SimpleWindow {
	return win.clear_list_selection(name)
}

// remove_selected_table_rows removes every selected row from a table
// (works with both single and multi selection). Returns the removed rows.
pub fn (win &SimpleWindow) remove_selected_table_rows(name string) [][]string {
	selected := win.get_table_selected_indexes(name)
	if selected.len == 0 {
		return [][]string{}
	}
	rows := win.get_table_rows(name)
	mut removed := [][]string{}
	mut remaining := [][]string{}
	for i, row in rows {
		if i in selected {
			removed << row
		} else {
			remaining << row
		}
	}
	win.set_table_rows(name, remaining)
	return removed
}

// on_table_select registers a callback fired when the table selection changes.
// The callback value is the 0-based selected row index as a string (-1 when cleared).
pub fn (win &SimpleWindow) on_table_select(name string, callback StringEventCallback) &SimpleWindow {
	return win.on_change(name, callback)
}

// on_table_double_click registers a callback fired when a table row is
// double-clicked. The callback value is the 0-based row index as a string.
pub fn (win &SimpleWindow) on_table_double_click(name string, callback StringEventCallback) &SimpleWindow {
	return win.on_list_double_click(name, callback)
}

// on_table_column_select registers a callback fired when a table column is
// selected while column selection mode is enabled. The callback value is the
// 0-based selected column index as a string.
pub fn (win &SimpleWindow) on_table_column_select(name string, callback StringEventCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.handlers << ControlEventHandler{
			control_name: name
			event_name:   'column_change'
			string_cb:    callback
		}
	}
	return win
}

// ==========================================
// 9. Quick Validation
// ==========================================

// require_fields checks that every named control has a non-blank value.
// Empty controls receive an inline "required" error marker; filled ones are cleared.
// Returns true when all fields are filled.
pub fn (win &SimpleWindow) require_fields(names []string) bool {
	mut all_filled := true
	for name in names {
		if win.get_value(name).trim_space() == '' {
			win.set_error(name, 'This field is required')
			all_filled = false
		} else {
			win.clear_error(name)
		}
	}
	return all_filled
}

// validate_email is a ready-made ControlValidator that accepts basic
// user@domain.tld addresses. Returns an error message or ''.
pub fn validate_email(value string) string {
	v := value.trim_space()
	if v == '' {
		return 'Email is required'
	}
	user := v.all_before('@')
	domain := v.all_after('@')
	if user == '' || domain == '' || domain == v || !domain.contains('.') || domain.starts_with('.')
		|| domain.ends_with('.') || v.contains(' ') {
		return 'Enter a valid email address'
	}
	return ''
}

// validate_number is a ready-made ControlValidator that accepts integers
// and floating point numbers. Returns an error message or ''.
pub fn validate_number(value string) string {
	v := value.trim_space()
	if v == '' {
		return 'A number is required'
	}
	if !is_numeric_text(v) {
		return 'Enter a valid number'
	}
	return ''
}

// is_numeric_text reports whether the string parses as an integer or float.
fn is_numeric_text(s string) bool {
	_ := strconv.atof64(s) or { return false }
	return true
}

// min_len_validator builds a ControlValidator requiring at least min characters
// (leading/trailing whitespace is ignored).
pub fn min_len_validator(min int) ControlValidator {
	return fn [min] (value string) string {
		if value.trim_space().len < min {
			return 'Must be at least ${min} characters'
		}
		return ''
	}
}

// ==========================================
// 10. Batch Value Access
// ==========================================

// clear_fields empties every named text-based control and clears its error state.
pub fn (win &SimpleWindow) clear_fields(names []string) &SimpleWindow {
	for name in names {
		win.set_text(name, '')
		win.clear_error(name)
	}
	return win
}

// ==========================================
// 11. List Sorting, Reordering & Live Search
// ==========================================

// sort_list_items sorts the items of a list box alphabetically
// (case-insensitive). Pass ascending = false for reverse order.
pub fn (win &SimpleWindow) sort_list_items(name string, ascending bool) &SimpleWindow {
	mut items := win.get_list_items(name)
	items.sort_with_compare(fn (a &string, b &string) int {
		return compare_strings(a.to_lower(), b.to_lower())
	})
	if !ascending {
		items.reverse_in_place()
	}
	return win.update_list_items(name, items)
}

// move_list_item moves the item at index `from` to index `to`
// (useful for Move Up / Move Down buttons).
pub fn (win &SimpleWindow) move_list_item(name string, from int, to int) &SimpleWindow {
	mut items := win.get_list_items(name)
	if from < 0 || from >= items.len || to < 0 || to >= items.len || from == to {
		return win
	}
	item := items[from]
	items.delete(from)
	items.insert(to, item)
	return win.update_list_items(name, items)
}

// bind_search_to_list wires a search field (or any text input) to a list box so
// typing filters the visible rows live (case-insensitive substring match).
// Clearing the search restores the full item set. The full set is snapshotted
// when this is called, so re-bind after replacing the list's master items.
pub fn (win &SimpleWindow) bind_search_to_list(search_name string, list_name string) &SimpleWindow {
	master := win.get_list_items(list_name).clone()
	win.on_change(search_name, fn [master, list_name] (mut w SimpleWindow, value string) {
		query := value.trim_space().to_lower()
		if query == '' {
			w.update_list_items(list_name, master)
			return
		}
		w.update_list_items(list_name, master.filter(it.to_lower().contains(query)))
	})
	return win
}

// ==========================================
// 12. Table Sorting & CSV Import/Export
// ==========================================

// sort_table_by_column sorts table rows by the given 0-based column. When every
// cell in that column parses as a number the sort is numeric, otherwise it is a
// case-insensitive text sort. Pass ascending = false for reverse order.
pub fn (win &SimpleWindow) sort_table_by_column(name string, column int, ascending bool) &SimpleWindow {
	rows := win.get_table_rows(name)
	if rows.len < 2 {
		return win
	}
	mut keys := []string{cap: rows.len}
	mut numeric := true
	for row in rows {
		cell := if column >= 0 && column < row.len { row[column] } else { '' }
		keys << cell
		if numeric && !is_numeric_text(cell.trim_space()) {
			numeric = false
		}
	}
	mut idxs := []int{len: rows.len, init: index}
	if numeric {
		mut nums := []f64{cap: keys.len}
		for k in keys {
			nums << strconv.atof64(k.trim_space()) or { 0.0 }
		}
		idxs.sort_with_compare(fn [nums] (a &int, b &int) int {
			if nums[*a] < nums[*b] {
				return -1
			}
			if nums[*a] > nums[*b] {
				return 1
			}
			return 0
		})
	} else {
		lows := keys.map(it.to_lower())
		idxs.sort_with_compare(fn [lows] (a &int, b &int) int {
			return compare_strings(lows[*a], lows[*b])
		})
	}
	if !ascending {
		idxs.reverse_in_place()
	}
	return win.set_table_rows(name, idxs.map(rows[it]))
}

// move_table_row moves the row at index `from` to index `to`.
pub fn (win &SimpleWindow) move_table_row(name string, from int, to int) &SimpleWindow {
	mut rows := win.get_table_rows(name)
	if from < 0 || from >= rows.len || to < 0 || to >= rows.len || from == to {
		return win
	}
	row := rows[from]
	rows.delete(from)
	rows.insert(to, row)
	return win.set_table_rows(name, rows)
}

// save_table_to_csv exports every table row to a CSV file.
// If a relative path is passed, it resolves to the recommended user application storage location.
pub fn (win &SimpleWindow) save_table_to_csv(name string, path string) ! {
	target_path := win.resolve_storage_path(path)
	mut writer := csv.new_writer()
	for row in win.get_table_rows(name) {
		writer.write(row)!
	}
	os.write_file(target_path, writer.str())!
}

// load_table_from_csv replaces a table's rows with the contents of a CSV file.
// Checks relative path and recommended storage location.
pub fn (win &SimpleWindow) load_table_from_csv(name string, path string) ! {
	mut target_path := path
	if !path.starts_with('/') && !path.starts_with('~') {
		if !os.exists(path) {
			target_path = win.resolve_storage_path(path)
		}
	} else if path.starts_with('~') {
		target_path = path.replace('~', os.home_dir())
	}
	content := os.read_file(target_path)!
	mut reader := csv.new_reader(content)
	mut rows := [][]string{}
	for {
		row := reader.read() or { break }
		rows << row
	}
	win.set_table_rows(name, rows)
}

// ==========================================
// 13. Additional Ergonomics & QoL Helpers
// ==========================================

// choose shows a Choice/Dropdown dialog box and returns the selected 0-based option index.
pub fn (win &SimpleWindow) choose(title string, message string, choices []string) int {
	return win.choice_dialog(title, message, choices)
}

// ask_text prompts the user for text input with a dialog box, returning the response.
pub fn (win &SimpleWindow) ask_text(title string, message string, default_val string) string {
	return win.prompt(title, message, default_val)
}

// choose_file opens a native file dialog selection panel.
pub fn (win &SimpleWindow) choose_file() string {
	return win.select_file()
}

// choose_file_ext opens a native file dialog selection panel filtered by file extensions.
pub fn (win &SimpleWindow) choose_file_ext(extensions string) string {
	return win.select_file_with_extensions(extensions)
}

// choose_folder opens a native directory selection panel.
pub fn (win &SimpleWindow) choose_folder() string {
	return win.select_folder()
}

// choose_save_file opens a native save file dialog panel.
pub fn (win &SimpleWindow) choose_save_file() string {
	return win.save_file_picker()
}

// get_int_or returns the numeric value of a control, or fallback if empty or invalid.
pub fn (win &SimpleWindow) get_int_or(name string, fallback int) int {
	if !win.has_control(name) {
		return fallback
	}
	kind := win.get_control_kind(name)
	if kind in ['input', 'password', 'textarea', 'label'] {
		val := win.get_value(name).trim_space()
		if val == '' {
			return fallback
		}
		return val.int()
	}
	return win.get_number_value(name)
}

// get_float_or returns the floating point value of a control, or fallback if empty or invalid.
pub fn (win &SimpleWindow) get_float_or(name string, fallback f64) f64 {
	if !win.has_control(name) {
		return fallback
	}
	kind := win.get_control_kind(name)
	if kind in ['input', 'password', 'textarea', 'label'] {
		val := win.get_value(name).trim_space()
		if val == '' {
			return fallback
		}
		return strconv.atof64(val) or { fallback }
	}
	return win.get_number_value(name)
}

// get_text_or returns the text value of a control, or fallback if empty.
pub fn (win &SimpleWindow) get_text_or(name string, fallback string) string {
	if !win.has_control(name) {
		return fallback
	}
	val := win.get_text(name)
	if val == '' {
		return fallback
	}
	return val
}

// clear_errors_for clears the inline error state for the specified list of controls.
pub fn (win &SimpleWindow) clear_errors_for(names []string) &SimpleWindow {
	for name in names {
		win.clear_error(name)
	}
	return win
}

// add_list_items appends multiple items to the end of a list box.
pub fn (win &SimpleWindow) add_list_items(name string, new_items []string) &SimpleWindow {
	if new_items.len == 0 {
		return win
	}
	mut items := win.get_list_items(name)
	items << new_items
	return win.update_list_items(name, items)
}

// add_table_rows appends multiple rows to the end of a table.
pub fn (win &SimpleWindow) add_table_rows(name string, new_rows [][]string) &SimpleWindow {
	if new_rows.len == 0 {
		return win
	}
	mut rows := win.get_table_rows(name)
	rows << new_rows
	return win.set_table_rows(name, rows)
}

// move_selected_list_item_up moves the currently selected list box item up by one slot.
pub fn (win &SimpleWindow) move_selected_list_item_up(name string) &SimpleWindow {
	idx := win.get_list_selected(name)
	if idx > 0 {
		win.move_list_item(name, idx, idx - 1)
		win.set_list_selected(name, idx - 1)
	}
	return win
}

// move_selected_list_item_down moves the currently selected list box item down by one slot.
pub fn (win &SimpleWindow) move_selected_list_item_down(name string) &SimpleWindow {
	idx := win.get_list_selected(name)
	count := win.get_list_count(name)
	if idx >= 0 && idx < count - 1 {
		win.move_list_item(name, idx, idx + 1)
		win.set_list_selected(name, idx + 1)
	}
	return win
}

// move_selected_table_row_up moves the currently selected table row up by one slot.
pub fn (win &SimpleWindow) move_selected_table_row_up(name string) &SimpleWindow {
	idx := win.get_table_selected(name)
	if idx > 0 {
		win.move_table_row(name, idx, idx - 1)
		win.set_table_selected(name, idx - 1)
	}
	return win
}

// move_selected_table_row_down moves the currently selected table row down by one slot.
pub fn (win &SimpleWindow) move_selected_table_row_down(name string) &SimpleWindow {
	idx := win.get_table_selected(name)
	count := win.get_table_row_count(name)
	if idx >= 0 && idx < count - 1 {
		win.move_table_row(name, idx, idx + 1)
		win.set_table_selected(name, idx + 1)
	}
	return win
}

// get_table_column_values extracts all cell values of a specific 0-based column.
pub fn (win &SimpleWindow) get_table_column_values(name string, column int) []string {
	rows := win.get_table_rows(name)
	mut values := []string{cap: rows.len}
	for row in rows {
		if column >= 0 && column < row.len {
			values << row[column]
		} else {
			values << ''
		}
	}
	return values
}

// save_list_to_file writes every list box item to a plain text file, one item per line.
// If a relative path is passed, it resolves to the recommended user application storage location.
pub fn (win &SimpleWindow) save_list_to_file(name string, path string) ! {
	target_path := win.resolve_storage_path(path)
	items := win.get_list_items(name)
	os.write_file(target_path, items.join('\n'))!
}

// load_list_from_file replaces a list box's items with lines from a plain text file.
// Checks relative path and recommended storage location.
pub fn (win &SimpleWindow) load_list_from_file(name string, path string) ! {
	mut target_path := path
	if !path.starts_with('/') && !path.starts_with('~') {
		if !os.exists(path) {
			target_path = win.resolve_storage_path(path)
		}
	} else if path.starts_with('~') {
		target_path = path.replace('~', os.home_dir())
	}
	content := os.read_file(target_path)!
	mut items := []string{}
	for line in content.split_into_lines() {
		trimmed := line.trim_right('\r\n')
		items << trimmed
	}
	win.update_list_items(name, items)
}

// copy_control_to_clipboard copies the text value of a named control to the system clipboard.
pub fn (win &SimpleWindow) copy_control_to_clipboard(name string) &SimpleWindow {
	val := win.get_text(name)
	win.clipboard_copy(val)
	return win
}

// paste_from_clipboard_to_control replaces the named control's text with the system clipboard content.
pub fn (win &SimpleWindow) paste_from_clipboard_to_control(name string) &SimpleWindow {
	val := win.clipboard_read()
	win.set_text(name, val)
	return win
}

// confirm_discard_changes prompts the user if the window has dirty changes.
// Returns true if there are no dirty changes or if the user confirms they want to discard them.
pub fn (win &SimpleWindow) confirm_discard_changes(title string, message string) bool {
	if !win.is_dirty() {
		return true
	}
	return win.ask(title, message)
}

// validate_url is a ready-made ControlValidator that accepts basic http:// or https:// URLs.
pub fn validate_url(value string) string {
	v := value.trim_space()
	if v == '' {
		return 'URL is required'
	}
	if !v.starts_with('http://') && !v.starts_with('https://') {
		return 'Enter a valid URL starting with http:// or https://'
	}
	return ''
}

// validate_alphanumeric is a ready-made ControlValidator that accepts only letters and digits.
pub fn validate_alphanumeric(value string) string {
	v := value.trim_space()
	if v == '' {
		return 'This field is required'
	}
	for c in v {
		if !c.is_letter() && !c.is_digit() {
			return 'Only letters and digits are allowed'
		}
	}
	return ''
}

// max_len_validator builds a ControlValidator requiring at most max characters.
pub fn max_len_validator(max int) ControlValidator {
	return fn [max] (value string) string {
		if value.trim_space().len > max {
			return 'Must be at most ${max} characters'
		}
		return ''
	}
}

// find_list_item returns the 0-based index of the first item matching value, or -1.
pub fn (win &SimpleWindow) find_list_item(name string, item string) int {
	items := win.get_list_items(name)
	for i, val in items {
		if val == item {
			return i
		}
	}
	return -1
}

// has_list_item returns true if the list box contains the item.
pub fn (win &SimpleWindow) has_list_item(name string, item string) bool {
	return win.find_list_item(name, item) != -1
}

// ==========================================
// 14. Advanced Table Querying & Diagnostics
// ==========================================

// get_table_row_where returns the first row where the cell in the specified column matches the value.
pub fn (win &SimpleWindow) get_table_row_where(name string, column int, value string) []string {
	rows := win.get_table_rows(name)
	for row in rows {
		if column >= 0 && column < row.len && row[column] == value {
			return row
		}
	}
	return []string{}
}

// get_table_rows_where returns all rows where the cell in the specified column matches the value.
pub fn (win &SimpleWindow) get_table_rows_where(name string, column int, value string) [][]string {
	rows := win.get_table_rows(name)
	mut matching := [][]string{}
	for row in rows {
		if column >= 0 && column < row.len && row[column] == value {
			matching << row
		}
	}
	return matching
}

// get_table_column_sum returns the sum of all numeric values in a column.
pub fn (win &SimpleWindow) get_table_column_sum(name string, column int) f64 {
	rows := win.get_table_rows(name)
	mut total := 0.0
	for row in rows {
		if column >= 0 && column < row.len {
			cell := row[column].trim_space()
			total += strconv.atof64(cell) or { 0.0 }
		}
	}
	return total
}

// get_table_column_average returns the average of all numeric values in a column.
pub fn (win &SimpleWindow) get_table_column_average(name string, column int) f64 {
	rows := win.get_table_rows(name)
	if rows.len == 0 {
		return 0.0
	}
	sum := win.get_table_column_sum(name, column)
	return sum / f64(rows.len)
}

// get_table_column_average_numeric returns the average of parseable numeric
// values only (non-numeric and missing cells are skipped).
pub fn (win &SimpleWindow) get_table_column_average_numeric(name string, column int) f64 {
	rows := win.get_table_rows(name)
	mut sum := 0.0
	mut numeric_count := 0
	for row in rows {
		if column < 0 || column >= row.len {
			continue
		}
		value := strconv.atof64(row[column].trim_space()) or { continue }
		sum += value
		numeric_count++
	}
	if numeric_count == 0 {
		return 0.0
	}
	return sum / f64(numeric_count)
}

// ==========================================
// 15. JSON Table/List Persistence
// ==========================================

// save_table_to_json exports all table rows to a JSON file.
// If a relative path is passed, it resolves to the recommended user application storage location.
pub fn (win &SimpleWindow) save_table_to_json(name string, path string) ! {
	target_path := win.resolve_storage_path(path)
	rows := win.get_table_rows(name)
	content := json2.encode(rows)
	os.write_file(target_path, content)!
}

// load_table_from_json replaces a table's rows with the contents of a JSON file.
// Checks relative path and recommended storage location.
pub fn (win &SimpleWindow) load_table_from_json(name string, path string) ! {
	mut target_path := path
	if !path.starts_with('/') && !path.starts_with('~') {
		if !os.exists(path) {
			target_path = win.resolve_storage_path(path)
		}
	} else if path.starts_with('~') {
		target_path = path.replace('~', os.home_dir())
	}
	content := os.read_file(target_path)!
	rows := json2.decode[[][]string](content)!
	win.set_table_rows(name, rows)
}

// save_list_to_json exports all list items to a JSON file.
// If a relative path is passed, it resolves to the recommended user application storage location.
pub fn (win &SimpleWindow) save_list_to_json(name string, path string) ! {
	target_path := win.resolve_storage_path(path)
	items := win.get_list_items(name)
	content := json2.encode(items)
	os.write_file(target_path, content)!
}

// load_list_from_json replaces a list's items with the contents of a JSON file.
// Checks relative path and recommended storage location.
pub fn (win &SimpleWindow) load_list_from_json(name string, path string) ! {
	mut target_path := path
	if !path.starts_with('/') && !path.starts_with('~') {
		if !os.exists(path) {
			target_path = win.resolve_storage_path(path)
		}
	} else if path.starts_with('~') {
		target_path = path.replace('~', os.home_dir())
	}
	content := os.read_file(target_path)!
	items := json2.decode[[]string](content)!
	win.update_list_items(name, items)
}

// ==========================================
// 14. Animation & Transition Helpers
// ==========================================

// animate_opacity animates the window's opacity to the target value over a duration.
pub fn (win &SimpleWindow) animate_opacity(opacity f64, duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_opacity(win.window_info, opacity, duration_ms)
	}
	return win
}

// animate_control_opacity animates a specific control's opacity to the target value.
pub fn (win &SimpleWindow) animate_control_opacity(name string, opacity f64, duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_control_opacity(win.window_info, name.str, opacity, duration_ms)
	}
	return win
}

// fade_in is a helper to fade a control to 1.0 opacity.
pub fn (win &SimpleWindow) fade_in(name string, duration_ms int) &SimpleWindow {
	return win.animate_control_opacity(name, 1.0, duration_ms)
}

// fade_out is a helper to fade a control to 0.0 opacity.
pub fn (win &SimpleWindow) fade_out(name string, duration_ms int) &SimpleWindow {
	return win.animate_control_opacity(name, 0.0, duration_ms)
}

// fade_in_window is a helper to fade the window to 1.0 opacity.
pub fn (win &SimpleWindow) fade_in_window(duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_fade_in(win.window_info, duration_ms)
		return win
	}
	return win.animate_opacity(1.0, duration_ms)
}

// fade_out_window is a helper to fade the window to 0.0 opacity.
pub fn (win &SimpleWindow) fade_out_window(duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_fade_out(win.window_info, duration_ms)
		return win
	}
	return win.animate_opacity(0.0, duration_ms)
}

// shake shakes a specific control horizontally.
pub fn (win &SimpleWindow) shake(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_control_shake(win.window_info, name.str)
	}
	return win
}

// shake_window shakes the entire window.
pub fn (win &SimpleWindow) shake_window() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_shake(win.window_info)
	}
	return win
}

// animate_width animates the width constraint of a control.
pub fn (win &SimpleWindow) animate_width(name string, width int, duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_control_width(win.window_info, name.str, width, duration_ms)
	}
	return win
}

// animate_height animates the height constraint of a control.
pub fn (win &SimpleWindow) animate_height(name string, height int, duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_control_height(win.window_info, name.str, height, duration_ms)
	}
	return win
}

// animate_size animates both width and height constraints of a control.
pub fn (win &SimpleWindow) animate_size(name string, width int, height int, duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_control_size(win.window_info, name.str, width, height, duration_ms)
	}
	return win
}

// animate_window_size animates the window's size.
pub fn (win &SimpleWindow) animate_window_size(width int, height int, duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_size(win.window_info, width, height, duration_ms)
	}
	return win
}

// animate_window_position animates the window's position (x, y).
pub fn (win &SimpleWindow) animate_window_position(x int, y int, duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_position(win.window_info, x, y, duration_ms)
	}
	return win
}

// animate_window_bounds animates both window position and size.
pub fn (win &SimpleWindow) animate_window_bounds(x int, y int, width int, height int, duration_ms int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_animate_bounds(win.window_info, x, y, width, height, duration_ms)
	}
	return win
}

// ==========================================
// 15. Global Reset & Cleanup Helpers
// ==========================================

// clear_all_errors clears the inline error state for all registered controls.
pub fn (win &SimpleWindow) clear_all_errors() &SimpleWindow {
	for control in win.controls {
		win.clear_error(control.name)
	}
	return win
}

// clear_all_fields clears the text or boolean states of all controls to empty/unchecked.
pub fn (win &SimpleWindow) clear_all_fields() &SimpleWindow {
	for control in win.controls {
		if control.kind in ['checkbox', 'switch', 'spinner'] {
			win.set_checked(control.name, false)
		} else if control.kind in ['number', 'slider', 'progress', 'levelindicator', 'stepper',
			'knob'] {
			win.set_value_int(control.name, 0)
		} else if control.kind in ['input', 'password', 'textarea', 'date', 'mode', 'theme', 'listbox',
			'color', 'search', 'dropdown', 'segmented', 'radiogroup', 'combobox', 'pathcontrol',
			'tokenfield'] {
			win.set_text(control.name, '')
		}
	}
	return win
}

// reset_all_fields restores every control in the window to its initial default value.
pub fn (win &SimpleWindow) reset_all_fields() &SimpleWindow {
	mut names := []string{cap: win.controls.len}
	for control in win.controls {
		names << control.name
	}
	return win.reset_many(names)
}

// ==========================================
// 16. Additional Ready-Made Validation Rules
// ==========================================

// validate_ip is a ready-made ControlValidator that accepts basic IPv4 addresses.
pub fn validate_ip(value string) string {
	v := value.trim_space()
	if v == '' {
		return 'IP address is required'
	}
	parts := v.split('.')
	if parts.len != 4 {
		return 'Enter a valid IPv4 address (e.g. 192.168.1.1)'
	}
	for part in parts {
		if part == '' {
			return 'Enter a valid IPv4 address (e.g. 192.168.1.1)'
		}
		for c in part {
			if !c.is_digit() {
				return 'Enter a valid IPv4 address (e.g. 192.168.1.1)'
			}
		}
		val := part.int()
		if val < 0 || val > 255 {
			return 'IP segments must be between 0 and 255'
		}
	}
	return ''
}

// validate_phone is a ready-made ControlValidator that accepts digits, spaces, hyphens, parentheses, and a leading plus.
pub fn validate_phone(value string) string {
	v := value.trim_space()
	if v == '' {
		return 'Phone number is required'
	}
	mut digit_count := 0
	for i, c in v {
		if c.is_digit() {
			digit_count++
		} else if c in [` `, `-`, `(`, `)`] {
			continue
		} else if c == `+` && i == 0 {
			continue
		} else {
			return 'Enter a valid phone number (only digits, spaces, -, (), + are allowed)'
		}
	}
	if digit_count < 7 {
		return 'Phone number is too short'
	}
	return ''
}

// range_validator builds a ControlValidator requiring the numeric value of the control to be within [min, max].
pub fn range_validator(min f64, max f64) ControlValidator {
	return fn [min, max] (value string) string {
		v := value.trim_space()
		if v == '' {
			return 'A value is required'
		}
		num := strconv.atof64(v) or { return 'Enter a valid number' }
		if num < min || num > max {
			return 'Value must be between ${min} and ${max}'
		}
		return ''
	}
}

// ==========================================
// 17. Token Field (Bubble Tags) Ergonomics
// ==========================================

// get_tokens parses the comma-separated text value of a token field into a cleaned slice of strings.
pub fn (win &SimpleWindow) get_tokens(name string) []string {
	val := win.get_text(name)
	mut tokens := []string{}
	for part in val.split(',') {
		trimmed := part.trim_space()
		if trimmed != '' {
			tokens << trimmed
		}
	}
	return tokens
}

// set_tokens sets the value of a token field from a slice of strings.
pub fn (win &SimpleWindow) set_tokens(name string, tokens []string) &SimpleWindow {
	mut cleaned := []string{}
	for t in tokens {
		trimmed := t.trim_space()
		if trimmed != '' {
			cleaned << trimmed
		}
	}
	win.set_text(name, cleaned.join(', '))
	return win
}

// add_token appends a token to the token field if it isn't already present.
pub fn (win &SimpleWindow) add_token(name string, token string) &SimpleWindow {
	trimmed := token.trim_space()
	if trimmed == '' {
		return win
	}
	mut tokens := win.get_tokens(name)
	if trimmed !in tokens {
		tokens << trimmed
		win.set_tokens(name, tokens)
	}
	return win
}

// remove_token removes a token from the token field if present.
pub fn (win &SimpleWindow) remove_token(name string, token string) &SimpleWindow {
	trimmed := token.trim_space()
	if trimmed == '' {
		return win
	}
	mut tokens := win.get_tokens(name)
	idx := tokens.index(trimmed)
	if idx >= 0 {
		tokens.delete(idx)
		win.set_tokens(name, tokens)
	}
	return win
}

// ==========================================
// 18. Advanced Table Mapping & Filtering
// ==========================================

// has_table_row returns true if any row has row[column] == value.
pub fn (win &SimpleWindow) has_table_row(name string, column int, value string) bool {
	return win.find_table_row(name, column, value) != -1
}

// map_table_column maps a function over all values of a specific table column in-place.
pub fn (win &SimpleWindow) map_table_column(name string, column int, f fn (val string) string) &SimpleWindow {
	mut rows := win.get_table_rows(name)
	mut modified := false
	for mut row in rows {
		if column >= 0 && column < row.len {
			row[column] = f(row[column])
			modified = true
		}
	}
	if modified {
		win.set_table_rows(name, rows)
	}
	return win
}

// filter_table_rows returns a filtered copy of rows that match the predicate.
pub fn (win &SimpleWindow) filter_table_rows(name string, predicate fn (row []string) bool) [][]string {
	rows := win.get_table_rows(name)
	mut filtered := [][]string{}
	for row in rows {
		if predicate(row) {
			filtered << row
		}
	}
	return filtered
}

// find_table_row_where returns the index of the first row matching the predicate, or -1.
pub fn (win &SimpleWindow) find_table_row_where(name string, predicate fn (row []string) bool) int {
	rows := win.get_table_rows(name)
	for i, row in rows {
		if predicate(row) {
			return i
		}
	}
	return -1
}

// ==========================================
// 19. List Box Row Insertion & Safely Selected Text
// ==========================================

// insert_list_item inserts a single item at the given 0-based index.
pub fn (win &SimpleWindow) insert_list_item(name string, index int, item string) &SimpleWindow {
	mut items := win.get_list_items(name)
	if index >= 0 && index <= items.len {
		items.insert(index, item)
		win.update_list_items(name, items)
	}
	return win
}

// update_list_item replaces the list item at the given 0-based index.
pub fn (win &SimpleWindow) update_list_item(name string, index int, item string) &SimpleWindow {
	mut items := win.get_list_items(name)
	if index >= 0 && index < items.len {
		items[index] = item
		win.update_list_items(name, items)
	}
	return win
}

// get_list_selected_text_or returns the text of the selected row, or a fallback if none selected.
pub fn (win &SimpleWindow) get_list_selected_text_or(name string, fallback string) string {
	val := win.get_list_selected_text(name)
	if val == '' {
		return fallback
	}
	return val
}

// ==========================================
// 20. Widget QoL Helpers
// ==========================================

// toggle_spinner flips a spinner's active/spinning state and returns the new active state.
pub fn (win &SimpleWindow) toggle_spinner(name string) bool {
	new_state := !win.get_bool(name)
	win.set_bool(name, new_state)
	return new_state
}

// start_spinner starts the spinner's animation.
pub fn (win &SimpleWindow) start_spinner(name string) &SimpleWindow {
	win.set_bool(name, true)
	return win
}

// stop_spinner stops the spinner's animation.
pub fn (win &SimpleWindow) stop_spinner(name string) &SimpleWindow {
	win.set_bool(name, false)
	return win
}

// increment_progress increments/decrements a progress bar value, bounding it in 0..100.
pub fn (win &SimpleWindow) increment_progress(name string, delta int) int {
	mut current := win.get_progress(name)
	mut new_val := current + delta
	if new_val < 0 {
		new_val = 0
	}
	if new_val > 100 {
		new_val = 100
	}
	win.set_progress(name, new_val)
	return new_val
}

// set_stat_card updates the metric value, trend string, and trend style for a stat card.
pub fn (win &SimpleWindow) set_stat_card(name string, value string, trend string, trend_style string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_stat_card_value(win.window_info, name.str, value.str, trend.str,
			trend_style.str)
	}
	return win
}

// set_banner updates the message text of a banner callout.
pub fn (win &SimpleWindow) set_banner(name string, text string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_banner_value(win.window_info, name.str, text.str)
	}
	return win.set_text(name, text)
}

// set_badge updates the text and style of a badge control.
pub fn (win &SimpleWindow) set_badge(name string, text string, style string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_badge_value(win.window_info, name.str, text.str, style.str)
	}
	return win.set_text(name, text)
}

// get_badge retrieves the text of a badge control.
pub fn (win &SimpleWindow) get_badge(name string) string {
	return win.get_text(name)
}

// set_status_indicator updates the status state (active, warning, error, idle) of a status indicator light.
pub fn (win &SimpleWindow) set_status_indicator(name string, status string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_status_indicator_value(win.window_info, name.str, status.str)
	}
	return win.set_text(name, status)
}

// get_status_indicator retrieves the status string of a status indicator control.
pub fn (win &SimpleWindow) get_status_indicator(name string) string {
	return win.get_text(name)
}

// set_metric_meter updates the numeric value of a metric meter control.
pub fn (win &SimpleWindow) set_metric_meter(name string, value int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_metric_meter_value(win.window_info, name.str, value)
	}
	return win.set_value_int(name, value)
}

// get_metric_meter retrieves the current integer value of a metric meter control.
pub fn (win &SimpleWindow) get_metric_meter(name string) int {
	return win.get_value_int(name)
}

// set_avatar_card updates title, subtitle, and status pill of an avatar card.
pub fn (win &SimpleWindow) set_avatar_card(name string, title string, subtitle string, status string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_avatar_card_value(win.window_info, name.str, title.str, subtitle.str,
			status.str)
	}
	return win
}

// get_vertical_slider retrieves the integer value of a vertical slider control.
pub fn (win &SimpleWindow) get_vertical_slider(name string) int {
	return win.get_value_int(name)
}

// set_vertical_slider updates the integer value of a vertical slider control.
pub fn (win &SimpleWindow) set_vertical_slider(name string, value int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_vertical_slider_value(win.window_info, name.str, value)
	}
	return win.set_value_int(name, value)
}

// get_chip_selected returns the currently selected chip in a chip group.
pub fn (win &SimpleWindow) get_chip_selected(name string) string {
	return win.get_text(name)
}

// set_chip_selected sets the selected chip in a chip group.
pub fn (win &SimpleWindow) set_chip_selected(name string, chip string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_chip_group_selected(win.window_info, name.str, chip.str)
	}
	return win.set_text(name, chip)
}

// set_time_picker updates the time value of a standalone time picker control.
pub fn (win &SimpleWindow) set_time_picker(name string, time_value string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_time_picker_value(win.window_info, name.str, time_value.str)
	}
	return win.set_text(name, time_value)
}

// get_time_picker retrieves the selected time string from a time picker control.
pub fn (win &SimpleWindow) get_time_picker(name string) string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_time_picker_value(win.window_info, name.str)
		if res != unsafe { nil } {
			unsafe {
				if res[0] != 0 {
					return tos_clone(res)
				}
			}
		}
	}
	return win.get_text(name)
}

// set_tray_icon updates the symbol and title of a system menu bar tray icon.
pub fn (win &SimpleWindow) set_tray_icon(name string, symbol string, title string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_tray_icon_value(win.window_info, name.str, symbol.str, title.str)
	}
	return win.set_text(name, title)
}

// set_collapsible_section_expanded updates the disclosure expand state of a collapsible section.
pub fn (win &SimpleWindow) set_collapsible_section_expanded(name string, expanded bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_collapsible_section_expanded(win.window_info, name.str, if expanded {
			1
		} else {
			0
		})
	}
	return win
}

// set_code_editor updates the text content of a code editor view.
pub fn (win &SimpleWindow) set_code_editor(name string, code string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_code_editor_value(win.window_info, name.str, code.str)
	}
	return win.set_text(name, code)
}

// get_code_editor retrieves the text content of a code editor view.
pub fn (win &SimpleWindow) get_code_editor(name string) string {
	if win.window_info != unsafe { nil } {
		res := C.window_get_code_editor_value(win.window_info, name.str)
		if res != unsafe { nil } {
			unsafe {
				if res[0] != 0 {
					return tos_clone(res)
				}
			}
		}
	}
	return win.get_text(name)
}

// add_timeline_entry appends an event item with timestamp, title, detail, and status color to a timeline stream.
pub fn (win &SimpleWindow) add_timeline_entry(name string, time_str string, title string, detail string, style string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_add_timeline_entry(win.window_info, name.str, time_str.str, title.str,
			detail.str, style.str)
	}
	return win
}

// clear_timeline removes all event items from a timeline activity stream view.
pub fn (win &SimpleWindow) clear_timeline(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_clear_timeline(win.window_info, name.str)
	}
	return win
}

// set_toolbar_visible toggles the visibility of the native window titlebar toolbar.
pub fn (win &SimpleWindow) set_toolbar_visible(visible bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_toolbar_visible(win.window_info, if visible { 1 } else { 0 })
	}
	return win
}

// add_labeled_file_picker adds a descriptive label above or alongside a native file picker field.
pub fn (win &SimpleWindow) add_labeled_file_picker(label string, name string, initial_path string, button_title string, folder_only bool) &SimpleWindow {
	win.add_label('${name}_label', label)
	return win.add_file_picker_field(name, initial_path, button_title, folder_only)
}

// ==========================================
// Window Management Ergonomic Shortcuts
// ==========================================

// make_frameless turns the window into a clean borderless window with optional shadow.
pub fn (win &SimpleWindow) make_frameless() &SimpleWindow {
	return win.set_titlebar_visible(false).set_has_shadow(true)
}

// make_vibrant configures window background vibrancy material and background blur.
pub fn (win &SimpleWindow) make_vibrant(material string) &SimpleWindow {
	return win.set_vibrancy(material).set_background_blur(true)
}

// make_click_through toggles mouse click-through behavior for overlay windows.
pub fn (win &SimpleWindow) make_click_through(enabled bool) &SimpleWindow {
	return win.set_ignores_mouse_events(enabled)
}

// make_always_on_top configures window stay-on-top behavior.
pub fn (win &SimpleWindow) make_always_on_top(enabled bool) &SimpleWindow {
	return win.set_always_on_top(enabled)
}

// make_modal sets the window z-level to modal window tier.
pub fn (win &SimpleWindow) make_modal() &SimpleWindow {
	return win.set_window_level('modal')
}

// make_panel configures the window as a floating tool panel that hides when app deactivates.
pub fn (win &SimpleWindow) make_panel() &SimpleWindow {
	return win.set_window_level('floating').set_hides_on_deactivate(true)
}

// center_and_focus centers the window on active display and brings it to front.
pub fn (win &SimpleWindow) center_and_focus() &SimpleWindow {
	return win.center_on_active_screen().order_front()
}

// make_translucent sets window transparency level (0.0 to 1.0).
pub fn (win &SimpleWindow) make_translucent(alpha f64) &SimpleWindow {
	return win.set_alpha(alpha)
}

// make_sticky_space makes the window stick across all Virtual Desktops / Spaces.
pub fn (win &SimpleWindow) make_sticky_space() &SimpleWindow {
	return win.set_collection_behavior('can_join_all_spaces')
}

// shake_on_error triggers window error shake animation and flashes window frame.
pub fn (win &SimpleWindow) shake_on_error() &SimpleWindow {
	return win.shake_window().flash_frame(true)
}

// set_fixed_size locks the window to a fixed width and height and disables resizability.
pub fn (win &SimpleWindow) set_fixed_size(width int, height int) &SimpleWindow {
	return win.set_size(width, height).set_min_size(width, height).set_max_size(width,
		height).set_resizable(false)
}

// set_size_preset resizes the window based on standard preset names (e.g. 'compact', 'medium', 'large', 'hd', 'full_hd', 'dialog', 'login', 'settings', 'sidebar', 'splash', 'square').
pub fn (win &SimpleWindow) set_size_preset(preset string) &SimpleWindow {
	norm := preset.to_lower().trim_space().replace('_', '-').replace(' ', '-')
	match norm {
		'small', 'compact' { return win.set_size(400, 300) }
		'medium', 'standard' { return win.set_size(640, 480) }
		'large' { return win.set_size(800, 600) }
		'xlarge', 'xl' { return win.set_size(1024, 768) }
		'hd', '720p' { return win.set_size(1280, 720) }
		'full-hd', '1080p', 'fhd' { return win.set_size(1920, 1080) }
		'dialog', 'alert', 'popup' { return win.set_size(420, 220) }
		'login', 'auth' { return win.set_size(380, 450) }
		'settings', 'preferences' { return win.set_size(550, 400) }
		'sidebar', 'panel' { return win.set_size(300, 600) }
		'splash' { return win.set_size(500, 300) }
		'square' { return win.set_size(500, 500) }
		else { return win.set_size(640, 480) }
	}
}

// set_preset_size is an alias for set_size_preset.
pub fn (win &SimpleWindow) set_preset_size(preset string) &SimpleWindow {
	return win.set_size_preset(preset)
}

// get_size retrieves the current window width and height as a (width, height) tuple.
pub fn (win &SimpleWindow) get_size() (int, int) {
	return win.get_width(), win.get_height()
}

// set_minimum_size sets the minimum allowed window dimensions. Alias for set_min_size.
pub fn (win &SimpleWindow) set_minimum_size(width int, height int) &SimpleWindow {
	return win.set_min_size(width, height)
}

// set_maximum_size sets the maximum allowed window dimensions. Alias for set_max_size.
pub fn (win &SimpleWindow) set_maximum_size(width int, height int) &SimpleWindow {
	return win.set_max_size(width, height)
}

// get_minimum_size retrieves minimum window dimensions (w, h). Alias for get_min_size.
pub fn (win &SimpleWindow) get_minimum_size() (int, int) {
	return win.get_min_size()
}

// get_maximum_size retrieves maximum window dimensions (w, h). Alias for get_max_size.
pub fn (win &SimpleWindow) get_maximum_size() (int, int) {
	return win.get_max_size()
}

// get_position returns current screen coordinates as a (x, y) tuple.
pub fn (win &SimpleWindow) get_position() (int, int) {
	return win.get_x(), win.get_y()
}

// set_position_preset positions the window on screen based on standard corner or edge presets.
pub fn (win &SimpleWindow) set_position_preset(preset string) &SimpleWindow {
	norm := preset.to_lower().trim_space().replace('_', '-').replace(' ', '-')
	match norm {
		'top-left' { return win.align('top-left') }
		'top-right' { return win.align('top-right') }
		'bottom-left' { return win.align('bottom-left') }
		'bottom-right' { return win.align('bottom-right') }
		'top-center' { return win.align('top-center') }
		'bottom-center' { return win.align('bottom-center') }
		'center', 'middle', 'middle-center', 'screen-center' { return win.center() }
		'left', 'middle-left' { return win.align('middle-left') }
		'right', 'middle-right' { return win.align('middle-right') }
		else { return win.center() }
	}
}

// set_corner_position is an alias for set_position_preset.
pub fn (win &SimpleWindow) set_corner_position(corner string) &SimpleWindow {
	return win.set_position_preset(corner)
}

// recenter is a friendly alias for win.center().
pub fn (win &SimpleWindow) recenter() &SimpleWindow {
	return win.center()
}

// center_on_screen centers the window on the active display. Alias for center_on_active_screen.
pub fn (win &SimpleWindow) center_on_screen() &SimpleWindow {
	return win.center_on_active_screen()
}

// show unhides the window and brings it to front. Alias for show_window.
pub fn (win &SimpleWindow) show() &SimpleWindow {
	return win.show_window()
}

// set_window_title sets the window title bar text. Alias for set_title.
pub fn (win &SimpleWindow) set_window_title(title string) &SimpleWindow {
	return win.set_title(title)
}

// set_topmost keeps the window above other application windows. Alias for set_always_on_top.
pub fn (win &SimpleWindow) set_topmost(enabled bool) &SimpleWindow {
	return win.set_always_on_top(enabled)
}

// is_topmost checks if the window is configured to stay above other windows. Alias for get_always_on_top.
pub fn (win &SimpleWindow) is_topmost() bool {
	return win.get_always_on_top()
}

// is_frameless returns true if the window titlebar is hidden.
pub fn (win &SimpleWindow) is_frameless() bool {
	return !win.is_titlebar_visible()
}

// set_dark_theme toggles window theme between Apple Dark and Apple Light.
pub fn (win &SimpleWindow) set_dark_theme(dark bool) &SimpleWindow {
	if dark {
		return win.set_theme('Apple Dark')
	}
	return win.set_theme('Apple Light')
}

// toggle_window_theme switches window theme between Light Mode and Dark Mode.
pub fn (win &SimpleWindow) toggle_window_theme() &SimpleWindow {
	if win.is_dark_theme() {
		return win.set_theme('Apple Light')
	}
	return win.set_theme('Apple Dark')
}

// is_dark_theme reports whether the window currently has a dark background color.
pub fn (win &SimpleWindow) is_dark_theme() bool {
	bg := win.background_color.to_lower().trim_space()
	if bg in ['#1c1c1e', '#161618', '#281a24', '#0d1f18', '#211815', '#1e1e2e', '#2e3440', '#282a36',
		'#0d0d15', '#002b36', '#0d1117', '#0f172a', '#14532d', 'dark', 'black'] {
		return true
	}
	if bg.starts_with('#') && bg.len >= 7 {
		r := hex_byte_val(bg[1..3])
		g := hex_byte_val(bg[3..5])
		b := hex_byte_val(bg[5..7])
		return ((r * 299) + (g * 587) + (b * 114)) / 1000 < 128
	}
	return false
}

fn hex_byte_val(s string) int {
	if s.len < 2 {
		return 255
	}
	mut val := 0
	for c in s {
		val *= 16
		if c >= `0` && c <= `9` {
			val += int(c - `0`)
		} else if c >= `a` && c <= `f` {
			val += int(10 + c - `a`)
		} else if c >= `A` && c <= `F` {
			val += int(10 + c - `A`)
		}
	}
	return val
}

// restore restores the window from minimized or maximized state to standard layout.
pub fn (win &SimpleWindow) restore() &SimpleWindow {
	if win.is_minimized() {
		win.deminimize()
	}
	if win.is_maximized() {
		win.unmaximize()
	}
	return win
}

// restore_window is an alias for restore.
pub fn (win &SimpleWindow) restore_window() &SimpleWindow {
	return win.restore()
}

// trigger_shake triggers an animated horizontal window shake. Alias for shake_window.
pub fn (win &SimpleWindow) trigger_shake() &SimpleWindow {
	return win.shake_window()
}

// flash_and_shake flashes the window frame and triggers an animated shake for error feedback.
pub fn (win &SimpleWindow) flash_and_shake() &SimpleWindow {
	return win.flash_frame(true).shake_window()
}

// attention bounces the Dock icon to catch user attention. Alias for bounce_dock_icon(true).
pub fn (win &SimpleWindow) attention() &SimpleWindow {
	return win.bounce_dock_icon(true)
}

// make_fixed_dialog configures window as a fixed-size dialog (title, size, centered, non-resizable, no min/max buttons).
pub fn (win &SimpleWindow) make_fixed_dialog(title string, width int, height int) &SimpleWindow {
	return win.set_title(title)
		.set_fixed_size(width, height)
		.center()
		.set_minimizable(false)
		.set_maximizable(false)
}

// make_splash_screen configures window as a borderless centered splash screen.
pub fn (win &SimpleWindow) make_splash_screen(width int, height int) &SimpleWindow {
	return win.make_frameless()
		.set_fixed_size(width, height)
		.center()
		.set_always_on_top(true)
}

// make_utility_panel configures window as a floating tool panel (always on top, HUD vibrancy, auto-hide on blur).
pub fn (win &SimpleWindow) make_utility_panel() &SimpleWindow {
	return win.set_window_level('floating')
		.set_hides_on_deactivate(true)
		.set_always_on_top(true)
		.set_vibrancy('hud')
}

// ==========================================
// 21. Reactive Bindings & Data QoL
// ==========================================

// confirm_then shows a Yes/No dialog and runs the callback only when the user
// confirms. Returns true when the callback was executed.
pub fn (win &SimpleWindow) confirm_then(title string, question string, callback VoidEventCallback) bool {
	if !win.confirm(title, question) {
		return false
	}
	unsafe {
		mut w := &SimpleWindow(win)
		callback(mut w)
	}
	return true
}

// bind_value_to_label mirrors a control's value into a label whenever it
// changes, rendered as prefix + value + suffix. The current value is applied
// immediately. Works with any control that fires on_change (sliders, steppers,
// inputs, dropdowns, checkboxes, ...).
pub fn (win &SimpleWindow) bind_value_to_label(source string, label string, prefix string, suffix string) &SimpleWindow {
	win.set_text(label, prefix + win.get_value(source) + suffix)
	win.on_change(source, fn [label, prefix, suffix] (mut w SimpleWindow, value string) {
		w.set_text(label, prefix + value + suffix)
	})
	return win
}

// bind_checkbox_enables keeps a group of controls enabled while the checkbox
// (or switch) is checked and disabled while it is unchecked. The current
// checkbox state is applied immediately.
pub fn (win &SimpleWindow) bind_checkbox_enables(checkbox string, names []string) &SimpleWindow {
	checked := win.get_checked(checkbox)
	for name in names {
		win.set_control_enabled(name, checked)
	}
	win.on_change(checkbox, fn [checkbox, names] (mut w SimpleWindow, value string) {
		state := w.get_checked(checkbox)
		for name in names {
			w.set_control_enabled(name, state)
		}
	})
	return win
}

// bind_char_counter keeps a label updated with "used/max" as the user types
// and flags the input with an inline error while the limit is exceeded.
pub fn (win &SimpleWindow) bind_char_counter(input string, counter_label string, max int) &SimpleWindow {
	win.set_text(counter_label, '${win.get_text(input).runes().len}/${max}')
	win.on_change(input, fn [input, counter_label, max] (mut w SimpleWindow, value string) {
		used := value.runes().len
		w.set_text(counter_label, '${used}/${max}')
		if used > max {
			w.set_error(input, 'Maximum ${max} characters')
		} else {
			w.clear_error(input)
		}
	})
	return win
}

// bind_checkbox_disables keeps a group of controls disabled while the checkbox
// (or switch) is checked and enabled while unchecked. The current checkbox
// state is applied immediately.
pub fn (win &SimpleWindow) bind_checkbox_disables(checkbox string, names []string) &SimpleWindow {
	checked := win.get_checked(checkbox)
	for name in names {
		win.set_control_enabled(name, !checked)
	}
	win.on_change(checkbox, fn [checkbox, names] (mut w SimpleWindow, value string) {
		state := w.get_checked(checkbox)
		for name in names {
			w.set_control_enabled(name, !state)
		}
	})
	return win
}

// bind_checkbox_shows shows (unhides) a group of controls while the checkbox
// (or switch) is checked and hides them while unchecked. Applied immediately.
pub fn (win &SimpleWindow) bind_checkbox_shows(checkbox string, names []string) &SimpleWindow {
	checked := win.get_checked(checkbox)
	for name in names {
		win.set_control_visible(name, checked)
	}
	win.on_change(checkbox, fn [checkbox, names] (mut w SimpleWindow, value string) {
		state := w.get_checked(checkbox)
		for name in names {
			w.set_control_visible(name, state)
		}
	})
	return win
}

// bind_checkbox_hides hides a group of controls while the checkbox (or switch)
// is checked and shows them while unchecked. Applied immediately.
pub fn (win &SimpleWindow) bind_checkbox_hides(checkbox string, names []string) &SimpleWindow {
	checked := win.get_checked(checkbox)
	for name in names {
		win.set_control_visible(name, !checked)
	}
	win.on_change(checkbox, fn [checkbox, names] (mut w SimpleWindow, value string) {
		state := w.get_checked(checkbox)
		for name in names {
			w.set_control_visible(name, !state)
		}
	})
	return win
}

// bind_inputs_to_button disables a button unless ALL specified input fields contain
// non-empty text. Evaluates and applies state immediately and on every change.
pub fn (win &SimpleWindow) bind_inputs_to_button(inputs []string, button string) &SimpleWindow {
	mut initial_valid := true
	for name in inputs {
		if win.get_text(name).trim_space() == '' {
			initial_valid = false
			break
		}
	}
	win.set_control_enabled(button, initial_valid)

	for input_name in inputs {
		win.on_change(input_name, fn [inputs, button] (mut w SimpleWindow, value string) {
			mut valid := true
			for name in inputs {
				if w.get_text(name).trim_space() == '' {
					valid = false
					break
				}
			}
			w.set_control_enabled(button, valid)
		})
	}
	return win
}

// bind_value_to_progress syncs an integer value control (slider, stepper, number)
// to a progress indicator bar. Applied immediately and on change.
pub fn (win &SimpleWindow) bind_value_to_progress(source string, progress string) &SimpleWindow {
	win.set_value_int(progress, win.get_value_int(source))
	win.on_change(source, fn [progress] (mut w SimpleWindow, value string) {
		w.set_value_int(progress, value.int())
	})
	return win
}

// bind_dropdown_to_label updates a label's text dynamically based on a lookup
// map of dropdown option values. Applied immediately and on change.
pub fn (win &SimpleWindow) bind_dropdown_to_label(dropdown string, label string, mapping map[string]string) &SimpleWindow {
	val := win.get_value(dropdown)
	if val in mapping {
		win.set_text(label, mapping[val])
	}
	win.on_change(dropdown, fn [label, mapping] (mut w SimpleWindow, value string) {
		if value in mapping {
			w.set_text(label, mapping[value])
		}
	})
	return win
}

// bind_two_way keeps two controls bi-directionally synchronized (e.g. slider and
// number input) without infinite event feedback loops.
pub fn (win &SimpleWindow) bind_two_way(control_a string, control_b string) &SimpleWindow {
	win.set_text(control_b, win.get_text(control_a))
	win.on_change(control_a, fn [control_b] (mut w SimpleWindow, value string) {
		if w.get_text(control_b) != value {
			w.set_text(control_b, value)
		}
	})
	win.on_change(control_b, fn [control_a] (mut w SimpleWindow, value string) {
		if w.get_text(control_a) != value {
			w.set_text(control_a, value)
		}
	})
	return win
}

// countdown counts a numeric label down to zero once per second, then stops
// its timer and invokes the callback. The label text must hold just the number
// of remaining seconds (e.g. set it to '10' before calling).
pub fn (win &SimpleWindow) countdown(label string, seconds int, callback VoidEventCallback) &SimpleWindow {
	timer_name := 'countdown_${label}'
	win.set_text(label, '${seconds}')
	win.set_interval(timer_name, 1000, fn [label, timer_name, callback] (mut w SimpleWindow) {
		remaining := w.get_text(label).int() - 1
		w.set_text(label, '${if remaining > 0 { remaining } else { 0 }}')
		if remaining <= 0 {
			w.stop_interval(timer_name)
			callback(mut w)
		}
	})
	return win
}

// dedupe_list_items removes duplicate list items, keeping the first occurrence.
pub fn (win &SimpleWindow) dedupe_list_items(name string) &SimpleWindow {
	items := win.get_list_items(name)
	mut seen := map[string]bool{}
	mut unique := []string{cap: items.len}
	for item in items {
		if item !in seen {
			seen[item] = true
			unique << item
		}
	}
	if unique.len != items.len {
		win.update_list_items(name, unique)
	}
	return win
}

// reverse_list_items reverses the order of a list box's items.
pub fn (win &SimpleWindow) reverse_list_items(name string) &SimpleWindow {
	mut items := win.get_list_items(name)
	items.reverse_in_place()
	return win.update_list_items(name, items)
}

// keep_list_items keeps only the list items matching the predicate
// (a destructive filter).
pub fn (win &SimpleWindow) keep_list_items(name string, predicate fn (item string) bool) &SimpleWindow {
	return win.update_list_items(name, win.get_list_items(name).filter(predicate(it)))
}

// map_list_items rewrites every list item through the transform function.
pub fn (win &SimpleWindow) map_list_items(name string, f fn (item string) string) &SimpleWindow {
	return win.update_list_items(name, win.get_list_items(name).map(f(it)))
}

// dedupe_table_rows removes duplicate table rows (all cells equal), keeping
// the first occurrence.
pub fn (win &SimpleWindow) dedupe_table_rows(name string) &SimpleWindow {
	rows := win.get_table_rows(name)
	mut seen := map[string]bool{}
	mut unique := [][]string{cap: rows.len}
	for row in rows {
		key := row.join('\x00')
		if key !in seen {
			seen[key] = true
			unique << row
		}
	}
	if unique.len != rows.len {
		win.set_table_rows(name, unique)
	}
	return win
}

// count_table_rows_where returns how many table rows match the predicate.
pub fn (win &SimpleWindow) count_table_rows_where(name string, predicate fn (row []string) bool) int {
	mut count := 0
	for row in win.get_table_rows(name) {
		if predicate(row) {
			count++
		}
	}
	return count
}

// copy_list_to_clipboard copies all list box items to the clipboard,
// one item per line.
pub fn (win &SimpleWindow) copy_list_to_clipboard(name string) &SimpleWindow {
	return win.copy_to_clipboard(win.get_list_items(name).join('\n'))
}

// copy_table_to_clipboard copies all table rows to the clipboard as
// tab-separated lines — ready to paste into Numbers, Excel, or a spreadsheet.
pub fn (win &SimpleWindow) copy_table_to_clipboard(name string) &SimpleWindow {
	return win.copy_to_clipboard(win.get_table_rows(name).map(it.join('\t')).join('\n'))
}

// required_validator builds a validator that rejects empty or
// whitespace-only values.
pub fn required_validator() ControlValidator {
	return fn (value string) string {
		if value.trim_space() == '' {
			return 'This field is required'
		}
		return ''
	}
}

// one_of_validator builds a validator requiring the value to be one of the
// given options (case-insensitive, whitespace trimmed).
pub fn one_of_validator(options []string) ControlValidator {
	return fn [options] (value string) string {
		low := value.trim_space().to_lower()
		for opt in options {
			if opt.to_lower() == low {
				return ''
			}
		}
		return 'Must be one of: ${options.join(', ')}'
	}
}

// chain_validators combines several validators into a single one; the first
// non-empty error message wins.
pub fn chain_validators(validators ...ControlValidator) ControlValidator {
	return fn [validators] (value string) string {
		for validator in validators {
			msg := validator(value)
			if msg != '' {
				return msg
			}
		}
		return ''
	}
}

// ==========================================
// 22. Workflow, Text & Data Extras
// ==========================================
struct DebounceState {
mut:
	value string
}

// on_change_debounced fires the callback only after the user stops changing
// the control for `ms` milliseconds — perfect for search-as-you-type without
// hammering expensive work on every keystroke. The callback receives the most
// recent value.
pub fn (win &SimpleWindow) on_change_debounced(name string, ms int, callback StringEventCallback) &SimpleWindow {
	timer_name := 'debounce_${name}'
	mut state := &DebounceState{}
	// Register the one-shot timer handler once; it always reads the latest value.
	idx := win.find_handler(timer_name, 'timer')
	handler := ControlEventHandler{
		control_name: timer_name
		event_name:   'timer'
		void_cb:      fn [timer_name, state, callback] (mut w SimpleWindow) {
			w.stop_interval(timer_name)
			callback(mut w, state.value)
		}
	}
	unsafe {
		mut w := &SimpleWindow(win)
		if idx >= 0 {
			w.handlers[idx] = handler
		} else {
			w.handlers << handler
		}
	}
	win.on_change(name, fn [timer_name, ms, mut state] (mut w SimpleWindow, value string) {
		state.value = value
		w.stop_interval(timer_name)
		if w.window_info != unsafe { nil } {
			C.window_set_interval(w.window_info, ms, timer_name.str)
		}
	})
	return win
}

// submit_on_enter registers the same Enter-key callback on several text
// controls at once — handy for "press Enter anywhere in the form to submit".
pub fn (win &SimpleWindow) submit_on_enter(names []string, callback VoidEventCallback) &SimpleWindow {
	for name in names {
		win.on_enter(name, callback)
	}
	return win
}

// ask_int prompts the user for a number and returns it, falling back to
// `default_val` when the response is empty, cancelled, or not numeric.
pub fn (win &SimpleWindow) ask_int(title string, message string, default_val int) int {
	resp := win.prompt(title, message, default_val.str()).trim_space()
	if resp == '' || !is_numeric_text(resp) {
		return default_val
	}
	return resp.int()
}

// append_timestamped_line appends a line prefixed with the current [HH:MM:SS]
// time — ideal for activity logs in a textarea.
pub fn (win &SimpleWindow) append_timestamped_line(name string, line string) &SimpleWindow {
	return win.append_line(name, '[${time.now().hhmmss()}] ${line}')
}

// get_word_count returns the number of whitespace-separated words in a
// text control's value.
pub fn (win &SimpleWindow) get_word_count(name string) int {
	return win.get_text(name).fields().len
}

// get_line_count returns the number of lines in a text control's value
// (0 for an empty control).
pub fn (win &SimpleWindow) get_line_count(name string) int {
	text := win.get_text(name)
	if text == '' {
		return 0
	}
	return text.split('\n').len
}

// swap_list_items swaps the items at two indexes in a list box.
pub fn (win &SimpleWindow) swap_list_items(name string, i int, j int) &SimpleWindow {
	mut items := win.get_list_items(name)
	if i < 0 || j < 0 || i >= items.len || j >= items.len || i == j {
		return win
	}
	tmp := items[i]
	items[i] = items[j]
	items[j] = tmp
	return win.update_list_items(name, items)
}

// swap_table_rows swaps the rows at two indexes in a table.
pub fn (win &SimpleWindow) swap_table_rows(name string, i int, j int) &SimpleWindow {
	mut rows := win.get_table_rows(name)
	if i < 0 || j < 0 || i >= rows.len || j >= rows.len || i == j {
		return win
	}
	tmp := rows[i]
	rows[i] = rows[j]
	rows[j] = tmp
	return win.set_table_rows(name, rows)
}

// add_table_row_unique appends a row only when an identical row (all cells
// equal) is not already present. Returns true when the row was added.
pub fn (win &SimpleWindow) add_table_row_unique(name string, row []string) bool {
	for existing in win.get_table_rows(name) {
		if existing == row {
			return false
		}
	}
	win.add_table_row(name, row)
	return true
}

// select_list_item_by_text selects the first list row whose text matches
// exactly. Returns true when a match was found and selected.
pub fn (win &SimpleWindow) select_list_item_by_text(name string, text string) bool {
	idx := win.find_list_item(name, text)
	if idx < 0 {
		return false
	}
	win.set_list_selected(name, idx)
	return true
}

// enable_autosave saves every control value to a JSON file at a fixed
// interval (silently skipping failed writes). Pair with load_values_if_exists
// at startup for crash-safe forms.
pub fn (win &SimpleWindow) enable_autosave(path string, interval_ms int) &SimpleWindow {
	return win.set_interval('autosave_values', interval_ms, fn [path] (mut w SimpleWindow) {
		w.save_values_to_file(path) or {}
	})
}

// load_values_if_exists restores control values from a JSON file when it
// exists, returning true when values were loaded.
// Checks both relative path and recommended user storage location.
pub fn (win &SimpleWindow) load_values_if_exists(path string) bool {
	mut target_path := path
	if !path.starts_with('/') && !path.starts_with('~') {
		if !os.exists(path) {
			target_path = win.resolve_storage_path(path)
		}
	} else if path.starts_with('~') {
		target_path = path.replace('~', os.home_dir())
	}
	if !os.exists(target_path) {
		return false
	}
	win.load_values_from_file(target_path) or { return false }
	return true
}

// ==========================================
// 27. Advanced Form Validation & Utility Shortcuts
// ==========================================

// validate_required checks that all named controls contain non-empty string values.
// If any control is empty, it flashes the missing control, sets focus to it, and returns (false, missing_name).
pub fn (win &SimpleWindow) validate_required(names []string) (bool, string) {
	for name in names {
		val := win.get_text(name).trim_space()
		if val.len == 0 {
			win.flash_control(name)
			win.set_focus(name)
			win.set_status('Required field missing: ${name}')
			return false, name
		}
	}
	return true, ''
}

// trim_all trims leading and trailing whitespace from text controls in a single call.
pub fn (win &SimpleWindow) trim_all(names []string) &SimpleWindow {
	for name in names {
		win.set_text(name, win.get_text(name).trim_space())
	}
	return win
}

// uppercase_all converts text values in multiple named controls to uppercase.
pub fn (win &SimpleWindow) uppercase_all(names []string) &SimpleWindow {
	for name in names {
		win.set_text(name, win.get_text(name).to_upper())
	}
	return win
}

// lowercase_all converts text values in multiple named controls to lowercase.
pub fn (win &SimpleWindow) lowercase_all(names []string) &SimpleWindow {
	for name in names {
		win.set_text(name, win.get_text(name).to_lower())
	}
	return win
}

// clear_form resets all registered form input controls across the window.
pub fn (win &SimpleWindow) clear_form() &SimpleWindow {
	for ctrl in win.controls {
		match ctrl.kind {
			'input', 'textarea', 'password' {
				win.set_value(ctrl.name, '')
			}
			'checkbox', 'toggle', 'switch' {
				win.set_bool(ctrl.name, false)
			}
			'slider', 'progress', 'number' {
				win.set_number_value(ctrl.name, 0)
			}
			else {}
		}
	}
	return win
}

// reset_to_defaults is an alias for clear_form.
pub fn (win &SimpleWindow) reset_to_defaults() &SimpleWindow {
	return win.clear_form()
}

// set_status_temporary displays a temporary status message and restores the previous message after duration_ms.
pub fn (win &SimpleWindow) set_status_temporary(message string, duration_ms int) &SimpleWindow {
	prev_status := win.status_text
	win.set_status(message)
	win.run_after(duration_ms, fn [prev_status] (mut w SimpleWindow) {
		w.set_status(prev_status)
	})
	return win
}

// toast_info displays an informational toast notification.
pub fn (win &SimpleWindow) toast_info(message string) &SimpleWindow {
	return win.toast('ℹ️  ${message}')
}

// toast_success displays a success toast notification.
pub fn (win &SimpleWindow) toast_success(message string) &SimpleWindow {
	return win.toast('✅  ${message}')
}

// toast_warn displays a warning toast notification.
pub fn (win &SimpleWindow) toast_warn(message string) &SimpleWindow {
	return win.toast('⚠️  ${message}')
}

// toast_error displays an error toast notification.
pub fn (win &SimpleWindow) toast_error(message string) &SimpleWindow {
	return win.toast('❌  ${message}')
}

// play_sound plays a macOS system audio sound by name (e.g. "Glass", "Ping", "Hero", "Pop", "Tink", "Submarine").
pub fn (win &SimpleWindow) play_sound(sound_name string) &SimpleWindow {
	play_system_sound(sound_name)
	return win
}

// speak speaks text out loud using macOS text-to-speech engine. Alias for say.
pub fn (win &SimpleWindow) speak(text string) &SimpleWindow {
	return win.say(text)
}

// save_layout saves window dimensions (w, h) and screen position (x, y) to the recommended user application storage location.
pub fn (win &SimpleWindow) save_layout(app_name string) &SimpleWindow {
	resolved_app := if app_name.trim_space() != '' {
		app_name.trim_space()
	} else {
		win.get_app_name()
	}
	path := win.get_app_storage_path('layout.json', resolved_app)
	x, y, w, h := win.get_bounds()
	m := {
		'x':      x.str()
		'y':      y.str()
		'width':  w.str()
		'height': h.str()
	}
	encoded := json2.encode(m)
	os.write_file(path, encoded) or {}
	return win
}

// restore_layout restores window position and size from saved layout in the recommended user application storage location.
pub fn (win &SimpleWindow) restore_layout(app_name string) &SimpleWindow {
	resolved_app := if app_name.trim_space() != '' {
		app_name.trim_space()
	} else {
		win.get_app_name()
	}
	mut path := win.get_app_storage_path('layout.json', resolved_app)
	if !os.exists(path) {
		legacy_path := os.join_path(os.home_dir(), '.${resolved_app}_layout.json')
		if os.exists(legacy_path) {
			path = legacy_path
		} else {
			return win
		}
	}
	content := os.read_file(path) or { return win }
	m := json2.decode[map[string]string](content) or { return win }
	w := m['width'] or { '' }.int()
	h := m['height'] or { '' }.int()
	x := m['x'] or { '' }.int()
	y := m['y'] or { '' }.int()
	if w > 0 && h > 0 {
		win.set_size(w, h)
	}
	if x > 0 || y > 0 {
		win.set_position(x, y)
	}
	return win
}

// auto_save_layout automatically restores saved window layout on startup and saves layout on window close.
pub fn (win &SimpleWindow) auto_save_layout(app_name string) &SimpleWindow {
	win.restore_layout(app_name)
	win.on_close(fn [app_name] (mut w SimpleWindow) {
		w.save_layout(app_name)
	})
	return win
}

// donut adds an auto-named circular donut / radial gauge chart.
pub fn (win &SimpleWindow) donut(title string, percentage f64) &SimpleWindow {
	return win.add_donut_chart('', title, percentage)
}

// code_box adds an auto-named code studio container.
pub fn (win &SimpleWindow) code_box(filename string, language string, code string) &SimpleWindow {
	return win.add_code_studio('', filename, language, code)
}

// score_card adds an auto-named rating score card.
pub fn (win &SimpleWindow) score_card(title string, score f64, reviews int, breakdown []f64) &SimpleWindow {
	return win.add_score_card('', title, score, reviews, breakdown)
}

// floating_toolbar adds an auto-named capsule floating toolbar.
pub fn (win &SimpleWindow) floating_toolbar(title string, actions []string) &SimpleWindow {
	return win.add_floating_toolbar('', title, actions)
}

// user_profile adds an auto-named user profile card.
pub fn (win &SimpleWindow) user_profile(avatar_path string, name_text string, handle string, role string, bio string) &SimpleWindow {
	return win.add_user_profile_card('', avatar_path, name_text, handle, role, bio, true, '[Connect]')
}

// product_card adds an auto-named product showcase card.
pub fn (win &SimpleWindow) product_card(image_path string, title string, price string) &SimpleWindow {
	return win.add_product_card('', image_path, title, '', price, 'POPULAR', '[Buy Now]')
}

// gallery adds an auto-named interactive image gallery.
pub fn (win &SimpleWindow) gallery(images []string) &SimpleWindow {
	mut captions := []string{len: images.len, init: ''}
	return win.add_image_gallery('', images, captions, 0)
}

// app_tile adds an auto-named 3D app launcher tile.
pub fn (win &SimpleWindow) app_tile(icon_path string, title string, status string) &SimpleWindow {
	return win.add_app_launcher_tile('', icon_path, title, '', status)
}

// media_player adds an auto-named audio/media player card.
pub fn (win &SimpleWindow) media_player(cover_path string, title string, artist string) &SimpleWindow {
	return win.add_media_player('', cover_path, title, artist, 240, 0, false)
}

// heatmap adds an auto-named GitHub-style contribution heatmap.
pub fn (win &SimpleWindow) heatmap(title string, weeks int, matrix [][]int) &SimpleWindow {
	return win.add_activity_heatmap('', title, weeks, matrix)
}

// masked_input adds an auto-named masked input field.
pub fn (win &SimpleWindow) masked_input(mask string, value string) &SimpleWindow {
	return win.add_masked_input('', mask, value)
}

// editable_label adds an auto-named inline editable label.
pub fn (win &SimpleWindow) editable_label(text string) &SimpleWindow {
	return win.add_inline_editable_label('', text)
}

// nav_rail adds an auto-named navigation rail.
pub fn (win &SimpleWindow) nav_rail(items []SidebarItem) &SimpleWindow {
	return win.add_nav_rail('', items)
}
