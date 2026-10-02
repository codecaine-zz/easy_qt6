module main

import simplegui
import os

struct PreviewerState {
pub mut:
	current_dir       string = 'demos'
	v_files           []string
	selected_file     string
	original_code     string
	current_file_path string
	theme_name        string = 'nord'
	is_dirty          bool
	show_line_numbers bool = true
}

fn main() {
	mut state := &PreviewerState{}

	// Locate initial directory (check 'demos' or current directory)
	if !os.exists(state.current_dir) && os.exists('vlang_simplegui/demos') {
		state.current_dir = 'vlang_simplegui/demos'
	} else if !os.exists(state.current_dir) {
		state.current_dir = '.'
	}

	// Scan available V files in initial folder
	scan_folder(mut state)

	// Create main studio window with clean proportions
	mut win := simplegui.new_simple_window('SimpleGUI RAD Code Explorer & Live Previewer',
		1440, 940)
	win.set_theme(state.theme_name)
	win.set_padding(14)
	win.set_spacing(10)

	// Header Title & Description Banner

	win.add_label('studio_heading', 'SimpleGUI RAD Code Explorer & Interactive Live Previewer')
		.bold(true)
		.font_size(18)
		.font_color('#88c0d0')

	win.add_label('studio_sub', 'Open any V project folder, browse code live, auto-format with `v fmt`, inspect compilation diagnostics, jump to line numbers, and copy RAD code.')
		.font_size(11)
		.font_color('#a3be8c')

	win.add_separator()

	// Top Toolbar Actions
	win.add_toolbar_item('tb_folder', 'Open Folder...', 'Browse and select any folder containing V files',
		'folder')
	win.add_toolbar_item('tb_new', 'New File...', 'Create a new V source file in current folder',
		'doc.badge.plus')
	win.add_toolbar_item('tb_refresh', 'Refresh Folder', 'Rescan V files in current workspace folder',
		'arrow.clockwise')
	win.add_toolbar_item('tb_run', 'Live Run File', 'Compile and launch native window preview live',
		'play.circle.fill')
	win.add_toolbar_item('tb_copy', 'Copy V Code', 'Copy editor source code to clipboard',
		'doc.on.doc')
	win.add_toolbar_item('tb_fmt', 'Format Code (v fmt)', 'Auto-format V source code using v fmt',
		'wand.and.stars')
	win.add_toolbar_item('tb_reset', 'Reset Code', 'Revert live edits to original file contents',
		'arrow.counterclockwise')
	win.add_toolbar_item('tb_save', 'Save Code...', 'Save changes back to disk', 'square.and.arrow.down')
	win.add_toolbar_item('tb_line_numbers', 'Toggle Line Numbers', 'Show or hide line number gutter in code editor',
		'list.number')
	win.add_toolbar_item('tb_console_clear', 'Clear Console', 'Clear build & output diagnostic console',
		'trash')

	// Top Control Filter & Palette Bar
	win.begin_row('top_bar')
	win.add_button('btn_open_folder', '📂 Open Folder...')
	win.add_button('btn_new_file', '📄 New File')
	win.add_button('btn_refresh', '🔄 Refresh')

	win.add_label('lbl_search', '🔍 Filter:')
		.bold(true)
		.font_size(12)
		.font_color('#88c0d0')
	win.add_search_field('search_demos', 'Filter V files...')

	win.add_label('lbl_tpl', '⚡ Templates:')
		.bold(true)
		.font_size(12)
		.font_color('#ebcb8b')
	win.add_dropdown('tpl_picker', [
		'Select Quick Template...',
		'Minimal Window',
		'Form & Input Row',
		'Multi-Column Grid Table',
		'Tabbed Layout View',
		'Interactive Event Handlers',
		'Canvas & Custom Drawing',
		'Toolbar & Dialog Alerts',
	], 'Select Quick Template...')

	win.add_label('lbl_theme', '🎨 Theme:')
		.bold(true)
		.font_size(12)
		.font_color('#b48ead')
	win.add_dropdown('theme_picker', ['Dracula', 'Nord', 'Dark', 'Light'], 'Nord')
	win.end_row()

	win.set_control_width('search_demos', 160)
	win.set_control_width('tpl_picker', 180)
	win.set_control_width('theme_picker', 110)

	win.add_separator()

	// Initial file selection setup
	if state.v_files.len > 0 {
		state.selected_file = state.v_files[0]
		state.current_file_path = os.join_path(state.current_dir, state.selected_file)
		state.original_code = os.read_file(state.current_file_path) or { get_default_sample_code() }
	} else {
		state.selected_file = 'untitled.v'
		state.original_code = get_default_sample_code()
	}

	// Section Headers Row for Split View
	win.begin_row('split_headers')

	win.add_label('lbl_demo_count', '📚 Folder: ${os.file_name(state.current_dir)} (${state.v_files.len} Files)')
		.bold(true)
		.font_size(13)
		.font_color('#eceff4')

	win.add_label('code_stats', format_stats_header(state.selected_file, state.original_code,
		false))
		.bold(true)
		.font_size(13)
		.font_color('#81a1c1')
	win.end_row()

	win.set_control_width('lbl_demo_count', 330)
	win.set_control_width('code_stats', 1040)

	// Initial code display formatted with line numbers by default
	initial_display_code := if state.show_line_numbers {
		add_line_numbers(state.original_code)
	} else {
		state.original_code
	}

	// Main Split Studio Layout: Left File List (330px), Right Code Editor (1040px)
	win.begin_row('main_studio_split')
	win.add_list_box('demo_list', state.v_files)
	win.add_textarea('code_editor', initial_display_code)
	win.end_row()

	win.set_control_width('demo_list', 330)
	win.set_control_height('demo_list', 440)
	win.set_control_width('code_editor', 1040)
	win.set_control_height('code_editor', 440)

	// Bind live search filtering to file list
	win.bind_search_to_list('search_demos', 'demo_list')

	// Navigation & Jump Row (Go To Line & Search Code - clean empty inputs)
	win.begin_row('nav_row')
	win.add_button('btn_line_numbers', '#️⃣ Line Numbers')

	win.add_label('lbl_goto', '🎯 Jump Line:')
		.bold(true)
		.font_size(12)
		.font_color('#a3be8c')
	win.add_input('input_goto_line', '')
	win.add_button('btn_goto_line', 'Jump')

	win.add_label('lbl_find_code', '🔍 Search Code:')
		.bold(true)
		.font_size(12)
		.font_color('#d08770')
	win.add_input('input_find_code', '')
	win.add_button('btn_find_code', 'Find')
	win.end_row()

	win.set_control_width('input_goto_line', 80)
	win.set_control_width('input_find_code', 180)

	// Action Buttons Row below Editor
	win.begin_row('action_row')
	win.add_button('btn_run', '▶ Live Run Window')
	win.add_spinner('live_spinner', false)
	win.add_button('btn_copy', '📋 Copy V Code')
	win.add_button('btn_fmt', '⚡ Format with `v fmt`')
	win.add_button('btn_reset', '↺ Reset Code')
	win.add_button('btn_save', '💾 Save File')
	win.end_row()

	// Compiler Diagnostics & Output Log Console

	win.add_label('lbl_console_title', '🛠️ Compiler Diagnostics & Execution Console Log:')
		.bold(true)
		.font_size(12)
		.font_color('#bd93f9')

	win.add_console('output_console', 120)
	win.append_console('output_console', '🚀 SimpleGUI RAD Code Explorer ready. Select a file or click ▶ Live Run Window.',
		1)

	// Enable File & Folder Drag Drop support
	win.on_file_drop(fn [mut state] (mut w simplegui.SimpleWindow, files []string) {
		if files.len > 0 {
			target := files[0]
			if os.is_dir(target) {
				change_workspace_folder(mut state, mut w, target)
			} else if target.ends_with('.v') {
				folder := os.dir(target)
				change_workspace_folder(mut state, mut w, folder)
				file_name := os.file_name(target)
				load_file_by_name(mut state, mut w, file_name)
			}
		}
	})

	// --- Event Listeners ---

	// Open Folder Actions
	win.on_toolbar_click('tb_folder', fn [mut state] (mut w simplegui.SimpleWindow) {
		chosen := w.select_folder()
		if chosen != '' {
			change_workspace_folder(mut state, mut w, chosen)
		}
	})
	win.on_click('btn_open_folder', fn [mut state] (mut w simplegui.SimpleWindow) {
		chosen := w.select_folder()
		if chosen != '' {
			change_workspace_folder(mut state, mut w, chosen)
		}
	})

	// New File Actions
	win.on_toolbar_click('tb_new', fn [mut state] (mut w simplegui.SimpleWindow) {
		create_new_file(mut state, mut w)
	})
	win.on_click('btn_new_file', fn [mut state] (mut w simplegui.SimpleWindow) {
		create_new_file(mut state, mut w)
	})

	// Refresh Folder Actions
	win.on_toolbar_click('tb_refresh', fn [mut state] (mut w simplegui.SimpleWindow) {
		refresh_current_folder(mut state, mut w)
	})
	win.on_click('btn_refresh', fn [mut state] (mut w simplegui.SimpleWindow) {
		refresh_current_folder(mut state, mut w)
	})

	// Toggle Line Numbers Action
	win.on_toolbar_click('tb_line_numbers', fn [mut state] (mut w simplegui.SimpleWindow) {
		toggle_line_numbers(mut state, mut w)
	})
	win.on_click('btn_line_numbers', fn [mut state] (mut w simplegui.SimpleWindow) {
		toggle_line_numbers(mut state, mut w)
	})

	// Go To Line Action (Click Jump or Change Input)
	win.on_click('btn_goto_line', fn (mut w simplegui.SimpleWindow) {
		line_val := w.get_text('input_goto_line')
		jump_to_line_number(mut w, line_val, true)
	})
	win.on_change('input_goto_line', fn (mut w simplegui.SimpleWindow, val string) {
		if val.trim_space() != '' && val.bytes().all(it >= `0` && it <= `9`) {
			jump_to_line_number(mut w, val, false)
		}
	})

	// Find in Code Action (Click Find or Change Input)
	win.on_click('btn_find_code', fn (mut w simplegui.SimpleWindow) {
		query := w.get_text('input_find_code')
		find_in_editor_code(mut w, query, true)
	})
	win.on_change('input_find_code', fn (mut w simplegui.SimpleWindow, query string) {
		if query.trim_space() != '' {
			find_in_editor_code(mut w, query, false)
		}
	})

	// Clear Console Action
	win.on_toolbar_click('tb_console_clear', fn (mut w simplegui.SimpleWindow) {
		w.clear_console('output_console')
		w.set_status('Cleared output console log.')
	})

	// Listbox Selection Handlers (Change & Double-click)
	win.on_change('demo_list', fn [mut state] (mut w simplegui.SimpleWindow, val string) {
		load_selected_v_file(mut state, mut w, val)
	})

	win.on_list_double_click('demo_list', fn [mut state] (mut w simplegui.SimpleWindow, val string) {
		load_selected_v_file(mut state, mut w, val)
	})

	// Live Code Editor Typing / Change Event
	win.on_change('code_editor', fn [mut state] (mut w simplegui.SimpleWindow, value string) {
		clean_code := strip_line_numbers(value)
		state.is_dirty = (clean_code != strip_line_numbers(state.original_code))
		w.set_text('code_stats', format_stats_header(state.selected_file, clean_code,
			state.is_dirty))
	})

	// Toolbar & Button Actions
	win.on_toolbar_click('tb_run', fn [state] (mut w simplegui.SimpleWindow) {
		run_live_demo(mut w, state.selected_file)
	})
	win.on_click('btn_run', fn [state] (mut w simplegui.SimpleWindow) {
		run_live_demo(mut w, state.selected_file)
	})

	win.on_toolbar_click('tb_copy', fn (mut w simplegui.SimpleWindow) {
		copy_code_to_clipboard(mut w)
	})
	win.on_click('btn_copy', fn (mut w simplegui.SimpleWindow) {
		copy_code_to_clipboard(mut w)
	})

	win.on_toolbar_click('tb_fmt', fn (mut w simplegui.SimpleWindow) {
		format_code_with_vfmt(mut w)
	})
	win.on_click('btn_fmt', fn (mut w simplegui.SimpleWindow) {
		format_code_with_vfmt(mut w)
	})

	win.on_toolbar_click('tb_reset', fn [mut state] (mut w simplegui.SimpleWindow) {
		reset_editor_code(mut state, mut w)
	})
	win.on_click('btn_reset', fn [mut state] (mut w simplegui.SimpleWindow) {
		reset_editor_code(mut state, mut w)
	})

	win.on_toolbar_click('tb_save', fn [mut state] (mut w simplegui.SimpleWindow) {
		save_editor_code(mut state, mut w)
	})
	win.on_click('btn_save', fn [mut state] (mut w simplegui.SimpleWindow) {
		save_editor_code(mut state, mut w)
	})

	// Template Snippet Inserter
	win.on_change('tpl_picker', fn [mut state] (mut w simplegui.SimpleWindow, tpl_name string) {
		mut resolved_name := tpl_name
		if tpl_name.bytes().all(it >= `0` && it <= `9`) {
			idx := tpl_name.int()
			tpl_items := [
				'Select Quick Template...',
				'Minimal Window',
				'Form & Input Row',
				'Multi-Column Grid Table',
				'Tabbed Layout View',
				'Interactive Event Handlers',
				'Canvas & Custom Drawing',
				'Toolbar & Dialog Alerts',
			]
			if idx >= 0 && idx < tpl_items.len {
				resolved_name = tpl_items[idx]
			}
		}
		if resolved_name == '' || resolved_name == '0'
			|| resolved_name == 'Select Quick Template...' {
			return
		}
		snippet := get_template_snippet(resolved_name)
		if snippet != '' {
			display_code := if state.show_line_numbers {
				add_line_numbers(snippet)
			} else {
				snippet
			}
			w.set_text('code_editor', display_code)
			state.is_dirty = (snippet != state.original_code)
			w.set_text('code_stats', format_stats_header(state.selected_file, snippet,
				state.is_dirty))
			w.append_console('output_console', '⚡ Loaded quick template snippet: ${resolved_name}',
				1)
			w.set_status('Loaded quick template snippet into editor: ${resolved_name}')
		}
	})

	// Studio Theme Picker
	win.on_change('theme_picker', fn [mut state] (mut w simplegui.SimpleWindow, theme string) {
		mut chosen := theme
		if theme.bytes().all(it >= `0` && it <= `9`) {
			idx := theme.int()
			themes := ['Dracula', 'Nord', 'Dark', 'Light']
			if idx >= 0 && idx < themes.len {
				chosen = themes[idx]
			}
		}
		state.theme_name = chosen.to_lower()
		w.set_theme(state.theme_name)

		title_color := match state.theme_name {
			'dracula' { '#8be9fd' }
			'nord' { '#88c0d0' }
			'dark' { '#ff6b6b' }
			else { '#0066cc' }
		}
		sub_color := match state.theme_name {
			'dracula' { '#6272a4' }
			'nord' { '#a3be8c' }
			'dark' { '#e0e0e0' }
			else { '#4a5568' }
		}
		w.set_control_font_color('studio_heading', title_color)
		w.set_control_font_color('studio_sub', sub_color)
		w.append_console('output_console', '🎨 Studio workspace theme changed to ${chosen}.',
			0)
		w.set_status('Studio workspace theme updated to ${chosen}.')
	})

	win.set_status('SimpleGUI Previewer loaded. Folder: ${state.current_dir} (${state.v_files.len} .v files)')

	// Capture screenshot if automated inspection is requested
	capture_path := os.getenv('SIMPLEGUI_CAPTURE')
	if capture_path != '' {
		win.after(1200, fn [capture_path] (mut w simplegui.SimpleWindow) {
			w.capture_screenshot(capture_path)
			w.after(400, fn (mut w2 simplegui.SimpleWindow) {
				w2.close()
			})
		})
	}

	win.run()
}

// ----------------------------------------------------
// Line Numbering & Navigation Helpers
// ----------------------------------------------------

fn strip_line_numbers(code string) string {
	mut lines := []string{}
	for line in code.split_into_lines() {
		mut clean := line
		if line.contains(' | ') {
			parts := line.split_nth(' | ', 2)
			if parts.len == 2 {
				left := parts[0].trim_space()
				if left.bytes().all(it >= `0` && it <= `9`) {
					clean = parts[1]
				}
			}
		}
		lines << clean
	}
	return lines.join('\n')
}

fn add_line_numbers(code string) string {
	clean := strip_line_numbers(code)
	lines := clean.split_into_lines()
	mut result := []string{}
	for idx, line in lines {
		num := idx + 1
		result << '${num:3d} | ${line}'
	}
	return result.join('\n')
}

fn toggle_line_numbers(mut state PreviewerState, mut w simplegui.SimpleWindow) {
	state.show_line_numbers = !state.show_line_numbers
	current := w.get_text('code_editor')
	if state.show_line_numbers {
		w.set_text('code_editor', add_line_numbers(current))
		w.append_console('output_console', '#️⃣ Enabled line numbers in code editor.',
			1)
		w.set_status('Enabled line numbers in editor.')
	} else {
		w.set_text('code_editor', strip_line_numbers(current))
		w.append_console('output_console', '#️⃣ Disabled line numbers in code editor.',
			1)
		w.set_status('Disabled line numbers in editor.')
	}
}

fn jump_to_line_number(mut w simplegui.SimpleWindow, line_str string, focus bool) {
	if line_str.trim_space() == '' {
		return
	}
	line_num := line_str.int()
	raw_code := strip_line_numbers(w.get_text('code_editor'))
	lines := raw_code.split_into_lines()

	if line_num <= 0 || line_num > lines.len {
		w.append_console('output_console', '⚠️ Line ${line_num} is out of bounds (1-${lines.len}).',
			2)
		w.set_status('Out of bounds line number (1-${lines.len}).')
		return
	}

	target_code := lines[line_num - 1].trim_space()
	w.append_console('output_console', '🎯 Line ${line_num}: ${target_code}', 4)
	w.set_status('🎯 Jumped to Line ${line_num}/${lines.len}: ${target_code}')
	w.textarea_goto_line('code_editor', line_num, focus)
}

fn find_in_editor_code(mut w simplegui.SimpleWindow, query string, focus bool) {
	if query.trim_space() == '' {
		return
	}
	raw_code := strip_line_numbers(w.get_text('code_editor'))
	lines := raw_code.split_into_lines()
	mut matches := []int{}

	for idx, line in lines {
		if line.to_lower().contains(query.to_lower()) {
			matches << (idx + 1)
		}
	}

	if matches.len == 0 {
		w.append_console('output_console', '🔍 Search for "${query}": 0 matches found.',
			2)
		w.set_status('Search: 0 matches found for "${query}"')
	} else {
		w.append_console('output_console', '🔍 Search for "${query}": Found ${matches.len} matches on lines: ${matches}',
			1)
		first_line := matches[0]
		target_text := lines[first_line - 1].trim_space()
		w.set_status('🔍 Found ${matches.len} matches for "${query}". Line ${first_line}: ${target_text}')
		w.textarea_goto_line('code_editor', first_line, focus)
	}
}

// ----------------------------------------------------
// Folder & File Management Helpers
// ----------------------------------------------------

fn scan_folder(mut state PreviewerState) {
	state.v_files.clear()
	if os.exists(state.current_dir) {
		files := os.ls(state.current_dir) or { []string{} }
		for f in files {
			if f.ends_with('.v') {
				state.v_files << f
			}
		}
		state.v_files.sort()
	}
}

fn refresh_current_folder(mut state PreviewerState, mut w simplegui.SimpleWindow) {
	scan_folder(mut state)
	w.update_list_items('demo_list', state.v_files)
	w.bind_search_to_list('search_demos', 'demo_list')
	w.set_text('lbl_demo_count', '📚 Folder: ${os.file_name(state.current_dir)} (${state.v_files.len} Files)')
	w.append_console('output_console', '🔄 Refreshed workspace folder: ${state.current_dir} (${state.v_files.len} files)',
		1)
	w.set_status('Refreshed folder list (${state.v_files.len} .v files found).')
}

fn change_workspace_folder(mut state PreviewerState, mut w simplegui.SimpleWindow, folder_path string) {
	if !os.exists(folder_path) || !os.is_dir(folder_path) {
		w.append_console('output_console', '❌ Invalid directory path: ${folder_path}',
			3)
		w.set_status('Invalid directory path: ${folder_path}')
		return
	}

	state.current_dir = folder_path
	scan_folder(mut state)

	if state.v_files.len == 0 {
		w.set_status('Folder loaded: ${folder_path} (No .v files found)')
		w.set_text('lbl_demo_count', '📚 Folder: ${os.file_name(folder_path)} (0 Files)')
		w.update_list_items('demo_list', []string{})
		w.append_console('output_console', '📁 Opened folder: ${folder_path} (0 .v files)',
			2)
		return
	}

	w.update_list_items('demo_list', state.v_files)
	w.bind_search_to_list('search_demos', 'demo_list')
	w.set_text('lbl_demo_count', '📚 Folder: ${os.file_name(folder_path)} (${state.v_files.len} Files)')

	// Auto load first file in selected folder
	load_file_by_name(mut state, mut w, state.v_files[0])
	w.append_console('output_console', '📁 Workspace changed to: ${folder_path} (${state.v_files.len} .v files)',
		1)
	w.set_status('Loaded folder: ${folder_path} (${state.v_files.len} .v files found)')
}

fn create_new_file(mut state PreviewerState, mut w simplegui.SimpleWindow) {
	mut base_name := 'new_demo.v'
	mut counter := 1
	for os.exists(os.join_path(state.current_dir, base_name)) {
		base_name = 'new_demo_${counter}.v'
		counter++
	}
	new_path := os.join_path(state.current_dir, base_name)
	sample := get_default_sample_code()
	os.write_file(new_path, sample) or {
		w.append_console('output_console', '❌ Failed to create file: ${new_path}', 3)
		w.set_status('Failed to create new file.')
		return
	}

	scan_folder(mut state)
	w.update_list_items('demo_list', state.v_files)
	w.set_text('lbl_demo_count', '📚 Folder: ${os.file_name(state.current_dir)} (${state.v_files.len} Files)')

	load_file_by_name(mut state, mut w, base_name)
	w.append_console('output_console', '📄 Created new V source file: ${base_name}',
		4)
	w.set_status('Created new V source file: ${base_name}')
}

fn load_selected_v_file(mut state PreviewerState, mut w simplegui.SimpleWindow, val string) {
	if val == '' {
		return
	}
	mut target_name := ''

	// 1. If Cocoa passed row index number (e.g. "0", "1", "2")
	if val.bytes().all(it >= `0` && it <= `9`) {
		idx := val.int()
		items := w.get_list_items('demo_list')
		if idx >= 0 && idx < items.len {
			target_name = items[idx]
		}
	}

	// 2. Fallback: try direct text lookup from list box
	if target_name == '' {
		target_name = w.get_list_selected_text('demo_list')
	}

	// 3. Fallback: use raw val if it matches an existing file name
	if target_name == '' && val in state.v_files {
		target_name = val
	}

	if target_name == '' {
		return
	}

	load_file_by_name(mut state, mut w, target_name)
}

fn load_file_by_name(mut state PreviewerState, mut w simplegui.SimpleWindow, file_name string) {
	file_path := os.join_path(state.current_dir, file_name)
	if os.exists(file_path) {
		content := os.read_file(file_path) or {
			w.append_console('output_console', '❌ Error reading file: ${file_name}',
				3)
			w.set_status('Error reading file: ${file_name}')
			return
		}
		state.selected_file = file_name
		state.current_file_path = file_path
		state.original_code = content
		state.is_dirty = false

		display_code := if state.show_line_numbers { add_line_numbers(content) } else { content }
		w.set_text('code_editor', display_code)
		w.set_text('code_stats', format_stats_header(file_name, content, false))
		w.append_console('output_console', '📄 Loaded file: ${file_name} (${content.len} bytes)',
			0)
		w.set_status('Loaded V file: ${file_path} (${content.len} bytes)')
	} else {
		w.append_console('output_console', '❌ File not found at: ${file_path}', 3)
		w.set_status('File not found at: ${file_path}')
	}
}

fn reset_editor_code(mut state PreviewerState, mut w simplegui.SimpleWindow) {
	display_code := if state.show_line_numbers {
		add_line_numbers(state.original_code)
	} else {
		state.original_code
	}
	w.set_text('code_editor', display_code)
	state.is_dirty = false
	w.set_text('code_stats', format_stats_header(state.selected_file, state.original_code,
		false))
	w.append_console('output_console', '↺ Reverted code to original contents on disk.',
		1)
	w.set_status('Reverted editor code to original file contents.')
}

// ----------------------------------------------------
// UI Action Helpers
// ----------------------------------------------------

fn format_stats_header(file_name string, content string, is_dirty bool) string {
	clean := strip_line_numbers(content)
	lines := clean.split_into_lines().len
	words := clean.split_any(' \n\t').filter(it != '').len
	chars := clean.len
	mut fn_count := 0
	for line in clean.split_into_lines() {
		if line.trim_space().starts_with('fn ') {
			fn_count++
		}
	}
	dirty_tag := if is_dirty { ' * [Modified]' } else { '' }
	return '📄 File: ${file_name}${dirty_tag}  |  📏 ${lines} Lines  |  📝 ${words} Words  |  🔤 ${chars} Chars  |  ⚙️ ${fn_count} Functions'
}

fn copy_code_to_clipboard(mut w simplegui.SimpleWindow) {
	raw_code := strip_line_numbers(w.get_text('code_editor'))
	w.copy_to_clipboard(raw_code)
	w.append_console('output_console', '📋 Copied clean V source code (${raw_code.len} chars) to system clipboard.',
		1)
	w.set_status('V source code (${raw_code.len} chars) successfully copied to system clipboard.')
}

fn format_code_with_vfmt(mut w simplegui.SimpleWindow) {
	raw_code := strip_line_numbers(w.get_text('code_editor'))
	if raw_code.trim_space().len == 0 {
		return
	}
	tmp_path := os.join_path(os.temp_dir(), 'vfmt_temp.v')
	os.write_file(tmp_path, raw_code) or {
		w.append_console('output_console', '❌ Formatting error: Could not write temporary file.',
			3)
		w.set_status('Formatting error: Could not write temporary file for v fmt.')
		return
	}
	defer {
		os.rm(tmp_path) or {}
	}

	res := os.execute('v fmt -w ${os.quoted_path(tmp_path)}')
	if res.exit_code == 0 {
		formatted := os.read_file(tmp_path) or { raw_code }
		if formatted.trim_space().len > 0 {
			w.set_text('code_editor', formatted)
			w.append_console('output_console', '⚡ Code formatted cleanly with `v fmt`.',
				4)
			w.set_status('Code formatted cleanly with `v fmt`.')
		}
	} else {
		if res.output.trim_space().len > 0 && !res.output.contains('error:') {
			w.set_text('code_editor', res.output)
			w.append_console('output_console', '⚡ Code formatted with `v fmt`.', 4)
			w.set_status('Code formatted cleanly with `v fmt`.')
		} else {
			w.append_console('output_console', '⚠️ v fmt warning/error: ${res.output.trim_space()}',
				2)
			w.set_status('v fmt notice: ${res.output.trim_space()}')
		}
	}
}

fn save_editor_code(mut state PreviewerState, mut w simplegui.SimpleWindow) {
	raw_code := strip_line_numbers(w.get_text('code_editor'))
	if state.current_file_path != '' && os.exists(state.current_file_path) {
		os.write_file(state.current_file_path, raw_code) or {
			w.append_console('output_console', '❌ Save error: Failed to write to ${state.current_file_path}',
				3)
			w.set_status('Save error: Failed to write to file ${state.current_file_path}')
			return
		}
		state.original_code = raw_code
		state.is_dirty = false
		w.set_text('code_stats', format_stats_header(state.selected_file, raw_code, false))
		w.append_console('output_console', '💾 Saved file: ${state.current_file_path}',
			4)
		w.set_status('Saved updated code to: ${state.current_file_path}')
	} else {
		save_path := w.save_file_picker()
		if save_path != '' {
			mut real_path := save_path
			if !real_path.ends_with('.v') {
				real_path += '.v'
			}
			os.write_file(real_path, raw_code) or {
				w.append_console('output_console', '❌ Save error: Failed to save to ${real_path}',
					3)
				w.set_status('Save error: Failed to save file to ${real_path}')
				return
			}
			state.current_file_path = real_path
			state.selected_file = os.file_name(real_path)
			state.original_code = raw_code
			state.is_dirty = false
			w.set_text('code_stats', format_stats_header(state.selected_file, raw_code,
				false))
			w.append_console('output_console', '💾 Saved file to: ${real_path}', 4)
			w.set_status('Saved file to: ${real_path}')
		}
	}
}

fn run_live_demo(mut w simplegui.SimpleWindow, demo_name string) {
	raw_code := strip_line_numbers(w.get_text('code_editor'))
	if raw_code.trim_space().len == 0 {
		w.append_console('output_console', '❌ Execution error: Code editor is empty.',
			3)
		w.set_status('Execution error: Code editor is empty.')
		return
	}

	tmp_runner_path := os.join_path(os.temp_dir(), 'simplegui_live_runner.v')
	tmp_exe_path := os.join_path(os.temp_dir(), 'simplegui_live_runner_bin')
	os.write_file(tmp_runner_path, raw_code) or {
		w.append_console('output_console', '❌ Execution error: Could not write temporary runner file.',
			3)
		w.set_status('Execution error: Could not create temporary execution runner file.')
		return
	}

	w.start_spinner('live_spinner')
	w.set_text('btn_run', '⏳ Compiling...')
	w.append_console('output_console', '⚡ Compiling V source code for ${demo_name}...',
		1)
	w.set_status('⏳ Compiling and launching live window preview for ${demo_name}...')

	w.after(30, fn [demo_name, tmp_runner_path, tmp_exe_path] (mut w2 simplegui.SimpleWindow) {
		// Quick build validation check
		build_res := os.execute('v -o ${os.quoted_path(tmp_exe_path)} ${os.quoted_path(tmp_runner_path)}')
		if build_res.exit_code != 0 {
			w2.stop_spinner('live_spinner')
			w2.set_text('btn_run', '▶ Live Run Window')
			w2.append_console('output_console', '❌ Compilation Failed for ${demo_name}:',
				3)
			for err_line in build_res.output.split_into_lines() {
				if err_line.trim_space().len > 0 {
					w2.append_console('output_console', '   ${err_line}', 3)
				}
			}
			w2.set_status('❌ Compilation failed for ${demo_name}. Check Diagnostic Console.')
			return
		}

		w2.append_console('output_console', '✅ Compilation succeeded! Launching live preview window...',
			4)
		w2.set_text('btn_run', '🚀 Running...')

		// Execute compiled binary in background spawn thread
		spawn fn [tmp_exe_path] () {
			os.execute(os.quoted_path(tmp_exe_path))
		}()

		w2.after(1500, fn [demo_name] (mut w3 simplegui.SimpleWindow) {
			w3.stop_spinner('live_spinner')
			w3.set_text('btn_run', '▶ Live Run Window')
			w3.set_status('✅ Live window running: ${demo_name}')
		})
	})
}

fn get_default_sample_code() string {
	return 'module main

import simplegui

fn main() {
	mut win := simplegui.new_simple_window("SimpleGUI RAD Demo App", 640, 480)
	win.set_theme("dracula")
	win.set_padding(16)

	win.add_heading("Welcome to SimpleGUI RAD Explorer")
	win.add_label("intro", "Edit this V code live and click \'Live Run Window\' to compile and run.")

	win.begin_row("row1")
		win.add_input("txt_input", "Type something here...")
		win.add_button("btn_submit", "Click Me")
	win.end_row()

	win.on_click("btn_submit", fn (mut w simplegui.SimpleWindow) {
		val := w.get_text("txt_input")
		w.set_status("Input value received: \${val}")
	})

	win.run()
}'
}

fn get_template_snippet(tpl_name string) string {
	match tpl_name {
		'1', 'Minimal Window' {
			return '// Minimal SimpleGUI Window Template
mut win := simplegui.new_simple_window("Minimal App", 500, 350)
win.add_heading("Hello World")
win.add_button("btn_ok", "OK")
win.on_click("btn_ok", fn (mut w simplegui.SimpleWindow) {
	w.set_status("Clicked OK")
})
win.run()'
		}
		'2', 'Form & Input Row' {
			return '// Form Controls & Input Row Template
win.begin_row("form_row")
	win.add_label("lbl_name", "User Name:")
	win.add_input("txt_name", "Jane Doe")
	win.add_checkbox("chk_remember", "Remember Me", true)
win.end_row()'
		}
		'3', 'Multi-Column Grid Table' {
			return '// Multi-Column Data Grid Table Template
win.add_grid("data_table", ["ID", "Name", "Role", "Status"], [
	["101", "Alice Smith", "Engineer", "Active"],
	["102", "Bob Johnson", "Designer", "Pending"],
	["103", "Carol White", "Manager", "Active"]
])'
		}
		'4', 'Tabbed Layout View' {
			return '// Tabbed Segmented View Template
win.add_segmented_control("view_tabs", ["Dashboard", "Analytics", "Settings"], "Dashboard")
win.on_change("view_tabs", fn (mut w simplegui.SimpleWindow, val string) {
	w.set_status("Switched tab view to: \${val}")
})'
		}
		'5', 'Interactive Event Handlers' {
			return '// Event Handler Callbacks Template
win.on_hover("btn_action", fn (mut w simplegui.SimpleWindow) {
	w.set_status("Mouse hovered over action button")
})
win.on_change("txt_search", fn (mut w simplegui.SimpleWindow, text string) {
	w.set_status("Search query changed: \${text}")
})'
		}
		'6', 'Canvas & Custom Drawing' {
			return '// Canvas & Custom Drawing Template
win.add_canvas("my_canvas", 400, 250)
win.on_canvas_draw("my_canvas", fn (mut w simplegui.SimpleWindow) {
	// Custom graphics drawing code
	w.set_status("Canvas redrawn")
})'
		}
		'7', 'Toolbar & Dialog Alerts' {
			return '// Toolbar & Action Dialogs Template
win.add_toolbar_item("tb_info", "Info", "Show Info Alert", "info.circle")
win.on_toolbar_click("tb_info", fn (mut w simplegui.SimpleWindow) {
	w.show_message("SimpleGUI Alert", "Hello from native Cocoa toolbar dialog!")
})'
		}
		else {
			return ''
		}
	}
}
