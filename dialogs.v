module simplegui

// toast displays a temporary floating toast notification banner on the window.
pub fn (win &SimpleWindow) toast(message string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_show_toast(win.window_info, message.str)
	}
	return win
}

// alert displays a native modal alert dialog with a title and message.
pub fn (win &SimpleWindow) alert(title string, message string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_show_alert(win.window_info, title.str, message.str)
	}
	return win
}

// alert_with_style displays a styled modal alert dialog (warning, info, critical).
pub fn (win &SimpleWindow) alert_with_style(title string, message string, style string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_show_alert_with_style(win.window_info, title.str, message.str, style.str)
	}
	return win
}

// confirm displays a modal confirmation dialog returning true if confirmed.
pub fn (win &SimpleWindow) confirm(title string, message string) bool {
	if win.window_info != unsafe { nil } {
		return C.window_show_confirm(win.window_info, title.str, message.str) == 1
	}
	return false
}

// prompt displays a text entry modal dialog returning user input string.
pub fn (win &SimpleWindow) prompt(title string, message string, default_val string) string {
	if win.window_info != unsafe { nil } {
		res := C.window_show_prompt(win.window_info, title.str, message.str, default_val.str)
		return unsafe { res.vstring() }
	}
	return ''
}

// choice_dialog displays a selection modal returning the index of the selected choice.
pub fn (win &SimpleWindow) choice_dialog(title string, message string, choices []string) int {
	if win.window_info != unsafe { nil } {
		mut c_choices := []&u8{}
		for choice in choices {
			c_choices << choice.str
		}
		return C.window_show_choice_dialog(win.window_info, title.str, message.str, c_choices.data,
			choices.len)
	}
	return -1
}

// show_color_sampler activates the native macOS screen magnifying glass tool and returns the clicked hex color.
pub fn (win &SimpleWindow) show_color_sampler() string {
	if win.window_info != unsafe { nil } {
		res := C.window_show_color_sampler(win.window_info)
		if res != unsafe { nil } {
			return unsafe { cstring_to_vstring(res) }
		}
	}
	return ''
}

// select_file opens a native file selection dialog and returns the chosen file path.
pub fn (win &SimpleWindow) select_file() string {
	if win.window_info != unsafe { nil } {
		res := C.window_select_file(win.window_info)
		return unsafe { res.vstring() }
	}
	return ''
}

// select_file_with_extensions opens a file dialog restricted to specific file extensions.
pub fn (win &SimpleWindow) select_file_with_extensions(extensions string) string {
	if win.window_info != unsafe { nil } {
		res := C.window_select_file_with_extensions(win.window_info, extensions.str)
		return unsafe { res.vstring() }
	}
	return ''
}

// select_folder opens a native folder selection dialog and returns the chosen directory path.
pub fn (win &SimpleWindow) select_folder() string {
	if win.window_info != unsafe { nil } {
		res := C.window_select_folder(win.window_info)
		return unsafe { res.vstring() }
	}
	return ''
}

// save_file_picker opens a native save file dialog and returns the chosen path.
pub fn (win &SimpleWindow) save_file_picker() string {
	if win.window_info != unsafe { nil } {
		res := C.window_save_file_picker(win.window_info)
		return unsafe { res.vstring() }
	}
	return ''
}

// on_toolbar_click registers a click event callback for a named toolbar button.
pub fn (win &SimpleWindow) on_toolbar_click(name string, callback VoidEventCallback) &SimpleWindow {
	return win.on_click(name, callback)
}

// show_sheet_alert displays an attached sheet modal alert on the window.
pub fn (win &SimpleWindow) show_sheet_alert(title string, message string, style string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_show_sheet_alert(win.window_info, title.str, message.str, style.str)
	}
	return win
}

// alert_banner displays an inline alert banner notification.
pub fn (win &SimpleWindow) alert_banner(title string, message string, style string) &SimpleWindow {
	return win.add_alert_banner('', title, message, style)
}

// show_color_picker opens the native macOS color panel and returns the selected HEX color string (e.g. "#FF5733").
pub fn (win &SimpleWindow) show_color_picker(initial_hex string) string {
	res := C.window_show_color_picker(win.window_info, initial_hex.str)
	return unsafe { tos3(res) }
}

// select_multiple_files opens a native file dialog allowing selection of multiple files.
pub fn (win &SimpleWindow) select_multiple_files(extensions string) []string {
	res := C.window_select_multiple_files(win.window_info, extensions.str)
	raw := unsafe { tos3(res) }
	if raw.len == 0 || raw == '[]' {
		return []string{}
	}
	mut list := []string{}
	cleaned := raw.trim('[]').trim_space()
	if cleaned.len == 0 {
		return []string{}
	}
	parts := cleaned.split(',')
	for p in parts {
		trimmed := p.trim_space().trim('"')
		if trimmed.len > 0 {
			list << trimmed
		}
	}
	return list
}

// save_file_picker_with_name opens a native save dialog with a pre-filled default filename and allowed extensions.
pub fn (win &SimpleWindow) save_file_picker_with_name(default_filename string, allowed_extensions string) string {
	res := C.window_save_file_picker_with_name(win.window_info, default_filename.str, allowed_extensions.str)
	return unsafe { tos3(res) }
}

// Package-level standalone dialog helpers:

// show_color_picker opens the modal color picker dialog without an active window instance.
pub fn show_color_picker(initial_hex string) string {
	res := C.window_show_color_picker(unsafe { nil }, initial_hex.str)
	return unsafe { tos3(res) }
}

// select_multiple_files opens a native file picker dialog allowing multi-selection.
pub fn select_multiple_files(extensions string) []string {
	res := C.window_select_multiple_files(unsafe { nil }, extensions.str)
	raw := unsafe { tos3(res) }
	if raw.len == 0 || raw == '[]' {
		return []string{}
	}
	mut list := []string{}
	cleaned := raw.trim('[]').trim_space()
	if cleaned.len == 0 {
		return []string{}
	}
	parts := cleaned.split(',')
	for p in parts {
		trimmed := p.trim_space().trim('"')
		if trimmed.len > 0 {
			list << trimmed
		}
	}
	return list
}

// save_file_picker_with_name opens a save file dialog with default name and extensions.
pub fn save_file_picker_with_name(default_filename string, allowed_extensions string) string {
	res := C.window_save_file_picker_with_name(unsafe { nil }, default_filename.str, allowed_extensions.str)
	return unsafe { tos3(res) }
}
