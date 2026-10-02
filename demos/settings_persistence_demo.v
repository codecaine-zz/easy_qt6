module main

import json2
import simplegui

// Struct for typed settings persistence
struct AppSettings {
mut:
	server_host string
	server_port int
	env_mode    string
	log_switch  bool
	max_conn    int
}

fn main() {
	mut win := simplegui.new_simple_window('Settings Persistence Demo', 550, 550)
		.add_form_field('Server Address', 'server_host', '127.0.0.1')
		.add_form_number('Port Number', 'server_port', 8080)
		.add_form_dropdown('Environment', 'env_mode', ['Development', 'Staging', 'Production'],
			'Development')
		.add_form_switch('Enable Logging', 'log_switch', 'Verbose Logs', true)
		.add_form_slider('Max Connections', 'max_conn', 50)
		.add_separator()
		.begin_row

	// 2. Action Buttons

	('btn_row1')
		.add_button('btn_save_struct', '1. Save (Struct)')
		.add_button('btn_load_struct', '2. Load (Struct)')
		.end_row()
		.begin_row('btn_row2')
		.add_button('btn_save_file', '3. Save (win.save_values_to_file)')
		.add_button('btn_load_file', '4. Load + Manual Int Fix')
		.add_button('btn_reset', 'Reset Form')
		.end_row()

	// --- Event Handlers ---

	// Option A (Recommended): Type-safe Struct Persistence via V's json2 module
	win.on_click('btn_save_struct', fn (mut win simplegui.SimpleWindow) {
		mut cfg := AppSettings{}
		win.bind_to_struct(mut cfg)

		json_str := json2.encode(cfg)
		win.write_file('app_settings.json', json_str)
		win.set_status('Saved typed settings via struct to app_settings.json')
	})

	win.on_click('btn_load_struct', fn (mut win simplegui.SimpleWindow) {
		content := win.read_file('app_settings.json')
		if content == '' // 1. Controls Setup

		{
			win.set_status('No app_settings.json file found!')
			return
		}

		cfg := json2.decode[AppSettings](content) or {
			win.set_status('Failed to parse settings JSON: ${err}')
			return
		}

		win.load_from_struct(cfg)
		win.set_status('Loaded struct settings! Port correctly restored to: ${cfg.server_port}')
	})

	// Option B: win.save_values_to_file / win.load_values_from_file with manual integer fix
	win.on_click('btn_save_file', fn (mut win simplegui.SimpleWindow) {
		win.save_values_to_file('raw_values.json') or {
			win.set_status('Failed to save file: ${err}')
			return
		}
		win.set_status('Saved raw control values to raw_values.json')
	})

	win.on_click('btn_load_file', fn (mut win simplegui.SimpleWindow) {
		win.load_values_from_file('raw_values.json') or {
			win.set_status('Failed to load file: ${err}')
			return
		}

		// Parse and re-apply integer values explicitly
		port := win.get_int('server_port')
		win.set_value_int('server_port', port)

		win.set_status('Loaded raw settings and restored port: ${port}')
	})

	// Reset form controls back to defaults
	win.on_click('btn_reset', fn (mut win simplegui.SimpleWindow) {
		win.reset_all_fields()
		win.set_status('Reset form controls to initial baseline.')
	})

	win.run()
}
