module main

import simplegui
import simplegui.colorutils

fn main() {
	// Initialize HDR Showcase Window
	mut win := simplegui.new_simple_window('macOS High Dynamic Range (HDR / EDR) Showcase', 840, 960)
	win.set_window_hdr(true)
	win.set_window_color_space('display_p3')

	// 1. Header & Hardware EDR Diagnostics
	win.add_label('header', ' macOS High Dynamic Range & Extended Dynamic Range (EDR) Architecture')
	win.set_control_font_size('header', 16)

	is_hdr := win.is_hdr_supported()
	headroom := win.get_screen_edr_headroom()
	pot_headroom := win.get_screen_max_potential_edr_headroom()
	color_space := win.get_window_color_space()

	hdr_status := if is_hdr {
		'Active (Hardware EDR Headroom: ${headroom:.2f}x, Max Potential: ${pot_headroom:.2f}x)'
	} else {
		'SDR Mode (Headroom: ${headroom:.2f}x — standard dynamic range display)'
	}

	win.begin_row('row_diag')
	win.add_label('lbl_diag', 'Display Hardware Status:')
	win.add_label('val_diag', hdr_status)
	win.end_row()

	win.begin_row('row_cs')
	win.add_label('lbl_cs', 'Window Color Space:')
	win.add_label('val_cs', color_space)
	win.end_row()

	// 2. Interactive HDR Highlights & Glow Controls
	win.add_separator()
	win.add_label('sec_glow', '1. Custom HDR Controls (CALayer EDR & Extended Display P3)')
	win.add_label('sec_glow_sub', 'Unlike standard AppKit buttons that clip to SDR 1.0 white, custom EDR rendering shines with peak HDR luminance.')

	win.add_hdr_glow_box('hdr_badge', '⚡ Peak HDR Highlight Element', 2.5, '#00D4FF')

	win.begin_row('row_intensity')
	win.add_label('lbl_slider', 'EDR Headroom Multiplier (1.0x to 4.0x):')
	win.add_slider('slider_intensity', 25) // 2.5x
	win.add_label('lbl_intensity_val', '2.5x')
	win.end_row()

	win.begin_row('row_colors')
	win.add_label('lbl_preset', 'Color Presets:')
	win.add_button('btn_cyan', 'Neon Cyan')
	win.add_button('btn_gold', 'HDR Solar Gold')
	win.add_button('btn_magenta', 'Hyper Magenta')
	win.add_button('btn_white', 'Pure Peak White (>1.0)')
	win.end_row()

	// 3. NSImageView HDR Support (macOS 14 Sonoma+)
	win.add_separator()
	win.add_label('sec_img', '2. Native NSImageView HDR Support (preferredImageDynamicRange)')
	win.add_label('sec_img_sub', 'Displays HDR photos, 32-bit float Display P3 images, and gain maps with extended luminance.')

	// Load our bundled 32-bit floating-point Display P3 Linear HDR sample image
	win.add_hdr_image('hdr_photo', 'demos/assets/hdr_sample.tiff', 'high')

	win.begin_row('row_img_modes')
	win.add_label('lbl_img_mode', 'Image Dynamic Range:')
	win.add_button('btn_dr_high', 'High (Full HDR)')
	win.add_button('btn_dr_constrained', 'Constrained High')
	win.add_button('btn_dr_standard', 'Standard (SDR)')
	win.end_row()

	win.add_label('lbl_active_dr', 'Current Mode: High (Full HDR — unconstrained peak luminance up to display headroom)')

	win.begin_row('row_img_switch')
	win.add_label('lbl_img_source', 'Test Images:')
	win.add_button('btn_load_hdr_sample', 'True HDR Sample (Display P3 Float)')
	win.add_button('btn_load_sdr_photo', 'Standard Photo (SDR PNG)')
	win.end_row()

	// 4. Metal MTKView Canvas (WWDC 2021 EDR Pattern)
	win.add_separator()
	win.add_label('sec_metal', '3. Hardware-Accelerated Metal Canvas (CAMetalLayer EDR)')
	win.add_label('sec_metal_sub', 'CAMetalLayer with RGBA16Float pixel format and Extended Linear Display P3 color space.')

	win.add_hdr_mtk_view('metal_hdr_canvas')

	// 5. colorutils.HDRColor Tone-Mapping Analysis
	win.add_separator()
	win.add_label('sec_math', '4. Color Space & Tone Mapping (colorutils.HDRColor)')

	base_rgb := colorutils.RGB{r: 255, g: 140, b: 0}
	hdr_col := colorutils.hdr_color_from_rgb(base_rgb, 3.0)
	reinhard := hdr_col.tone_map_reinhard()
	aces := hdr_col.tone_map_aces()

	win.add_textarea('tone_info', 'Source Base RGB: ${base_rgb.str()}\n' +
		'HDR Color with 3.0x Headroom: ${hdr_col.str()}\n' +
		'Reinhard Tone-Mapped SDR: ${reinhard.str()} (${reinhard.hex()})\n' +
		'ACES Filmic Tone-Mapped SDR: ${aces.str()} (${aces.hex()})\n' +
		'Linear Exposure: ${hdr_col.linear_exposure():.2f}x\n' +
		'Is HDR: ${hdr_col.is_hdr()}')

	win.add_separator()
	win.add_label('status', 'Ready')

	// Event Handlers
	win.on_change('slider_intensity', fn (mut w simplegui.SimpleWindow, val string) {
		ival := val.int()
		factor := f64(ival) / 10.0
		if factor < 1.0 { return }
		w.set_hdr_glow_box_intensity('hdr_badge', factor)
		w.set_control_contents_headroom('hdr_badge', factor)
		w.set_text('lbl_intensity_val', '${factor:.1f}x')
	})

	win.on_click('btn_cyan', fn (mut w simplegui.SimpleWindow) {
		w.set_hdr_glow_box_color('hdr_badge', '#00D4FF')
		w.set_control_hdr_color('hdr_badge', 'glow', 0.0, 0.83, 1.0, 1.0, 2.5)
	})

	win.on_click('btn_gold', fn (mut w simplegui.SimpleWindow) {
		w.set_hdr_glow_box_color('hdr_badge', '#FFB703')
		w.set_control_hdr_color('hdr_badge', 'glow', 1.0, 0.72, 0.01, 1.0, 3.0)
	})

	win.on_click('btn_magenta', fn (mut w simplegui.SimpleWindow) {
		w.set_hdr_glow_box_color('hdr_badge', '#FF007F')
		w.set_control_hdr_color('hdr_badge', 'glow', 1.0, 0.0, 0.5, 1.0, 2.8)
	})

	win.on_click('btn_white', fn (mut w simplegui.SimpleWindow) {
		w.set_hdr_glow_box_color('hdr_badge', '#FFFFFF')
		w.set_control_hdr_color('hdr_badge', 'glow', 1.5, 1.5, 1.5, 1.0, 3.5)
	})

	win.on_click('btn_dr_high', fn (mut w simplegui.SimpleWindow) {
		w.set_image_dynamic_range('hdr_photo', 'high')
		w.set_control_dynamic_range('hdr_photo', 'high')
		w.set_text('lbl_active_dr', 'Current Mode: High (Full HDR — unconstrained peak luminance up to display headroom)')
		w.set_status('Image Dynamic Range: High (Full HDR)')
		w.toast('Dynamic Range set to High (Full HDR)')
	})

	win.on_click('btn_dr_constrained', fn (mut w simplegui.SimpleWindow) {
		w.set_image_dynamic_range('hdr_photo', 'constrained')
		w.set_control_dynamic_range('hdr_photo', 'constrained')
		w.set_text('lbl_active_dr', 'Current Mode: Constrained High (Highlights compressed to preserve battery / surrounding text)')
		w.set_status('Image Dynamic Range: Constrained High')
		w.toast('Dynamic Range set to Constrained High')
	})

	win.on_click('btn_dr_standard', fn (mut w simplegui.SimpleWindow) {
		w.set_image_dynamic_range('hdr_photo', 'standard')
		w.set_control_dynamic_range('hdr_photo', 'standard')
		w.set_text('lbl_active_dr', 'Current Mode: Standard (SDR — highlights strictly clamped to 1.0 peak white)')
		w.set_status('Image Dynamic Range: Standard (SDR)')
		w.toast('Dynamic Range set to Standard (SDR)')
	})

	win.on_click('btn_load_hdr_sample', fn (mut w simplegui.SimpleWindow) {
		w.set_image_path('hdr_photo', 'demos/assets/hdr_sample.tiff')
		w.set_status('Loaded True HDR Sample (Extended Linear Display P3 Float)')
		w.toast('Loaded True HDR Sample')
	})

	win.on_click('btn_load_sdr_photo', fn (mut w simplegui.SimpleWindow) {
		w.set_image_path('hdr_photo', 'preview.png')
		w.set_status('Loaded Standard SDR Photo (preview.png)')
		w.toast('Loaded Standard SDR Photo')
	})

	win.on_click('hdr_badge', fn (mut w simplegui.SimpleWindow) {
		w.toast('HDR Highlight badge clicked!')
	})

	win.run()
}
