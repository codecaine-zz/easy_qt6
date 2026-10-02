module main

import simplegui

fn main() {
	mut win := simplegui.new_simple_window('Labeled Control Rows Demo', 550, 500)
		.add_labeled_slider('Volume Level', 'vol_slider', 65)
		.add_labeled_dropdown('Choose Language', 'lang_select', ['Vlang', 'Go', 'Rust', 'Python'],
			'Vlang')
		.add_labeled_number('Target Age', 'age_num', 25)
		.add_labeled_date_picker('Start Date', 'start_date', '2026-07-20')
		.add_labeled_progress('Download State', 'progress_bar', 75)
		.add_separator()
		.begin_row

	// 2. Form Action Buttons

	('btn_row')
		.add_button('btn_get', 'Read Values')
		.add_button('btn_update', 'Increase Volume & Progress')
		.add_button('btn_reset', 'Reset Form')
		.end_row()

	// --- Event Handlers ---

	// Reading current values from labeled control rows
	win.on_click('btn_get', fn (mut win simplegui.SimpleWindow) {
		vol := win.get_int('vol_slider')
		lang := win.get_text('lang_select')
		age := win.get_int('age_num')
		date := win.get_text('start_date')
		prog := win.get_int('progress_bar')

		println('=== Labeled Control Values ===')
		println('Volume: ${vol}%')
		println('Language: ${lang}')
		println('Age: ${age}')
		println('Date: ${date}')
		println('Progress: ${prog}%')

		win.set_status('Read values into stdout!')
	})

	// Dynamically updating values of labeled controls
	win.on_click('btn_update', fn (mut win simplegui.SimpleWindow) {
		new_vol := win.increment('vol_slider', 10)
		win.increment_progress('progress_bar', 10)

		win.set_status('Incremented volume to ${new_vol}% and updated progress bar.')
	})

	// Resetting controls back to default registration states
	win.on_click('btn_reset', fn (mut win simplegui.SimpleWindow) {
		win.reset_all_fields()
		win.set_status('Reset form fields to defaults.')
	})

	win.run()
}
