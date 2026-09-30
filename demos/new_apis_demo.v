module main

import os
import simplegui

fn main() {
	mut win := simplegui.new_simple_window('SimpleGUI New APIs Demo', 1000, 700)

	win.split_view('main_split', true, fn [mut win] () {
		// Left Pane: Controls & macOS APIs
		win.add_label('lbl_controls', 'New Component APIs').font_size(16).bold(true)
		win.add_separator()

		// 1. Progress Bar (Indeterminate)
		win.add_progress_indicator('prog1', 0).width(300)
		win.begin_row('row_prog')
		win.add_button('btn_prog_start', 'Start Indeterminate')
		win.add_button('btn_prog_stop', 'Stop Indeterminate')
		win.end_row()

		win.on_click('btn_prog_start', fn (mut win simplegui.SimpleWindow) {
			win.set_progress_indeterminate('prog1', true)
			win.start_progress_animation('prog1')
		})
		win.on_click('btn_prog_stop', fn (mut win simplegui.SimpleWindow) {
			win.stop_progress_animation('prog1')
			win.set_progress_indeterminate('prog1', false)
			win.set_progress('prog1', 50)
		})

		// 2. Slider (Tick marks)
		win.add_label('lbl_slider', 'Slider with Tick Marks')
		win.add_slider('slider1', 50).width(300)
		win.set_slider_tick_marks('slider1', 11, true)

		// 3. Path Control
		win.add_label('lbl_path', 'Path Control')
		win.add_path_control('path1', os.home_dir()).width(300)
		win.set_path_control_style('path1', 'navigation')

		win.add_separator()
		win.add_label('lbl_macos', 'macOS Native Integrations').font_size(16).bold(true)

		win.begin_row('row_macos1')
		win.add_button('btn_speak', '🗣 Speak Text')
		win.add_button('btn_haptic', '📳 Haptic Feedback')
		win.end_row()

		win.on_click('btn_speak', fn (mut win simplegui.SimpleWindow) {
			win.speak_native('Welcome to Simple GUI!', '')
		})
		win.on_click('btn_haptic', fn (mut win simplegui.SimpleWindow) {
			win.haptic_feedback('generic')
		})

		win.begin_row('row_macos2')
		win.add_button('btn_apps', 'List Running Apps')
		win.add_button('btn_battery', 'Battery %')
		win.end_row()

		win.on_click('btn_apps', fn (mut win simplegui.SimpleWindow) {
			apps := simplegui.get_running_apps()
			win.textarea_insert_text('txt_log', '\nRunning apps count: ${apps.len}')
			win.textarea_scroll_to_end('txt_log')
		})
		win.on_click('btn_battery', fn (mut win simplegui.SimpleWindow) {
			pct := simplegui.get_battery_percentage()
			win.textarea_insert_text('txt_log', '\nBattery: ${pct}%')
			win.textarea_scroll_to_end('txt_log')
		})

	}, fn [mut win] () {
		// Right Pane: Text Area & Dialogs
		win.add_label('lbl_txt', 'Text Area APIs').font_size(16).bold(true)
		
		win.add_textarea('txt_log', 'This is a rich text area.').width(400).height(200)
		
		win.begin_row('row_txt')
		win.add_button('btn_insert', 'Insert Text')
		win.add_button('btn_clear', 'Clear Text')
		win.end_row()

		win.on_click('btn_insert', fn (mut win simplegui.SimpleWindow) {
			win.textarea_insert_text('txt_log', '\n[Inserted Line]')
			win.textarea_scroll_to_end('txt_log')
		})
		win.on_click('btn_clear', fn (mut win simplegui.SimpleWindow) {
			win.textarea_clear('txt_log')
		})

		win.add_separator()
		win.add_label('lbl_dialogs', 'File Pickers & Dialogs').font_size(16).bold(true)

		win.begin_row('row_dialogs1')
		win.add_button('btn_color', 'Color Picker')
		win.add_button('btn_multi', 'Multi-File Picker')
		win.end_row()

		win.on_click('btn_color', fn (mut win simplegui.SimpleWindow) {
			color := win.show_color_picker('#FF5555')
			win.textarea_insert_text('txt_log', '\nSelected Color: ${color}')
			win.textarea_scroll_to_end('txt_log')
		})
		win.on_click('btn_multi', fn (mut win simplegui.SimpleWindow) {
			files := win.select_multiple_files('png,jpg,v')
			win.textarea_insert_text('txt_log', '\nSelected ${files.len} files.')
			win.textarea_scroll_to_end('txt_log')
		})

		win.begin_row('row_dialogs2')
		win.add_button('btn_save', 'Save File Dialog')
		win.end_row()

		win.on_click('btn_save', fn (mut win simplegui.SimpleWindow) {
			file := win.save_file_picker_with_name('my_export.txt', 'txt,csv')
			win.textarea_insert_text('txt_log', '\nSave Path: ${file}')
			win.textarea_scroll_to_end('txt_log')
		})

	})

	// Configure the split view divider
	win.set_split_divider_style('main_split', 'thin')
	win.set_split_position('main_split', 0, 400.0)

	win.run()
}
