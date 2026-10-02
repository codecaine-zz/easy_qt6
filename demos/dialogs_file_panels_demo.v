module main

import simplegui

fn main() {
	mut win := simplegui.new_simple_window('Dialogs & File Panels Demo', 550, 600)
		.add_label('lbl_title', 'Click a button to trigger native macOS dialogs:')
		.add_separator()
		.begin_row

	// 1. Alert & Prompt Dialog Buttons

	('row_dialogs1')
		.add_button('btn_info', 'Info Alert')
		.add_button('btn_warn', 'Warn Alert')
		.add_button('btn_error', 'Error Dialog')
		.end_row()
		.begin_row('row_dialogs2')
		.add_button('btn_ask', 'Ask (Yes/No)')
		.add_button('btn_choose', 'Choose Option')
		.add_button('btn_ask_text', 'Ask Text')
		.end_row()
		.add_separator()
		.begin_row

	// 2. File & Directory Picker Buttons

	('row_files1')
		.add_button('btn_file', 'Choose Any File')
		.add_button('btn_file_ext', 'Choose Image (PNG/JPG)')
		.end_row()
		.begin_row('row_files2')
		.add_button('btn_folder', 'Choose Directory')
		.add_button('btn_save', 'Choose Save Target')
		.end_row()
		.add_separator()
		.add_button('btn_quit', 'Quit Application')

	// --- Event Handlers ---

	// 1. Alert Dialog Shortcuts
	win.on_click('btn_info', fn (mut win simplegui.SimpleWindow) {
		win.info('Information', 'This is an informational popup message.')
		win.set_status('Displayed Info alert.')
	})

	win.on_click('btn_warn', fn (mut win simplegui.SimpleWindow) {
		win.warn('Warning', 'Low memory warning or potential issue.')
		win.set_status('Displayed Warning alert.')
	})

	win.on_click('btn_error', fn (mut win simplegui.SimpleWindow) {
		win.error_dialog('Critical Error', 'Failed to perform the requested operation!')
		win.set_status('Displayed Error dialog.')
	})

	// 2. Interactive Dialog Shortcuts
	win.on_click('btn_ask', fn (mut win simplegui.SimpleWindow) {
		confirmed := win.ask('Confirmation', 'Do you want to proceed with this task?')
		win.set_status('Ask confirmation result: ${confirmed}')
	})

	win.on_click('btn_choose', fn (mut win simplegui.SimpleWindow) {
		choice_idx := win.choose('Select Environment', 'Pick a deployment environment:',
			['Development', 'Staging', 'Production'])
		win.set_status('Selected choice index: ${choice_idx}')
	})

	win.on_click('btn_ask_text', fn (mut win simplegui.SimpleWindow) {
		response := win.ask_text('User Profile', 'Enter your display handle:', 'GuestUser')
		win.set_status('User entered handle: "${response}"')
	})

	// 3. Native File Panel Shortcuts
	win.on_click('btn_file', fn (mut win simplegui.SimpleWindow) {
		path := win.choose_file()
		win.set_status('Chosen file: ${path}')
	})

	win.on_click('btn_file_ext', fn (mut win simplegui.SimpleWindow) {
		path := win.choose_file_ext('png,jpg,jpeg')
		win.set_status('Chosen image: ${path}')
	})

	win.on_click('btn_folder', fn (mut win simplegui.SimpleWindow) {
		folder_path := win.choose_folder()
		win.set_status('Chosen directory: ${folder_path}')
	})

	win.on_click('btn_save', fn (mut win simplegui.SimpleWindow) {
		save_path := win.choose_save_file()
		win.set_status('Chosen save target: ${save_path}')
	})

	// 4. Application Exit
	win.on_click('btn_quit', fn (mut win simplegui.SimpleWindow) {
		if win.ask('Exit App', 'Are you sure you want to quit?') {
			win.quit() // Terminate event loop immediately
		}
	})

	win.run()
}
