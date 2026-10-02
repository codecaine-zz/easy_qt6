module main

import simplegui
import os

fn test_has_control_list_controls_and_safe_accessors() {
	mut win := simplegui.new_simple_window('Ergonomics Test', 400, 300)
	win.add_input('txt_name', 'Alice')
	win.add_checkbox('chk_terms', 'Agree', true)
	win.add_number('num_age', 30)

	assert win.has_control('txt_name') == true
	assert win.has_control('chk_terms') == true
	assert win.has_control('num_age') == true
	assert win.has_control('non_existent') == false

	ctrls := win.list_controls()
	assert 'txt_name' in ctrls
	assert 'chk_terms' in ctrls
	assert 'num_age' in ctrls

	// Test safe optional accessors
	name_opt := win.get_text_opt('txt_name') or { 'fallback' }
	assert name_opt == 'Alice'

	missing_text := win.get_text_opt('non_existent') or { 'missing' }
	assert missing_text == 'missing'

	chk_opt := win.get_checked_opt('chk_terms') or { false }
	assert chk_opt == true

	missing_chk := win.get_checked_opt('non_existent')
	assert missing_chk == none

	age_opt := win.get_value_int_opt('num_age') or { 0 }
	assert age_opt == 30

	missing_age := win.get_value_int_opt('non_existent')
	assert missing_age == none

	ctrl_entry := win.get_control_opt('txt_name') or { simplegui.ControlEntry{} }
	assert ctrl_entry.name == 'txt_name'

	missing_entry := win.get_control_opt('non_existent')
	assert missing_entry == none
}

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

fn test_control_discovery_helpers_are_available() {
	mut win := simplegui.SimpleWindow{}
	win.add_input('name', 'Ada')
	win.add_button('run', 'Run')

	assert win.has_control('name') == true
	assert win.has_control('missing') == false
	assert win.list_controls().contains('name')
	assert win.list_controls().contains('run')
	assert win.get_control_kind('name') == 'input'
	assert win.get_control_kind('missing') == ''
}

fn test_tree_view_helpers_support_crud_and_paths() {
	mut win := simplegui.SimpleWindow{}
	win.add_tree_view('org', 180)

	win.set_tree('org', [
		simplegui.tree_root('root', 'Company'),
		simplegui.tree_child('eng', 'root', 'Engineering'),
		simplegui.tree_child('frontend', 'eng', 'Frontend'),
		simplegui.tree_child('backend', 'eng', 'Backend'),
		simplegui.tree_child('design', 'root', 'Design'),
	])

	assert win.get_tree_nodes('org').len == 5
	assert win.has_tree_node('org', 'backend') == true

	backend := win.get_tree_node('org', 'backend') or { panic('expected backend node') }
	assert backend.text == 'Backend'

	win.set_tree_node_text('org', 'backend', 'Backend Platform')
	backend_updated := win.get_tree_node('org', 'backend') or {
		panic('expected backend node after update')
	}
	assert backend_updated.text == 'Backend Platform'

	win.add_tree_node('org', simplegui.tree_child('qa', 'eng', 'QA'))
	assert win.has_tree_node('org', 'qa') == true

	win.set_tree_selected('org', 'frontend')
	assert win.get_tree_selected('org') == 'frontend'

	win.expand_tree('org')
	win.open_tree('org')
	win.expand_tree_node('org', 'eng', true)
	win.collapse_tree_node('org', 'eng', false)
	win.collapse_tree('org')
	win.close_tree('org')

	win.remove_tree_node('org', 'eng', false)
	frontend := win.get_tree_node('org', 'frontend') or {
		panic('expected frontend node after reparent')
	}
	assert frontend.parent_id == 'root'

	win.remove_tree_node('org', 'design', true)
	assert win.has_tree_node('org', 'design') == false

	win.clear_tree_selection('org')
	assert win.get_tree_selected('org') == ''

	win.set_tree_paths_with_separator('org', [
		'Company > Ops > Platform',
		'Company > Ops > Security',
	], ' > ')
	assert win.has_tree_node('org', 'Company/Ops/Platform') == true

	win.clear_tree('org')
	assert win.get_tree_nodes('org').len == 0
}

fn test_window_title_visibility_helpers_are_available() {
	mut win := simplegui.SimpleWindow{}

	assert win.get_titlebar_visible() == true
	assert win.get_title_visible() == true
	win.set_titlebar_visible(false)
	win.set_title_visible(false)
	assert win.get_titlebar_visible() == false
	assert win.get_title_visible() == false
}

fn test_qol_helpers_support_struct_binding_and_tables() {
	mut win := simplegui.SimpleWindow{}
	win.add_input('username', 'Ada')
	win.add_number('age', 28)
	win.add_checkbox('wants_newsletter', 'Subscribe', true)
	win.add_vertical_spacer(10)
	win.add_horizontal_spacer(20)
	win.add_separator()
	win.add_table('processes', ['PID', 'Name'])
	win.set_table_rows('processes', [['1', 'V']])
	win.set_values({
		'username': 'Grace'
	})

	assert win.get_values()['username'] == 'Grace'
	assert win.get_values()['username'] == 'Grace'

	mut data := BindingExample{}
	win.bind_to_struct(mut data)
	assert data.username == 'Grace'
	assert data.age == 28
	assert data.wants_newsletter == true

	win.load_from_struct(BindingExample{ username: 'Linus', age: 54, wants_newsletter: false })
	assert win.get_text('username') == 'Linus'
	assert win.get_value_int('age') == 54
	assert win.get_checked('wants_newsletter') == false

	win.enable_status_bar('')
	win.show_window()
	win.run_on_main_thread(fn (mut w simplegui.SimpleWindow) {})
}

fn test_batch_ergonomic_helpers_are_available() {
	mut win := simplegui.SimpleWindow{}
	win.add_input('name', 'Ada')
	win.add_password('secret', 's3cr3t')
	win.add_checkbox('ready', 'Ready', false)
	win.add_number('age', 30)

	win.set_many_texts({
		'name':   'Grace'
		'secret': 'topsecret'
	})
	win.set_many_checked({
		'ready': true
	})
	win.set_many_numbers({
		'age': 42
	})
	win.set_many_visibility({
		'name': false
	})
	win.set_many_enabled({
		'age': false
	})
	win.set_many_errors({
		'name': 'Required'
	})
	win.set_many_placeholders({
		'name': 'Type your name'
	})
	win.set_many_tooltips({
		'name': 'This field is required'
	})
	win.clear_many(['name', 'secret'])
	win.reset_many(['name', 'secret', 'ready', 'age'])

	win.with_busy_state(['name', 'age'], 'Saving...', fn (mut w simplegui.SimpleWindow) {
		assert w.get_control_enabled('name') == false
		assert w.get_control_enabled('age') == false
	})

	texts := win.get_many_texts(['name', 'secret'])
	checks := win.get_many_checked(['ready'])
	numbers := win.get_many_numbers(['age'])
	visibilities := win.get_many_visibility(['name'])
	enabled := win.get_many_enabled(['age'])

	assert texts['name'] == 'Ada'
	assert texts['secret'] == 's3cr3t'
	assert checks['ready'] == false
	assert numbers['age'] == 30
	assert win.get_placeholder('name') == 'Type your name'
	assert win.get_tooltip('name') == 'This field is required'
	assert win.get_control_enabled('name') == true
	assert win.get_control_enabled('age') == false
	assert win.get_status() == 'Saving...'
	assert visibilities['name'] == false
	assert enabled['age'] == false
	assert win.get_error('name') == 'Required'
}

fn test_ergonomic_helpers_are_available_and_resettable() {
	mut win := simplegui.SimpleWindow{}
	win.add_input('name', 'Ada')
	win.add_password('secret', 's3cr3t')
	win.add_html_view('preview', '<h1>Preview</h1>')
	win.add_drop_zone('dropzone', 'Drop files here')
	win.add_checkbox('ready', 'Ready', true)
	win.add_number('age', 30)
	win.add_button('run', 'Run')

	win.set_padding(16)
	win.set_spacing(10)
	win.add_group_box('profile', 'Profile')
	win.add_tabs('mode', ['Simple', 'Advanced'])
	win.add_scroll_view('details', 120)
	win.set_focus('name')
	win.set_placeholder('name', 'Type here')
	win.set_error('name', 'Required')
	win.set_tooltip('secret', 'Use a strong password')
	win.set_default_button('run')
	win.on_enter('name', fn (mut w simplegui.SimpleWindow) {})
	win.on_key('a', fn (mut w simplegui.SimpleWindow, value string) {})
	win.on_close(fn (mut w simplegui.SimpleWindow) {})
	win.run_after(5, fn (mut w simplegui.SimpleWindow) {})
	win.toast('Saved')
	win.copy_to_clipboard('hello')
	win.open_url('https://example.com')

	assert win.get_padding() == 16
	assert win.get_spacing() == 10
	assert win.get_text('secret') == 's3cr3t'
	win.set_html('preview', '<p>Updated</p>')
	assert win.inspect_controls().contains('profile')
	assert win.inspect_controls().contains('mode')
	assert win.inspect_controls().contains('details')

	win.clear('name')
	assert win.get_text('name') == ''

	win.set_text('name', 'Grace')
	win.set_checked('ready', false)
	win.set_value_int('age', 42)
	win.reset_form()
	assert win.get_text('name') == 'Ada'
	assert win.get_checked('ready') == true
	assert win.get_value_int('age') == 30
	assert win.dump_values()['name'] == 'Ada'
}

fn test_high_level_form_helpers_are_available() {
	mut win := simplegui.SimpleWindow{}
	win.add_heading('Profile')
	win.add_form_field('Name', 'name', 'Ada')
	win.add_form_field('City', 'city', 'London')
	win.add_form_textarea('Notes', 'notes', 'Hello')
	win.add_toggle('ready', 'Ready', true)
	win.add_number_field('age', 42)
	win.add_action('run', 'Run', fn (mut w simplegui.SimpleWindow) {})

	assert win.has_control('heading_0') == true
	assert win.has_control('name') == true
	assert win.has_control('city') == true
	assert win.has_control('notes') == true
	assert win.has_control('ready') == true
	assert win.has_control('age') == true
	assert win.has_control('run') == true
	assert win.get_text('name') == 'Ada'
	assert win.get_text('notes') == 'Hello'
	assert win.get_checked('ready') == true
	assert win.get_value_int('age') == 42
	assert win.dispatch_event('run', 'click', '') == true
}

fn test_config_form_and_validation_helpers_are_available() {
	mut win := simplegui.new_simple_window('Test Window', 100, 100)
	win.configure(fn (mut cfg simplegui.WindowConfig) {
		cfg.title = 'Configured Window'
		cfg.width = 420
		cfg.height = 320
		cfg.padding = 18
		cfg.spacing = 8
		cfg.background_color = '#112233'
		cfg.font_color = 'white'
		cfg.resizable = false
	})
	win.form('Profile', fn (mut w simplegui.SimpleWindow) {
		w.add_input('email', 'ada@example.com')
		w.add_checkbox('newsletter', 'Newsletter', true)
	})
	win.section('Account', fn (mut w simplegui.SimpleWindow) {
		w.add_input('username', 'ada')
	})

	assert win.get_title() == 'Configured Window'
	assert win.get_padding() == 18
	assert win.get_spacing() == 8
	assert win.get_background_color() == '#112233'
	assert win.get_font_color() == 'white'
	assert win.get_resizable() == false
	assert win.has_control('email') == true
	assert win.has_control('username') == true

	win.set_text('email', '')
	errs := win.validate_controls({
		'email':    simplegui.validate_not_empty
		'username': simplegui.validate_not_empty
	})
	assert errs['email'] == 'Required'
	assert errs['username'] == ''
	assert win.get_error('email') == 'Required'
}

fn test_new_control_helpers_and_window_constraints() {
	mut win := simplegui.new_simple_window('Test Window', 100, 100)

	win.dropdown(['Low', 'High'], 'High')
		.segmented(['Simple', 'Advanced'], 'Advanced')
		.radio_group(['Admin', 'User'], 'User')
		.toggle_switch('Enable alerts', true)
		.search_field('Search here')
		.set_resizable(false)
		.set_min_size(320, 240)
		.set_max_size(800, 600)
		.set_minimizable(false)
		.set_maximizable(false)

	assert win.has_control('default_dropdown') == true
	assert win.get_text('default_dropdown') == 'High'
	assert win.has_control('default_segmented') == true
	assert win.get_value_int('default_segmented') == 1
	assert win.has_control('default_radiogroup') == true
	assert win.get_text('default_radiogroup') == 'User'
	assert win.has_control('default_switch') == true
	assert win.get_bool('default_switch') == true
	assert win.has_control('default_search') == true
	assert win.get_text('default_search') == ''
	assert win.get_resizable() == false
	assert win.get_minimizable() == false
	assert win.get_maximizable() == false

	// Test new window operations
	win.set_opacity(0.85)
	assert win.get_opacity() == 0.85

	win.set_size(640, 480)
	assert win.get_width() == 640
	assert win.get_height() == 480

	win.set_position(150, 150)
	assert win.get_x() >= 0
	assert win.get_y() >= 0

	win.set_titlebar_visible(false)
	_ = win.is_minimized()
	_ = win.is_maximized()
	_ = win.is_fullscreen()
	_ = win.is_active()

	win.center()
	win.align('top-left')
	win.align('top-right')
	win.align('bottom-left')
	win.align('bottom-right')
	win.align('center')

	// Test request attention
	win.request_attention(false)
	win.request_attention(true)
	win.bounce_dock(false)

	// Test window focus/blur/minimize/restore events
	win.on_window_focus(fn (mut w simplegui.SimpleWindow) {
		println('focused!')
	})
	win.on_window_blur(fn (mut w simplegui.SimpleWindow) {
		println('blurred!')
	})
	win.on_window_minimize(fn (mut w simplegui.SimpleWindow) {
		println('minimized!')
	})
	win.on_window_restore(fn (mut w simplegui.SimpleWindow) {
		println('restored!')
	})

	win.minimize()
	win.maximize()
	win.toggle_fullscreen()
}

fn test_consistent_nameless_helpers() {
	mut win := simplegui.new_simple_window('Test Window', 100, 100)

	win.input('My Input')
		.textarea('My Textarea')
		.checkbox('My Checkbox', true)
		.number(123)
		.button('My Button')

	assert win.get_input() == 'My Input'
	assert win.get_textarea() == 'My Textarea'
	assert win.get_checkbox() == true
	assert win.get_number() == 123

	win.set_input('Updated Input')
		.set_textarea('Updated Textarea')
		.set_checkbox(false)
		.set_number(456)
		.set_button('Updated Button')

	assert win.get_input() == 'Updated Input'
	assert win.get_textarea() == 'Updated Textarea'
	assert win.get_checkbox() == false
	assert win.get_number() == 456
}

fn test_form_generation_from_struct() {
	mut win := simplegui.new_simple_window('Test Window', 100, 100)
	p := TestProfile{
		username: 'Grace'
		score:    100
		active:   true
	}
	win.add_form_from_struct(p)

	assert win.has_control('username') == true
	assert win.has_control('score') == true
	assert win.has_control('active') == true

	assert win.get_text('username') == 'Grace'
	assert win.get_value_int('score') == 100
	assert win.get_checked('active') == true
}

fn test_reset_form_does_not_clear_buttons_or_labels() {
	mut win := simplegui.new_simple_window('Test Window', 100, 100)
	win.add_button('my_button', 'Button Title')
	win.add_input('my_input', 'Input Value')

	// Modify their values manually to simulate change
	win.set_text('my_button', 'Changed Title')
	win.set_text('my_input', 'Changed Value')

	win.reset_form()
	// Button was not reset because it is not an input control
	assert win.get_text('my_button') == 'Changed Title'
	// Input was reset to its initial value
	assert win.get_text('my_input') == 'Input Value'

	win.clear_all()
	// Button was not cleared because it is not an input control
	assert win.get_text('my_button') == 'Changed Title'
	// Input was cleared
	assert win.get_text('my_input') == ''
}

fn test_table_column_selection_helpers() {
	mut win := simplegui.SimpleWindow{}
	win.add_table('scores', ['ID', 'Name', 'Score'])
	win.set_table_rows('scores', [
		['1', 'Ada', '10'],
		['2', 'Grace', '25'],
		['3', 'Linus', '15'],
	])

	assert win.get_table_column_selection('scores') == false
	win.set_table_column_selection('scores', true)
	assert win.get_table_column_selection('scores') == true

	assert win.get_table_selected_column('scores') == -1
	win.set_table_selected_column('scores', 2)
	assert win.get_table_selected_column('scores') == 2
	assert win.get_table_selected_column_values('scores') == ['10', '25', '15']

	win.set_table_selected_column('scores', -1)
	assert win.get_table_selected_column('scores') == -1
	assert win.get_table_selected_column_values('scores') == []string{}

	win.set_table_selected_column('scores', 1)
	removed_values := win.remove_selected_table_column('scores')
	assert removed_values == ['Ada', 'Grace', 'Linus']
	assert win.get_table_column_count('scores') == 2
	assert win.get_table_rows('scores') == [
		['1', '10'],
		['2', '25'],
		['3', '15'],
	]
	assert win.get_table_selected_column('scores') == -1
}

fn test_grid_cache_count_sort_and_clear_helpers() {
	mut win := simplegui.SimpleWindow{}
	win.add_grid('g', ['A', 'B'], [
		['2', 'Ship'],
		['1', 'Build'],
	])

	assert win.grid_get_row_count('g') == 2
	assert win.grid_get_column_count('g') == 2

	win.grid_add_column('g', 'C')
	assert win.grid_get_column_count('g') == 3

	win.grid_delete_column('g', 2)
	assert win.grid_get_column_count('g') == 2

	win.grid_sort_by_column('g', 0, true)
	assert win.grid_get_row('g', 0) == ['1', 'Build']

	win.grid_clear('g')
	assert win.grid_get_row_count('g') == 0
	assert win.grid_get_rows('g') == [][]string{}
}

fn test_batch_clear_fields_and_validators() {
	mut win := simplegui.SimpleWindow{}
	win.add_input('a', 'hello')
	win.add_input('b', 'world')
	win.set_error('a', 'bad')
	win.clear_fields(['a', 'b'])
	assert win.get_value('a') == ''
	assert win.get_value('b') == ''
	assert win.get_error('a') == ''

	assert simplegui.validate_email('ada@lovelace.dev') == ''
	assert simplegui.validate_email('not-an-email') != ''
	assert simplegui.validate_email('@nope.com') != ''
	assert simplegui.validate_email('user@nodot') != ''

	assert simplegui.validate_number('42') == ''
	assert simplegui.validate_number('3.14') == ''
	assert simplegui.validate_number('abc') != ''
	assert simplegui.validate_number('') != ''

	min3 := simplegui.min_len_validator(3)
	assert min3('ab') != ''
	assert min3('abc') == ''
}

fn test_additional_ergonomics_helpers() {
	mut win := simplegui.SimpleWindow{}
	win.add_input('txt_int', '123')
	win.add_input('txt_float', '12.34')
	win.add_input('txt_empty', '')
	win.add_slider('num_slider', 42)
	win.add_checkbox('chk', 'Checked', true)

	// Test get_int on text input
	assert win.get_int('txt_int') == 123
	assert win.get_int('num_slider') == 42

	// Test get_int_or, get_float_or, get_text_or
	assert win.get_int_or('txt_int', 999) == 123
	assert win.get_int_or('txt_empty', 999) == 999
	assert win.get_int_or('non_existent', 999) == 999
	assert win.get_int_or('num_slider', 999) == 42

	assert win.get_float_or('txt_float', 9.9) == 12.34
	assert win.get_float_or('txt_empty', 9.9) == 9.9
	assert win.get_float_or('non_existent', 9.9) == 9.9

	assert win.get_text_or('txt_int', 'fallback') == '123'
	assert win.get_text_or('txt_empty', 'fallback') == 'fallback'
	assert win.get_text_or('non_existent', 'fallback') == 'fallback'

	// Test clear_errors_for
	win.set_error('txt_int', 'err1')
	win.set_error('txt_float', 'err2')
	win.clear_errors_for(['txt_int'])
	assert win.get_error('txt_int') == ''
	assert win.get_error('txt_float') == 'err2'

	// Test add_list_items
	win.add_list_box('fruits', ['Apple'])
	win.add_list_items('fruits', ['Banana', 'Cherry'])
	assert win.get_list_items('fruits') == ['Apple', 'Banana', 'Cherry']
	assert win.has_list_item('fruits', 'Banana') == true
	assert win.has_list_item('fruits', 'Orange') == false
	assert win.find_list_item('fruits', 'Cherry') == 2

	// Test move_selected_list_item_up / down
	win.set_list_selected('fruits', 1) // select Banana
	win.move_selected_list_item_down('fruits') // Banana down (to index 2)
	assert win.get_list_items('fruits') == ['Apple', 'Cherry', 'Banana']
	assert win.get_list_selected('fruits') == 2

	win.move_selected_list_item_up('fruits') // Banana up (to index 1)
	assert win.get_list_items('fruits') == ['Apple', 'Banana', 'Cherry']
	assert win.get_list_selected('fruits') == 1

	// Test add_table_rows and get_table_column_values
	win.add_table('items', ['Name', 'Price'])
	win.add_table_rows('items', [['Apple', '1'], ['Banana', '2']])
	assert win.get_table_rows('items') == [['Apple', '1'], ['Banana', '2']]
	assert win.get_table_column_values('items', 0) == ['Apple', 'Banana']
	assert win.get_table_column_values('items', 1) == ['1', '2']

	// Test move_selected_table_row_up / down
	win.set_table_selected('items', 0)
	win.move_selected_table_row_down('items')
	assert win.get_table_rows('items') == [['Banana', '2'], ['Apple', '1']]
	assert win.get_table_selected('items') == 1

	win.move_selected_table_row_up('items')
	assert win.get_table_rows('items') == [['Apple', '1'], ['Banana', '2']]
	assert win.get_table_selected('items') == 0

	// Test save_list_to_file and load_list_from_file
	list_path := os.join_path(os.temp_dir(), 'simplegui_list_test.txt')
	win.save_list_to_file('fruits', list_path) or { assert false, err.msg() }
	win.clear_list_items('fruits')
	win.load_list_from_file('fruits', list_path) or { assert false, err.msg() }
	assert win.get_list_items('fruits') == ['Apple', 'Banana', 'Cherry']
	os.rm(list_path) or {}

	// Test confirm_discard_changes (dirty tracking)
	assert win.confirm_discard_changes('Discard?', 'Discard?') == true
	win.set_text('txt_int', 'new_val') // make dirty
	// since win.window_info is nil, dialogs won't show.
	// confirmation will return false because ask calls confirm which returns false on nil info.
	assert win.confirm_discard_changes('Discard?', 'Discard?') == false
	win.commit_changes()
	assert win.confirm_discard_changes('Discard?', 'Discard?') == true

	// Test new validators
	assert simplegui.validate_url('https://google.com') == ''
	assert simplegui.validate_url('invalid-url') != ''
	assert simplegui.validate_alphanumeric('abc123') == ''
	assert simplegui.validate_alphanumeric('abc-123') != ''
	max5 := simplegui.max_len_validator(5)
	assert max5('abcde') == ''
	assert max5('abcdef') != ''

	// Test settings persistence with checkboxes and numbers
	settings_path := os.join_path(os.temp_dir(), 'simplegui_persist_test.json')
	win.save_values_to_file(settings_path) or { assert false, err.msg() }

	// Reset values
	win.set_checked('chk', false)
	win.set_value_int('num_slider', 0)
	win.set_text('txt_int', '')

	// Load values
	win.load_values_from_file(settings_path) or { assert false, err.msg() }
	assert win.get_checked('chk') == true
	assert win.get_value_int('num_slider') == 42
	assert win.get_text('txt_int') == 'new_val'
	os.rm(settings_path) or {}
}

fn test_new_ergonomic_features() {
	mut win := simplegui.new_simple_window('Ergonomic Test', 100, 100)

	// 1. Labeled form helpers / aliases
	win.add_form_password('Password:', 'pwd', 's3cr3t')
	win.add_form_slider('Slider:', 'sld', 45)
	win.add_form_number('Num:', 'nbr', 7)
	win.add_form_dropdown('Quality:', 'quality', ['Low', 'High'], 'High')
	win.add_form_date_picker('Date:', 'dt', '2026-07-18')
	win.add_form_progress('Prog:', 'prg', 20)
	win.add_form_switch('Enable:', 'swh', 'Notifications', true)

	assert win.has_control('pwd') == true
	assert win.has_control('sld') == true
	assert win.has_control('nbr') == true
	assert win.has_control('quality') == true
	assert win.has_control('dt') == true
	assert win.has_control('prg') == true
	assert win.has_control('swh') == true

	// 2. Generic set/get_as
	win.set('pwd', 'new_password')
	win.set('sld', '80')
	win.set('swh', 'false')
	win.set('nbr', '42')

	assert win.get_as[string]('pwd') == 'new_password'
	assert win.get_as[int]('sld') == 80
	assert win.get_as[bool]('swh') == false
	assert win.get_as[int]('nbr') == 42

	// 3. Compile-time struct validation
	win.add_input('name', 'Ad') // invalid (<3 characters)
	win.add_input('email', 'not-an-email') // invalid
	win.add_number('age', 16) // invalid (<18)

	assert win.validate_struct[TestValidationStruct]() == false
	assert win.get_error('name') != ''
	assert win.get_error('email') != ''
	assert win.get_error('age') != ''

	win.set('name', 'Ada')
	win.set('email', 'ada@example.com')
	win.set('age', '28')

	assert win.validate_struct[TestValidationStruct]() == true
	assert win.get_error('name') == ''
	assert win.get_error('email') == ''
	assert win.get_error('age') == ''

	// 4. Table querying and diagnostics
	win.add_table('scores', ['Name', 'Points'])
	win.add_table_rows('scores', [
		['Ada', '10'],
		['Grace', '25'],
		['Linus', '25'],
	])

	assert win.get_table_row_where('scores', 0, 'Grace') == ['Grace', '25']
	assert win.get_table_row_where('scores', 0, 'Nobody') == []string{}
	assert win.get_table_rows_where('scores', 1, '25') == [['Grace', '25'], ['Linus', '25']]
	assert win.get_table_column_sum('scores', 1) == 60.0
	assert win.get_table_column_average('scores', 1) == 20.0

	// 5. JSON persistence for lists & tables
	list_json_path := os.join_path(os.temp_dir(), 'test_list.json')
	table_json_path := os.join_path(os.temp_dir(), 'test_table.json')

	win.add_list_box('fruits', ['Apple', 'Banana', 'Cherry'])
	win.save_list_to_json('fruits', list_json_path) or { assert false, err.msg() }
	win.clear_list_items('fruits')
	win.load_list_from_json('fruits', list_json_path) or { assert false, err.msg() }
	assert win.get_list_items('fruits') == ['Apple', 'Banana', 'Cherry']

	win.save_table_to_json('scores', table_json_path) or { assert false, err.msg() }
	win.clear_table('scores')
	win.load_table_from_json('scores', table_json_path) or { assert false, err.msg() }
	assert win.get_table_rows('scores') == [['Ada', '10'], ['Grace', '25'], ['Linus', '25']]

	os.rm(list_json_path) or {}
	os.rm(table_json_path) or {}

	// 6. Async task runner
	mut async_called := false
	win.run_async(fn () {
		// background task
	}, fn [mut async_called] (mut w simplegui.SimpleWindow) {
		async_called = true
	})
}

fn test_animation_helpers() {
	mut win := simplegui.SimpleWindow{}
	win.add_input('test_input', 'value')

	// Test opacity methods (can be chained)

	win.animate_opacity(0.5, 500)
		.animate_control_opacity('test_input', 0.8, 400)
		.fade_in('test_input', 300)
		.fade_out('test_input', 300)
		.fade_in_window(300)
		.fade_out_window(300)

	// Test shake methods

	win.shake('test_input')
		.shake_window()

	// Test size methods

	win.animate_width('test_input', 120, 300)
		.animate_height('test_input', 40, 300)
		.animate_size('test_input', 150, 45, 300)

	// Test window size/position methods

	win.animate_window_size(800, 600, 300)
		.animate_window_position(100, 100, 300)
		.animate_window_bounds(100, 100, 800, 600, 300)

	assert true
}

fn test_new_ergonomic_helpers_added() {
	mut win := simplegui.new_simple_window('New Ergonomic Helpers Test', 200, 200)

	// 1. Global Reset & Cleanup Helpers
	win.add_input('txt_input', 'initial text')
	win.add_checkbox('chk_input', 'Check me', true)
	win.add_spinner('loading_spinner', true)
	win.add_progress_indicator('prog_bar', 50)
	win.set_error('txt_input', 'Validation error')

	assert win.get_error('txt_input') == 'Validation error'
	win.clear_all_errors()
	assert win.get_error('txt_input') == ''

	assert win.get_text('txt_input') == 'initial text'
	assert win.get_bool('chk_input') == true
	assert win.get_bool('loading_spinner') == true
	assert win.get_progress('prog_bar') == 50

	win.clear_all_fields()
	assert win.get_text('txt_input') == ''
	assert win.get_bool('chk_input') == false
	assert win.get_bool('loading_spinner') == false
	assert win.get_progress('prog_bar') == 0

	win.reset_all_fields()
	assert win.get_text('txt_input') == 'initial text'
	assert win.get_bool('chk_input') == true
	assert win.get_bool('loading_spinner') == true
	assert win.get_progress('prog_bar') == 50

	// 2. Validation Additions
	assert simplegui.validate_ip('192.168.1.1') == ''
	assert simplegui.validate_ip('256.0.0.1') != ''
	assert simplegui.validate_ip('192.168.1') != ''
	assert simplegui.validate_ip('abc.def.ghi.jkl') != ''

	assert simplegui.validate_phone('+1 (555) 123-4567') == ''
	assert simplegui.validate_phone('123') != ''
	assert simplegui.validate_phone('123-abc-4567') != ''

	range_val := simplegui.range_validator(10.0, 50.0)
	assert range_val('25') == ''
	assert range_val('5') != ''
	assert range_val('60') != ''
	assert range_val('not-a-number') != ''

	// 3. Token Field Ergonomics
	win.add_token_field('tags', 'one, two')
	assert win.get_tokens('tags') == ['one', 'two']

	win.set_tokens('tags', ['alpha', 'beta', 'gamma'])
	assert win.get_tokens('tags') == ['alpha', 'beta', 'gamma']

	win.add_token('tags', 'delta')
	assert win.get_tokens('tags') == ['alpha', 'beta', 'gamma', 'delta']
	win.add_token('tags', 'beta') // duplicate
	assert win.get_tokens('tags') == ['alpha', 'beta', 'gamma', 'delta']

	win.remove_token('tags', 'gamma')
	assert win.get_tokens('tags') == ['alpha', 'beta', 'delta']

	// 4. Advanced Table Mapping & Filtering
	win.add_table('users', ['Username', 'Role'])
	win.add_table_rows('users', [
		['alice', 'admin'],
		['bob', 'user'],
		['charlie', 'user'],
	])

	assert win.has_table_row('users', 0, 'bob') == true
	assert win.has_table_row('users', 0, 'david') == false

	assert win.find_table_row_where('users', fn (row []string) bool {
		return row[1] == 'admin'
	}) == 0

	users_filtered := win.filter_table_rows('users', fn (row []string) bool {
		return row[1] == 'user'
	})
	assert users_filtered.len == 2
	assert users_filtered[0][0] == 'bob'
	assert users_filtered[1][0] == 'charlie'

	win.map_table_column('users', 0, fn (val string) string {
		return val.to_upper()
	})
	assert win.get_table_cell('users', 0, 0) == 'ALICE'
	assert win.get_table_cell('users', 1, 0) == 'BOB'

	// 5. List Box Row Insertion & Safely Selected Text
	win.add_list_box('tasks', ['Task A', 'Task B'])
	assert win.get_list_selected_text_or('tasks', 'No Task') == 'No Task'

	win.insert_list_item('tasks', 1, 'Task Intermediary')
	assert win.get_list_items('tasks') == ['Task A', 'Task Intermediary', 'Task B']

	win.update_list_item('tasks', 0, 'Task Alpha')
	assert win.get_list_items('tasks') == ['Task Alpha', 'Task Intermediary', 'Task B']

	// 6. Widget QoL Helpers
	assert win.get_bool('loading_spinner') == true
	assert win.toggle_spinner('loading_spinner') == false
	assert win.get_bool('loading_spinner') == false
	win.start_spinner('loading_spinner')
	assert win.get_bool('loading_spinner') == true
	win.stop_spinner('loading_spinner')
	assert win.get_bool('loading_spinner') == false

	assert win.get_progress('prog_bar') == 50
	assert win.increment_progress('prog_bar', 10) == 60
	assert win.get_progress('prog_bar') == 60
	assert win.increment_progress('prog_bar', -70) == 0
}

fn test_developer_inspection_controls() {
	mut win := simplegui.new_simple_window('Developer Inspection Controls Test', 800,
		600)

	// 1. Diff View
	win.add_diff_view('diff_1', 'line 1\nline 2', 'line 1\nline 2 updated', 120)
	assert win.has_control('diff_1') == true
	win.set_diff_view('diff_1', 'old', 'new')

	// 2. JSON Tree
	win.add_json_tree('json_1', '{"key": "value"}', 100)
	assert win.has_control('json_1') == true
	win.set_json_tree('json_1', '{"updated": true}')

	// 3. HTTP Request Card
	win.add_http_request_card('http_1', 'GET', 'https://api.example.com', 200, 45)
	assert win.has_control('http_1') == true
	win.set_http_request_card('http_1', 'POST', 'https://api.example.com/v2', 201, 80)

	// 4. Terminal View
	win.add_terminal_view('term_1', '$ echo hello', 100)
	assert win.has_control('term_1') == true
	win.append_terminal_line('term_1', 'hello', 0)
	win.clear_terminal('term_1')

	// 5. Resource Monitor
	win.add_resource_monitor('res_1', 25, 50, 10, 1024)
	assert win.has_control('res_1') == true
	win.set_resource_monitor('res_1', 80, 90, 40, 2048)

	// 6. Env Vars
	win.add_env_vars('env_1', 'App Config', ['PORT', 'ENV'], ['8080', 'prod'])
	assert win.has_control('env_1') == true
	win.set_env_vars('env_1', ['PORT'], ['9090'])
}

fn test_reactive_bindings_and_data_qol() {
	mut win := simplegui.SimpleWindow{}

	// List QoL: dedupe / reverse / keep / map
	win.add_list_box('lst', ['b', 'a', 'b', 'c', 'a'])
	win.dedupe_list_items('lst')
	assert win.get_list_items('lst') == ['b', 'a', 'c']
	win.reverse_list_items('lst')
	assert win.get_list_items('lst') == ['c', 'a', 'b']
	win.keep_list_items('lst', fn (item string) bool {
		return item != 'a'
	})
	assert win.get_list_items('lst') == ['c', 'b']
	win.map_list_items('lst', fn (item string) string {
		return item.to_upper()
	})
	assert win.get_list_items('lst') == ['C', 'B']

	// Table QoL: dedupe / count_where
	win.add_table('tbl', ['Name', 'Role'])
	win.set_table_rows('tbl', [
		['Ada', 'Engineer'],
		['Ada', 'Engineer'],
		['Grace', 'Admiral'],
		['Alan', 'Engineer'],
	])
	win.dedupe_table_rows('tbl')
	assert win.get_table_row_count('tbl') == 3
	engineers := win.count_table_rows_where('tbl', fn (row []string) bool {
		return row[1] == 'Engineer'
	})
	assert engineers == 2

	// Validators: required / one_of / chain
	required := simplegui.required_validator()
	assert required('') != ''
	assert required('   ') != ''
	assert required('x') == ''

	role := simplegui.one_of_validator(['dev', 'admin'])
	assert role('dev') == ''
	assert role('ADMIN') == ''
	assert role('guest') != ''

	chained := simplegui.chain_validators(simplegui.required_validator(), simplegui.one_of_validator([
		'dev',
	]))
	assert chained('') != ''
	assert chained('ops') != ''
	assert chained('dev') == ''

	// bind_value_to_label mirrors changes (dispatch_event fires on set_bool/set_text)
	win.add_slider('vol', 40)
	win.add_label('vol_lbl', '')
	win.bind_value_to_label('vol', 'vol_lbl', 'Volume: ', '%')
	assert win.get_text('vol_lbl') == 'Volume: 40%'

	// bind_checkbox_enables applies the initial checkbox state
	win.add_checkbox('gate', 'Gate', false)
	win.add_input('gated_input', 'x')
	win.bind_checkbox_enables('gate', ['gated_input'])
	assert win.get_control_enabled('gated_input') == false

	// bind_char_counter seeds the counter label immediately
	win.add_input('bio', 'hello')
	win.add_label('bio_count', '')
	win.bind_char_counter('bio', 'bio_count', 20)
	assert win.get_text('bio_count') == '5/20'
}

fn test_ergonomic_window_apis() {
	mut win := simplegui.new_simple_window('Window Test', 640, 480)

	// Fixed size and sizing presets
	win.set_fixed_size(500, 400)
	w, h := win.get_size()
	assert w == 500
	assert h == 400
	assert win.get_resizable() == false

	win.set_size_preset('hd')
	w_hd, h_hd := win.get_size()
	assert w_hd == 1280
	assert h_hd == 720

	win.set_preset_size('dialog')
	w_dlg, h_dlg := win.get_size()
	assert w_dlg == 420
	assert h_dlg == 220

	// Min/max size aliases
	win.set_minimum_size(300, 200)
	min_w, min_h := win.get_minimum_size()
	assert min_w == 300
	assert min_h == 200

	win.set_maximum_size(1000, 800)
	max_w, max_h := win.get_maximum_size()
	assert max_w == 1000
	assert max_h == 800

	// Position and corner alignment presets
	win.set_position_preset('top-left')
	win.set_corner_position('bottom-right')
	win.recenter()
	win.center_on_screen()

	// Title, topmost, frameless aliases
	win.set_window_title('New Title')
	assert win.get_title() == 'New Title'

	win.set_topmost(true)
	assert win.is_topmost() == true
	win.set_topmost(false)
	assert win.is_topmost() == false

	win.make_frameless()
	assert win.is_frameless() == true

	// Theme shortcuts
	win.set_dark_theme(true)
	assert win.is_dark_theme() == true
	win.set_dark_theme(false)
	assert win.is_dark_theme() == false
	win.toggle_window_theme()
	assert win.is_dark_theme() == true

	// Visibility, restore, attention & shake shortcuts
	win.show()
	win.restore()
	win.restore_window()
	win.trigger_shake()
	win.flash_and_shake()
	win.attention()

	// Window Archetypes
	mut dlg := simplegui.new_simple_window('Old Title', 100, 100)
	dlg.make_fixed_dialog('Dialog Window', 400, 250)
	assert dlg.get_title() == 'Dialog Window'
	dlg_w, dlg_h := dlg.get_size()
	assert dlg_w == 400
	assert dlg_h == 250
	assert dlg.get_resizable() == false
	assert dlg.get_minimizable() == false
	assert dlg.get_maximizable() == false

	mut splash := simplegui.new_simple_window('Splash', 100, 100)
	splash.make_splash_screen(500, 300)
	assert splash.is_frameless() == true
	assert splash.is_topmost() == true
	spl_w, spl_h := splash.get_size()
	assert spl_w == 500
	assert spl_h == 300

	mut panel := simplegui.new_simple_window('Panel', 300, 400)
	panel.make_utility_panel()
	assert panel.is_topmost() == true

	// Section 27: Advanced Validation & Utility Shortcuts
	mut form_win := simplegui.new_simple_window('Form Test', 600, 400)
	form_win.add_input('f_name', '  ada  ')
	form_win.add_input('f_email', '')

	valid, missing := form_win.validate_required(['f_name', 'f_email'])
	assert valid == false
	assert missing == 'f_email'

	form_win.set_value('f_email', 'ada@test.com')
	valid_ok, _ := form_win.validate_required(['f_name', 'f_email'])
	assert valid_ok == true

	form_win.trim_all(['f_name'])
	assert form_win.get_value('f_name') == 'ada'

	form_win.uppercase_all(['f_name'])
	assert form_win.get_value('f_name') == 'ADA'

	form_win.lowercase_all(['f_name'])
	assert form_win.get_value('f_name') == 'ada'

	form_win.clear_form()
	assert form_win.get_value('f_name') == ''
	assert form_win.get_value('f_email') == ''

	form_win.toast_info('Info Test')
	form_win.toast_success('Success Test')
	form_win.toast_warn('Warn Test')
	form_win.toast_error('Error Test')
	form_win.set_status_temporary('Temp Status', 500)

	form_win.save_layout('unit_test_app')
	form_win.restore_layout('unit_test_app')
}

fn test_recommended_storage_and_app_state() {
	mut win := simplegui.new_simple_window('Storage Studio Test App', 500, 400)

	// Test app name deduction
	app_name := win.get_app_name()
	assert app_name.len > 0
	assert app_name == 'Storage_Studio_Test_App'

	// Test storage and cache directories
	storage_dir := win.get_app_storage_dir()
	assert storage_dir.len > 0
	assert storage_dir.contains('Storage_Studio_Test_App')
	assert os.exists(storage_dir) == true

	cache_dir := win.get_app_cache_dir()
	assert cache_dir.len > 0
	assert os.exists(cache_dir) == true

	// Test storage path resolution
	state_path := win.get_app_storage_path('test_state.json')
	assert state_path.starts_with(storage_dir)
	assert state_path.ends_with('test_state.json')

	resolved_rel := win.resolve_storage_path('relative_data.json')
	assert resolved_rel.starts_with(storage_dir)

	// Test form state persistence to recommended store location
	win.add_input('txt_user', 'AntigravityUser')
	win.add_checkbox('chk_notifications', 'Enable Notifications', true)
	win.add_slider('sld_volume', 85)

	// Save app state
	win.save_app_state('test_preset') or {
		assert false, 'save_app_state failed: ${err}'
	}
	assert win.has_saved_state('test_preset') == true

	// Clear controls
	win.set_text('txt_user', 'ClearedUser')
	win.set_checked('chk_notifications', false)
	win.set_value_int('sld_volume', 10)

	// Restore app state
	loaded := win.load_app_state('test_preset')
	assert loaded == true
	assert win.get_text('txt_user') == 'AntigravityUser'
	assert win.is_checked('chk_notifications') == true
	assert win.get_value_int('sld_volume') == 85

	// Test relative path save_values_to_file / load_values_from_file
	win.save_values_to_file('rel_save_test.json') or {
		assert false, 'save_values_to_file relative path failed: ${err}'
	}
	win.set_text('txt_user', 'AnotherUser')
	win.load_values_from_file('rel_save_test.json') or {
		assert false, 'load_values_from_file relative path failed: ${err}'
	}
	assert win.get_text('txt_user') == 'AntigravityUser'

	// Test clear_app_state
	cleared := win.clear_app_state('test_preset')
	assert cleared == true
	assert win.has_saved_state('test_preset') == false

	// Clean up relative test file
	rel_path := win.get_app_storage_path('rel_save_test.json')
	if os.exists(rel_path) {
		os.rm(rel_path) or {}
	}
}
