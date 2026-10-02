module main

import simplegui

fn main() {
	mut win := simplegui.new_simple_window('Native macOS Controls Showcase', 820, 680)

	win.set_padding(20)
		.set_spacing(14)
		.set_title('macOS Native Controls & System Panels Demo')

	win.add_heading('Native macOS AppKit Controls & System Integration')

	win.add_label('lbl_desc', 'This demo showcases native macOS panels: NSBrowser (Column Browser), NSSharingServicePicker (Share Sheet), NSFontPanel (Font Picker), and Quick Look preview.')
		.font_size(12)
		.font_color('#8e8e93')

	// --- 1. NSBrowser: Miller Columns Multi-Column Browser ---
	win.add_section_header('sec_browser', '1. NSBrowser: Multi-Column Cascading Browser', 'Miller Columns navigation like Finder Column View')

	win.add_browser_view('finder_browser', 180)

	// Populate column 0 (Root categories)
	win.set_browser_column_items('finder_browser', 0, [
		'Documents',
		'Downloads',
		'Applications',
		'Source Code',
		'Pictures',
	])

	// Populate column 1 (Sub-directories)
	win.set_browser_column_items('finder_browser', 1, [
		'SimpleGUI Project',
		'V Language Core',
		'Design Assets',
		'Reports & Notes',
	])

	// Populate column 2 (Files)
	win.set_browser_column_items('finder_browser', 2, [
		'main.v',
		'controls.v',
		'window.m',
		'API.md',
		'README.md',
	])

	// --- 2. System Share Sheet & Font Panel Actions ---
	win.add_section_header('sec_panels', '2. System Share Sheet & Font Picker Panels', 'Direct integration with macOS System Services')

	win.begin_row('row_actions')

	win.add_button('btn_share_url', '🔗 Share SimpleGUI Repo')
		.tooltip('Open macOS Share Sheet for URL')
		.onclick(fn (mut w simplegui.SimpleWindow) {
			w.show_share_sheet(['https://github.com/codecaine/vlang_simplegui',
				'Build native macOS desktop apps with V and SimpleGUI!'], 'btn_share_url')
			w.toast_info('Opened macOS System Share Sheet')
		})

	win.add_button('btn_font_panel', '🔤 Open Font Picker Panel')
		.tooltip('Launch macOS NSFontPanel')
		.onclick(fn (mut w simplegui.SimpleWindow) {
			w.show_font_picker('target_text')
			w.toast_success('Launched macOS System Font Panel')
		})

	win.add_button('btn_quick_look', '👁️ Quick Look File Preview')
		.tooltip('Launch Quick Look / File Viewer')
		.onclick(fn (mut w simplegui.SimpleWindow) {
			w.preview_file('docs/API.md')
			w.toast_info('Triggered native file preview')
		})

	win.end_row()

	// --- 3. Preview text for font modifications ---
	win.add_section_header('sec_preview', '3. Interactive Text Preview Surface', 'Observe live typography changes')
	win.add_textarea('target_text', 'The quick brown fox jumps over the lazy dog.\n\nSimpleGUI provides seamless 100% native macOS AppKit integration.')
		.height(90)
		.font_size(14)

	// Footer Status Dock
	win.add_status_dock('dock_info', 'Native macOS AppKit Controls Active', '#34c759', 'Ready')

	win.run()
}
