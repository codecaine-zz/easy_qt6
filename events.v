module simplegui

// ControlValidator represents a validation function signature for control string values.
pub type ControlValidator = fn (value string) string

// set_value updates the value of a control by name and dispatches a change event.
pub fn (win &SimpleWindow) set_value(name string, value string) &SimpleWindow {
	win.set_control_value(name, value)
	unsafe {
		mut w := &SimpleWindow(win)
		w.dispatch_event(name, 'change', value)
	}
	return win
}

// on_key registers a keyboard shortcut event handler for the window.
pub fn (win &SimpleWindow) on_key(key string, callback StringEventCallback) &SimpleWindow {
	norm_key := normalize_key_shortcut(key)
	unsafe {
		mut w := &SimpleWindow(win)
		w.handlers << ControlEventHandler{
			control_name: 'window'
			event_name:   'key'
			filter_value: norm_key
			string_cb:    callback
		}
	}
	return win
}

// run_after executes a void callback after a specified duration in milliseconds.
pub fn (win &SimpleWindow) run_after(ms int, callback VoidEventCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.handlers << ControlEventHandler{
			control_name: 'window'
			event_name:   'run_after'
			void_cb:      callback
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_run_after(win.window_info, ms, c'window')
	}
	return win
}

// on_click registers an event handler for click events on a control.
pub fn (win &SimpleWindow) on_click(name string, callback VoidEventCallback) &SimpleWindow {
	idx := win.find_handler(name, 'click')
	mut handler := ControlEventHandler{
		control_name: name
		event_name:   'click'
		void_cb:      callback
	}
	unsafe {
		mut w := &SimpleWindow(win)
		if idx >= 0 {
			w.handlers[idx] = handler
		} else {
			w.handlers << handler
		}
	}
	return win
}

// on_change registers an event handler for value change events on a control.
pub fn (win &SimpleWindow) on_change(name string, callback StringEventCallback) &SimpleWindow {
	idx := win.find_handler(name, 'change')
	mut handler := ControlEventHandler{
		control_name: name
		event_name:   'change'
		string_cb:    callback
	}
	unsafe {
		mut w := &SimpleWindow(win)
		if idx >= 0 {
			w.handlers[idx] = handler
		} else {
			w.handlers << handler
		}
	}
	return win
}

// on_cell_button_click registers an event handler for table grid cell action buttons.
pub fn (win &SimpleWindow) on_cell_button_click(name string, callback StringEventCallback) &SimpleWindow {
	idx := win.find_handler(name, 'click_cell_button')
	mut handler := ControlEventHandler{
		control_name: name
		event_name:   'click_cell_button'
		string_cb:    callback
	}
	unsafe {
		mut w := &SimpleWindow(win)
		if idx >= 0 {
			w.handlers[idx] = handler
		} else {
			w.handlers << handler
		}
	}
	return win
}

// on_change_step registers a callback for wizard stepper step changes.
pub fn (win &SimpleWindow) on_change_step(name string, callback StringEventCallback) &SimpleWindow {
	idx := win.find_handler(name, 'change_step')
	mut handler := ControlEventHandler{
		control_name: name
		event_name:   'change_step'
		string_cb:    callback
	}
	unsafe {
		mut w := &SimpleWindow(win)
		if idx >= 0 {
			w.handlers[idx] = handler
		} else {
			w.handlers << handler
		}
	}
	return win
}

// on_click_tag registers a callback for tag cloud chip clicks.
pub fn (win &SimpleWindow) on_click_tag(name string, callback StringEventCallback) &SimpleWindow {
	idx := win.find_handler(name, 'click_tag')
	mut handler := ControlEventHandler{
		control_name: name
		event_name:   'click_tag'
		string_cb:    callback
	}
	unsafe {
		mut w := &SimpleWindow(win)
		if idx >= 0 {
			w.handlers[idx] = handler
		} else {
			w.handlers << handler
		}
	}
	return win
}

// on_any_event registers an observer callback that receives all dispatched events on the window.
pub fn (win &SimpleWindow) on_any_event(callback AnyEventCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.any_event_handlers << callback
	}
	return win
}

// dispatch_event dispatches an event by name to registered handlers and observers.
pub fn (win &SimpleWindow) dispatch_event(name string, event_name string, value string) bool {
	if win.debug_mode {
		println('[simplegui DEBUG] Dispatching Event: "${event_name}" on Control: "${name}" (Value: "${value}")')
		win.set_status('[DEBUG] ${event_name} on "${name}" -> "${value}"')
	}

	// Broadcast live event to window observers & system subscribers
	for cb in win.any_event_handlers {
		unsafe {
			mut w := &SimpleWindow(win)
			cb(mut w, name, event_name, value)
		}
	}
	sys_broadcast_event(win.title, name, event_name, value)
	mut handler_idx := -1
	if event_name == 'file_drop' {
		handler_idx = win.find_handler_by_filter(name, event_name, value)
		if handler_idx < 0 {
			handler_idx = win.find_handler_by_filter('window', event_name, value)
		}
	} else {
		handler_idx = win.find_handler_by_filter(name, event_name, value)
	}
	if handler_idx < 0 {
		return false
	}
	handler := win.handlers[handler_idx]
	if event_name == 'run_after' {
		unsafe {
			mut w := &SimpleWindow(win)
			w.handlers.delete(handler_idx)
		}
	}
	if event_name == 'file_drop' {
		files := value.split('|')
		unsafe {
			mut w := &SimpleWindow(win)
			handler.file_drop_cb(mut w, files)
		}
		return true
	} else if handler.void_cb != unsafe { nil } {
		unsafe {
			mut w := &SimpleWindow(win)
			handler.void_cb(mut w)
		}
		return true
	} else if handler.string_cb != unsafe { nil } {
		unsafe {
			mut w := &SimpleWindow(win)
			handler.string_cb(mut w, value)
		}
		return true
	}
	return false
}

// click programmatically triggers a click event on a named control.
pub fn (win &SimpleWindow) click(name string) bool {
	return win.dispatch_event(name, 'click', '')
}

@[export: 'vlang_is_window_valid']
fn vlang_is_window_valid(win_ptr voidptr) bool {
	if win_ptr == unsafe { nil } {
		return false
	}
	reg := get_window_registry()
	for _, w in reg.windows {
		if voidptr(w) == win_ptr {
			return true
		}
	}
	return false
}

// Cocoa event dispatcher to V
@[export: 'vlang_dispatch_event']
fn vlang_dispatch_event(win_ptr voidptr, name_str &u8, event_str &u8, value_str &u8) {
	if win_ptr == unsafe { nil } || !vlang_is_window_valid(win_ptr) {
		return
	}
	mut win := unsafe { &SimpleWindow(win_ptr) }
	name := unsafe { name_str.vstring() }
	event := unsafe { event_str.vstring() }
	value := unsafe { value_str.vstring() }

	// Update V struct state with the new value from Cocoa
	if event == 'change' {
		idx := win.find_control(name)
		if idx >= 0 {
			kind := win.controls[idx].kind
			if kind == 'checkbox' || kind == 'toggle' {
				win.controls[idx].checked = (value == 'true')
			} else if kind in ['number', 'slider', 'vertical_slider', 'progress', 'stepper', 'knob',
				'levelindicator'] {
				win.controls[idx].number = value.int()
			}
			win.controls[idx].value = value
		}
	} else if event == 'column_change' {
		win.table_selected_columns[name] = value.int()
	}

	win.dispatch_event(name, event, value)
}

@[export: 'vlang_dispatch_close_requested']
fn vlang_dispatch_close_requested(win_ptr voidptr) bool {
	if win_ptr == unsafe { nil } || !vlang_is_window_valid(win_ptr) {
		return true
	}
	mut win := unsafe { &SimpleWindow(win_ptr) }
	allowed := win.can_close()
	if allowed && win.auto_save_state {
		win.save_app_form_state_or()
	}
	return allowed
}

// set_interval starts a recurring timer that triggers a callback every ms milliseconds.
pub fn (win &SimpleWindow) set_interval(timer_name string, ms int, callback VoidEventCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.handlers << ControlEventHandler{
			control_name: timer_name
			event_name:   'timer'
			void_cb:      callback
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_set_interval(win.window_info, ms, timer_name.str)
	}
	return win
}

// stop_interval stops a recurring timer by name.
pub fn (win &SimpleWindow) stop_interval(timer_name string) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_stop_interval(win.window_info, timer_name.str)
	}
	return win
}

// on_hover registers an event handler for mouse hover enter events on a control.
pub fn (win &SimpleWindow) on_hover(name string, callback VoidEventCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.handlers << ControlEventHandler{
			control_name: name
			event_name:   'hover_enter'
			void_cb:      callback
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_enable_hover_events(win.window_info, name.str)
	}
	return win
}

// on_hover_exit registers an event handler for mouse hover exit events on a control.
pub fn (win &SimpleWindow) on_hover_exit(name string, callback VoidEventCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.handlers << ControlEventHandler{
			control_name: name
			event_name:   'hover_exit'
			void_cb:      callback
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_enable_hover_events(win.window_info, name.str)
	}
	return win
}

// add_menu_item adds a menu item to an application menu and registers its click callback.
pub fn (win &SimpleWindow) add_menu_item(menu_name string, item_title string, shortcut string, callback VoidEventCallback) &SimpleWindow {
	handler_name := 'menu_${menu_name}_${item_title}'
	win.on_click(handler_name, callback)
	if win.window_info != unsafe { nil } {
		C.window_add_menu_item(win.window_info, menu_name.str, item_title.str, shortcut.str,
			handler_name.str)
	}
	return win
}

// add_context_menu_item adds a context menu item to a control and registers its click callback.
pub fn (win &SimpleWindow) add_context_menu_item(control_name string, item_title string, callback VoidEventCallback) &SimpleWindow {
	handler_name := 'context_${control_name}_${item_title}'
	win.on_click(handler_name, callback)
	if win.window_info != unsafe { nil } {
		C.window_add_context_menu_item(win.window_info, control_name.str, item_title.str,
			handler_name.str)
	}
	return win
}

// on_file_drop registers a callback invoked when files are drag-and-dropped onto the window.
pub fn (win &SimpleWindow) on_file_drop(callback FileDropCallback) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.handlers << ControlEventHandler{
			control_name: 'window'
			event_name:   'file_drop'
			file_drop_cb: callback
		}
	}
	return win
}

// add_dock_menu_item adds a menu item to the macOS application dock menu.
pub fn (win &SimpleWindow) add_dock_menu_item(title string, callback VoidEventCallback) &SimpleWindow {
	handler_name := 'dock_menu_${title.replace(' ', '_').to_lower()}'
	win.on_click(handler_name, callback)
	if win.window_info != unsafe { nil } {
		C.window_add_dock_menu_item(win.window_info, title.str, handler_name.str)
	}
	return win
}

// onclick registers a click callback on the last created control.
pub fn (win &SimpleWindow) onclick(callback VoidEventCallback) &SimpleWindow {
	if win.last_control != '' {
		win.on_click(win.last_control, callback)
	}
	return win
}

// onchange registers a change callback on the last created control.
pub fn (win &SimpleWindow) onchange(callback StringEventCallback) &SimpleWindow {
	if win.last_control != '' {
		win.on_change(win.last_control, callback)
	}
	return win
}

// onhover registers a hover enter callback on the last created control.
pub fn (win &SimpleWindow) onhover(callback VoidEventCallback) &SimpleWindow {
	if win.last_control != '' {
		win.on_hover(win.last_control, callback)
	}
	return win
}

// onhover_exit registers a hover exit callback on the last created control.
pub fn (win &SimpleWindow) onhover_exit(callback VoidEventCallback) &SimpleWindow {
	if win.last_control != '' {
		win.on_hover_exit(win.last_control, callback)
	}
	return win
}

// Shorthand aliases for value access
