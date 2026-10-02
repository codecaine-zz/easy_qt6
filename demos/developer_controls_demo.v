module main

import simplegui
import math

struct AppState {
mut:
	chart_type         string
	tick_count         int
	action_col_enabled bool
}

fn main() {
	// Initialize App State
	mut state := &AppState{
		chart_type:         'area'
		tick_count:         0
		action_col_enabled: true
	}

	mut win := simplegui.new_simple_window('Developer Controls Demo', 800, 980)
	win.configure(fn (mut cfg simplegui.WindowConfig) {
		cfg.padding = 15
		cfg.spacing = 10
	})

	win.add_heading('Developer-Oriented UI Controls')

	// Breadcrumb navigation control
	win.add_label('lbl_breadcrumbs', 'Path Navigator (Click segment to truncate, or use Reset):')
	win.begin_row('breadcrumbs_row')
	win.add_breadcrumbs('nav_breadcrumbs', ['Home', 'Workspace', 'simplegui'])
	win.add_button('btn_reset_breadcrumbs', 'Reset Path')
	win.end_row()

	win.on_change('nav_breadcrumbs', fn (mut w simplegui.SimpleWindow, value string) {
		w.append_console('log_console', '[SUCCESS] Navigated to breadcrumb segment: ${value}\n',
			4)
		if value == 'Home' {
			w.set_breadcrumbs('nav_breadcrumbs', ['Home'])
		} else if value == 'Workspace' {
			w.set_breadcrumbs('nav_breadcrumbs', ['Home', 'Workspace'])
		}
	})

	win.on_click('btn_reset_breadcrumbs', fn (mut w simplegui.SimpleWindow) {
		w.set_breadcrumbs('nav_breadcrumbs', ['Home', 'Workspace', 'simplegui'])
		w.append_console('log_console', '[INFO] Restored breadcrumbs path\n', 1)
	})

	win.add_vertical_spacer(5)

	// 1. Shortcut Recorder section
	win.add_label('lbl_shortcut_desc', '1. Shortcut Recorder (Click field and press key combination):')
	win.begin_row('shortcut_row')
	win.add_shortcut_recorder('rec_shortcut')
	win.add_button('btn_clear_shortcut', 'Clear')
	win.end_row()

	win.on_change('rec_shortcut', fn (mut w simplegui.SimpleWindow, value string) {
		if value == '' {
			w.append_console('log_console', '[INFO] Shortcut cleared\n', 1)
		} else {
			w.append_console('log_console', '[SUCCESS] Captured shortcut: ${value}\n',
				4)
		}
	})

	win.on_click('btn_clear_shortcut', fn (mut w simplegui.SimpleWindow) {
		w.set_text('rec_shortcut', '')
		w.append_console('log_console', '[INFO] Shortcut cleared manually\n', 1)
	})

	win.add_vertical_spacer(10)

	// 2. Line/Area Chart & Circular Progress section
	win.begin_row('chart_and_progress_row')
	win.group('chart_group', '2. Dynamic Line/Area Chart', fn (mut w simplegui.SimpleWindow) {
		w.add_chart('data_chart', 'area', 180)
	})

	win.group('progress_group', '3. Circular Progress Gauge', fn (mut w simplegui.SimpleWindow) {
		w.add_circular_progress('progress_gauge', 0, 0, 100)
		w.add_label('lbl_gauge_val', 'Current Progress: 0%')
	})
	win.end_row()

	win.add_vertical_spacer(10)

	// 3. Property Grid & Color Grid section
	win.begin_row('property_and_color_row')
	win.group('inspector_group', '4. Property Inspector (Press Enter after editing)',
		fn (mut w simplegui.SimpleWindow) {
			w.add_property_grid('props_inspector', {
				'Chart Mode':  'area'
				'Timer Speed': '500ms'
				'Status Code': '200 OK'
			})
		})

	win.group('color_group', '5. Theme Color Palette Grid', fn (mut w simplegui.SimpleWindow) {
		w.add_color_grid('palette_grid', [
			'#0096FF',
			'#FF5733',
			'#33FF57',
			'#F0E68C',
			'#9400D3',
			'#FFC0CB',
		])
	})
	win.end_row()

	win.on_change('props_inspector', fn (mut w simplegui.SimpleWindow, value string) {
		parts := value.split(':')
		if parts.len == 2 {
			w.append_console('log_console', '[INFO] Property Grid Update -> Key: "${parts[0]}", Value: "${parts[1]}"\n',
				1)
		}
	})

	win.on_change('palette_grid', fn (mut w simplegui.SimpleWindow, value string) {
		w.append_console('log_console', '[SUCCESS] Selected color swatch: ${value}\n',
			4)
	})

	win.add_vertical_spacer(10)

	// 4. Excel-like Editable Grid section
	win.begin_row('grid_row')
	win.group('grid_group', '6. Excel-like Editable Grid (Double-click cell to edit)',
		fn (mut w simplegui.SimpleWindow) {
			w.add_grid('data_grid', ['ID', 'Task Name', 'Completed', 'Action'], [
				['1', 'Design UI Mockups', 'true', 'Run'],
				['2', 'Write Cocoa Bridge', 'true', 'Start'],
				['3', 'Add V Wrappers', 'false', 'Stop'],
			])
			w.grid_set_column_type('data_grid', 2, 'checkbox')
			w.grid_set_column_type('data_grid', 3, 'button')
			w.grid_set_column_enabled('data_grid', 0, false)
			w.grid_set_row_enabled('data_grid', 1, false)
			w.grid_set_cell_enabled('data_grid', 2, 1, false)
		})

	win.group('grid_actions_group', 'Grid Operations (CRUD)', fn (mut w simplegui.SimpleWindow) {
		w.add_button('btn_grid_add_row', 'Add Row')
		w.add_button('btn_grid_delete_row', 'Delete Selected Row')
		w.add_button('btn_grid_add_col', 'Add Column')
		w.add_button('btn_grid_delete_col', 'Delete Last Column')
		w.add_button('btn_grid_autosize', 'Auto-size Columns')
		w.add_button('btn_grid_toggle_col_enabled', 'Toggle Action Column')
		w.add_button('btn_grid_clear', 'Clear Grid Data')
	})
	win.end_row()

	win.on_change('data_grid', fn (mut w simplegui.SimpleWindow, value string) {
		parts := value.split(':')
		if parts.len == 3 {
			w.append_console('log_console', '[INFO] Grid edited -> Cell (${parts[0]}, ${parts[1]}): "${parts[2]}"\n',
				1)
		}
	})

	win.on_click('btn_grid_add_row', fn (mut w simplegui.SimpleWindow) {
		w.grid_add_row('data_grid', ['4', 'Write Documentation', 'false', 'Build'])
		w.append_console('log_console', '[SUCCESS] Row added to grid.\n', 4)
	})

	win.on_cell_button_click('data_grid', fn (mut w simplegui.SimpleWindow, value string) {
		parts := value.split(':')
		if parts.len == 2 {
			w.append_console('log_console', '[INFO] Grid Cell Button Clicked -> Coordinate: (${parts[0]}, ${parts[1]})\n',
				1)
		}
	})

	win.on_click('btn_grid_delete_row', fn (mut w simplegui.SimpleWindow) {
		selected_idx := w.grid_get_selected_row('data_grid')
		if selected_idx >= 0 {
			w.grid_delete_row('data_grid', selected_idx)
			w.append_console('log_console', '[WARNING] Deleted selected row ${selected_idx} from grid.\n',
				2)
		} else {
			w.append_console('log_console', '[ERROR] Please select a row in the grid first.\n',
				3)
		}
	})

	win.on_click('btn_grid_add_col', fn (mut w simplegui.SimpleWindow) {
		w.grid_add_column('data_grid', 'Notes')
		w.append_console('log_console', '[SUCCESS] Column "Notes" added to grid.\n', 4)
	})

	win.on_click('btn_grid_delete_col', fn (mut w simplegui.SimpleWindow) {
		w.grid_delete_column('data_grid', 3)
		w.append_console('log_console', '[WARNING] Deleted column at index 3.\n', 2)
	})

	win.on_click('btn_grid_autosize', fn (mut w simplegui.SimpleWindow) {
		w.grid_autosize_columns('data_grid')
		w.append_console('log_console', '[SUCCESS] Grid columns auto-sized.\n', 4)
	})

	win.on_click('btn_grid_clear', fn (mut w simplegui.SimpleWindow) {
		w.grid_clear('data_grid')
		w.append_console('log_console', '[WARNING] Grid cleared.\n', 2)
	})

	win.on_click('btn_grid_toggle_col_enabled', fn [mut state] (mut w simplegui.SimpleWindow) {
		state.action_col_enabled = !state.action_col_enabled
		w.grid_set_column_enabled('data_grid', 3, state.action_col_enabled)
		w.append_console('log_console', '[INFO] Column 3 (Action buttons) Enabled: ${state.action_col_enabled}\n',
			1)
	})

	win.on_column_click('data_grid', fn (mut w simplegui.SimpleWindow, value string) {
		w.append_console('log_console', '[INFO] Grid Column Header Clicked -> Column Index: ${value}\n',
			1)
	})

	win.add_vertical_spacer(5)

	// 5. Log Console section
	win.add_label('lbl_console_desc', '7. Colored Log Console:')
	win.add_console('log_console', 150)

	win.add_vertical_spacer(5)

	// Action buttons to log custom messages
	win.begin_row('action_row')
	win.add_button('btn_log_info', 'Log Info')
	win.add_button('btn_log_warn', 'Log Warn')
	win.add_button('btn_log_err', 'Log Error')
	win.add_button('btn_log_ok', 'Log Success')
	win.add_button('btn_clear_logs', 'Clear Console')
	win.end_row()

	win.on_click('btn_log_info', fn (mut w simplegui.SimpleWindow) {
		w.append_console('log_console', '[INFO] This is an informational log message.\n',
			1)
	})

	win.on_click('btn_log_warn', fn (mut w simplegui.SimpleWindow) {
		w.append_console('log_console', '[WARNING] CPU usage has exceeded 80% threshold!\n',
			2)
	})

	win.on_click('btn_log_err', fn (mut w simplegui.SimpleWindow) {
		w.append_console('log_console', '[ERROR] Failed to bind port 8080: Permission denied.\n',
			3)
	})

	win.on_click('btn_log_ok', fn (mut w simplegui.SimpleWindow) {
		w.append_console('log_console', '[SUCCESS] Connection established successfully.\n',
			4)
	})

	win.on_click('btn_clear_logs', fn (mut w simplegui.SimpleWindow) {
		w.clear_console('log_console')
	})

	// Initial console message
	win.append_console('log_console', 'Developer Log Console initialized.\nType a shortcut or click the Log buttons above to see events.\n',
		0)

	// Setup a timer to update the chart and circular progress values dynamically
	win.set_interval('chart_timer', 500, fn [mut state] (mut w simplegui.SimpleWindow) {
		state.tick_count++
		mut chart_vals := []f64{}

		// Generate a nice sine wave mixed with some noise
		for i in 0 .. 20 {
			angle := f64(state.tick_count + i) * 0.3
			val := 50.0 + 30.0 * math.sin(angle) + f64(state.tick_count % 5)
			chart_vals << val
		}
		w.set_chart_data('data_chart', chart_vals)

		// Update circular progress gauge (0 to 100%)
		gauge_val := int((state.tick_count * 5) % 105)
		w.set_circular_progress('progress_gauge', gauge_val)
		w.set_text('lbl_gauge_val', 'Current Progress: ${gauge_val}%')
	})

	win.run()
}
