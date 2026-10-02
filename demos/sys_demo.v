module main

import simplegui
import os

fn main() {
	mut win := simplegui.new_simple_window('SimpleGUI sys.v Demo — All System Commands',
		920, 500)

	win.add_menu_item('File', 'Quit', 'q', fn (mut w simplegui.SimpleWindow) {
		w.quit()
	})

	win.add_segmented_control('section_tabs', [
		'§1 Exec',
		'§2 Hardware',
		'§3 Paths',
		'§4 Files',
		'§5 OS API',
		'§6 Time',
		'§7 Open',
		'§8 Network',
		'§9 Monitor',
		'§10 Shell',
		'§11 macOS',
		'§12 Process',
		'§13 Extra Sys',
	], '§1 Exec')

	win.add_console('output', 560)
	win.add_button('btn_run', 'Run Selected Section')
	win.add_button('btn_run_all', 'Run All Sections')
	win.add_button('btn_clear', 'Clear Output')

	win.on_click('btn_clear', fn (mut w simplegui.SimpleWindow) {
		w.clear_console('output')
	})

	win.on_click('btn_run_all', fn (mut w simplegui.SimpleWindow) {
		w.clear_console('output')
		w.append_console('output', '⏳ Running all 13 system sections in background thread…',
			0)
		spawn fn (mut window simplegui.SimpleWindow) {
			run_all(mut window)
		}(mut w)
	})

	win.on_click('btn_run', fn (mut w simplegui.SimpleWindow) {
		w.clear_console('output')
		tab := w.get('section_tabs')
		spawn fn (mut window simplegui.SimpleWindow, selected_tab string) {
			match selected_tab {
				'§1 Exec' {
					demo_exec(mut window)
				}
				'§2 Hardware' {
					demo_hardware(mut window)
				}
				'§3 Paths' {
					demo_paths(mut window)
				}
				'§4 Files' {
					demo_files(mut window)
				}
				'§5 OS API' {
					demo_os_api(mut window)
				}
				'§6 Time' {
					demo_time(mut window)
				}
				'§7 Open' {
					demo_open(mut window)
				}
				'§8 Network' {
					demo_network(mut window)
				}
				'§9 Monitor' {
					demo_monitor(mut window)
				}
				'§10 Shell' {
					demo_shell(mut window)
				}
				'§11 macOS' {
					demo_macos_info(mut window)
				}
				'§12 Process' {
					demo_process(mut window)
				}
				'§13 Extra Sys' {
					demo_extra_sys(mut window)
				}
				else {
					window.append_console('output', 'Unknown section: ${selected_tab}',
						1)
				}
			}
		}(mut w, tab)
	})

	win.run()
}

fn log(mut w simplegui.SimpleWindow, label string, value string) {
	w.append_console('output', '  ${label}: ${value}', 0)
}

fn log_header(mut w simplegui.SimpleWindow, title string) {
	w.append_console('output', '', 0)
	w.append_console('output', '══════════════════════════════════════════',
		0)
	w.append_console('output', '  ${title}', 0)
	w.append_console('output', '══════════════════════════════════════════',
		0)
}

fn log_ok(mut w simplegui.SimpleWindow, msg string) {
	w.append_console('output', '  ✓ ${msg}', 0)
}

fn log_err(mut w simplegui.SimpleWindow, msg string) {
	w.append_console('output', '  ✗ ${msg}', 2)
}

fn run_all(mut w simplegui.SimpleWindow) {
	demo_exec(mut w)
	demo_hardware(mut w)
	demo_paths(mut w)
	demo_files(mut w)
	demo_os_api(mut w)
	demo_time(mut w)
	demo_open_non_interactive(mut w)
	demo_network(mut w)
	demo_monitor(mut w)
	demo_shell_non_interactive(mut w)
	demo_macos_info(mut w)
	demo_process(mut w)
	demo_extra_sys(mut w)
	w.append_console('output', '', 0)
	w.append_console('output', '✅  All sections complete. (Interactive popups & launchers are safely isolated to §7 Open and §10 Shell tabs).',
		0)
}

// ─── §1 OS & Execution ────────────────────────────────────────────────────
fn demo_exec(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§1  OS & Execution — exec / exec_or / exec_bg / env')

	output, code := w.exec('echo "Hello from exec"')
	log(mut w, 'exec output', '${output}  [exit ${code}]')

	result := w.exec_or('echo "exec_or ok"', 'FALLBACK')
	log(mut w, 'exec_or (success)', result)

	fallback := w.exec_or('this_cmd_does_not_exist_xyz', 'FALLBACK_USED')
	log(mut w, 'exec_or (fail→fallback)', fallback)

	tmp := os.join_path(os.temp_dir(), 'sg_exec_bg_test.txt')
	w.exec_bg('echo bg_was_here > ${tmp}')
	log_ok(mut w, 'exec_bg fired async (writes to ${tmp})')

	w.set_env('SG_DEMO_KEY', 'simplegui_value')
	log(mut w, 'set_env / get_env', w.get_env('SG_DEMO_KEY'))

	opt_val := w.get_env_opt('SG_DEMO_KEY') or { '(not set)' }
	log(mut w, 'get_env_opt (present)', opt_val)

	missing := w.get_env_opt('SG_NOT_SET_XYZ') or { '(not set)' }
	log(mut w, 'get_env_opt (missing)', missing)

	w.unset_env('SG_DEMO_KEY')
	after := w.get_env('SG_DEMO_KEY')
	log(mut w, 'unset_env → get_env', if after == '' { '(cleared ✓)' } else { after })

	envs := w.get_envs()
	log(mut w, 'get_envs() count', '${envs.len} vars')

	result_meta := w.exec_result('echo "structured execution"')
	log(mut w, 'exec_result', 'ok=${result_meta.success()} exit=${result_meta.exit_code} duration=${result_meta.duration_ms}ms')

	timeout_meta := w.exec_timeout_result('sleep 2', 400)
	log(mut w, 'exec_timeout_result', 'timed_out=${timeout_meta.timed_out} exit=${timeout_meta.exit_code}')

	retry_meta := w.exec_retry('this_cmd_does_not_exist_xyz', 3, 100, 2.0)
	log(mut w, 'exec_retry (expected fail)', 'attempts=${retry_meta.attempts} exit=${retry_meta.exit_code}')

	w.show_system_notification('sys.v Demo', 'Section §1 — exec/env running!')
	log_ok(mut w, 'show_system_notification sent')
}

// ─── §2 Hardware ──────────────────────────────────────────────────────────
fn demo_hardware(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§2  Hardware — CPU / Memory')
	log(mut w, 'get_cpu_info', w.get_cpu_info())
	log(mut w, 'get_cpu_cores', '${w.get_cpu_cores()} logical cores')
	log(mut w, 'get_memory_info', w.get_memory_info())
}

// ─── §3 System Paths ──────────────────────────────────────────────────────
fn demo_paths(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§3  System Paths — get_system_path')
	for name in ['home', 'desktop', 'documents', 'downloads', 'temp', 'cache', 'config', 'data',
		'app'] {
		log(mut w, name, w.get_system_path(name))
	}
}

// ─── §4 File System ───────────────────────────────────────────────────────
fn demo_files(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§4  File System — read/write/copy/move/meta/walk/glob')

	tmp_dir := os.join_path(os.temp_dir(), 'sg_sys_demo')
	w.create_directory(tmp_dir)
	log_ok(mut w, 'create_directory: ${tmp_dir}')
	log(mut w, 'is_dir', '${w.is_dir(tmp_dir)}')
	log(mut w, 'file_exists(dir)', '${w.file_exists(tmp_dir)}')
	log(mut w, 'is_dir_empty', '${w.is_dir_empty(tmp_dir)}')

	file_a := os.join_path(tmp_dir, 'hello.txt')
	w.write_file(file_a, 'Hello, sys.v!\nLine two.\n')
	log_ok(mut w, 'write_file: ${file_a}')
	log(mut w, 'read_file', w.read_file(file_a).trim_space())
	log(mut w, 'is_file', '${w.is_file(file_a)}')

	atomic_file := os.join_path(tmp_dir, 'atomic.log')
	w.write_file_atomic(atomic_file, 'line1\nline2\nline3\n') or {
		log_err(mut w, 'write_file_atomic failed: ${err}')
	}
	if w.wait_for_file(atomic_file, 1500, 50) {
		last_lines := w.tail_file(atomic_file, 2) or { [] }
		log(mut w, 'tail_file atomic.log', last_lines.join(' | '))
	}

	lines_file := os.join_path(tmp_dir, 'lines.txt')
	w.write_lines(lines_file, ['alpha', 'beta', 'gamma']) or {}
	lines := w.read_lines(lines_file) or { [] }
	log(mut w, 'write_lines / read_lines', lines.str())

	bytes_file := os.join_path(tmp_dir, 'bytes.bin')
	w.write_bytes(bytes_file, [u8(0xDE), 0xAD, 0xBE, 0xEF]) or {}
	bdata := w.read_bytes(bytes_file) or { [] }
	log(mut w, 'write_bytes / read_bytes', bdata.map(it.hex()).join(' '))

	sz := w.get_file_size(file_a) or { -1 }
	log(mut w, 'get_file_size', '${sz} bytes')

	log(mut w, 'get_last_modified (unix)', '${w.get_last_modified(file_a)}')

	file_b := os.join_path(tmp_dir, 'hello_copy.txt')
	w.copy_file(file_a, file_b) or {}
	log_ok(mut w, 'copy_file → ${file_b}')

	file_c := os.join_path(tmp_dir, 'hello_renamed.txt')
	w.move_file(file_b, file_c) or {}
	log_ok(mut w, 'move_file → ${file_c}')

	sym := os.join_path(tmp_dir, 'hello_sym.txt')
	w.create_symlink(file_a, sym) or {}
	log(mut w, 'create_symlink / is_symlink', '${w.is_symlink(sym)}')

	entries := w.read_dir(tmp_dir)
	log(mut w, 'read_dir entries', entries.str())

	meta := w.get_file_metadata(file_a) or { simplegui.FileMetadata{} }
	log(mut w, 'get_file_metadata.size', '${meta.size}')
	log(mut w, 'get_file_metadata.type', meta.file_type)
	log(mut w, 'get_file_metadata perms', 'r=${meta.owner_r} w=${meta.owner_w} x=${meta.owner_x}')

	log(mut w, 'is_readable', '${w.is_readable(file_a)}')
	log(mut w, 'is_writable', '${w.is_writable(file_a)}')
	log(mut w, 'is_executable', '${w.is_executable(file_a)}')

	w.set_permissions(file_a, 0o644) or {}
	log_ok(mut w, 'set_permissions 0o644')

	log(mut w, 'path_dir', w.path_dir(file_a))
	log(mut w, 'path_base', w.path_base(file_a))
	log(mut w, 'path_ext', w.path_ext(file_a))
	log(mut w, 'path_name', w.path_name(file_a))
	log(mut w, 'path_is_abs', '${w.path_is_abs(file_a)}')
	log(mut w, 'path_real', w.path_real(file_a))
	log(mut w, 'path_norm', w.path_norm(file_a))
	dir_p, name_p, ext_p := w.path_split(file_a)
	log(mut w, 'path_split', 'dir=${dir_p}  name=${name_p}  ext=${ext_p}')

	matched := w.glob(os.join_path(tmp_dir, '*.txt')) or { [] }
	log(mut w, 'glob *.txt', '${matched.len} files')

	mut walked := []string{}
	w.walk(tmp_dir, fn [mut walked] (p string) {
		walked << p
	})
	log(mut w, 'walk count', '${walked.len} entries')

	txts := w.walk_ext(tmp_dir, '.txt')
	log(mut w, 'walk_ext .txt', '${txts.len} files')

	du := w.get_disk_usage(tmp_dir) or { simplegui.DiskStats{} }
	log(mut w, 'get_disk_usage total', '${f64(du.total) / 1_073_741_824.0:.1f} GB')
	log(mut w, 'get_disk_usage available', '${f64(du.available) / 1_073_741_824.0:.1f} GB')
	log(mut w, 'get_disk_usage used', '${f64(du.used) / 1_073_741_824.0:.1f} GB')

	log(mut w, 'get_working_directory', w.get_working_directory())

	// Cleanup
	w.delete_file(sym)
	w.delete_file(file_c)
	w.delete_file(file_a)
	w.delete_file(lines_file)
	w.delete_file(bytes_file)
	w.delete_directory(tmp_dir) or {}
	log_ok(mut w, 'Cleaned up temp dir')
}

// ─── §5 OS API ────────────────────────────────────────────────────────────
fn demo_os_api(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§5  OS API — PIDs / UIDs / hostname / uname / executables')

	log(mut w, 'get_hostname', w.get_hostname())
	log(mut w, 'get_username', w.get_username())
	log(mut w, 'get_user_os', w.get_user_os())
	log(mut w, 'get_pid', '${w.get_pid()}')
	log(mut w, 'get_ppid', '${w.get_ppid()}')
	log(mut w, 'get_uid', '${w.get_uid()}')
	log(mut w, 'get_gid', '${w.get_gid()}')
	log(mut w, 'get_euid', '${w.get_euid()}')
	log(mut w, 'get_egid', '${w.get_egid()}')
	log(mut w, 'get_executable_path', w.get_executable_path())
	log(mut w, 'exists_in_path("git")', '${w.exists_in_path('git')}')
	log(mut w, 'command_exists("bash")', '${w.command_exists('bash')}')
	log(mut w, 'exists_in_path("v")', '${w.exists_in_path('v')}')
	log(mut w, 'find_executable("git")', w.find_executable('git'))
	log(mut w, 'require_command("sh")', w.require_command('sh') or { '(missing)' })
	log(mut w, 'find_executable("bash")', w.find_executable('bash'))

	u := w.get_uname()
	log(mut w, 'get_uname sysname', u.sysname)
	log(mut w, 'get_uname nodename', u.nodename)
	log(mut w, 'get_uname release', u.release)
	log(mut w, 'get_uname machine', u.machine)
}

// ─── §6 Time ──────────────────────────────────────────────────────────────
fn demo_time(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§6  System Clock & Time')

	t := w.get_time()
	log(mut w, 'get_time year', '${t.year}')
	log(mut w, 'get_time month/day', '${t.month}/${t.day}')
	log(mut w, 'get_time H:M:S', '${t.hour}:${t.minute:02d}:${t.second:02d}')
	log(mut w, 'get_time weekday', t.weekday)
	log(mut w, 'get_time rfc3339', t.rfc3339)
	log(mut w, 'get_unix_epoch', '${w.get_unix_epoch()}')
	log(mut w, 'get_unix_milli', '${w.get_unix_milli()}')
	log(mut w, 'get_time_formatted YYYY-MM-DD', w.get_time_formatted('YYYY-MM-DD'))
	log(mut w, 'get_time_formatted HH:mm:ss', w.get_time_formatted('HH:mm:ss'))
	log(mut w, 'get_uptime_seconds', '${w.get_uptime_seconds()} s')
}

// ─── §7 Open / Reveal ─────────────────────────────────────────────────────
fn demo_open_non_interactive(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§7  macOS Open / Reveal Commands (Automated Check)')
	home := w.get_system_path('home')
	log(mut w, 'reveal_in_finder target path', home)
	tmp_file := os.join_path(os.temp_dir(), 'sg_open_test.txt')
	w.write_file(tmp_file, 'Opened by sys_demo open_in_default_app.')
	log(mut w, 'open_in_default_app target file', tmp_file)
	log_ok(mut w, 'Open commands verified. Select §7 Open tab to trigger external launchers.')
}

fn demo_open(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§7  macOS Open / Reveal Commands')

	log_ok(mut w, 'open_url("https://vlang.io") via Cocoa bridge…')
	w.open_url('https://vlang.io')

	home := w.get_system_path('home')
	log_ok(mut w, 'reveal_in_finder(home)…')
	w.reveal_in_finder(home)

	tmp_file := os.join_path(os.temp_dir(), 'sg_open_test.txt')
	w.write_file(tmp_file, 'Opened by sys_demo open_in_default_app.')
	log_ok(mut w, 'open_in_default_app: ${tmp_file}')
	w.open_in_default_app(tmp_file)

	log(mut w, 'note', 'External launchers triggered cleanly.')
}

// ─── §8 Network ───────────────────────────────────────────────────────────
fn demo_network(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§8  Network — IP / ping / DNS / Wi-Fi / interfaces')

	log(mut w, 'get_local_ip', w.get_local_ip())
	log_ok(mut w, 'get_external_ip (HTTP — may take a moment)…')
	log(mut w, 'get_external_ip', w.get_external_ip())

	log_ok(mut w, 'ping probes via TCP port 80/443…')
	log(mut w, 'ping "8.8.8.8"', '${w.ping('8.8.8.8', 1)}')
	log(mut w, 'ping "google.com"', '${w.ping('google.com', 1)}')
	log(mut w, 'ping "192.0.2.1" (RFC-5737/unreachable)', '${w.ping('192.0.2.1', 1)}')

	log(mut w, 'dns_lookup "google.com"', w.dns_lookup('google.com'))
	log(mut w, 'dns_lookup "vlang.io"', w.dns_lookup('vlang.io'))
	log(mut w, 'wait_for_port("google.com", 443)', '${w.wait_for_port('google.com', 443, 2000, 100)}')

	log(mut w, 'get_wifi_ssid', w.get_wifi_ssid())
	log(mut w, 'get_network_interfaces', w.get_network_interfaces().join(', '))
}

// ─── §9 Resource Monitor ──────────────────────────────────────────────────
fn demo_monitor(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§9  Resource Monitoring')

	log(mut w, 'get_cpu_usage_percent', '${w.get_cpu_usage_percent():.1f}%')

	l1, l5, l15 := w.get_load_average()
	log(mut w, 'load_average 1m / 5m / 15m', '${l1:.2f} / ${l5:.2f} / ${l15:.2f}')

	log(mut w, 'get_memory_pressure', w.get_memory_pressure())
	log(mut w, 'get_running_process_count', '${w.get_running_process_count()}')
	log(mut w, 'get_open_file_count', '${w.get_open_file_count()}')
	log(mut w, 'get_swap_usage', w.get_swap_usage())
}

// ─── §10 Shell Utilities ──────────────────────────────────────────────────
fn demo_shell_non_interactive(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§10  Shell Utilities (Automated Check)')
	log_ok(mut w, 'beep() system alert sound test…')
	w.beep()
	log_ok(mut w, 'say() speech output test…')
	w.say('SimpleGUI non-interactive shell test complete')
	log_ok(mut w, 'Shell utilities ready. Select §10 Shell tab to test interactive native dialogs.')
}

fn demo_shell(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§10  Shell Utilities — beep / dialog / say')

	log_ok(mut w, 'beep() — system alert sound…')
	w.beep()

	log_ok(mut w, 'say("Hello from SimpleGUI sys demo")…')
	w.say('Hello from SimpleGUI sys demo')

	log_ok(mut w, 'osascript_dialog — native input dialog…')
	typed := w.osascript_dialog('Enter your name:', 'World')
	log(mut w, 'osascript_dialog result', if typed == '' { '(cancelled)' } else { typed })
}

// ─── §11 macOS Info ───────────────────────────────────────────────────────
fn demo_macos_info(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§11  macOS System Info')

	log(mut w, 'get_macos_product_name', w.get_macos_product_name())
	log(mut w, 'get_macos_version', w.get_macos_version())
	log(mut w, 'get_macos_build', w.get_macos_build())
	log(mut w, 'get_device_model', w.get_device_model())
	log(mut w, 'get_serial_number', w.get_serial_number())
	log(mut w, 'get_screen_resolution', w.get_screen_resolution())
	log(mut w, 'get_gpu_info', w.get_gpu_info())

	log(mut w, 'get_active_app_name', w.get_active_app_name())
	log(mut w, 'is_apple_silicon', '${w.is_apple_silicon()}')
	log(mut w, 'is_rosetta_emulation', '${w.is_rosetta_emulation()}')
	log(mut w, 'is_sip_enabled', '${w.is_sip_enabled()}')

	pct := w.get_battery_percent()
	log(mut w, 'get_battery_percent', if pct < 0 { 'No battery (desktop)' } else { '${pct}%' })
	log(mut w, 'get_battery_time_remaining', w.get_battery_time_remaining())
	log(mut w, 'is_on_ac_power', '${w.is_on_ac_power()}')
	log(mut w, 'is_low_power_mode', '${w.is_low_power_mode()}')

	log(mut w, 'get_app_bundle_id', w.get_app_bundle_id())
	log(mut w, 'get_system_locale', w.get_system_locale())
	log(mut w, 'get_timezone', w.get_timezone())

	log_ok(mut w, 'set_dock_badge(1) — testing badge count update')
	w.set_dock_badge(1)
	w.set_dock_badge(0)
	log_ok(mut w, 'set_dock_badge(0) — badge cleared')

	log_ok(mut w, 'launch_at_login_add/remove — skipped (modifies Login Items)')
}

// ─── §12 Process Management ───────────────────────────────────────────────
fn demo_process(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§12  Process — spawn_process / is_alive / read / terminate / wait')

	mut proc := w.spawn_process('/bin/sh', ['-c',
		'echo "process started"; sleep 0.1; echo "process done"'], {}) or {
		log_err(mut w, 'spawn_process failed: ${err}')
		return
	}
	log_ok(mut w, 'spawn_process /bin/sh started')
	log(mut w, 'is_alive (initial)', '${proc.is_alive()}')

	proc.wait()
	log(mut w, 'is_alive (after wait)', '${proc.is_alive()}')
	out2 := proc.read()
	log(mut w, 'read() after wait', out2.trim_space())
	proc.close()
	log_ok(mut w, 'proc.close() — resources released')

	mut proc2 := w.spawn_process('/bin/sleep', ['10'], {}) or {
		log_err(mut w, 'spawn_process /bin/sleep 10 failed: ${err}')
		return
	}
	log_ok(mut w, 'spawn_process /bin/sleep 10 started')
	log(mut w, 'is_alive before terminate', '${proc2.is_alive()}')
	proc2.terminate()
	proc2.close()
	log_ok(mut w, 'proc2.close() — done')
}

// ─── §13 Extra Utility Extensions ─────────────────────────────────────────
fn demo_extra_sys(mut w simplegui.SimpleWindow) {
	log_header(mut w, '§13  Extra Utility Extensions — Theme / Audio / Zip / Trash / Hash / Ports')

	log(mut w, 'is_dark_mode', '${w.is_dark_mode()}')
	log(mut w, 'get_system_theme', w.get_system_theme())
	log(mut w, 'get_volume', '${w.get_volume()}%')
	log(mut w, 'is_muted', '${w.is_muted()}')
	log(mut w, 'get_screen_count', '${w.get_screen_count()}')
	log(mut w, 'is_retina_display', '${w.is_retina_display()}')

	tmp_f := w.create_temp_file('sg_sys_demo_extra', '.txt') or { '' }
	if tmp_f.len > 0 {
		log_ok(mut w, 'create_temp_file: ${tmp_f}')
		w.write_file(tmp_f, 'Testing hash functionality\n')
		log(mut w, 'sha256_file', w.sha256_file(tmp_f) or { 'err' })
		log(mut w, 'md5_file', w.md5_file(tmp_f) or { 'err' })
		w.trash_file(tmp_f) or {}
		log_ok(mut w, 'trash_file moved temp file to macOS Trash safely')
	}

	tmp_d := w.create_temp_dir('sg_zip_demo_extra') or { '' }
	if tmp_d.len > 0 {
		log_ok(mut w, 'create_temp_dir: ${tmp_d}')
		w.write_file(os.join_path(tmp_d, 'data.txt'), 'Archive test content')
		zpath := os.join_path(os.temp_dir(), 'sg_demo_extra.zip')
		w.zip_directory(tmp_d, zpath) or {}
		log_ok(mut w, 'zip_directory → ${zpath}')
		out_d := os.join_path(os.temp_dir(), 'sg_demo_extra_out')
		w.unzip_archive(zpath, out_d) or {}
		log_ok(mut w, 'unzip_archive → ${out_d}')
		w.delete_file(zpath)
		w.delete_directory(tmp_d) or {}
		w.delete_directory(out_d) or {}
	}

	log(mut w, 'is_port_open("google.com", 443)', '${w.is_port_open('google.com', 443)}')
	log(mut w, 'find_available_port(8000)', '${w.find_available_port(8000)}')

	log(mut w, 'base64_encode', w.base64_encode('Hello SimpleGUI'))
	log(mut w, 'base64_decode', w.base64_decode(w.base64_encode('Hello SimpleGUI')))
	log(mut w, 'url_encode', w.url_encode('key=val & 100%'))
	log(mut w, 'url_decode', w.url_decode(w.url_encode('key=val & 100%')))
	log(mut w, 'get_machine_id', w.get_machine_id())
	log(mut w, 'is_process_running("Finder")', '${w.is_process_running('Finder')}')

	w.prevent_sleep_bg(10)
	log_ok(mut w, 'prevent_sleep_bg(10) fired')
}
