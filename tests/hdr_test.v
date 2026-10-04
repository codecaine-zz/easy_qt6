module main

import simplegui

fn test_hdr_screen_apis() {
	screens := simplegui.get_screens()
	assert screens.len > 0

	main_screen := simplegui.get_main_screen()
	assert main_screen.width > 0
	// max_edr_headroom should be at least 1.0 (1.0 for SDR, >1.0 for HDR)
	assert main_screen.max_edr_headroom >= 1.0

	// Global helpers
	is_hdr := simplegui.is_hdr_supported()
	headroom := simplegui.get_screen_edr_headroom()
	assert headroom >= 1.0
	if is_hdr {
		assert headroom > 1.0 || main_screen.max_potential_edr_headroom > 1.0
	}
}

fn test_hdr_window_apis() {
	win := simplegui.new_simple_window('Test HDR Window', 500, 400)

	// Screen queries via window
	assert win.get_screen_edr_headroom() >= 1.0
	assert win.get_screen_max_potential_edr_headroom() >= 1.0
	assert win.get_screen_reference_edr_headroom() >= 0.0

	// Window HDR configuration
	win.set_window_hdr(true)
	win.set_window_color_space('display_p3')
	win.set_window_color_space('extended_srgb')

	// Control: HDR Image View
	win.add_hdr_image('hdr_img1', '', 'high')
	win.set_image_dynamic_range('hdr_img1', 'high')
	win.enable_image_hdr('hdr_img1', true)
	win.set_control_edr('hdr_img1', true)
	win.set_control_dynamic_range('hdr_img1', 'high')
	win.set_control_contents_headroom('hdr_img1', 2.5)

	// Control: HDR Metal Canvas View (MTKView)
	win.add_hdr_mtk_view('mtk_hdr')
	win.set_mtk_view_hdr('mtk_hdr', true, 'extended_linear_display_p3')

	// Control: HDR Glow Box / Highlight Custom Control
	win.add_hdr_glow_box('glow_btn', 'HDR Neon Button', 2.5, '#00D4FF')
	win.set_hdr_glow_box_intensity('glow_btn', 3.0)
	win.set_hdr_glow_box_color('glow_btn', '#FF0055')
	win.set_hdr_glow_box_label('glow_btn', 'HDR Ultra Glow')

	// HDR Color Application
	win.set_control_hdr_color('glow_btn', 'glow', 0.2, 0.9, 1.0, 1.0, 3.0)
	win.set_control_hdr_color('glow_btn', 'border', 1.0, 0.8, 0.2, 1.0, 2.0)

	println('✅ All HDR simplegui APIs tested successfully!')
}
