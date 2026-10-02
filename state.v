module simplegui

import json2
import os
import time

pub type StringEventCallback = fn (mut win SimpleWindow, value string)

pub type VoidEventCallback = fn (mut win SimpleWindow)

pub type FileDropCallback = fn (mut win SimpleWindow, files []string)

pub struct MenuItem {
pub:
	title    string
	shortcut string
	callback VoidEventCallback = unsafe { nil }
}

pub struct WindowConfig {
pub mut:
	title                        string
	width                        int
	height                       int
	padding                      int
	spacing                      int
	background_color             string
	font_color                   string
	always_on_top                bool
	responsive_layout            bool
	resizable                    bool
	minimizable                  bool
	maximizable                  bool
	closable                     bool
	has_shadow                   bool
	movable_by_window_background bool
	titlebar_visible             bool
	title_visible                bool
}

pub struct WindowParams {
	title                        &char
	width                        int
	height                       int
	win_ptr                      voidptr
	padding                      int
	spacing                      int
	always_on_top                int
	responsive_layout            int
	resizable                    int
	minimizable                  int
	maximizable                  int
	closable                     int
	has_shadow                   int
	movable_by_window_background int
	titlebar_visible             int
	title_visible                int
}

pub struct WindowInfo {
	app          voidptr
	app_delegate voidptr
}

@[heap]
pub struct SimpleWindow {
mut:
	window_info                  &WindowInfo = unsafe { nil }
	width                        int
	height                       int
	title                        string
	controls                     []ControlEntry
	status_text                  string
	handlers                     []ControlEventHandler
	any_event_handlers           []AnyEventCallback
	background_color             string
	font_color                   string
	padding                      int
	spacing                      int
	always_on_top                bool
	responsive_layout            bool = true
	placeholders                 map[string]string
	tooltips                     map[string]string
	errors                       map[string]string
	default_button               string
	debug_mode                   bool
	last_control                 string
	min_width                    int
	min_height                   int
	max_width                    int
	max_height                   int
	resizable                    bool = true
	minimizable                  bool = true
	maximizable                  bool = true
	closable                     bool = true
	has_shadow                   bool = true
	movable_by_window_background bool
	titlebar_visible             bool = true
	title_visible                bool = true
	subtitle                     string
	corner_radius                f64
	vibrancy_material            string
	window_level                 string = 'normal'
	movable                      bool   = true
	ignores_mouse_events         bool
	hides_on_deactivate          bool
	prevents_app_termination     bool = true
	represented_filename         string
	frame_autosave_name          string
	document_edited              bool
	titlebar_appears_transparent bool
	full_size_content_view       bool
	background_blur              bool
	sharing_type                 string = 'read_write'
	list_items                   map[string][]string
	tree_nodes                   map[string][]TreeNode
	table_rows                   map[string][][]string
	table_columns                map[string][]string
	table_selected_columns       map[string]int
	table_column_selection       map[string]bool
	grid_rows                    map[string][][]string
	grid_headers                 map[string][]string
	grid_selected_rows           map[string]int
	grid_selected_columns        map[string]int
	on_close_requested_fn        CloseRequestedCallback = unsafe { nil }
pub mut:
	ws_client       voidptr = unsafe { nil }
	state_store     map[string]string
	state_listeners map[string][]StringEventCallback
	app_id          string
	auto_save_state bool = true
	fullscreen      bool
	theme           Theme
}

pub type Control = ControlEntry

@[heap]
pub struct ControlEntry {
pub mut:
	name             string
	kind             string
	label            string
	value            string
	checked          bool
	number           int
	background_color string
	font_color       string
	width            int
	height           int
	font_size        int
	visible          bool
	enabled          bool
	initial_value    string
	initial_checked  bool
	initial_number   int
	placeholder      string
	error_text       string
	alignment        string
	expand_fill      bool
}

// set_width sets the width of this control.
pub fn (c &ControlEntry) set_width(w int) &ControlEntry {
	unsafe {
		mut mc := &ControlEntry(c)
		mc.width = w
	}
	return c
}

// set_height sets the height of this control.
pub fn (c &ControlEntry) set_height(h int) &ControlEntry {
	unsafe {
		mut mc := &ControlEntry(c)
		mc.height = h
	}
	return c
}

// set_size sets width and height of this control.
pub fn (c &ControlEntry) set_size(w int, h int) &ControlEntry {
	unsafe {
		mut mc := &ControlEntry(c)
		mc.width = w
		mc.height = h
	}
	return c
}

// set_elevation sets the soft drop shadow elevation hint level (0 to 4).
pub fn (c &ControlEntry) set_elevation(elevation int) &ControlEntry {
	return c
}

// set_vector_icon sets a procedural vector icon glyph name (e.g. 'search', 'folder', 'check', 'trash').
pub fn (c &ControlEntry) set_vector_icon(icon string) &ControlEntry {
	return c
}

// set_visible sets control visibility.
pub fn (c &ControlEntry) set_visible(vis bool) &ControlEntry {
	unsafe {
		mut mc := &ControlEntry(c)
		mc.visible = vis
	}
	return c
}

// set_enabled sets control interactivity.
pub fn (c &ControlEntry) set_enabled(en bool) &ControlEntry {
	unsafe {
		mut mc := &ControlEntry(c)
		mc.enabled = en
	}
	return c
}

// set_tooltip sets control tooltip text.
pub fn (c &ControlEntry) set_tooltip(tip string) &ControlEntry {
	return c
}

// set_font_size sets control font size in points.
pub fn (c &ControlEntry) set_font_size(size int) &ControlEntry {
	unsafe {
		mut mc := &ControlEntry(c)
		mc.font_size = size
	}
	return c
}

// set_font_color sets control text font color.
pub fn (c &ControlEntry) set_font_color(color string) &ControlEntry {
	unsafe {
		mut mc := &ControlEntry(c)
		mc.font_color = color
	}
	return c
}

// set_bg_color sets control background color.
pub fn (c &ControlEntry) set_bg_color(color string) &ControlEntry {
	unsafe {
		mut mc := &ControlEntry(c)
		mc.background_color = color
	}
	return c
}

pub type CloseRequestedCallback = fn (mut win SimpleWindow) bool

pub type AnyEventCallback = fn (mut win SimpleWindow, control_name string, event_name string, value string)

struct ControlEventHandler {
mut:
	control_name string
	event_name   string
	filter_value string
	string_cb    StringEventCallback = unsafe { nil }
	void_cb      VoidEventCallback   = unsafe { nil }
	file_drop_cb FileDropCallback    = unsafe { nil }
}

// new_simple_window creates and initializes a new SimpleWindow instance.
pub fn (win &SimpleWindow) add_chart(name string, chart_type string, height int) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('chart')
	}
	if win.debug_mode {
		println('[simplegui DEBUG] Created Control: "${real_name}" (Type: "chart", Style: "${chart_type}", Height: ${height})')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.upsert_control(real_name, 'chart', '', '', false, 0)
	}
	if win.window_info != unsafe { nil } {
		C.window_add_chart_control(win.window_info, real_name.str, chart_type.str, height)
	}
	return win
}

// set_chart_data updates the values drawn in the chart control.
pub fn (win &SimpleWindow) grid_set_column_type(name string, col_idx int, col_type string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_grid_set_column_type(win.window_info, name.str, col_idx, col_type.str)
	}
	return win
}

// grid_set_column_width resizes a specific column to a fixed width.
pub fn (win &SimpleWindow) set_level_type(level_type string) &SimpleWindow {
	return win.set_window_level(level_type)
}

// get_window_level retrieves the current window level.
pub struct ControlInfo {
pub mut:
	name             string
	kind             string
	label            string
	value            string
	checked          bool
	number           int
	enabled          bool
	visible          bool
	width            int
	height           int
	placeholder      string
	error_text       string
	tooltip          string
	background_color string
	font_color       string
	font_size        int
}

// spy_control inspects and returns detailed information about a single control by name.
pub struct TreeNode {
pub mut:
	id        string
	parent_id string
	text      string
}

// tree_node creates a TreeNode with explicit id, parent id, and display text.
struct MainThreadCallback {
mut:
	win &SimpleWindow     = unsafe { nil }
	cb  VoidEventCallback = unsafe { nil }
}

// Theme represents a complete color scheme for SimpleWindow interface styling.
pub struct Theme {
pub:
	name             string
	background_color string
	font_color       string
	accent_color     string
	description      string
	is_dark          bool
}

// list_themes returns all built-in production theme preset names.
pub fn (win &SimpleWindow) append_terminal_line(name string, line_text string, line_type int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_append_terminal_line(win.window_info, name.str, line_text.str, line_type)
	}
	return win
}

// clear_terminal clears output in terminal view widget.
pub fn (win &SimpleWindow) add_status_banner(name string, title string, message string, style_type string) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('status_banner')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.controls << ControlEntry{
			name:  real_name
			kind:  'status_banner'
			value: title
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_add_status_banner_control(win.window_info, real_name.str, title.str,
			message.str, style_type.str)
	}
	return win
}

// status_banner adds an auto-named status banner alert strip.
pub fn (win &SimpleWindow) status_banner(title string, message string, style_type string) &SimpleWindow {
	return win.add_status_banner('', title, message, style_type)
}

// set_status_banner updates title and message in status banner alert strip.
pub fn (win &SimpleWindow) set_status_banner(name string, title string, message string, style_type string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_status_banner_text(win.window_info, name.str, title.str, message.str,
			style_type.str)
	}
	return win
}

// add_pill_toggle adds a rounded pill segment option toggle bar.
pub fn (win &SimpleWindow) add_info_callout(name string, title string, message string, style_type string, button_text string) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('info_callout')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.controls << ControlEntry{
			name:  real_name
			kind:  'info_callout'
			value: title
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_add_info_callout_control(win.window_info, real_name.str, title.str, message.str,
			style_type.str, button_text.str)
	}
	return win
}

// info_callout adds an auto-named info callout card widget.
pub fn (win &SimpleWindow) info_callout(title string, message string, style_type string, button_text string) &SimpleWindow {
	return win.add_info_callout('', title, message, style_type, button_text)
}

// =============================================================================
// Reactive & Key-Value State Store API
// =============================================================================
// The State Store acts as a centralized reactive database for the window application.
// Any control or handler can update a key using `win.set_state("key", "val")` and
// registered listeners created via `win.on_state_change("key", callback)` automatically fire!

// set_state updates a key-value pair in the window's state store.
// If reactive listeners are registered for `key`, they are automatically invoked with the new value.
pub fn (win &SimpleWindow) set_state(key string, val string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.state_store[key] = val

		// Trigger registered state listeners if any exist for this key
		if key in w.state_listeners {
			for cb in w.state_listeners[key] {
				cb(mut w, val)
			}
		}
	}
	return win
}

// get_state retrieves the string value associated with `key` from the state store.
// Optional `default_val`: Allows specifying a custom fallback string if `key` is missing or empty (defaults to `""`).
// Example: `theme := win.get_state('theme', 'Apple Dark')`
pub fn (win &SimpleWindow) get_state(key string, default_val ...string) string {
	fallback := if default_val.len > 0 { default_val[0] } else { '' }
	return win.get_state_or(key, fallback)
}

// get_state_or retrieves the string value for `key`.
// Default Return Value: Returns custom `fallback` string if `key` is unset or contains an empty string `""`.
pub fn (win &SimpleWindow) get_state_or(key string, fallback string) string {
	val := win.state_store[key] or { '' }
	if val == '' {
		return fallback
	}
	return val
}

// has_state returns `true` if `key` exists in the state store dictionary.
pub fn (win &SimpleWindow) has_state(key string) bool {
	return key in win.state_store
}

// remove_state deletes a key-value entry from the state store.
pub fn (win &SimpleWindow) remove_state(key string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.state_store.delete(key)
	}
	return win
}

// clear_state removes all key-value entries from the state store.
pub fn (win &SimpleWindow) clear_state() &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.state_store.clear()
	}
	return win
}

// set_state_int converts an integer `val` to a string and stores it under `key`.
pub fn (win &SimpleWindow) set_state_int(key string, val int) &SimpleWindow {
	return win.set_state(key, val.str())
}

// get_state_int retrieves the value of `key` parsed as an integer.
// Optional `default_val`: Allows specifying a custom fallback integer returned if `key` is missing or unparseable (defaults to `0`).
// Example: `counter := win.get_state_int('click_count', 10)`
pub fn (win &SimpleWindow) get_state_int(key string, default_val ...int) int {
	fallback := if default_val.len > 0 { default_val[0] } else { 0 }
	return win.get_state_int_or(key, fallback)
}

// get_state_int_or retrieves the value of `key` parsed as an integer, returning `fallback` if missing or unparseable.
pub fn (win &SimpleWindow) get_state_int_or(key string, fallback int) int {
	if key in win.state_store {
		str_val := win.state_store[key].trim_space()
		if str_val.len > 0 {
			parsed := str_val.int()
			if parsed != 0 || str_val == '0' {
				return parsed
			}
		}
	}
	return fallback
}

// set_state_bool converts a boolean `val` to a string ('true'/'false') and stores it under `key`.
pub fn (win &SimpleWindow) set_state_bool(key string, val bool) &SimpleWindow {
	return win.set_state(key, val.str())
}

// get_state_bool retrieves the value of `key` as a boolean.
// Optional `default_val`: Allows specifying a custom fallback boolean returned if `key` is missing or unset (defaults to `false`).
// Example: `enabled := win.get_state_bool('feature_flag', true)`
pub fn (win &SimpleWindow) get_state_bool(key string, default_val ...bool) bool {
	fallback := if default_val.len > 0 { default_val[0] } else { false }
	return win.get_state_bool_or(key, fallback)
}

// get_state_bool_or retrieves the boolean state value of `key`, returning `fallback` if missing.
pub fn (win &SimpleWindow) get_state_bool_or(key string, fallback bool) bool {
	if key in win.state_store {
		val := win.state_store[key].to_lower().trim_space()
		if val == 'true' || val == '1' {
			return true
		} else if val == 'false' || val == '0' {
			return false
		}
	}
	return fallback
}

// set_state_f64 converts a floating point `val` to string and stores it under `key`.
pub fn (win &SimpleWindow) set_state_f64(key string, val f64) &SimpleWindow {
	return win.set_state(key, val.str())
}

// get_state_f64 retrieves the value of `key` parsed as a 64-bit float.
// Optional `default_val`: Allows specifying a custom fallback float returned if `key` is missing or unparseable (defaults to `0.0`).
// Example: `ratio := win.get_state_f64('zoom_ratio', 1.0)`
pub fn (win &SimpleWindow) get_state_f64(key string, default_val ...f64) f64 {
	fallback := if default_val.len > 0 { default_val[0] } else { 0.0 }
	return win.get_state_f64_or(key, fallback)
}

// get_state_f64_or retrieves the float value of `key`, returning `fallback` if missing or unparseable.
pub fn (win &SimpleWindow) get_state_f64_or(key string, fallback f64) f64 {
	if key in win.state_store {
		str_val := win.state_store[key].trim_space()
		if str_val.len > 0 {
			parsed := str_val.f64()
			if parsed != 0.0 || str_val in ['0', '0.0', '0.'] {
				return parsed
			}
		}
	}
	return fallback
}

// toggle_state_bool flips the boolean state of `key` (true -> false, false -> true) and returns the new boolean value.
pub fn (win &SimpleWindow) toggle_state_bool(key string) bool {
	curr := win.get_state_bool(key)
	next := !curr
	win.set_state_bool(key, next)
	return next
}

// increment_state_int adds `delta` to the current integer value of `key` and returns the new total.
pub fn (win &SimpleWindow) increment_state_int(key string, delta int) int {
	curr := win.get_state_int(key)
	next := curr + delta
	win.set_state_int(key, next)
	return next
}

// on_state_change registers a reactive listener callback `cb` that executes whenever `key` changes in the state store.
// If `key` already has a value in the state store, the callback is executed immediately with the current value.
pub fn (win &SimpleWindow) on_state_change(key string, cb StringEventCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.state_listeners[key] << cb
		if key in w.state_store {
			cb(mut w, w.state_store[key])
		}
	}
	return win
}

// =============================================================================
// Atomic File Operations & Disk Persistence
// =============================================================================

// write_file_atomic writes data safely to a temporary file before atomically renaming it,
// ensuring that crashes, power cuts, or concurrent readers never observe corrupted partial files.
pub fn write_file_atomic(file_path string, content string) ! {
	resolved := resolve_user_path(file_path)
	parent_dir := os.dir(resolved)
	if parent_dir != '' && !os.exists(parent_dir) {
		os.mkdir_all(parent_dir) or { return error('Failed to create parent directory: ${parent_dir} (${err.msg()})') }
	}

	rand_id := '${os.getpid()}_${time.now().unix_nano()}'
	tmp_path := '${resolved}.${rand_id}.tmp'

	os.write_file(tmp_path, content) or {
		return error('Failed to write temporary state file: ${tmp_path} (${err.msg()})')
	}

	$if windows {
		if os.exists(resolved) {
			os.rm(resolved) or {}
		}
	}
	os.mv(tmp_path, resolved) or {
		os.rm(tmp_path) or {}
		return error('Failed to atomically rename state file to: ${resolved} (${err.msg()})')
	}
}

// save_state_to_file serializes a key-value state store dictionary to JSON at target path atomically.
pub fn save_state_to_file(file_path string, store map[string]string) ! {
	resolved := resolve_user_path(file_path)
	data := json2.encode[map[string]string](store)
	write_file_atomic(resolved, data)!
}

// load_state_from_file reads and deserializes a JSON state map from disk.
pub fn load_state_from_file(file_path string) !map[string]string {
	resolved := resolve_user_path(file_path)
	if !os.exists(resolved) {
		return error('State file not found: ${resolved}')
	}
	content := os.read_file(resolved)!
	if content.trim_space() == '' {
		return map[string]string{}
	}
	loaded := json2.decode[map[string]string](content)!
	return loaded
}

// save_state_json serializes the entire state store dictionary to a JSON file at `file_path`.
// Automatically expands user home/env paths and writes atomically to prevent file corruption.
pub fn (win &SimpleWindow) save_state_json(file_path string) ! {
	save_state_to_file(file_path, win.state_store)!
}

// load_state_json reads a JSON file from `file_path`, updates the state store, and triggers reactive listeners.
// Automatically expands user home/env paths.
pub fn (win &SimpleWindow) load_state_json(file_path string) ! {
	loaded := load_state_from_file(file_path)!
	for key, val in loaded {
		win.set_state(key, val)
	}
}

// save_app_state persists the window's state store and control values into the recommended OS user state directory.
pub fn (win &SimpleWindow) save_app_state(args ...string) ! {
	mut app_name := win.get_app_id()
	mut file_name := 'state.json'
	if args.len >= 2 {
		app_name = args[0]
		file_name = args[1]
	} else if args.len == 1 {
		mut s := args[0].trim_space()
		if s.ends_with('.json') {
			file_name = s
		} else {
			app_name = s
		}
	}

	mut data := map[string]string{}
	// Persist reactive state store entries
	for k, v in win.state_store {
		data[k] = v
	}
	// Persist control values
	for ctrl in win.controls {
		data[ctrl.name] = win.get_text(ctrl.name)
	}

	target_file := get_app_state_file(app_name, file_name)
	save_state_to_file(target_file, data)!

	// Also sync to app_storage_path for backward compatibility with presets
	if args.len == 1 {
		mut preset := args[0].trim_space()
		if !preset.ends_with('.json') {
			preset += '.json'
		}
		storage_path := win.get_app_storage_path(preset)
		if storage_path != target_file {
			save_state_to_file(storage_path, data) or {}
		}
	}
}

// save_app_state_or persists the state store into the recommended OS user directory, returning a boolean success flag.
pub fn (win &SimpleWindow) save_app_state_or(args ...string) bool {
	win.save_app_state(...args) or { return false }
	return true
}

// load_app_state reads persisted JSON state from disk, updates the store,
// and restores matching control values. Returns true if loaded successfully.
pub fn (win &SimpleWindow) load_app_state(args ...string) bool {
	mut app_name := win.get_app_id()
	mut file_name := 'state.json'
	if args.len >= 2 {
		app_name = args[0]
		file_name = args[1]
	} else if args.len == 1 {
		mut s := args[0].trim_space()
		if s.ends_with('.json') {
			file_name = s
		} else {
			app_name = s
		}
	}

	mut target_file := get_app_state_file(app_name, file_name)
	if !os.exists(target_file) {
		fallback := get_app_config_file(app_name, file_name)
		if os.exists(fallback) {
			target_file = fallback
		} else if args.len == 1 {
			mut preset := args[0].trim_space()
			if !preset.ends_with('.json') {
				preset += '.json'
			}
			storage_path := win.get_app_storage_path(preset)
			if os.exists(storage_path) {
				target_file = storage_path
			} else {
				return false
			}
		} else {
			return false
		}
	}

	loaded := load_state_from_file(target_file) or { return false }
	for key, val in loaded {
		win.set_state(key, val)
		if win.has_control(key) {
			kind := win.get_control_kind(key)
			if kind in ['checkbox', 'switch', 'spinner'] {
				win.set_checked(key, val == 'true' || val == '1')
			} else if kind in ['number', 'slider', 'progress', 'levelindicator', 'stepper', 'knob'] {
				win.set_value_int(key, val.int())
			} else {
				win.set_text(key, val)
			}
		}
	}
	return true
}

// load_app_state_or loads state from recommended OS user state directory, returning whether it succeeded.
pub fn (win &SimpleWindow) load_app_state_or(args ...string) bool {
	return win.load_app_state(...args)
}

// has_saved_app_state checks whether a persisted state file exists.
pub fn (win &SimpleWindow) has_saved_app_state(args ...string) bool {
	mut app_name := win.get_app_id()
	mut file_name := 'state.json'
	if args.len >= 2 {
		app_name = args[0]
		file_name = args[1]
	} else if args.len == 1 {
		mut s := args[0].trim_space()
		if s.ends_with('.json') {
			file_name = s
		} else {
			app_name = s
		}
	}

	target_file := get_app_state_file(app_name, file_name)
	if os.exists(target_file) {
		return true
	}
	fallback := get_app_config_file(app_name, file_name)
	if os.exists(fallback) {
		return true
	}
	if args.len == 1 {
		mut preset := args[0].trim_space()
		if !preset.ends_with('.json') {
			preset += '.json'
		}
		storage_path := win.get_app_storage_path(preset)
		if os.exists(storage_path) {
			return true
		}
	}
	return false
}

// has_saved_state is an ergonomic alias for has_saved_app_state.
pub fn (win &SimpleWindow) has_saved_state(args ...string) bool {
	return win.has_saved_app_state(...args)
}

// clear_app_state deletes the persisted state file.
pub fn (win &SimpleWindow) clear_app_state(args ...string) bool {
	mut app_name := win.get_app_id()
	mut file_name := 'state.json'
	if args.len >= 2 {
		app_name = args[0]
		file_name = args[1]
	} else if args.len == 1 {
		mut s := args[0].trim_space()
		if s.ends_with('.json') {
			file_name = s
		} else {
			app_name = s
		}
	}

	mut deleted := false
	target_file := get_app_state_file(app_name, file_name)
	if os.exists(target_file) {
		os.rm(target_file) or {}
		deleted = true
	}
	fallback := get_app_config_file(app_name, file_name)
	if os.exists(fallback) {
		os.rm(fallback) or {}
		deleted = true
	}
	if args.len == 1 {
		mut preset := args[0].trim_space()
		if !preset.ends_with('.json') {
			preset += '.json'
		}
		storage_path := win.get_app_storage_path(preset)
		if os.exists(storage_path) {
			os.rm(storage_path) or {}
			deleted = true
		}
	}
	return deleted
}

// save_state is a convenience method that saves current state and form values.
pub fn (win &SimpleWindow) save_state(args ...string) ! {
	win.save_app_state(...args)!
}

// load_state is a convenience method that restores state and form values.
pub fn (win &SimpleWindow) load_state(args ...string) bool {
	return win.load_app_state(...args)
}

// save_window_session persists current window dimensions, active theme, and state keys to session.json.
pub fn (win &SimpleWindow) save_window_session(app_name string) ! {
	mut session_data := map[string]string{}
	for k, v in win.state_store {
		session_data[k] = v
	}
	session_data['__win_width'] = win.width.str()
	session_data['__win_height'] = win.height.str()
	session_data['__win_theme'] = win.theme.name
	session_data['__win_fullscreen'] = win.is_fullscreen().str()

	target_file := get_app_state_file(app_name, 'session.json')
	save_state_to_file(target_file, session_data)!
}

// restore_window_session loads session.json and restores state, theme, and window dimensions.
pub fn (win &SimpleWindow) restore_window_session(app_name string) bool {
	target_file := get_app_state_file(app_name, 'session.json')
	if !os.exists(target_file) {
		return false
	}
	loaded := load_state_from_file(target_file) or { return false }
	for k, v in loaded {
		if k == '__win_theme' {
			win.set_theme(v)
		} else if k == '__win_fullscreen' {
			if v == 'true' {
				win.set_fullscreen(true)
			}
		} else if k == '__win_width' {
			w := v.int()
			if w > 100 {
				unsafe {
					mut sw := &SimpleWindow(win)
					sw.width = w
				}
				win.set_width(w)
			}
		} else if k == '__win_height' {
			h := v.int()
			if h > 100 {
				unsafe {
					mut sw := &SimpleWindow(win)
					sw.height = h
				}
				win.set_height(h)
			}
		} else {
			win.set_state(k, v)
		}
	}
	return true
}

// enable_auto_save_state configures the window to automatically persist its state on window close.
pub fn (win &SimpleWindow) enable_auto_save_state(app_name string, file_name ...string) &SimpleWindow {
	fname := if file_name.len > 0 && file_name[0] != '' { file_name[0] } else { 'state.json' }
	win.on_close(fn [app_name, fname] (mut w SimpleWindow) {
		w.save_app_state_or(app_name, fname)
	})
	return win
}

// =============================================================================
// Control & State Binding API (Two-Way Data Binding)
// =============================================================================

// bind_state establishes automatic two-way data binding between control `control_name` and state store `key`.
// Updates to state key automatically update the UI control value, and UI control edits automatically sync back to state key.
pub fn (win &SimpleWindow) bind_state(control_name string, key string) &SimpleWindow {
	if key in win.state_store {
		win.set_text(control_name, win.state_store[key])
	} else {
		init_val := win.get_text(control_name)
		unsafe {
			mut w := &SimpleWindow(win)
			w.state_store[key] = init_val
		}
	}

	win.on_state_change(key, fn [control_name] (mut win SimpleWindow, val string) {
		if win.get_text(control_name) != val {
			win.set_text(control_name, val)
		}
	})

	win.on_change(control_name, fn [key] (mut win SimpleWindow, val string) {
		if win.get_state(key) != val {
			win.set_state(key, val)
		}
	})

	return win
}

// bind_control is an ergonomic alias for `bind_state`.
pub fn (win &SimpleWindow) bind_control(control_name string, key string) &SimpleWindow {
	return win.bind_state(control_name, key)
}

// bind_value is an ergonomic alias for `bind_state`.
pub fn (win &SimpleWindow) bind_value(control_name string, key string) &SimpleWindow {
	return win.bind_state(control_name, key)
}

// =============================================================================
// Form & Application State Auto-Persistence API
// =============================================================================

// should_persist_control determines whether a control's value should be automatically saved across sessions.
pub fn should_persist_control(ctrl &ControlEntry) bool {
	// Ignore controls without names or internal system controls
	if ctrl.name.len == 0 || ctrl.name.starts_with('__') {
		return false
	}

	// Supported input/preference control kinds
	if ctrl.kind !in [
		'input',
		'textbox',
		'search',
		'search_bar',
		'file_picker',
		'date_picker',
		'time_picker',
		'number',
		'dropdown',
		'select',
		'combobox',
		'segmented',
		'radio',
		'checkbox',
		'switch',
		'toggle',
		'slider',
		'step_slider',
		'range_slider',
		'stepper',
		'rating',
		'textarea',
	] {
		return false
	}

	lower := ctrl.name.to_lower()

	// Exclude output consoles, live logs, diffs, previews, and status messages
	if lower.contains('output') || lower.contains('stdout') || lower.contains('stderr')
		|| lower.contains('terminal') || lower.contains('console') || lower.contains('live_output')
		|| lower.contains('preview') || lower.contains('diff') || lower.contains('logs')
		|| lower.contains('log_area') || lower.contains('results') || lower.contains('msg_box')
		|| lower.contains('status_bar') || lower.contains('status_lbl') || lower.contains('telemetry')
		|| lower.contains('summary_card') {
		return false
	}

	// Exclude sensitive credentials, passwords, and tokens
	if lower.contains('password') || lower.contains('secret') || lower.contains('auth_token')
		|| lower.contains('private_key') {
		return false
	}

	return true
}

// get_app_id returns the unique application identifier used for storing per-app state files.
pub fn (win &SimpleWindow) get_app_id() string {
	if win.app_id.len > 0 {
		return win.app_id
	}
	if win.title.len > 0 {
		clean := win.title.to_lower().replace(' ', '_').replace('-', '_')
		mut res := ''
		for ch in clean {
			if (ch >= `a` && ch <= `z`) || (ch >= `0` && ch <= `9`) || ch == `_` {
				res += ch.ascii_str()
			}
		}
		if res.len > 0 {
			return res
		}
	}
	if os.args.len > 0 {
		base := os.file_name(os.args[0]).all_before_last('.')
		if base.len > 0 {
			return base
		}
	}
	return 'default_app'
}

// set_app_id explicitly sets a custom application identifier for state storage.
pub fn (win &SimpleWindow) set_app_id(id string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.app_id = id
	}
	return win
}

// enable_auto_save enables automatic state persistence when the window closes.
pub fn (win &SimpleWindow) enable_auto_save() &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.auto_save_state = true
	}
	return win
}

// disable_auto_save disables automatic state persistence for this window session.
pub fn (win &SimpleWindow) disable_auto_save() &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.auto_save_state = false
	}
	return win
}

// save_app_form_state persists user-entered form inputs, toggles, dropdown selections,
// window dimensions, active theme, and reactive state keys to `form_state.json`.
pub fn (win &SimpleWindow) save_app_form_state(app_name ...string) ! {
	app_id := if app_name.len > 0 && app_name[0].len > 0 { app_name[0] } else { win.get_app_id() }
	mut data := map[string]string{}

	// Persist window session geometry and theme
	data['__win_width'] = win.width.str()
	data['__win_height'] = win.height.str()
	data['__win_theme'] = win.theme.name
	data['__win_fullscreen'] = win.is_fullscreen().str()

	// Persist reactive state store entries
	for k, v in win.state_store {
		data['__state_' + k] = v
	}

	// Persist all user-editable interactive form controls
	for ctrl in win.controls {
		if !should_persist_control(&ctrl) {
			continue
		}
		val := win.get_text(ctrl.name)
		data[ctrl.name] = val
	}

	target_file := get_app_state_file(app_id, 'form_state.json')
	save_state_to_file(target_file, data)!
}

// save_app_form_state_or persists the form state, returning a boolean success indicator.
pub fn (win &SimpleWindow) save_app_form_state_or(app_name ...string) bool {
	win.save_app_form_state(...app_name) or { return false }
	return true
}

// restore_app_form_state loads `form_state.json` and restores form inputs, selections,
// theme, and window dimensions across application launches.
pub fn (win &SimpleWindow) restore_app_form_state(app_name ...string) bool {
	app_id := if app_name.len > 0 && app_name[0].len > 0 { app_name[0] } else { win.get_app_id() }
	mut target_file := get_app_state_file(app_id, 'form_state.json')
	if !os.exists(target_file) {
		target_file = get_app_config_file(app_id, 'form_state.json')
		if !os.exists(target_file) {
			saved_theme := get_saved_theme()
			if saved_theme != '' && saved_theme != win.theme.name {
				win.set_theme(saved_theme)
			}
			return false
		}
	}

	loaded := load_state_from_file(target_file) or { return false }

	// Restore window geometry if reasonable
	if w_str := loaded['__win_width'] {
		w := w_str.int()
		if w >= 300 && w <= 4000 {
			unsafe {
				mut sw := &SimpleWindow(win)
				sw.width = w
			}
			win.set_width(w)
		}
	}
	if h_str := loaded['__win_height'] {
		h := h_str.int()
		if h >= 200 && h <= 3000 {
			unsafe {
				mut sw := &SimpleWindow(win)
				sw.height = h
			}
			win.set_height(h)
		}
	}
	if f_str := loaded['__win_fullscreen'] {
		if f_str == 'true' {
			win.set_fullscreen(true)
		}
	}

	// Restore theme (either app session theme or global saved theme)
	if theme_str := loaded['__win_theme'] {
		if theme_str.len > 0 {
			win.set_theme(theme_str)
		}
	} else {
		saved_theme := get_saved_theme()
		if saved_theme != '' {
			win.set_theme(saved_theme)
		}
	}

	// Restore state store keys
	for k, v in loaded {
		if k.starts_with('__state_') && k.len > 8 {
			win.set_state(k[8..], v)
		}
	}

	// Restore form controls
	for ctrl in win.controls {
		if val := loaded[ctrl.name] {
			if should_persist_control(&ctrl) {
				if ctrl.kind in ['checkbox', 'switch', 'toggle'] {
					b_val := (val.to_lower().trim_space() in ['true', '1', 'yes', 'on'])
					win.set_bool(ctrl.name, b_val)
				} else if ctrl.kind in ['slider', 'number', 'progress', 'stepper', 'rating', 'spinner'] {
					win.set_number_value(ctrl.name, val.int())
				} else {
					win.set_text(ctrl.name, val)
				}
			}
		}

		// Ensure theme dropdowns always display the active window theme
		if ctrl.name in ['dd_app_theme', 'dd_theme', 'dd_theme_selector'] {
			win.set_text(ctrl.name, win.theme.name)
		}
	}
	return true
}

// clear_app_form_state deletes the persisted form state file for an application.
pub fn (win &SimpleWindow) clear_app_form_state(app_name ...string) ! {
	app_id := if app_name.len > 0 && app_name[0].len > 0 { app_name[0] } else { win.get_app_id() }
	state_file := get_app_state_file(app_id, 'form_state.json')
	if os.exists(state_file) {
		os.rm(state_file) or { return error('Failed to delete form state: ${err.msg()}') }
	}
	config_file := get_app_config_file(app_id, 'form_state.json')
	if os.exists(config_file) {
		os.rm(config_file) or {}
	}
}

// =============================================================================
// Control Pointer Accessors
// =============================================================================

// get_control_ptr returns a mutable reference pointer to the named ControlEntry, or an error if missing.
pub fn (win &SimpleWindow) get_control_ptr(name string) !&ControlEntry {
	idx := win.find_control(name)
	if idx < 0 {
		return error('Control "${name}" not found')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		return &w.controls[idx]
	}
}

// control returns a mutable reference pointer to the named ControlEntry directly, panicking if missing.
pub fn (win &SimpleWindow) control(name string) &ControlEntry {
	ptr := win.get_control_ptr(name) or {
		panic(err.msg())
	}
	return ptr
}
