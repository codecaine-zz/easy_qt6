module simplegui

import json2

// MacScreenInfo describes a connected display monitor.
pub struct MacScreenInfo {
pub:
	index          int
	x              int
	y              int
	width          int
	height         int
	visible_width  int
	visible_height int
	scale          f64
	is_main        bool
}

// MacAppInfo describes a running application process.
pub struct MacAppInfo {
pub:
	pid       int
	name      string
	bundle_id string
	is_active bool
	is_hidden bool
}

// SpeechVoice represents an NSSpeechSynthesizer voice installed on macOS.
pub struct SpeechVoice {
pub:
	id   string
	name string
	lang string
}

// ============================================================================
// SimpleWindow Fluent Extensions
// ============================================================================

// move_to_screen centers the window on the display at screen_index.
pub fn (win &SimpleWindow) move_to_screen(screen_index int) &SimpleWindow {
	if win.window_info != unsafe { nil } {
		C.window_move_to_screen(win.window_info, screen_index)
	}
	return win
}

// haptic_feedback generates trackpad haptic feedback ("generic", "alignment", "level_change").
pub fn (win &SimpleWindow) haptic_feedback(pattern string) &SimpleWindow {
	C.window_perform_haptic_feedback(pattern.str)
	return win
}

// speak_native speaks the given text aloud using macOS NSSpeechSynthesizer.
// Optional voice can be an empty string for the system default voice.
pub fn (win &SimpleWindow) speak_native(text string, voice string) &SimpleWindow {
	C.window_speech_speak(text.str, voice.str)
	return win
}

// stop_speech immediately stops any ongoing text-to-speech output.
pub fn (win &SimpleWindow) stop_speech() &SimpleWindow {
	C.window_speech_stop()
	return win
}

// is_speaking returns true if text-to-speech is currently playing.
pub fn (win &SimpleWindow) is_speaking() bool {
	return C.window_speech_is_speaking() == 1
}

// play_sound_file plays an audio file asynchronously via NSSound.
pub fn (win &SimpleWindow) play_sound_file(file_path string) bool {
	return C.window_play_sound_file(file_path.str) == 1
}

// stop_sound_file stops the currently playing sound file.
pub fn (win &SimpleWindow) stop_sound_file() &SimpleWindow {
	C.window_stop_sound_file()
	return win
}

// is_sound_file_playing returns true if an NSSound is actively playing.
pub fn (win &SimpleWindow) is_sound_file_playing() bool {
	return C.window_is_sound_file_playing() == 1
}

// recycle_to_trash moves a file or folder safely to the macOS Trash using NSFileManager.
pub fn (win &SimpleWindow) recycle_to_trash(file_path string) bool {
	return C.window_recycle_to_trash(file_path.str) == 1
}

// ============================================================================
// Package-Level macOS System APIs
// ============================================================================

// --- Text-to-Speech (NSSpeechSynthesizer) ---

// speak_native speaks text using the macOS default speech synthesizer voice.
pub fn speak_native(text string) {
	C.window_speech_speak(text.str, c'')
}

// speak_native_with_voice speaks text using a specific voice name (e.g. "Samantha", "Alex").
pub fn speak_native_with_voice(text string, voice string) {
	C.window_speech_speak(text.str, voice.str)
}

// stop_speech stops any active text-to-speech playback.
pub fn stop_speech() {
	C.window_speech_stop()
}

// is_speaking returns true if text-to-speech is currently active.
pub fn is_speaking() bool {
	return C.window_speech_is_speaking() == 1
}

// get_speech_voices returns the list of all available macOS voices with their identifiers and locales.
pub fn get_speech_voices() []SpeechVoice {
	res := C.window_speech_get_voices()
	if res == unsafe { nil } {
		return []
	}
	s := unsafe { tos3(res) }
	if s == '' || s == '[]' {
		return []
	}
	voices := json2.decode[[]SpeechVoice](s) or { return []SpeechVoice{} }
	return voices
}

// get_default_speech_voice returns the name of the system default speech synthesizer voice.
pub fn get_default_speech_voice() string {
	res := C.window_speech_get_default_voice()
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}

// --- Trackpad Haptics (NSHapticFeedbackManager) ---

// perform_haptic_feedback triggers physical trackpad feedback ("generic", "alignment", "level_change").
pub fn perform_haptic_feedback(pattern string) {
	C.window_perform_haptic_feedback(pattern.str)
}

// --- Pasteboard / Clipboard (NSPasteboard) ---

// copy_image_to_clipboard reads an image file from disk and puts it on the system pasteboard.
pub fn copy_image_to_clipboard(path string) bool {
	return C.window_clipboard_copy_image(path.str) == 1
}

// get_clipboard_image saves the image currently on the pasteboard to dest_png_path.
pub fn get_clipboard_image(dest_png_path string) bool {
	return C.window_clipboard_get_image(dest_png_path.str) == 1
}

// copy_files_to_clipboard places a list of file paths onto the pasteboard (e.g. for Finder pasting).
pub fn copy_files_to_clipboard(paths []string) bool {
	if paths.len == 0 {
		return false
	}
	mut c_paths := []&u8{}
	for p in paths {
		c_paths << p.str
	}
	return C.window_clipboard_copy_files(c_paths.data, c_paths.len) == 1
}

// get_clipboard_files returns any file paths copied to the pasteboard.
pub fn get_clipboard_files() []string {
	res := C.window_clipboard_get_files()
	if res == unsafe { nil } {
		return []
	}
	s := unsafe { tos3(res) }
	if s == '' || s == '[]' {
		return []
	}
	paths := json2.decode[[]string](s) or { return []string{} }
	return paths
}

// clear_clipboard clears the macOS general pasteboard.
pub fn clear_clipboard() {
	C.window_clipboard_clear()
}

// get_clipboard_change_count returns the pasteboard change counter (useful to detect clipboard updates).
pub fn get_clipboard_change_count() int {
	return C.window_clipboard_get_change_count()
}

// --- App & Workspace Management (NSWorkspace) ---

// recycle_to_trash moves the specified file or folder to the macOS Trash.
pub fn recycle_to_trash(path string) bool {
	return C.window_recycle_to_trash(path.str) == 1
}

// get_running_apps returns a list of all running macOS applications with their pid, name, and bundle ID.
pub fn get_running_apps() []MacAppInfo {
	res := C.window_get_running_apps()
	if res == unsafe { nil } {
		return []
	}
	s := unsafe { tos3(res) }
	if s == '' || s == '[]' {
		return []
	}
	apps := json2.decode[[]MacAppInfo](s) or { return []MacAppInfo{} }
	return apps
}

// activate_app brings an application to the foreground by bundle identifier or localized name.
pub fn activate_app(bundle_id_or_name string) bool {
	return C.window_activate_app(bundle_id_or_name.str) == 1
}

// terminate_app requests termination of an application process by pid.
pub fn terminate_app(pid int) bool {
	return C.window_terminate_app(pid) == 1
}

// hide_other_apps hides all applications except the current one.
pub fn hide_other_apps() {
	C.window_hide_other_apps()
}

// unhide_all_apps unhides all previously hidden applications.
pub fn unhide_all_apps() {
	C.window_unhide_all_apps()
}

// get_frontmost_app returns the name of the currently active/frontmost application.
pub fn get_frontmost_app() string {
	res := C.window_get_frontmost_app()
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}

// open_with_app opens a file using a specific application (e.g. "TextEdit", "Safari").
pub fn open_with_app(file_path string, app_name string) bool {
	return C.window_open_with_app(file_path.str, app_name.str) == 1
}

// get_default_app_for_extension returns the file path of the default application registered for a file extension (e.g. "txt", "pdf").
pub fn get_default_app_for_extension(ext string) string {
	res := C.window_get_default_app_for_extension(ext.str)
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}

// --- Sound Playback (NSSound) ---

// play_sound_file plays a sound file (.wav, .aiff, .mp3) asynchronously.
pub fn play_sound_file(file_path string) bool {
	return C.window_play_sound_file(file_path.str) == 1
}

// stop_sound_file stops the currently playing sound.
pub fn stop_sound_file() {
	C.window_stop_sound_file()
}

// is_sound_file_playing returns true if a sound file is actively playing.
pub fn is_sound_file_playing() bool {
	return C.window_is_sound_file_playing() == 1
}

// --- Power, Battery & Sleep (IOKit / IOPMAssertion) ---

// prevent_system_sleep prevents macOS from going to sleep or dimming the display.
// Returns an assertion ID that must later be passed to allow_system_sleep.
pub fn prevent_system_sleep(reason string) u32 {
	return C.window_prevent_sleep(reason.str)
}

// allow_system_sleep releases a power assertion created by prevent_system_sleep.
pub fn allow_system_sleep(assertion_id u32) {
	C.window_allow_sleep(assertion_id)
}

// get_battery_percentage returns the current battery level (0.0 to 100.0).
pub fn get_battery_percentage() f64 {
	return C.window_get_battery_percentage()
}

// is_battery_charging returns true if the Mac is currently connected to power and charging.
pub fn is_battery_charging() bool {
	return C.window_is_battery_charging() == 1
}

// is_on_battery_power returns true if the Mac is running on battery power (not connected to AC).
pub fn is_on_battery_power() bool {
	return C.window_is_on_battery_power() == 1
}

// get_battery_time_remaining_minutes returns the estimated minutes of battery life remaining, or -1 if calculating / AC power.
pub fn get_battery_time_remaining_minutes() int {
	return C.window_get_battery_time_remaining()
}

// --- User Defaults (NSUserDefaults) ---

// defaults_set_string stores a persistent key-value string in macOS NSUserDefaults.
pub fn defaults_set_string(key string, val string) {
	C.window_defaults_set_string(key.str, val.str)
}

// defaults_get_string retrieves a string from macOS NSUserDefaults.
pub fn defaults_get_string(key string) string {
	res := C.window_defaults_get_string(key.str)
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}

// defaults_set_bool stores a boolean in macOS NSUserDefaults.
pub fn defaults_set_bool(key string, val bool) {
	C.window_defaults_set_bool(key.str, if val { 1 } else { 0 })
}

// defaults_get_bool retrieves a boolean from macOS NSUserDefaults.
pub fn defaults_get_bool(key string) bool {
	return C.window_defaults_get_bool(key.str) == 1
}

// defaults_set_int stores an integer in macOS NSUserDefaults.
pub fn defaults_set_int(key string, val int) {
	C.window_defaults_set_int(key.str, val)
}

// defaults_get_int retrieves an integer from macOS NSUserDefaults.
pub fn defaults_get_int(key string) int {
	return C.window_defaults_get_int(key.str)
}

// defaults_remove deletes a key from macOS NSUserDefaults.
pub fn defaults_remove(key string) {
	C.window_defaults_remove(key.str)
}

// defaults_has returns true if a key exists in macOS NSUserDefaults.
pub fn defaults_has(key string) bool {
	return C.window_defaults_has(key.str) == 1
}

// --- macOS System Information ---

// get_macos_version returns the operating system version string (e.g. "macOS 14.4.1").
pub fn get_macos_version() string {
	res := C.window_get_macos_version_str()
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}

// get_macos_version_major returns the major macOS version (e.g. 14, 15).
pub fn get_macos_version_major() int {
	mut maj, mut min, mut patch := 0, 0, 0
	C.window_get_macos_version_numbers(&maj, &min, &patch)
	return maj
}

// get_macos_version_minor returns the minor macOS version.
pub fn get_macos_version_minor() int {
	mut maj, mut min, mut patch := 0, 0, 0
	C.window_get_macos_version_numbers(&maj, &min, &patch)
	return min
}

// get_macos_version_patch returns the patch macOS version.
pub fn get_macos_version_patch() int {
	mut maj, mut min, mut patch := 0, 0, 0
	C.window_get_macos_version_numbers(&maj, &min, &patch)
	return patch
}

// get_computer_name returns the localized name of the computer (as set in Sharing settings).
pub fn get_computer_name() string {
	res := C.window_get_computer_name()
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}

// get_user_full_name returns the full name of the currently logged-in macOS user.
pub fn get_user_full_name() string {
	res := C.window_get_user_full_name()
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}

// is_apple_silicon returns true if the application is executing on Apple Silicon (M1/M2/M3/M4/arm64).
pub fn is_apple_silicon() bool {
	return C.window_is_apple_silicon() == 1
}

// get_system_uptime_seconds returns the system uptime in seconds since boot.
pub fn get_system_uptime_seconds() f64 {
	return C.window_get_system_uptime()
}

// is_low_power_mode returns true if Low Power Mode is currently active on macOS 12+.
pub fn is_low_power_mode() bool {
	return C.window_is_low_power_mode() == 1
}

// --- Multi-Screen / Displays (NSScreen) ---

// get_screens returns information about all connected displays.
pub fn get_screens() []MacScreenInfo {
	res := C.window_get_screens_info()
	if res == unsafe { nil } {
		return []
	}
	s := unsafe { tos3(res) }
	if s == '' || s == '[]' {
		return []
	}
	screens := json2.decode[[]MacScreenInfo](s) or { return []MacScreenInfo{} }
	return screens
}

// get_main_screen returns information about the primary display screen.
pub fn get_main_screen() MacScreenInfo {
	screens := get_screens()
	for s in screens {
		if s.is_main {
			return s
		}
	}
	if screens.len > 0 {
		return screens[0]
	}
	return MacScreenInfo{}
}

// --- Dock & Application Activation Policy ---

// get_dock_badge returns the string badge currently set on the app's Dock icon.
pub fn get_dock_badge() string {
	res := C.window_get_dock_badge()
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}

// set_activation_policy configures the application activation policy ("regular", "accessory", "prohibited").
// "accessory" apps do not appear in the Dock or Cmd+Tab switcher (ideal for menu bar / status bar apps).
pub fn set_activation_policy(policy string) {
	C.window_set_activation_policy(policy.str)
}

// get_activation_policy returns the current activation policy ("regular", "accessory", "prohibited").
pub fn get_activation_policy() string {
	res := C.window_get_activation_policy()
	if res == unsafe { nil } {
		return 'regular'
	}
	return unsafe { tos3(res) }
}

// get_app_bundle_path returns the filesystem path of the application bundle.
pub fn get_app_bundle_path() string {
	res := C.window_get_app_bundle_path()
	if res == unsafe { nil } {
		return ''
	}
	return unsafe { tos3(res) }
}
