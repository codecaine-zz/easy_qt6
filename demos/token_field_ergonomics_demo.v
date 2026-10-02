module main

import simplegui

fn main() {
	mut win := simplegui.new_simple_window('Token Field Ergonomics Demo', 550, 500)
		.add_label('lbl_tags', 'Project Tags:')
		.add_token_field('project_tags', 'vlang, gui, desktop')
		.add_separator()
		.begin_row

	// 2. Buttons to interact with Token Helpers

	('btn_row1')
		.add_button('btn_get', '1. Get Tokens')
		.add_button('btn_set', '2. Set Tokens')
		.end_row()
		.begin_row('btn_row2')
		.add_button('btn_add', '3. Add Token ("macOS")')
		.add_button('btn_remove', '4. Remove Token ("gui")')
		.end_row()

	// --- Event Handlers ---

	// 1. win.get_tokens(name string) []string
	// Reads and parses the comma-separated text into a cleaned slice of strings
	win.on_click('btn_get', fn (mut win simplegui.SimpleWindow) {
		tokens := win.get_tokens('project_tags')
		println('Current Tokens Array: ${tokens}')
		win.set_status('Parsed ${tokens.len} token(s): ${tokens}')
	})

	// 2. win.set_tokens(name string, tokens []string)
	// Formats and replaces the token field with a new slice of string tags
	win.on_click('btn_set', fn (mut win simplegui.SimpleWindow) {
		new_tags := ['ui', 'native', 'fast']
		win.set_tokens('project_tags', new_tags)
		win.set_status('Replaced tokens with: ${new_tags}')
	})

	// 3. win.add_token(name string, token string)
	// Appends a single token tag if it is not already present in the field
	win.on_click('btn_add', fn (mut win simplegui.SimpleWindow) {
		win.add_token('project_tags', 'macOS')
		win.set_status('Added "macOS" token.')
	})

	// 4. win.remove_token(name string, token string)
	// Removes a specified token tag if present
	win.on_click('btn_remove', fn (mut win simplegui.SimpleWindow) {
		win.remove_token('project_tags', 'gui')
		win.set_status('Removed "gui" token.')
	})

	win.run()
}
