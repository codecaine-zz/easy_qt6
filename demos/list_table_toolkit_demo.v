module main

// List & Table Toolkit Demo
// Focused tour of the newest ergonomic add-ons:
//   - bind_search_to_list  : live search filtering of a list box
//   - sort_list_items      : case-insensitive A-Z / Z-A sorting
//   - move_list_item       : Move Up / Move Down reordering
//   - sort_table_by_column : numeric-aware column sorting
//   - move_table_row       : table row reordering
//   - save/load_table_to/from_csv : one-call CSV export & import
//   - validate_email / validate_number / min_len_validator : ready-made validators
//   - clear_fields         : batch clear inputs + error states
import simplegui
import os

const csv_path = os.join_path(os.temp_dir(), 'toolkit_inventory.csv')

fn main() {
	mut win := simplegui.new_simple_window('List & Table Toolkit', 760, 940)
		.set_theme('dark')
		.set_padding(14)
		.set_spacing(8)

	win.add_heading('List & Table Toolkit')

	// ------------------------------------------------------------------
	// 1. Contacts list: live search + sorting + reordering
	// ------------------------------------------------------------------
	win.group('grp_contacts', 'Contacts — Live Search, Sort & Reorder', fn (mut w simplegui.SimpleWindow) {
		w.add_search_field('contact_search', 'Type to filter contacts...')
		w.add_list_box('contacts', ['Grace Hopper', 'Ada Lovelace', 'Alan Turing', 'Katherine Johnson',
			'Dennis Ritchie', 'Margaret Hamilton'])
		// One call wires the search field to the list (case-insensitive)
		w.bind_search_to_list('contact_search', 'contacts')
		w.add_action_row({
			'Sort A-Z':  fn (mut w simplegui.SimpleWindow) {
				w.sort_list_items('contacts', true)
				w.set_status('Contacts sorted ascending.')
			}
			'Sort Z-A':  fn (mut w simplegui.SimpleWindow) {
				w.sort_list_items('contacts', false)
				w.set_status('Contacts sorted descending.')
			}
			'Move Up':   fn (mut w simplegui.SimpleWindow) {
				idx := w.get_list_selected('contacts')
				if idx > 0 {
					w.move_list_item('contacts', idx, idx - 1)
					w.set_list_selected('contacts', idx - 1)
				}
			}
			'Move Down': fn (mut w simplegui.SimpleWindow) {
				idx := w.get_list_selected('contacts')
				if idx >= 0 && idx < w.get_list_count('contacts') - 1 {
					w.move_list_item('contacts', idx, idx + 1)
					w.set_list_selected('contacts', idx + 1)
				}
			}
		})
	})

	// ------------------------------------------------------------------
	// 2. New contact form: ready-made validators + clear_fields
	// ------------------------------------------------------------------
	win.group('grp_form', 'Add Contact — Validators', fn (mut w simplegui.SimpleWindow) {
		w.add_form_field('Name:', 'new_name', '')
		w.add_form_field('Email:', 'new_email', '')
		w.add_form_field('Age:', 'new_age', '')
		w.add_action_row({
			'Add Contact':  fn (mut w simplegui.SimpleWindow) {
				errors := w.validate_controls({
					'new_name':  simplegui.min_len_validator(3)
					'new_email': simplegui.validate_email
					'new_age':   simplegui.validate_number
				})
				if errors.len > 0 {
					w.set_status('Fix the ${errors.len} highlighted field(s).')
					return
				}
				w.add_list_item('contacts', w.get_text('new_name'))
				// Re-bind so the search snapshot includes the new contact
				w.bind_search_to_list('contact_search', 'contacts')
				w.clear_fields(['new_name', 'new_email', 'new_age'])
				w.toast('Contact added!')
			}
			'Clear Fields': fn (mut w simplegui.SimpleWindow) {
				w.clear_fields(['new_name', 'new_email', 'new_age'])
				w.set_status('Form cleared (values and error states).')
			}
		})
	})

	// ------------------------------------------------------------------
	// 3. Inventory table: numeric-aware sorting, reordering, CSV round trip
	// ------------------------------------------------------------------
	win.group('grp_inventory', 'Inventory — Column Sort, Reorder & CSV', fn (mut w simplegui.SimpleWindow) {
		w.add_table('inventory', ['Item', 'Qty', 'Price'])
		w.set_table_rows('inventory', [
			['Bolt', '120', '0.15'],
			['Anchor', '8', '12.50'],
			['Clip', '46', '0.99'],
			['Washer', '300', '0.05'],
			['Bracket', '17', '4.25'],
		])
		w.add_action_row({
			'Sort by Item':    fn (mut w simplegui.SimpleWindow) {
				// Text column -> case-insensitive alphabetical sort
				w.sort_table_by_column('inventory', 0, true)
				w.set_status('Sorted by Item (text sort).')
			}
			'Sort by Qty':     fn (mut w simplegui.SimpleWindow) {
				// Numeric column -> 8 < 17 < 46 < 120 < 300 (not text order!)
				w.sort_table_by_column('inventory', 1, true)
				w.set_status('Sorted by Qty (numeric sort).')
			}
			'Sort by Price ⌄': fn (mut w simplegui.SimpleWindow) {
				w.sort_table_by_column('inventory', 2, false)
				w.set_status('Sorted by Price, most expensive first.')
			}
		})
		w.add_action_row({
			'Row Up':     fn (mut w simplegui.SimpleWindow) {
				idx := w.get_table_selected('inventory')
				if idx > 0 {
					w.move_table_row('inventory', idx, idx - 1)
					w.set_table_selected('inventory', idx - 1)
				}
			}
			'Row Down':   fn (mut w simplegui.SimpleWindow) {
				idx := w.get_table_selected('inventory')
				if idx >= 0 && idx < w.get_table_row_count('inventory') - 1 {
					w.move_table_row('inventory', idx, idx + 1)
					w.set_table_selected('inventory', idx + 1)
				}
			}
			'Export CSV': fn (mut w simplegui.SimpleWindow) {
				w.save_table_to_csv('inventory', csv_path) or {
					w.error_dialog('Export Failed', err.msg())
					return
				}
				w.toast('Saved ${w.get_table_row_count('inventory')} rows to ${csv_path}')
			}
			'Import CSV': fn (mut w simplegui.SimpleWindow) {
				w.load_table_from_csv('inventory', csv_path) or {
					w.warn('Import Failed', 'Export the table first.\n${err.msg()}')
					return
				}
				w.toast('Reloaded inventory from CSV.')
			}
		})
	})

	win.set_status('Ready. Try the search field, sorts, and CSV round trip.')
	win.run()
}
