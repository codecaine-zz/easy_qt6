module main

import simplegui

fn test_system_info_apis() {
	// macOS version
	ver := simplegui.get_macos_version()
	assert ver.starts_with('macOS')

	maj := simplegui.get_macos_version_major()
	assert maj >= 10

	comp_name := simplegui.get_computer_name()
	assert comp_name.len > 0

	user_name := simplegui.get_user_full_name()
	assert user_name.len > 0

	uptime := simplegui.get_system_uptime_seconds()
	assert uptime > 0.0

	// Silicon check returns bool without crashing
	is_silicon := simplegui.is_apple_silicon()
	assert is_silicon == true || is_silicon == false
}

fn test_user_defaults() {
	key := 'test_simplegui_key'
	simplegui.defaults_set_string(key, 'hello_cocoa')
	assert simplegui.defaults_has(key) == true
	assert simplegui.defaults_get_string(key) == 'hello_cocoa'

	simplegui.defaults_set_bool('test_simplegui_bool', true)
	assert simplegui.defaults_get_bool('test_simplegui_bool') == true

	simplegui.defaults_set_int('test_simplegui_int', 42)
	assert simplegui.defaults_get_int('test_simplegui_int') == 42

	simplegui.defaults_remove(key)
	simplegui.defaults_remove('test_simplegui_bool')
	simplegui.defaults_remove('test_simplegui_int')
	assert simplegui.defaults_has(key) == false
}

fn test_screens_and_display() {
	screens := simplegui.get_screens()
	assert screens.len > 0

	main_screen := simplegui.get_main_screen()
	assert main_screen.width > 0
	assert main_screen.height > 0
	assert main_screen.scale >= 1.0
}

fn test_battery_and_power() {
	pct := simplegui.get_battery_percentage()
	assert pct >= 0.0 && pct <= 100.0

	time_left := simplegui.get_battery_time_remaining_minutes()
	assert time_left >= -1

	// Sleep prevention assertion test
	aid := simplegui.prevent_system_sleep('unit_test')
	if aid != 0 {
		simplegui.allow_system_sleep(aid)
	}
}

fn test_speech_voices() {
	def_voice := simplegui.get_default_speech_voice()
	assert def_voice.len > 0

	voices := simplegui.get_speech_voices()
	assert voices.len > 0
}

fn test_app_and_policy() {
	pol := simplegui.get_activation_policy()
	assert pol == 'regular' || pol == 'accessory' || pol == 'prohibited'

	apps := simplegui.get_running_apps()
	assert apps.len > 0

	front := simplegui.get_frontmost_app()
	assert front.len >= 0
}

fn test_haptics_and_sound() {
	simplegui.perform_haptic_feedback('generic')
	simplegui.perform_haptic_feedback('alignment')
	simplegui.perform_haptic_feedback('level_change')

	assert simplegui.is_sound_file_playing() == false
	simplegui.stop_sound_file()
}
