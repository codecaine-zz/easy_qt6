module main

import simplegui

fn main() {
	// Create the window using method chaining
	mut win := simplegui.new_simple_window('Beginner Friendly Demo', 480, 560)
	win.set_theme('dracula')
	win.set_padding(16)
	win.set_spacing(12)
	win.add_heading('Sign Up Form')

	win.add_heading('User Profile')

	win.add_form_field('Username:', 'username', '')
		.placeholder('Enter username...')
		.tooltip('Only letters and numbers')
		.onchange
	// Fluent onchange handler chained directly on creation
	(
		fn (mut w2 simplegui.SimpleWindow, val string) {
			if val.len < 3 {
				w2.error('Username must be at least 3 characters')
			} else {
				// Clear error for this control individually
				w2.clear_error('username')
			}
		}
	)

	win.add_form_field('Email Address:', 'email', '')
		.placeholder('Enter email...')
		.onchange
	// Fluent onchange handler chained directly on creation
	(
		fn (mut w2 simplegui.SimpleWindow, val string) {
			if !val.contains('@') {
				w2.error('Please enter a valid email')
			} else {
				// Clear error for this control individually
				w2.clear_error('email')
			}
		}
	)

	win.add_vertical_spacer(10)

	// Action buttons row with fluent onclick event binding
	win.row('action_row', fn (mut w simplegui.SimpleWindow) {
		w.add_button('submit', 'Register')
			.width(120)
			.onclick(on_register)

		w.add_button('reset', 'Reset Form')
			.width(120)
			.onclick(on_reset)
	})

	win.set_status('Welcome! Fill out the form above.')
	win.run()
}

fn on_register(mut win simplegui.SimpleWindow) {
	// Use new shorthand get() value accessor
	username := win.get('username')
	email := win.get('email')

	if username.len < 3 || !email.contains('@') {
		win.alert('Validation Error', 'Please resolve form errors before registering.')
		win.set_status('Registration failed due to invalid form data.')
		return
	}

	win.alert('Success!', 'Thank you, ${username}! You have registered with ${email}.')
	win.set_status('User ${username} registered successfully!')
}

fn on_reset(mut win simplegui.SimpleWindow) {
	win.reset_form()
	win.clear_errors()
	win.set_status('Form reset to initial empty state.')
}
