module main

import simplegui

struct BindingExample {
	username         string
	age              int
	wants_newsletter bool
}

struct CallbackState {
mut:
	called bool
}

struct TestProfile {
	username string
	score    int
	active   bool
}

struct EventChainState {
mut:
	clicked     bool
	changed_val string
}

struct ProjectRow {
	id        int
	name      string
	is_active bool
}

struct TestValidationStruct {
	name  string @[min_len: '3'; required]
	email string @[email; required]
	age   int    @[max: '99'; min: '18']
}

fn test_responsive_layout_api_is_available() {
	mut win := simplegui.SimpleWindow{}
	assert win.get_responsive_layout() == true
	win.set_responsive_layout(false)
	assert win.get_responsive_layout() == false
	win.set_responsive_layout(true)
	assert win.get_responsive_layout() == true
}

fn test_layout_rows() {
	mut win := simplegui.new_simple_window('Test Window', 100, 100)
	win.add_fields_row({
		'First Name': 'fn'
		'Last Name':  'ln'
	})

	assert win.has_control('fn') == true
	assert win.has_control('ln') == true
	assert win.has_control('fn_label') == true
	assert win.has_control('ln_label') == true
}

fn test_row_closure_layout() {
	mut win := simplegui.new_simple_window('Test Window', 100, 100)
	win.row('settings', fn (mut w simplegui.SimpleWindow) {
		w.add_input('db_host', 'localhost')
		w.add_number('db_port', 3306)
	})

	assert win.has_control('db_host') == true
	assert win.has_control('db_port') == true
	assert win.get_text('db_host') == 'localhost'
	assert win.get_value_int('db_port') == 3306
}

fn test_group_layout_nesting() {
	mut win := simplegui.new_simple_window('Test Window', 100, 100)
	win.group('profile', 'Profile Details', fn (mut w simplegui.SimpleWindow) {
		w.add_input('first_name', 'Ada')
		w.add_input('last_name', 'Lovelace')
	})

	assert win.has_control('profile') == true
	assert win.has_control('first_name') == true
	assert win.has_control('last_name') == true
	assert win.get('first_name') == 'Ada'
}

fn test_table_row_management_helpers() {
	mut win := simplegui.SimpleWindow{}
	win.add_table('inventory', ['ID', 'Name', 'Qty'])

	// Starts empty
	assert win.get_table_row_count('inventory') == 0
	assert win.get_table_rows('inventory') == [][]string{}

	// Bulk load + tracking
	win.set_table_rows('inventory', [['1', 'Bolt', '40'], ['2', 'Nut', '75']])
	assert win.get_table_row_count('inventory') == 2
	assert win.get_table_row('inventory', 0) == ['1', 'Bolt', '40']
	assert win.get_table_cell('inventory', 1, 1) == 'Nut'
	assert win.get_table_cell('inventory', 9, 9) == ''
	assert win.get_table_row('inventory', 9) == []string{}

	// Append / insert / update
	win.add_table_row('inventory', ['3', 'Washer', '120'])
	assert win.get_table_row_count('inventory') == 3
	win.insert_table_row('inventory', 0, ['0', 'Screw', '10'])
	assert win.get_table_row('inventory', 0) == ['0', 'Screw', '10']
	win.update_table_row('inventory', 0, ['0', 'Screw', '11'])
	assert win.get_table_cell('inventory', 0, 2) == '11'
	win.set_table_cell('inventory', 0, 1, 'Wood Screw')
	assert win.get_table_cell('inventory', 0, 1) == 'Wood Screw'

	// Search
	assert win.find_table_row('inventory', 1, 'Nut') == 2
	assert win.find_table_row('inventory', 1, 'Missing') == -1

	// Remove / clear
	win.remove_table_row('inventory', 0)
	assert win.get_table_row_count('inventory') == 3
	win.remove_table_row('inventory', 99) // out of range is a no-op
	assert win.get_table_row_count('inventory') == 3
	win.clear_table('inventory')
	assert win.get_table_row_count('inventory') == 0
}

fn test_table_strict_apis_and_row_normalization() {
	mut win := simplegui.SimpleWindow{}
	win.add_table('inventory', ['ID', 'Name', 'Qty'])

	win.set_table_rows_strict('inventory', [
		['1', 'Bolt'],
		['2', 'Nut', '75', 'ignored'],
	]) or { assert false, err.msg() }

	assert win.get_table_column_count('inventory') == 3
	assert win.get_table_rows('inventory') == [
		['1', 'Bolt', ''],
		['2', 'Nut', '75'],
	]

	win.add_table_row_strict('inventory', ['3', 'Washer', '10']) or { assert false, err.msg() }
	win.insert_table_row_strict('inventory', 1, ['1.5', 'Spacer', '5']) or {
		assert false, err.msg()
	}
	win.update_table_row_strict('inventory', 1, ['1.5', 'Spacer', '6']) or {
		assert false, err.msg()
	}
	win.set_table_cell_strict('inventory', 1, 2, '7') or { assert false, err.msg() }
	assert win.get_table_cell('inventory', 1, 2) == '7'

	idx := win.find_table_row_strict('inventory', 1, 'Spacer') or { panic(err) }
	assert idx == 1

	mut row_error := ''
	win.set_table_cell_strict('inventory', 99, 0, 'x') or { row_error = err.msg() }
	assert row_error.contains('row 99 out of range')

	mut missing_error := ''
	_ := win.find_table_row_strict('inventory', 1, 'Missing') or {
		missing_error = err.msg()
		-1
	}
	assert missing_error.contains('value not found')

	win.remove_table_row_strict('inventory', 0) or { assert false, err.msg() }
	assert win.get_table_row_count('inventory') == 3

	win.set_table_rows('inventory', [
		['1', 'Bolt', '10'],
		['2', 'Nut', 'n/a'],
		['3', 'Washer', '20'],
	])
	assert win.get_table_column_average('inventory', 2) == 10.0
	assert win.get_table_column_average_numeric('inventory', 2) == 15.0
}

fn test_advanced_layout_grid_flex_and_alignment() {
	mut win := simplegui.SimpleWindow{}

	// Grid closure container
	win.grid('user_grid', 2, 12, fn (mut w simplegui.SimpleWindow) {
		w.add_input('first_name', 'Grace')
			.align_left()
			.expand_fill()

		w.add_input('last_name', 'Hopper')
			.align_right()
	})

	// Flexbox closure container
	win.flex_box('toolbar_flex', 'row', 'space_between', 'center', fn (mut w simplegui.SimpleWindow) {
		w.add_button('btn_left', 'Back')
			.align_center()
		w.add_button('btn_right', 'Next')
	})

	assert win.has_control('first_name') == true
	assert win.has_control('last_name') == true
	assert win.has_control('btn_left') == true
	assert win.has_control('btn_right') == true

	assert win.get_control_alignment('first_name') == 'left'
	assert win.get_control_alignment('last_name') == 'right'
	assert win.get_control_alignment('btn_left') == 'center'

	assert win.get_control_expand_fill('first_name') == true
	assert win.get_control_expand_fill('last_name') == false

	win.set_control_alignment('last_name', 'center')
	assert win.get_control_alignment('last_name') == 'center'

	win.set_control_expand_fill('last_name', true)
	assert win.get_control_expand_fill('last_name') == true
}

fn test_group_box_options_border_and_caption() {
	mut win := simplegui.SimpleWindow{}

	// Group with title and default border
	win.group('grp_1', 'Section 1', fn (mut w simplegui.SimpleWindow) {
		w.add_input('inp_1', 'val1')
	})
	assert win.has_control('grp_1') == true
	assert win.has_control('inp_1') == true

	// Group with options: empty caption and no border
	win.group_with_options('grp_2', '', false, fn (mut w simplegui.SimpleWindow) {
		w.add_button('btn_2', 'Click')
	})
	assert win.has_control('grp_2') == true
	assert win.has_control('btn_2') == true

	// Group with config struct
	win.group_config('grp_3', simplegui.GroupConfig{ title: 'Config Title', border: true },
		fn (mut w simplegui.SimpleWindow) {
			w.add_label('lbl_3', 'Label Text')
		})
	assert win.has_control('grp_3') == true
	assert win.has_control('lbl_3') == true

	// Group with rich custom config struct (custom borders, radius, colors, card styling)
	win.group_config('grp_rich', simplegui.GroupConfig{
		title:             'Rich Custom Group'
		border:            true
		border_width:      2.5
		border_color:      '#3B82F6'
		corner_radius:     16.0
		bg_color:          '#F8FAFC'
		padding:           20
		shadow:            true
		show_caption:      true
		caption_color:     '#1E293B'
		caption_alignment: 'center'
	}, fn (mut w simplegui.SimpleWindow) {
		w.add_label('lbl_rich', 'Rich Content')
	})
	assert win.has_control('grp_rich') == true
	assert win.has_control('lbl_rich') == true

	// Card container helpers
	win.card('card_1', fn (mut w simplegui.SimpleWindow) {
		w.add_label('lbl_card1', 'Card Item 1')
	})
	assert win.has_control('card_1') == true

	win.card_with_title('card_2', 'User Profile Card', fn (mut w simplegui.SimpleWindow) {
		w.add_label('lbl_card2', 'Card Item 2')
	})
	assert win.has_control('card_2') == true

	// Dynamic setters
	win.set_group_border('grp_1', false)
	win.set_group_caption('grp_1', 'New Section Title')
	win.set_group_style('grp_1', simplegui.GroupConfig{
		border:        true
		border_width:  2.0
		border_color:  '#FF0000'
		corner_radius: 8.0
		bg_color:      '#FFFFFF'
		shadow:        true
	})
}
