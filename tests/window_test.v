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

fn test_status_updates_create_a_status_control() {
	mut win := simplegui.SimpleWindow{}
	win.set_status('Saving...')

	assert win.get_status() == 'Saving...'
	assert win.has_control('status') == true
	assert win.get_control_kind('status') == 'label'
}

fn test_window_always_on_top_state_is_stored() {
	mut win := simplegui.SimpleWindow{}

	assert win.get_always_on_top() == false
	win.set_always_on_top(true)
	assert win.get_always_on_top() == true
	win.set_always_on_top(false)
	assert win.get_always_on_top() == false
}

fn test_new_window_controls() {
	win := simplegui.new_simple_window('Test Window Controls', 400, 300)

	// Test default values
	assert win.get_closable() == true
	assert win.get_has_shadow() == true
	assert win.get_movable_by_window_background() == false
	assert win.is_visible() == false
	assert win.is_titlebar_visible() == true
	assert win.is_title_visible() == true

	// Test mutators
	win.set_closable(false)
	assert win.get_closable() == false

	win.set_has_shadow(false)
	assert win.get_has_shadow() == false

	win.set_movable_by_window_background(true)
	assert win.get_movable_by_window_background() == true

	win.set_title_visible(false)
	assert win.is_title_visible() == false

	// Test configuration block
	win.configure(fn (mut cfg simplegui.WindowConfig) {
		cfg.closable = true
		cfg.has_shadow = true
		cfg.movable_by_window_background = false
	})
	assert win.get_closable() == true
	assert win.get_has_shadow() == true
	assert win.get_movable_by_window_background() == false
}

fn test_new_window_commands_and_controls() {
	mut win := simplegui.SimpleWindow{}

	// Window commands
	win.set_subtitle('Workspace Config')
	win.set_titlebar_appears_transparent(true)
	win.set_full_size_content_view(true)
	win.set_movable(true)
	win.set_window_level('normal')
	win.set_aspect_ratio(16.0, 9.0)
	win.reset_aspect_ratio()
	win.bounce_dock_icon(false)

	// Rating Control
	win.add_star_rating('app_rating', 4, 5)
	assert win.has_control('app_rating') == true
	assert win.get_control_kind('app_rating') == 'rating'
	win.set_star_rating_value('app_rating', 5)

	// Range Slider Control
	win.add_range_slider('price_range', 0, 1000, 200, 800)
	assert win.has_control('price_range') == true
	assert win.get_control_kind('price_range') == 'range_slider'

	// Split Button Control
	win.add_split_button('action_btn', 'Deploy', ['Deploy to Staging', 'Deploy to Prod'])
	assert win.has_control('action_btn') == true
	assert win.get_control_kind('action_btn') == 'split_button'

	// Tag Cloud Control
	win.add_tag_cloud('user_tags', ['vlang', 'gui', 'macos', 'cocoa'])
	assert win.has_control('user_tags') == true
	assert win.get_control_kind('user_tags') == 'tag_cloud'

	// Wizard Stepper Control
	win.add_wizard_stepper('checkout_flow', ['Cart', 'Shipping', 'Payment', 'Review'],
		1)
	assert win.has_control('checkout_flow') == true
	assert win.get_control_kind('checkout_flow') == 'wizard_stepper'
}

fn test_new_useful_window_controls() {
	mut win := simplegui.new_simple_window('New Useful Controls Test', 800, 600)

	// 1. Quick Action Bar
	win.add_quick_action_bar('quick_bar', ['Refresh', 'Export', 'Settings'], ['🔄', '📤', '⚙️'])
	assert win.has_control('quick_bar') == true
	win.set_quick_action_enabled('quick_bar', 0, false)

	// 2. Accordion Group
	win.add_accordion_group('accordion_1', ['General Settings', 'Security & Privacy', 'Notifications'],
		0)
	assert win.has_control('accordion_1') == true
	win.set_accordion_expanded('accordion_1', 1, true)

	// 3. Segment Distribution Bar
	win.add_segment_distribution_bar('storage_bar', ['System', 'Apps', 'Documents', 'Free'],
		[40.0, 30.0, 15.0, 15.0], ['#007aff', '#34c759', '#ff9500', '#8e8e93'], 16)
	assert win.has_control('storage_bar') == true
	win.set_segment_distribution_values('storage_bar', [50.0, 25.0, 15.0, 10.0])

	// 4. Tag Input Field
	win.add_tag_input_field('tag_input', ['vlang', 'simplegui', 'macos'])
	assert win.has_control('tag_input') == true
	win.set_tag_input_tags('tag_input', ['vlang', 'simplegui', 'native', 'gui'])
	assert win.get_tag_input_tags('tag_input') == 'vlang,simplegui,native,gui'

	// 5. Status Dock
	win.add_status_dock('dock_footer', 'Connected to Server', '#34c759', '142 Records')
	assert win.has_control('dock_footer') == true
	win.set_status_dock_info('dock_footer', 'Syncing Data...', '#ff9500', '145 Records')

	// 6. Info Callout Card
	win.add_info_callout('callout_card', 'Update Available', 'SimpleGUI v1.5 is ready to install.',
		'info', 'Install Now')
	assert win.has_control('callout_card') == true
	win.set_info_callout_text('callout_card', 'Critical Update', 'Version v1.5 includes security improvements.')
}

fn test_new_window_management_commands() {
	mut win := simplegui.new_simple_window('Window Commands Test', 600, 400)
	win.set_vibrancy('sidebar')
	win.set_corner_radius(12.0)
	win.set_background_blur(true)
	win.flash_frame(false)
	win.center_on_active_screen()
	win.set_level_type('normal')
}

fn test_comprehensive_window_control_apis() {
	mut win := simplegui.new_simple_window('Comprehensive Window Control Test', 800, 500)

	// State & Subtitle & Transparency
	win.set_subtitle('v1.0.0 Release')
	assert win.get_subtitle() == 'v1.0.0 Release'

	win.set_titlebar_appears_transparent(true)
	assert win.get_titlebar_appears_transparent() == true

	win.set_full_size_content_view(true)
	assert win.get_full_size_content_view() == true

	// Vibrancy, Corner Radius & Blur
	win.set_vibrancy('hud')
	win.set_corner_radius(16.0)
	assert win.get_corner_radius() == 16.0
	win.set_background_blur(true)

	// Window Level
	win.set_window_level('floating')
	assert win.get_window_level() == 'floating'
	win.set_level_type('normal')

	// Screen Bounds & Alignment & Edge Snapping
	win.center_on_active_screen()
	win.snap_to_edge('top_left')
	win.set_bounds(100, 100, 900, 600)
	x, y, w, h := win.get_bounds()
	assert w == 900
	assert h == 600

	// Aspect Ratio
	win.set_aspect_ratio(16.0, 9.0)
	win.reset_aspect_ratio()
	assert win.has_aspect_ratio() == false

	// Behavior Flags
	win.set_movable(false)
	assert win.get_movable() == false
	win.set_movable(true)
	assert win.get_movable() == true

	win.set_ignores_mouse_events(true)
	assert win.get_ignores_mouse_events() == true
	win.set_ignores_mouse_events(false)

	win.set_hides_on_deactivate(true)
	assert win.get_hides_on_deactivate() == true

	win.set_prevents_app_termination(false)
	assert win.get_prevents_app_termination() == false

	// Document Integration
	win.set_represented_filename('/tmp/doc.txt')
	assert win.get_represented_filename() == '/tmp/doc.txt'

	win.set_document_edited(true)
	assert win.is_document_edited() == true
	win.set_document_edited(false)
	assert win.is_document_edited() == false

	// Opacity, Min/Max Size, Shadow & Title Visibility
	win.set_alpha(0.85)
	assert win.get_alpha() == 0.85
	win.set_alpha(1.0)

	win.set_min_size(500, 300)
	mw, mh := win.get_min_size()
	assert mw == 500
	assert mh == 300

	win.set_max_size(1200, 800)
	max_w, max_h := win.get_max_size()
	assert max_w == 1200
	assert max_h == 800

	win.set_has_shadow(false)
	assert win.get_has_shadow() == false
	win.set_has_shadow(true)
	assert win.get_has_shadow() == true

	win.set_title_visible(false)
	assert win.get_title_visible() == false
	win.set_title_visible(true)
	assert win.get_title_visible() == true

	win.set_collection_behavior('can_join_all_spaces')
	win.set_close_button_enabled(true)
	win.set_minimize_button_enabled(true)
	win.set_zoom_button_enabled(true)
	win.shake_window()

	// Animation & Attention
	win.flash_frame(true)
	win.bounce_dock_icon(true)
	win.fade_out_window(10)
	win.fade_in_window(10)
	win.bring_to_front()
	win.send_to_back()

	// Toolbar Style & Insets & Close Interception
	win.set_toolbar_style('unified')
	win.set_content_insets(10, 10, 10, 10)
	win.on_close(fn (mut w simplegui.SimpleWindow) {})

	// Ergonomic Shortcuts
	win.make_frameless()
	win.make_vibrant('hud')
	win.make_click_through(false)
	win.make_always_on_top(true)
	win.make_modal()
	win.make_panel()
	win.make_translucent(0.9)
	win.make_sticky_space()
	win.shake_on_error()
	win.center_and_focus()
}

fn test_new_window_control_apis() {
	mut win := simplegui.new_simple_window('New APIs Test', 600, 400)

	// ── Appearance Override ────────────────────────────────────────────
	win.set_window_appearance('dark')
	assert win.get_window_appearance() == 'dark'
	win.set_window_appearance('light')
	assert win.get_window_appearance() == 'light'
	win.set_window_appearance('auto')
	assert win.get_window_appearance() == 'auto'

	// is_system_dark_mode can be true or false, just ensure no panic
	_ := win.is_system_dark_mode()

	// ── Screen Info ───────────────────────────────────────────────────
	sx, sy, sw, sh := win.get_screen_frame()
	// Screen frame should be at least 100x100 on any real display
	assert sw >= 100 || (sx == 0 && sy == 0 && sw == 0 && sh == 0) // 0,0 before window is created

	fx, fy, fw, fh := win.get_screen_full_frame()
	// Accept zero (before window shown) or reasonable values
	assert fw >= 0
	_ = fx
	_ = fy
	_ = fh

	scale := win.get_screen_scale_factor()
	assert scale >= 1.0

	// ── Cursor Control ────────────────────────────────────────────────
	// Just ensure calls don't panic; cursor will be restored
	win.set_cursor_hidden(true)
	win.set_cursor_hidden(false)

	// Cursor icon & size
	win.set_cursor('crosshair')
	assert win.get_cursor() == 'crosshair'
	win.set_cursor_size(2.0)
	assert win.get_cursor_size() == 2.0
	win.reset_cursor()
	assert win.get_cursor() == 'arrow'
	assert win.get_cursor_size() == 1.0
	win.push_cursor('closed_hand')
	win.pop_cursor()
	win.add_label('cursor_lbl', 'hover target')
	win.set_control_cursor('cursor_lbl', 'pointing_hand')
	win.set_control_cursor('cursor_lbl', 'default')
	mx, my := win.get_mouse_location()
	_ = mx
	_ = my

	// ── Resize Indicator ─────────────────────────────────────────────
	// Note: showsResizeIndicator is deprecated in macOS; just verify calls don't panic
	win.set_shows_resize_indicator(false)
	_ := win.get_shows_resize_indicator() // value may be unreliable (deprecated API)
	win.set_shows_resize_indicator(true)
	_ = win.get_shows_resize_indicator()

	// ── Content Size Constraints ──────────────────────────────────────
	win.set_content_min_size(300, 200)
	cmin_w, cmin_h := win.get_content_min_size()
	assert cmin_w == 300
	assert cmin_h == 200

	win.set_content_max_size(1200, 900)
	cmax_w, cmax_h := win.get_content_max_size()
	assert cmax_w == 1200
	assert cmax_h == 900

	// 0 means unconstrained
	win.set_content_max_size(0, 0)
	ux, uy := win.get_content_max_size()
	assert ux == 0
	assert uy == 0

	// ── Tabbing APIs ──────────────────────────────────────────────────
	win.set_tabbing_mode('preferred')
	assert win.get_tabbing_mode() == 'preferred'
	win.set_tabbing_mode('disallowed')
	assert win.get_tabbing_mode() == 'disallowed'
	win.set_tabbing_mode('automatic')
	assert win.get_tabbing_mode() == 'automatic'

	win.set_tabbing_identifier('com.test.tabgroup')
	assert win.get_tabbing_identifier() == 'com.test.tabgroup'

	win.toggle_tab_bar()
	win.select_next_tab()
	win.select_previous_tab()

	// ── Sharing Type ─────────────────────────────────────────────────
	win.set_sharing_type('none')
	win.set_sharing_type('read_only')
	win.set_sharing_type('read_write')

	// ── Tab Count ────────────────────────────────────────────────────
	count := win.get_tab_count()
	assert count >= 1

	// ── Window Movability ─────────────────────────────────────────────
	win.set_movable(false)
	assert win.get_movable() == false
	assert win.is_movable() == false
	win.set_movable(true)
	assert win.get_movable() == true
	assert win.is_movable() == true
}
