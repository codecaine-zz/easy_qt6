module simplegui

import os

// add_theme_menu adds a theme menu control to the window layout.
pub fn (win &SimpleWindow) add_theme_menu(name string, selected string) &SimpleWindow {
	mut real_name := name
	if real_name == '' {
		real_name = win.auto_name('theme')
	}
	if win.debug_mode {
		println('[simplegui DEBUG] Created Control: "${real_name}" (Type: "theme", Selected: "${selected}")')
	}
	unsafe {
		mut w := &SimpleWindow(win)
		w.upsert_control(real_name, 'theme', '', selected, false, 0)
	}
	if win.window_info != unsafe { nil } {
		C.window_add_theme_menu_control(win.window_info, real_name.str, selected.str)
	}
	return win
}

// set_background_color sets the background color of the window.
pub fn (win &SimpleWindow) set_background_color(color string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.background_color = color
	}
	if win.window_info != unsafe { nil } {
		C.window_set_background_color(win.window_info, color.str)
	}
	return win
}

// get_background_color retrieves the background color of the window or target control.
pub fn (win &SimpleWindow) get_background_color() string {
	return win.background_color
}

// set_font_color sets the font color of the window or target control.
pub fn (win &SimpleWindow) set_font_color(color string) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.font_color = color
	}
	if win.window_info != unsafe { nil } {
		C.window_set_font_color(win.window_info, color.str)
	}
	return win
}

// get_font_color retrieves the font color of the window or target control.
pub fn (win &SimpleWindow) get_font_color() string {
	return win.font_color
}

// set_control_background_color sets the background color of the specified control.
pub fn (win &SimpleWindow) set_control_background_color(name string, color string) &SimpleWindow {
	idx := win.find_control(name)
	if idx >= 0 {
		unsafe {
			mut w := &SimpleWindow(win)
			mut entry := w.controls[idx]
			entry.background_color = color
			w.controls[idx] = entry
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_set_control_background_color_by_name(win.window_info, name.str, color.str)
	}
	return win
}

// get_control_background_color returns the background color of the specified control.
pub fn (win &SimpleWindow) get_control_background_color(name string) string {
	idx := win.find_control(name)
	if idx >= 0 {
		return win.controls[idx].background_color
	}
	return ''
}

// set_control_font_color sets the font color of the specified control.
pub fn (win &SimpleWindow) set_control_font_color(name string, color string) &SimpleWindow {
	idx := win.find_control(name)
	if idx >= 0 {
		unsafe {
			mut w := &SimpleWindow(win)
			mut entry := w.controls[idx]
			entry.font_color = color
			w.controls[idx] = entry
		}
	}
	if win.window_info != unsafe { nil } {
		C.window_set_control_font_color_by_name(win.window_info, name.str, color.str)
	}
	return win
}

// get_control_font_color returns the font color of the specified control.
pub fn (win &SimpleWindow) get_control_font_color(name string) string {
	idx := win.find_control(name)
	if idx >= 0 {
		return win.controls[idx].font_color
	}
	return ''
}

// list_themes returns a list of available built-in theme names.
pub fn list_themes() []string {
	return [
		'GitHub Dark',
		'Apple Dark',
		'Apple Light',
		'Deep Space OLED',
		'Tokyo Night',
		'Nord Arctic',
		'Dracula Vampire',
		'Cyberpunk Neon',
		'Catppuccin Mocha',
		'Monokai Pro',
		'Gruvbox Dark',
		'Cobalt Blue',
		'Emerald Forest',
		'Sunset Dusk',
		'GitHub Light',
		'Solarized Dark',
		'Solarized Light',
		'Warm Paper & Ink',
	]
}

// get_theme returns the Theme configuration matching theme_name (case-insensitive, normalized).
// Defaults to 'GitHub Dark' if the theme name is unknown.
pub fn get_theme(theme_name string) Theme {
	normalized := theme_name.to_lower().replace(' ', '_').replace('-', '_')
	match normalized {
		'apple_light', 'macos_light' {
			return Theme{
				name:             'Apple Light'
				background_color: '#f6f6f7'
				font_color:       '#1d1d1f'
				accent_color:     '#0071e3'
				description:      'Clean Apple macOS Aqua studio interface'
				is_dark:          false
			}
		}
		'apple_dark', 'dark', 'macos_dark' {
			return Theme{
				name:             'Apple Dark'
				background_color: '#1c1c1e'
				font_color:       '#f5f5f7'
				accent_color:     '#0a84ff'
				description:      'Refined Apple macOS Pro Dark Titanium surface'
				is_dark:          true
			}
		}
		'deep_space_oled', 'deep_space', 'oled', 'midnight', 'space_gray', 'midnight_space_gray' {
			return Theme{
				name:             'Deep Space OLED'
				background_color: '#090a0f'
				font_color:       '#e2e8f0'
				accent_color:     '#6366f1'
				description:      'Ultra-deep pitch OLED dark theme with electric indigo accents'
				is_dark:          true
			}
		}
		'tokyo_night', 'tokyo', 'tokyo_night_storm' {
			return Theme{
				name:             'Tokyo Night'
				background_color: '#1a1b26'
				font_color:       '#c0caf5'
				accent_color:     '#7aa2f7'
				description:      'Iconic Japanese twilight deep indigo theme'
				is_dark:          true
			}
		}
		'nord_arctic', 'nord' {
			return Theme{
				name:             'Nord Arctic'
				background_color: '#2e3440'
				font_color:       '#eceff4'
				accent_color:     '#88c0d0'
				description:      'Arctic polar night slate with frosty cyan contrast'
				is_dark:          true
			}
		}
		'dracula_vampire', 'dracula' {
			return Theme{
				name:             'Dracula Vampire'
				background_color: '#282a36'
				font_color:       '#f8f8f2'
				accent_color:     '#bd93f9'
				description:      'High-contrast gothic slate purple developer theme'
				is_dark:          true
			}
		}
		'cyberpunk_neon', 'cyberpunk', 'neon', 'synthwave' {
			return Theme{
				name:             'Cyberpunk Neon'
				background_color: '#120e24'
				font_color:       '#00f0ff'
				accent_color:     '#ff007f'
				description:      'Electric midnight purple with hot cyan text and neon pink accents'
				is_dark:          true
			}
		}
		'catppuccin_mocha', 'catppuccin', 'mocha' {
			return Theme{
				name:             'Catppuccin Mocha'
				background_color: '#1e1e2e'
				font_color:       '#cdd6f4'
				accent_color:     '#f5c2e7'
				description:      'Soothing modern lavender pastel dark mode'
				is_dark:          true
			}
		}
		'monokai_pro', 'monokai' {
			return Theme{
				name:             'Monokai Pro'
				background_color: '#2d2a2e'
				font_color:       '#fcfcfa'
				accent_color:     '#ffd866'
				description:      'Warm dark charcoal with radiant Monokai yellow/gold accents'
				is_dark:          true
			}
		}
		'gruvbox_dark', 'gruvbox' {
			return Theme{
				name:             'Gruvbox Dark'
				background_color: '#282828'
				font_color:       '#ebdbb2'
				accent_color:     '#fe8019'
				description:      'Warm retro earthy dark canvas with burnt orange accents'
				is_dark:          true
			}
		}
		'cobalt_blue', 'cobalt', 'navy_blue', 'navy' {
			return Theme{
				name:             'Cobalt Blue'
				background_color: '#0a192f'
				font_color:       '#ccd6f6'
				accent_color:     '#64ffda'
				description:      'Deep submarine oceanic navy with glowing aqua teal accents'
				is_dark:          true
			}
		}
		'emerald_forest', 'emerald', 'forest_green', 'forest', 'sonoma_emerald', 'sonoma' {
			return Theme{
				name:             'Emerald Forest'
				background_color: '#062319'
				font_color:       '#ecfdf5'
				accent_color:     '#10b981'
				description:      'Deep evergreen botanical pine with vivid emerald accents'
				is_dark:          true
			}
		}
		'sunset_dusk', 'sunset', 'apple_sunset', 'ventura_amber', 'ventura', 'amber' {
			return Theme{
				name:             'Sunset Dusk'
				background_color: '#231123'
				font_color:       '#fff1f2'
				accent_color:     '#f43f5e'
				description:      'Rich twilight velvet plum with warm sunset coral accents'
				is_dark:          true
			}
		}
		'default', 'github_dark', 'github_dimmed', 'github' {
			return Theme{
				name:             'GitHub Dark'
				background_color: '#22272e'
				font_color:       '#adbac7'
				accent_color:     '#539bf5'
				description:      'Official GitHub Dark Dimmed developer canvas (Default)'
				is_dark:          true
			}
		}
		'github_light' {
			return Theme{
				name:             'GitHub Light'
				background_color: '#ffffff'
				font_color:       '#1f2328'
				accent_color:     '#0969da'
				description:      'Crisp high-contrast GitHub light interface'
				is_dark:          false
			}
		}
		'solarized_dark' {
			return Theme{
				name:             'Solarized Dark'
				background_color: '#002b36'
				font_color:       '#93a1a1'
				accent_color:     '#268bd2'
				description:      'Precision engineered scientific teal dark theme'
				is_dark:          true
			}
		}
		'solarized_light' {
			return Theme{
				name:             'Solarized Light'
				background_color: '#fdf6e3'
				font_color:       '#586e75'
				accent_color:     '#b58900'
				description:      'Warm linen parchment precision light palette'
				is_dark:          false
			}
		}
		'warm_paper_&_ink', 'warm_paper', 'paper', 'soft_pastel', 'pastel' {
			return Theme{
				name:             'Warm Paper & Ink'
				background_color: '#fbf8f2'
				font_color:       '#18181b'
				accent_color:     '#78716c'
				description:      'Tactile Japanese fine washi paper with deep sumi ink text'
				is_dark:          false
			}
		}
		else {
			return Theme{
				name:             'GitHub Dark'
				background_color: '#22272e'
				font_color:       '#adbac7'
				accent_color:     '#539bf5'
				description:      'Official GitHub Dark Dimmed developer canvas (Default)'
				is_dark:          true
			}
		}
	}
}

// get_theme_config_path returns the file path used to persist the active window theme.
fn get_theme_config_path() string {
	base := os.config_dir() or { os.join_path(os.home_dir(), '.config') }
	return os.join_path(base, 'simplegui', 'theme.txt')
}

// get_saved_theme retrieves the persisted theme preference, defaulting to 'GitHub Dark'.
pub fn get_saved_theme() string {
	path := get_theme_config_path()
	if os.exists(path) {
		val := os.read_file(path) or { '' }.trim_space()
		if val != '' {
			return val
		}
	}
	return 'GitHub Dark'
}

// save_theme persists the active theme preference to disk.
pub fn save_theme(theme_name string) bool {
	path := get_theme_config_path()
	dir := os.dir(path)
	if !os.exists(dir) {
		os.mkdir_all(dir) or { return false }
	}
	os.write_file(path, theme_name.trim_space()) or { return false }
	return true
}

// save_theme persists the chosen theme for the window and saves it to user preferences.
pub fn (win &SimpleWindow) save_theme(theme_name string) &SimpleWindow {
	save_theme(theme_name)
	return win
}

// restore_saved_theme loads and applies the persisted theme preference (defaults to GitHub Dark).
pub fn (win &SimpleWindow) restore_saved_theme() string {
	theme := get_saved_theme()
	win.set_theme(theme)
	return theme
}

// apply_theme applies a Theme struct configuration to the window and persists the selection.
pub fn (win &SimpleWindow) apply_theme(t Theme) &SimpleWindow {
	unsafe {
		mut w := &SimpleWindow(win)
		w.theme = t
	}
	win.set_background_color(t.background_color)
	win.set_font_color(t.font_color)
	save_theme(t.name)
	return win
}

// set_theme sets the window background and font colors based on built-in theme name or preset.
pub fn (win &SimpleWindow) set_theme(theme_name string) &SimpleWindow {
	t := get_theme(theme_name)
	return win.apply_theme(t)
}

// get_theme retrieves the current active Theme struct of the window.
pub fn (win &SimpleWindow) get_theme() Theme {
	return win.theme
}

// get_theme_name retrieves the current active theme name of the window.
pub fn (win &SimpleWindow) get_theme_name() string {
	return win.theme.name
}

// color sets the background color of the last created control.
pub fn (win &SimpleWindow) color(hex_color string) &SimpleWindow {
	if win.last_control != '' {
		win.set_control_background_color(win.last_control, hex_color)
	}
	return win
}

// font_color sets the font color of the last created control.
pub fn (win &SimpleWindow) font_color(hex_color string) &SimpleWindow {
	if win.last_control != '' {
		win.set_control_font_color(win.last_control, hex_color)
	}
	return win
}

// is_system_dark_mode returns true if macOS system dark mode is active.
pub fn (win &SimpleWindow) is_system_dark_mode() bool {
	if win.window_info != unsafe { nil } {
		return C.window_is_system_dark_mode(win.window_info) == 1
	}
	return false
}

// ── Screen Info ──────────────────────────────────────────────────────────────

// get_screen_frame returns the usable (visible) frame of the screen containing this window,
// excluding the Dock and menu bar. Returns (x, y, width, height).
pub fn (win &SimpleWindow) get_screen_frame() (int, int, int, int) {
	if win.window_info != unsafe { nil } {
		mut x, mut y, mut w, mut h := 0, 0, 0, 0
		C.window_get_screen_frame(win.window_info, &x, &y, &w, &h)
		return x, y, w, h
	}
	return 0, 0, 0, 0
}
