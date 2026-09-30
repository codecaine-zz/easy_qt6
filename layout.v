module simplegui

// add_heading adds a heading label and separator line to the window layout.
pub fn (win &SimpleWindow) add_heading(title string) &SimpleWindow {
	heading_name := 'heading_${win.controls.len}'
	win.add_label(heading_name, title)
	win.add_separator()
	return win
}

// get_spacing returns the layout item spacing in pixels.
pub fn (win &SimpleWindow) get_spacing() int {
	return win.spacing
}

// add_group_box adds a group box control to the window layout.
pub fn (win &SimpleWindow) add_group_box(name string, title string) &SimpleWindow {
	return win.add_group_box_with_options(name, title, true)
}

// add_group_box_with_options adds a group box control with optional caption (title) and border settings.
pub fn (win &SimpleWindow) add_group_box_with_options(name string, title string, border bool) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('groupbox')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.upsert_control(real_name, 'groupbox', title, '', false, 0)
	}
	if win.window_info != unsafe { nil } {
		b_val := if border { 1 } else { 0 }
		C.window_add_group_box_control_with_options(win.window_info, real_name.str, title.str,
			b_val)
	}
	return win
}

// begin_group_box begins a group box layout container.
pub fn (win &SimpleWindow) begin_group_box(name string, title string) &SimpleWindow {
	return win.begin_group_box_with_options(name, title, true)
}

// begin_group_box_with_options begins a group box container with optional caption and border settings.
pub fn (win &SimpleWindow) begin_group_box_with_options(name string, title string, border bool) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('groupbox')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.upsert_control(real_name, 'groupbox', title, '', false, 0)
	}
	if win.window_info != unsafe { nil } {
		b_val := if border { 1 } else { 0 }
		C.window_begin_group_box(win.window_info, real_name.str, title.str, b_val)
	}
	return win
}

// end_group_box ends the current group box layout container.
pub fn (win &SimpleWindow) end_group_box() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_end_group_box(win.window_info)
	}
	return win
}

// set_group_border enables or disables the border of a group box container.
pub fn (win &SimpleWindow) set_group_border(name string, border bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		b_val := if border { 1 } else { 0 }
		C.window_set_group_border(win.window_info, name.str, b_val)
	}
	return win
}

// set_group_caption updates or sets the caption header title of a group box container.
pub fn (win &SimpleWindow) set_group_caption(name string, caption string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_group_caption(win.window_info, name.str, caption.str)
	}
	return win
}

// add_tabs adds a tabs control to the window layout.
pub fn (win &SimpleWindow) add_tabs(name string, titles []string) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('tabs')
	}
	mut joined := ''
	for i, title in titles {
		if i > 0 {
			joined += ','
		}
		joined += title
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.upsert_control(real_name, 'tabs', joined, '', false, 0)
	}
	if win.window_info != unsafe { nil } {
		mut c_titles := []&u8{}
		for title in titles {
			c_titles << title.str
		}
		C.window_add_tabs_control(win.window_info, real_name.str, c_titles.data, titles.len)
	}
	return win
}

// begin_tab_container begins a tabbed container view.
pub fn (win &SimpleWindow) begin_tab_container(name string, titles []string) &SimpleWindow {
	return win.add_tabs(name, titles)
}

// end_tab_container closes a tabbed container view.
pub fn (win &SimpleWindow) end_tab_container() &SimpleWindow {
	return win
}

// begin_tab_page starts the content block for a specific tab index or identifier.
pub fn (win &SimpleWindow) begin_tab_page(name string, tab_index int) &SimpleWindow {
	return win
}

// end_tab_page closes the current tab page content block.
pub fn (win &SimpleWindow) end_tab_page() &SimpleWindow {
	return win
}

// add_scroll_view adds a scroll view control to the window layout.
pub fn (win &SimpleWindow) add_scroll_view(name string, height int) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('scrollview')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.upsert_control(real_name, 'scrollview', '', '', false, 0)
	}
	if win.window_info != unsafe { nil } {
		C.window_add_scroll_view_control(win.window_info, real_name.str, height)
	}
	return win
}

// begin_row begins a horizontal row layout container.
pub fn (win &SimpleWindow) begin_row(name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_begin_row(win.window_info, name.str)
	}
	return win
}

// end_row ends the current row container layout.
pub fn (win &SimpleWindow) end_row() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_end_row(win.window_info)
	}
	return win
}

// begin_grid begins a multi-column grid layout container with specified column count and item spacing.
pub fn (win &SimpleWindow) begin_grid(name string, columns int, spacing int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_begin_grid(win.window_info, name.str, columns, spacing)
	}
	return win
}

// end_grid ends the current grid layout container.
pub fn (win &SimpleWindow) end_grid() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_end_grid(win.window_info)
	}
	return win
}

// begin_flex_box begins a flexbox container with direction ('row'|'column'), main-axis justification ('start'|'center'|'end'|'space_between'|'space_around'|'fill'), and cross-axis alignment ('start'|'center'|'end'|'stretch').
pub fn (win &SimpleWindow) begin_flex_box(name string, direction string, justify string, align string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_begin_flex_box(win.window_info, name.str, direction.str, justify.str,
			align.str)
	}
	return win
}

// end_flex_box ends the current flexbox container.
pub fn (win &SimpleWindow) end_flex_box() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_end_flex_box(win.window_info)
	}
	return win
}

// add_vertical_spacer adds a vertical spacer element of specified height to the window layout.
pub fn (win &SimpleWindow) add_vertical_spacer(height int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_add_vertical_spacer(win.window_info, height)
	}
	return win
}

// add_horizontal_spacer adds a horizontal spacer control to the window layout.
pub fn (win &SimpleWindow) add_horizontal_spacer(width int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_add_horizontal_spacer(win.window_info, width)
	}
	return win
}

// add_separator adds a separator control to the window layout.
pub fn (win &SimpleWindow) add_separator() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_add_separator(win.window_info)
	}
	return win
}

// add_action_row adds a row of action buttons with associated callbacks.
pub fn (win &SimpleWindow) add_action_row(actions map[string]VoidEventCallback) &SimpleWindow {
	row_name := win.auto_name('action_row')
	win.begin_row(row_name)
	for title, cb in actions {
		btn_name := win.auto_name('btn')
		win.add_action(btn_name, title, cb)
	}
	win.end_row()
	return win
}

// add_fields_row adds a row of form fields mapped by label and control name.
pub fn (win &SimpleWindow) add_fields_row(fields map[string]string) &SimpleWindow {
	row_name := win.auto_name('fields_row')
	win.begin_row(row_name)
	for label, name in fields {
		win.add_form_field(label, name, '')
	}
	win.end_row()
	return win
}

// row creates a horizontal row container and invokes the callback for child elements.
pub fn (win &SimpleWindow) row(name string, callback VoidEventCallback) &SimpleWindow {
	win.begin_row(name)
	unsafe {
		mut w := &SimpleWindow(win)
		callback(mut w)
	}
	win.end_row()
	return win
}

// grid creates a grid layout container and invokes the callback for child elements.
pub fn (win &SimpleWindow) grid(name string, columns int, spacing int, callback VoidEventCallback) &SimpleWindow {
	win.begin_grid(name, columns, spacing)
	unsafe {
		mut w := &SimpleWindow(win)
		callback(mut w)
	}
	win.end_grid()
	return win
}

// flex_box creates a flexbox layout container and invokes the callback for child elements.
pub fn (win &SimpleWindow) flex_box(name string, direction string, justify string, align string, callback VoidEventCallback) &SimpleWindow {
	win.begin_flex_box(name, direction, justify, align)
	unsafe {
		mut w := &SimpleWindow(win)
		callback(mut w)
	}
	win.end_flex_box()
	return win
}

// set_control_alignment sets the alignment of a named control ('left'|'center'|'right'|'top'|'bottom').
pub fn (win &SimpleWindow) set_control_alignment(name string, alignment string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_control_alignment_by_name(win.window_info, name.str, alignment.str)
	}
	for i in 0 .. win.controls.len {
		if win.controls[i].name == name {
			unsafe {
				win.controls[i].alignment = alignment
			}
			break
		}
	}
	return win
}

// set_control_expand_fill configures whether a named control expands to fill available layout space.
pub fn (win &SimpleWindow) set_control_expand_fill(name string, expand bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_control_expand_fill_by_name(win.window_info, name.str, if expand {
			1
		} else {
			0
		})
	}
	for i in 0 .. win.controls.len {
		if win.controls[i].name == name {
			unsafe {
				win.controls[i].expand_fill = expand
			}
			break
		}
	}
	return win
}

// align_left aligns the last created control to the left.
pub fn (win &SimpleWindow) align_left() &SimpleWindow {
	if win.last_control != '' {
		win.set_control_alignment(win.last_control, 'left')
	}
	return win
}

// align_center aligns the last created control to the center.
pub fn (win &SimpleWindow) align_center() &SimpleWindow {
	if win.last_control != '' {
		win.set_control_alignment(win.last_control, 'center')
	}
	return win
}

// align_right aligns the last created control to the right.
pub fn (win &SimpleWindow) align_right() &SimpleWindow {
	if win.last_control != '' {
		win.set_control_alignment(win.last_control, 'right')
	}
	return win
}

// align_top aligns the last created control to the top.
pub fn (win &SimpleWindow) align_top() &SimpleWindow {
	if win.last_control != '' {
		win.set_control_alignment(win.last_control, 'top')
	}
	return win
}

// align_bottom aligns the last created control to the bottom.
pub fn (win &SimpleWindow) align_bottom() &SimpleWindow {
	if win.last_control != '' {
		win.set_control_alignment(win.last_control, 'bottom')
	}
	return win
}

// expand_fill configures the last created control to expand and fill available container space.
pub fn (win &SimpleWindow) expand_fill() &SimpleWindow {
	if win.last_control != '' {
		win.set_control_expand_fill(win.last_control, true)
	}
	return win
}

// GroupConfig defines configuration options for a group box container or card.
pub struct GroupConfig {
pub mut:
	title             string
	border            bool
	border_width      f32
	border_color      string
	corner_radius     f32
	bg_color          string
	padding           int
	shadow            bool
	show_caption      bool
	caption_color     string
	caption_alignment string
}

// add_group_box_with_config adds a group box control using a detailed GroupConfig struct.
pub fn (win &SimpleWindow) add_group_box_with_config(name string, cfg GroupConfig) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('groupbox')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.upsert_control(real_name, 'groupbox', cfg.title, '', false, 0)
	}
	if win.window_info != unsafe { nil } {
		b_val := if cfg.border { 1 } else { 0 }
		s_val := if cfg.shadow { 1 } else { 0 }
		sc_val := if cfg.show_caption { 1 } else { 0 }
		C.window_add_group_box_control_with_config(win.window_info, real_name.str, cfg.title.str,
			b_val, cfg.border_width, cfg.border_color.str, cfg.corner_radius, cfg.bg_color.str,
			cfg.padding, s_val, sc_val, cfg.caption_color.str, cfg.caption_alignment.str)
	}
	return win
}

// begin_group_box_with_config begins a group box container layout using a detailed GroupConfig struct.
pub fn (win &SimpleWindow) begin_group_box_with_config(name string, cfg GroupConfig) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('groupbox')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.upsert_control(real_name, 'groupbox', cfg.title, '', false, 0)
	}
	if win.window_info != unsafe { nil } {
		b_val := if cfg.border { 1 } else { 0 }
		s_val := if cfg.shadow { 1 } else { 0 }
		sc_val := if cfg.show_caption { 1 } else { 0 }
		C.window_begin_group_box_with_config(win.window_info, real_name.str, cfg.title.str,
			b_val, cfg.border_width, cfg.border_color.str, cfg.corner_radius, cfg.bg_color.str,
			cfg.padding, s_val, sc_val, cfg.caption_color.str, cfg.caption_alignment.str)
	}
	return win
}

// group creates a group box container and invokes the callback for child elements.
pub fn (win &SimpleWindow) group(name string, title string, callback VoidEventCallback) &SimpleWindow {
	return win.group_with_options(name, title, true, callback)
}

// group_with_options creates a group box container with optional caption (title) and border settings.
pub fn (win &SimpleWindow) group_with_options(name string, title string, border bool, callback VoidEventCallback) &SimpleWindow {
	return win.group_config(name, GroupConfig{
		title:  title
		border: border
	}, callback)
}

// group_config creates a group box container using a GroupConfig struct.
pub fn (win &SimpleWindow) group_config(name string, cfg GroupConfig, callback VoidEventCallback) &SimpleWindow {
	win.begin_group_box_with_config(name, cfg)
	unsafe {
		mut w := &SimpleWindow(win)
		callback(mut w)
	}
	win.end_group_box()
	return win
}

// card creates a borderless card container layout with optional shadow and inner padding.
pub fn (win &SimpleWindow) card(name string, callback VoidEventCallback) &SimpleWindow {
	return win.group_config(name, GroupConfig{
		border:       false
		show_caption: false
		padding:      16
		shadow:       true
	}, callback)
}

// card_with_title creates a styled card container with a header title.
pub fn (win &SimpleWindow) card_with_title(name string, title string, callback VoidEventCallback) &SimpleWindow {
	return win.group_config(name, GroupConfig{
		title:         title
		border:        true
		corner_radius: 12.0
		padding:       16
		shadow:        true
	}, callback)
}

// set_group_style updates the visual style of an existing group box container.
pub fn (win &SimpleWindow) set_group_style(name string, cfg GroupConfig) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		b_val := if cfg.border { 1 } else { 0 }
		s_val := if cfg.shadow { 1 } else { 0 }
		C.window_set_group_style(win.window_info, name.str, b_val, cfg.border_width, cfg.border_color.str,
			cfg.corner_radius, cfg.bg_color.str, s_val)
	}
	return win
}

// play_sound plays a system sound by name.
pub fn play_sound(sound_name string) {
	C.window_play_system_sound(sound_name.str)
}

// begin_split_view begins a split view container in the layout.
pub fn (win &SimpleWindow) begin_split_view(name string, vertical bool) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		vert := if vertical { 1 } else { 0 }
		C.window_begin_split_view(win.window_info, name.str, vert)
	}
	return win
}


// end_split_view ends the current split view layout container.
pub fn (win &SimpleWindow) end_split_view() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_end_split_view(win.window_info)
	}
	return win
}

// set_split_position sets the position of a split view divider.
pub fn (win &SimpleWindow) set_split_position(name string, divider_index int, position f64) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_split_position(win.window_info, name.str, divider_index, position)
	}
	return win
}

// set_split_divider_style sets the divider style ("thin", "thick", "pane_splitter").
pub fn (win &SimpleWindow) set_split_divider_style(name string, style string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_set_split_divider_style(win.window_info, name.str, style.str)
	}
	return win
}

// split_view creates a two-pane split container with closures for pane1 and pane2.
pub fn (win &SimpleWindow) split_view(name string, vertical bool, pane1_fn fn (), pane2_fn fn ()) &SimpleWindow {
	win.begin_split_view(name, vertical)
	pane1_fn()
	win.split_view_next_pane()
	pane2_fn()
	win.end_split_view()
	return win
}

// begin_glass_box begins a glassmorphic container layout.
pub fn (win &SimpleWindow) begin_glass_box(name string, material string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_begin_glass_box(win.window_info, name.str, material.str)
	}
	return win
}

// end_glass_box ends the current glass box container layout.
pub fn (win &SimpleWindow) end_glass_box() &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_end_glass_box(win.window_info)
	}
	return win
}

// add_badge adds a badge control to the window layout.
