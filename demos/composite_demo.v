import simplegui

fn main() {
	mut win := simplegui.new_simple_window('High-Level Composite Controls Demo', 800, 600)

	win.add_label('lbl1', 'High Level Composite UI Components').font_size(20).bold(true)
	win.add_separator()

	win.add_label('lbl2', '1. Transfer List').font_size(16).bold(true)
	win.add_label('lbl_desc1', 'Easily move items between two lists!').font_size(12)

	left_items := ['Apple', 'Banana', 'Cherry', 'Date', 'Elderberry']
	right_items := ['Fig', 'Grape']

	win.add_transfer_list_opts('fruit_transfer', left_items, right_items, true)

	win.add_button('btn_get_fruits', 'Print Selected Fruits to Console')
	win.on_click('btn_get_fruits', fn (mut w simplegui.SimpleWindow) {
		selected := w.get_transfer_list_items('fruit_transfer')
		println('Selected Fruits: ${selected}')
	})

	win.add_separator()

	win.add_label('lbl3', '2. Property Grid').font_size(16).bold(true)
	win.add_label('lbl_desc2', 'Automatically builds an aligned form from a map of properties.').font_size(12)

	props := {
		'First Name': 'John'
		'Last Name': 'Doe'
		'Email': 'john.doe@example.com'
		'Phone': '555-1234'
	}
	
	win.add_property_grid('user_form', props)

	win.add_button('btn_get_props', 'Print Form Values to Console')
	win.on_click('btn_get_props', fn (mut w simplegui.SimpleWindow) {
		first := w.get_text('user_form_First Name')
		last := w.get_text('user_form_Last Name')
		println('Form Values: First: ${first}, Last: ${last}')
	})

	win.run()
}
