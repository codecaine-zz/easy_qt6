module main

import simplegui

struct FullProfile {
	req_field_1   string
	req_field_2   string
	user_bio      string
	pin_code      string
	custom_config string
	slider_val    int
	plan_select   string
	enable_pin    bool
	use_defaults  bool
	show_details  bool
	hide_banner   bool
}

fn main() {
	mut win := simplegui.new_simple_window('SimpleGUI — Complete Control Bindings Showcase (12 Bindings)',
		740, 920)
		.set_theme('dracula')
		.set_padding(18)
		.set_spacing(10)

	win.add_heading('SimpleGUI Complete Reactive Control Bindings Showcase')

	// =============================================================
	// Section 1: Checkbox & Switch Visibility & Enabling Bindings
	// =============================================================
	win.group('sec_group_1', '1. Checkbox & Switch Logic (enables / disables / shows / hides)',
		fn (mut w simplegui.SimpleWindow) {
			// 1.1 bind_checkbox_enables
			w.add_switch('enable_pin', '1. Enable Security PIN Input (bind_checkbox_enables)',
				false)
			w.add_input('pin_code', '9482').placeholder('Enter PIN...')

			w.add_separator()

			// 1.2 bind_checkbox_disables
			w.add_checkbox('use_defaults', '2. Use Default Configuration (bind_checkbox_disables custom setting)',
				false)
			w.add_input('custom_config', 'Port 8080').placeholder('Custom config...')

			w.add_separator()

			// 1.3 bind_checkbox_shows
			w.add_checkbox('show_details', '3. Show Advanced Debug Panel (bind_checkbox_shows)',
				false)
			w.add_input('debug_info', 'Debug Logs: Active verbose tracing mode').placeholder('Debug details...')

			w.add_separator()

			// 1.4 bind_checkbox_hides
			w.add_checkbox('hide_banner', '4. Hide Promotional Banner (bind_checkbox_hides)',
				false)
			w.add_label('promo_banner', '🔥 SPECIAL PROMO: Upgrade to Pro for 50% Off!')
		})

	// Apply section 1 bindings
	win.bind_checkbox_enables('enable_pin', ['pin_code'])
	win.bind_checkbox_disables('use_defaults', ['custom_config'])
	win.bind_checkbox_shows('show_details', ['debug_info'])
	win.bind_checkbox_hides('hide_banner', ['promo_banner'])

	win.add_vertical_spacer(6)

	// =============================================================
	// Section 2: Form Completeness Validation (bind_inputs_to_button)
	// =============================================================
	win.group('sec_group_2', '2. Required Inputs Form Validation (bind_inputs_to_button)',
		fn (mut w simplegui.SimpleWindow) {
			w.add_input('req_field_1', 'ada_lovelace').placeholder('Username (required)...')
			w.add_input('req_field_2', 'ada@example.com').placeholder('Email address (required)...')
			w.add_action_row({
				'5. Submit Account (Enabled when both filled)': on_submit
			})
		})

	// 1.5 bind_inputs_to_button
	win.bind_inputs_to_button(['req_field_1', 'req_field_2'], '5. Submit Account (Enabled when both filled)')

	win.add_vertical_spacer(6)

	// =============================================================
	// Section 3: Value Mirroring, Progress & Dropdown Lookup
	// =============================================================
	win.group('sec_group_3', '3. Value Mirroring, Progress Sync & Dropdown Lookup', fn (mut w simplegui.SimpleWindow) {
		// 1.6 bind_value_to_label & 1.7 bind_value_to_progress
		w.add_label('slider_lbl', '6. Master Volume: 75%')
		w.add_slider('slider_val', 75)
		w.add_progress_indicator('progress_bar', 75)

		w.add_separator()

		// 1.8 bind_dropdown_to_label
		w.add_dropdown('plan_select', ['Free Tier', 'Pro Developer', 'Enterprise'], 'Pro Developer')
		w.add_label('plan_desc_lbl', '8. Price: $29/mo (Unlimited projects)')
	})

	// 1.6 bind_value_to_label
	win.bind_value_to_label('slider_val', 'slider_lbl', '6. Master Volume: ', '%')
	// 1.7 bind_value_to_progress
	win.bind_value_to_progress('slider_val', 'progress_bar')
	// 1.8 bind_dropdown_to_label
	win.bind_dropdown_to_label('plan_select', 'plan_desc_lbl', {
		'Free Tier':     '8. Price: $0/mo (1 active project)'
		'Pro Developer': '8. Price: $29/mo (Unlimited projects & support)'
		'Enterprise':    '8. Price: Custom SLA & dedicated builds'
	})

	win.add_vertical_spacer(6)

	// =============================================================
	// Section 4: Two-Way Synchronization (bind_two_way)
	// =============================================================
	win.group('sec_group_4', '4. Two-Way Control Synchronization (bind_two_way)', fn (mut w simplegui.SimpleWindow) {
		w.add_input('sync_field_a', '9. Bi-directionally Synced Text')
			.placeholder('Type in field A...')

		w.add_input('sync_field_b', '9. Bi-directionally Synced Text')
			.placeholder('Or type in field B...')
	})

	// 1.9 bind_two_way
	win.bind_two_way('sync_field_a', 'sync_field_b')

	win.add_vertical_spacer(6)

	// =============================================================
	// Section 5: Character Counter & Search Filtering
	// =============================================================
	win.group('sec_group_5', '5. Character Counter & Search Box Filtering', fn (mut w simplegui.SimpleWindow) {
		// 1.10 bind_char_counter
		w.add_label('counter_lbl', '10. Bio Length: 0/25')

		w.add_input('user_bio', 'SimpleGUI reactive!')
			.placeholder('Bio (max 25 chars)...')

		w.add_separator()

		// 1.11 bind_search_to_list
		w.add_search_field('search_box', '11. Live filter contacts...')
		w.add_list_box('contacts_list', [
			'Ada Lovelace (Pioneer)',
			'Alan Turing (Cryptographer)',
			'Grace Hopper (COBOL Creator)',
			'John von Neumann (Architecture)',
			'Claude Shannon (Information Theory)',
			'Margaret Hamilton (Apollo Software)',
		])
	})

	// 1.10 bind_char_counter
	win.bind_char_counter('user_bio', 'counter_lbl', 25)
	// 1.11 bind_search_to_list
	win.bind_search_to_list('search_box', 'contacts_list')

	win.add_vertical_spacer(6)

	// =============================================================
	// Section 6: Struct Reflection Extraction (bind_to_struct)
	// =============================================================
	win.group('sec_group_6', '6. Form Extraction via Reflection (bind_to_struct)', fn (mut w simplegui.SimpleWindow) {
		w.add_action_row({
			'12. Extract All Control Values to Struct': on_export_struct
		})
	})

	win.set_status('All 12 Reactive Bindings Active & Ready.')
	win.run()
}

fn on_submit(mut win simplegui.SimpleWindow) {
	win.toast('Form validation passed! Account submitted.')
}

fn on_export_struct(mut win simplegui.SimpleWindow) {
	mut data := FullProfile{}
	win.bind_to_struct(mut data)

	msg := 'Extracted FullProfile Struct:\n' + '• Username (req_field_1): ${data.req_field_1}\n' +
		'• Email (req_field_2): ${data.req_field_2}\n' + '• User Bio: ${data.user_bio}\n' +
		'• Security PIN: ${data.pin_code}\n' + '• Custom Config: ${data.custom_config}\n' +
		'• Volume Slider: ${data.slider_val}%\n' + '• Selected Plan: ${data.plan_select}\n' +
		'• Enable PIN Switch: ${data.enable_pin}\n' + '• Use Defaults: ${data.use_defaults}\n' +
		'• Show Details: ${data.show_details}\n' + '• Hide Banner: ${data.hide_banner}'

	win.alert('12. bind_to_struct Reflection Export', msg)
	win.set_status('Extracted all 12 bound controls into struct.')
}
