import os
import time

struct Candidate {
	rect  string
	title string
}

fn get_window_rect_for_pid(target_pid int, binary_name string) string {
	res := os.execute('./tools/list_windows')
	if res.exit_code != 0 {
		return ''
	}

	mut candidates := []Candidate{}
	lines := res.output.split_into_lines()
	for line in lines {
		trimmed := line.trim_space()
		if trimmed == '' {
			continue
		}
		parts := trimmed.split('|')
		if parts.len >= 5 {
			pid_part := parts[0].trim_space()
			owner_part := parts[1].trim_space()
			window_part := parts[2].trim_space()
			rect_part := parts[4].trim_space()

			pid_str := pid_part.replace('PID:', '').trim_space()
			pid_val := pid_str.int()
			owner_val := owner_part.replace('Owner:', '').trim_space()
			rect_str := rect_part.replace('Rect:', '').trim_space()

			if pid_val == target_pid {
				candidates << Candidate{
					rect:  rect_str
					title: window_part
				}
			} else if owner_val.to_lower().contains(binary_name.to_lower()) {
				candidates << Candidate{
					rect:  rect_str
					title: window_part
				}
			}
		}
	}

	if candidates.len > 0 {
		for candidate in candidates {
			win_title := candidate.title.replace('Window:', '').trim_space()
			if win_title.len > 0 {
				return candidate.rect
			}
		}
		return candidates[0].rect
	}
	return ''
}

fn main() {
	// Compile list_windows tool if needed
	if !os.exists('tools/list_windows') {
		println('Compiling list_windows helper...')
		clang_res := os.execute('clang -framework Cocoa -framework CoreGraphics tools/list_windows.m -o tools/list_windows')
		if clang_res.exit_code != 0 {
			eprintln('❌ Could not compile list_windows: ${clang_res.output}')
			exit(1)
		}
	}

	os.mkdir_all('screenshots') or {
		eprintln('❌ Failed to create screenshots directory: ${err}')
		exit(1)
	}

	app_dir := 'applications'
	mut all_files := []string{}
	if os.args.len > 1 {
		for i in 1 .. os.args.len {
			mut arg := os.args[i]
			if !arg.ends_with('.v') {
				arg = arg + '.v'
			}
			filename := os.file_name(arg)
			all_files << filename
		}
	} else {
		files := os.ls(app_dir) or {
			eprintln('❌ Failed to list applications: ${err}')
			exit(1)
		}
		for f in files {
			if f.ends_with('.v') && f != 'ifconfig_studio.v' {
				all_files << f
			}
		}
		all_files.sort()
	}

	println('Found ${all_files.len} applications to capture.')

	for i, filename in all_files {
		app_path := os.join_path(app_dir, filename)
		basename := filename.replace('.v', '')
		bin_target := 'bin/${basename}'

		println('\n[${i + 1}/${all_files.len}] Processing: ${basename}...')

		// Compile
		println('  Compiling ${app_path} -> ${bin_target}...')
		comp_res := os.execute('v -nocache -o ${os.quoted_path(bin_target)} ${os.quoted_path(app_path)}')
		if comp_res.exit_code != 0 {
			eprintln('❌ Compilation failed for ${basename}:\n${comp_res.output}')
			continue
		}

		if !os.exists(bin_target) {
			eprintln('❌ Executable not found at ${bin_target}')
			continue
		}

		// Run
		println('  Launching ${bin_target}...')
		mut proc := os.new_process('./${bin_target}')
		proc.run()

		// Wait for UI to render
		time.sleep(2000 * time.millisecond)

		// Retrieve window rect
		rect := get_window_rect_for_pid(proc.pid, basename)
		screenshot_path := 'screenshots/${basename}.png'

		if rect != '' {
			println('  Found Window Rect: ${rect}')
			println('  Capturing screenshot to ${screenshot_path}...')
			sc_res := os.execute('screencapture -x -R ${rect} ${os.quoted_path(screenshot_path)}')
			if sc_res.exit_code != 0 {
				eprintln('⚠️ screencapture exited with code ${sc_res.exit_code}:\n${sc_res.output}')
			}

			if os.exists(screenshot_path) {
				println('  ✅ Successfully captured ${screenshot_path}')
			} else {
				eprintln('  ❌ Failed to create screenshot file for ${basename}')
			}
		} else {
			eprintln('  ❌ Could not locate active window for ${basename} (PID ${proc.pid})')
		}

		// Terminate
		proc.signal_term()
		time.sleep(100 * time.millisecond)
		if proc.is_alive() {
			proc.signal_kill()
		}
		proc.close()

		// Cleanup binary
		if os.exists(bin_target) {
			os.rm(bin_target) or {}
		}
	}

	println('\n🎉 All application captures complete!')
}
