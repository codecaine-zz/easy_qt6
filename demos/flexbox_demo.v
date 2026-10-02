module main

import simplegui

fn main() {
	mut win := simplegui.new_simple_window('Flexbox Examples Showcase', 750, 680)
		.set_padding(20)

	win.add_heading('SimpleGUI Flexbox Layout Examples')
	win.add_label('lbl_desc', 'Flexbox containers support direction (row/column), justify (main-axis distribution), and align (cross-axis alignment).')
	win.add_vertical_spacer(10)

	// Example 1: Action Toolbar Header (Row + Space-Between + Center Align)
	win.group('grp_ex1', '1. Navigation Header (row, space_between, center)', fn (mut w simplegui.SimpleWindow) {
		w.flex_box('flex_header', 'row', 'space_between', 'center', fn (mut f simplegui.SimpleWindow) {
			f.add_badge('badge_brand', '⚡ AppLogo', 'info')
			f.add_button('btn_nav_home', 'Home')
			f.add_button('btn_nav_docs', 'Docs')
			f.add_button('btn_nav_profile', 'Profile')
		})
	})

	win.add_vertical_spacer(10)

	// Example 2: Centered Hero / Status Card (Row + Center + Center)
	win.group('grp_ex2', '2. Centered Status Bar (row, center, center)', fn (mut w simplegui.SimpleWindow) {
		w.flex_box('flex_center_bar', 'row', 'center', 'center', fn (mut f simplegui.SimpleWindow) {
			f.add_badge('status_badge', 'STATUS: RUNNING', 'success')
			f.add_badge('env_badge', 'ENV: DEV', 'warning')
			f.add_button('btn_ping', 'Ping Server')
		})
	})

	win.add_vertical_spacer(10)

	// Example 3: Equal Distribution Chips (Row + Space-Around + Center)
	win.group('grp_ex3', '3. Evenly Spaced Action Badges (row, space_around, center)',
		fn (mut w simplegui.SimpleWindow) {
			w.flex_box('flex_around_bar', 'row', 'space_around', 'center', fn (mut f simplegui.SimpleWindow) {
				f.add_button('btn_tag1', '🏷️ Design')
				f.add_button('btn_tag2', '🏷️ Frontend')
				f.add_button('btn_tag3', '🏷️ Backend')
				f.add_button('btn_tag4', '🏷️ DevOps')
			})
		})

	win.add_vertical_spacer(10)

	// Example 4: Right-Aligned Action Bar (Row + End + Center)
	win.group('grp_ex4', '4. Form Action Buttons (row, end, center)', fn (mut w simplegui.SimpleWindow) {
		w.flex_box('flex_end_bar', 'row', 'end', 'center', fn (mut f simplegui.SimpleWindow) {
			f.add_button('btn_cancel', 'Cancel')
			f.add_button('btn_save', 'Save Changes')
		})
	})

	win.add_vertical_spacer(10)

	// Example 5: Vertical Stack Form (Column + Start + Stretch)
	win.group('grp_ex5', '5. Vertical Stack Form (column, start, stretch)', fn (mut w simplegui.SimpleWindow) {
		w.flex_box('flex_col_stack', 'column', 'start', 'stretch', fn (mut f simplegui.SimpleWindow) {
			f.add_banner('banner_tip', 'Vertical flexbox containers align items top-to-bottom.',
				'info')

			f.add_search_field('search_input', '')
				.placeholder('Search components...')
		})
	})

	win.run()
}
