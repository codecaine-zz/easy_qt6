#!/usr/bin/env -S v run

import os
import time

struct AppConfig {
	display_name string
	icon_file    string
	bundle_id    string
}

struct IconSize {
	name string
	size int
}

struct Task {
	index        int
	total        int
	app_id       string
	src_path     string
	out_dir      string
	display_name string
	icon_file    string
	bundle_id    string
	is_prod      bool
	bin_only     bool
}

struct TaskResult {
	index        int
	total        int
	display_name string
	out_target   string
	success      bool
	elapsed_ms   i64
	size_mb      f64
	err_msg      string
}

fn get_app_maps() map[string]AppConfig {
	mut m := map[string]AppConfig{}
	m['api_studio.v'] = AppConfig{'API Studio', 'api_client.png', 'com.simplegui.apistudio'}
	m['app_bundler_studio.v'] = AppConfig{'App Bundler Studio', 'launcher.png', 'com.simplegui.appbundlerstudio'}
	m['audiotag_studio.v'] = AppConfig{'Audio Tag Studio', 'audio_editor.png', 'com.simplegui.audiotagstudio'}
	m['brew_studio.v'] = AppConfig{'Brew Studio', 'package_manager.png', 'com.simplegui.brewstudio'}
	m['crypto_studio.v'] = AppConfig{'Crypto Studio', 'security.png', 'com.simplegui.cryptostudio'}
	m['cut_studio.v'] = AppConfig{'Cut Studio', 'utility.png', 'com.simplegui.cutstudio'}
	m['dataconvert_studio.v'] = AppConfig{'Data Convert Studio', 'csv_editor.png', 'com.simplegui.dataconvertstudio'}
	m['disk_studio.v'] = AppConfig{'Disk Studio', 'disk_utility.png', 'com.simplegui.diskstudio'}
	m['dns_studio.v'] = AppConfig{'DNS Studio', 'network_analyzer.png', 'com.simplegui.dnsstudio'}
	m['docker_studio.v'] = AppConfig{'Docker Studio', 'docker_monitor.png', 'com.simplegui.dockerstudio'}
	m['dot_studio.v'] = AppConfig{'Graphviz Studio', 'diagram_maker.png', 'com.simplegui.graphvizstudio'}
	m['exif_studio.v'] = AppConfig{'Exif Studio', 'image_viewer.png', 'com.simplegui.exifstudio'}
	m['fd_studio.v'] = AppConfig{'FD Studio', 'file_manager.png', 'com.simplegui.fdstudio'}
	m['ffmpeg_studio.v'] = AppConfig{'FFmpeg Studio', 'video_editor.png', 'com.simplegui.ffmpegstudio'}
	m['find_studio.v'] = AppConfig{'Find Studio', 'file_manager.png', 'com.simplegui.findstudio'}
	m['gawk_studio.v'] = AppConfig{'GAWK Studio', 'snippet_manager.png', 'com.simplegui.gawkstudio'}
	m['graph_studio.v'] = AppConfig{'Graph Studio', 'drawing_board.png', 'com.simplegui.graphstudio'}
	m['ifconfig_studio.v'] = AppConfig{'IFConfig Studio', 'network_analyzer.png', 'com.simplegui.ifconfigstudio'}
	m['imagemagick_studio.v'] = AppConfig{'ImageMagick Studio', 'image_optimizer.png', 'com.simplegui.imagemagickstudio'}
	m['jq_studio.v'] = AppConfig{'JQ Studio', 'dom_explorer.png', 'com.simplegui.jqstudio'}
	m['kalker_studio.v'] = AppConfig{'Kalker Studio', 'calculator.png', 'com.simplegui.kalkerstudio'}
	m['launchd_studio.v'] = AppConfig{'Launchd Studio', 'task_scheduler.png', 'com.simplegui.launchdstudio'}
	m['media_studio_hub.v'] = AppConfig{'Media Studio Hub', 'media.png', 'com.simplegui.mediastudiohub'}
	m['nmap_studio.v'] = AppConfig{'Nmap Studio', 'security.png', 'com.simplegui.nmapstudio'}
	m['numbat_studio.v'] = AppConfig{'Numbat Studio', 'calculator.png', 'com.simplegui.numbatstudio'}
	m['ocr_studio.v'] = AppConfig{'OCR Studio', 'transcription.png', 'com.simplegui.ocrstudio'}
	m['omnitool_studio.v'] = AppConfig{'Omnitool Studio', 'developer.png', 'com.simplegui.omnitoolstudio'}
	m['ouch_studio.v'] = AppConfig{'Ouch Studio', 'archive_manager.png', 'com.simplegui.ouchstudio'}
	m['pandoc_studio.v'] = AppConfig{'Pandoc Studio', 'markdown_editor.png', 'com.simplegui.pandocstudio'}
	m['programmer_calculator.v'] = AppConfig{'Programmer Calculator', 'calculator.png', 'com.simplegui.programmercalculator'}
	m['qalc_studio.v'] = AppConfig{'Qalc Studio', 'calculator.png', 'com.simplegui.qalcstudio'}
	m['recon_studio.v'] = AppConfig{'Recon Studio', 'security.png', 'com.simplegui.reconstudio'}
	m['regex_studio.v'] = AppConfig{'Regex Studio', 'regex_tester.png', 'com.simplegui.regexstudio'}
	m['rg_studio.v'] = AppConfig{'RG Studio', 'snippet_manager.png', 'com.simplegui.rgstudio'}
	m['rip_studio.v'] = AppConfig{'Rip Studio', 'file_manager.png', 'com.simplegui.ripstudio'}
	m['say_studio.v'] = AppConfig{'Say Studio', 'voice_recorder.png', 'com.simplegui.saystudio'}
	m['sd_studio.v'] = AppConfig{'SD Studio', 'text_editor.png', 'com.simplegui.sdstudio'}
	m['sed_studio.v'] = AppConfig{'Sed Studio', 'text_editor.png', 'com.simplegui.sedstudio'}
	m['sqlite_studio.v'] = AppConfig{'SQLite Studio', 'database_admin.png', 'com.simplegui.sqlitestudio'}
	m['statistics_studio.v'] = AppConfig{'Statistics Studio', 'spreadsheet.png', 'com.simplegui.statisticsstudio'}
	m['subfinder_studio.v'] = AppConfig{'Subfinder Studio', 'network_analyzer.png', 'com.simplegui.subfinderstudio'}
	m['task_manager.v'] = AppConfig{'Task Manager', 'system_monitor.png', 'com.simplegui.taskmanager'}
	m['text_editor.v'] = AppConfig{'Text Editor', 'text_editor.png', 'com.simplegui.texteditor'}
	m['tr_studio.v'] = AppConfig{'TR Studio', 'utility.png', 'com.simplegui.trstudio'}
	m['watchexec_studio.v'] = AppConfig{'Watchexec Studio', 'task_scheduler.png', 'com.simplegui.watchexecstudio'}
	m['wget2_studio.v'] = AppConfig{'Wget2 Studio', 'cloud_storage.png', 'com.simplegui.wget2studio'}
	m['yt_dlp_studio.v'] = AppConfig{'YT-DLP Studio', 'screen_recorder.png', 'com.simplegui.ytdlpstudio'}
	return m
}

fn to_clean_identifier(raw string) string {
	mut clean := ''
	for c in raw {
		if c.is_alnum() {
			clean += c.ascii_str()
		}
	}
	return clean.to_lower()
}

fn format_friendly_name(raw string) string {
	mut result := []string{}
	mut cleaned := ''
	for c in raw {
		if c in [`-`, `_`, ` `] {
			cleaned += ' '
		} else {
			cleaned += c.ascii_str()
		}
	}
	for word in cleaned.split(' ') {
		if word.len > 0 {
			result << word[0..1].to_upper() + word[1..]
		}
	}
	return result.join(' ')
}

fn ensure_icon_icns(icon_name string, cache_dir string) string {
	if icon_name == '' {
		return ''
	}

	src_png := if os.is_abs_path(icon_name) {
		icon_name
	} else {
		os.join_path(os.getwd(), 'resources', icon_name)
	}

	if !os.exists(src_png) {
		fallback_png := os.join_path(os.getwd(), 'resources', 'icon.png')
		if !os.exists(fallback_png) {
			return ''
		}
		return ensure_icon_icns('icon.png', cache_dir)
	}

	clean_name := os.file_name(src_png).replace('.png', '')
	icns_file := os.join_path(cache_dir, '${clean_name}.icns')

	if os.exists(icns_file) {
		return icns_file
	}

	iconset_dir := os.join_path(cache_dir, '${clean_name}.iconset')
	os.mkdir_all(iconset_dir) or { return '' }

	icon_sizes := [
		IconSize{'icon_16x16.png', 16},
		IconSize{'icon_16x16@2x.png', 32},
		IconSize{'icon_32x32.png', 32},
		IconSize{'icon_32x32@2x.png', 64},
		IconSize{'icon_128x128.png', 128},
		IconSize{'icon_128x128@2x.png', 256},
		IconSize{'icon_256x256.png', 256},
		IconSize{'icon_256x256@2x.png', 512},
		IconSize{'icon_512x512.png', 512},
		IconSize{'icon_512x512@2x.png', 1024},
	]

	for sz_info in icon_sizes {
		out_p := os.join_path(iconset_dir, sz_info.name)
		sips_cmd := 'sips -s format png -z ${sz_info.size} ${sz_info.size} ${os.quoted_path(src_png)} --out ${os.quoted_path(out_p)} > /dev/null 2>&1'
		os.execute(sips_cmd)
	}

	iconutil_cmd := 'iconutil -c icns ${os.quoted_path(iconset_dir)} -o ${os.quoted_path(icns_file)} > /dev/null 2>&1'
	res := os.execute(iconutil_cmd)
	os.rmdir_all(iconset_dir) or {}

	if res.exit_code == 0 && os.exists(icns_file) {
		return icns_file
	}
	return ''
}

fn compile_app(t Task, cached_icns_path string) TaskResult {
	t0 := time.now()

	if t.bin_only {
		out_bin := os.join_path(t.out_dir, t.app_id)
		mut cmd := 'v '
		if t.is_prod {
			cmd += '-prod -gc none '
		}
		cmd += '${os.quoted_path(t.src_path)} -o ${os.quoted_path(out_bin)}'

		res := os.execute(cmd)
		elapsed := time.since(t0)

		if res.exit_code == 0 && os.exists(out_bin) {
			sz := os.file_size(out_bin)
			return TaskResult{
				index:        t.index
				total:        t.total
				display_name: t.display_name
				out_target:   out_bin
				success:      true
				elapsed_ms:   elapsed.milliseconds()
				size_mb:      f64(sz) / 1024.0 / 1024.0
				err_msg:      ''
			}
		}

		return TaskResult{
			index:        t.index
			total:        t.total
			display_name: t.display_name
			out_target:   out_bin
			success:      false
			elapsed_ms:   elapsed.milliseconds()
			size_mb:      0.0
			err_msg:      res.output.trim_space()
		}
	}

	// macOS .app Bundle Setup
	app_bundle := os.join_path(t.out_dir, '${t.display_name}.app')
	contents_dir := os.join_path(app_bundle, 'Contents')
	macos_dir := os.join_path(contents_dir, 'MacOS')
	resources_dir := os.join_path(contents_dir, 'Resources')

	if os.exists(app_bundle) {
		os.rmdir_all(app_bundle) or {}
	}

	os.mkdir_all(macos_dir) or {
		return TaskResult{
			index:        t.index
			total:        t.total
			display_name: t.display_name
			out_target:   app_bundle
			success:      false
			elapsed_ms:   0
			size_mb:      0.0
			err_msg:      'Failed to create MacOS directory: ${err}'
		}
	}
	os.mkdir_all(resources_dir) or {}

	// 1. Copy AppIcon.icns
	mut has_icon := false
	if cached_icns_path != '' && os.exists(cached_icns_path) {
		dest_icns := os.join_path(resources_dir, 'AppIcon.icns')
		os.cp(cached_icns_path, dest_icns) or {}
		has_icon = true
	}

	// 2. Generate Info.plist
	mut plist_content := '<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.txt">
<plist version="1.0">
<dict>
    <key>CFBundleDevelopmentRegion</key>
    <string>English</string>
    <key>CFBundleExecutable</key>
    <string>${t.app_id}</string>
'
	if has_icon {
		plist_content += '    <key>CFBundleIconFile</key>
    <string>AppIcon</string>
'
	}
	plist_content += '    <key>CFBundleIdentifier</key>
    <string>${t.bundle_id}</string>
    <key>CFBundleInfoDictionaryVersion</key>
    <string>6.0</string>
    <key>CFBundleName</key>
    <string>${t.display_name}</string>
    <key>CFBundleDisplayName</key>
    <string>${t.display_name}</string>
    <key>CFBundlePackageType</key>
    <string>APPL</string>
    <key>CFBundleShortVersionString</key>
    <string>1.0.0</string>
    <key>CFBundleVersion</key>
    <string>1.0.0</string>
    <key>LSMinimumSystemVersion</key>
    <string>10.13.0</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
'
	plist_path := os.join_path(contents_dir, 'Info.plist')
	os.write_file(plist_path, plist_content) or {}

	// 3. Compile Binary into MacOS/
	target_bin := os.join_path(macos_dir, t.app_id)
	mut cmd := 'v '
	if t.is_prod {
		cmd += '-prod -gc none '
	}
	cmd += '${os.quoted_path(t.src_path)} -o ${os.quoted_path(target_bin)}'

	res := os.execute(cmd)
	if res.exit_code != 0 || !os.exists(target_bin) {
		elapsed := time.since(t0)
		return TaskResult{
			index:        t.index
			total:        t.total
			display_name: t.display_name
			out_target:   app_bundle
			success:      false
			elapsed_ms:   elapsed.milliseconds()
			size_mb:      0.0
			err_msg:      res.output.trim_space()
		}
	}

	os.execute('chmod +x ${os.quoted_path(target_bin)}')

	// 4. Code sign & clear quarantine attributes
	os.execute('codesign --force --deep --sign - ${os.quoted_path(app_bundle)} > /dev/null 2>&1')
	os.execute('xattr -cr ${os.quoted_path(app_bundle)} > /dev/null 2>&1')

	elapsed := time.since(t0)
	sz := os.file_size(target_bin)

	return TaskResult{
		index:        t.index
		total:        t.total
		display_name: t.display_name
		out_target:   app_bundle
		success:      true
		elapsed_ms:   elapsed.milliseconds()
		size_mb:      f64(sz) / 1024.0 / 1024.0
		err_msg:      ''
	}
}

fn main() {
	println('======================================================================')
	println('  SimpleGUI Batch Applications (.app) & Icon Generator (Parallel)')
	println('======================================================================')

	mut is_prod := false
	mut bin_only := false
	mut batch_size := 6
	mut target_filter := ''
	mut out_dir_arg := 'bin'
	mut install_deps_flag := false
	mut check_deps_flag := false

	mut i := 1
	for i < os.args.len {
		arg := os.args[i]
		if arg == '-prod' || arg == '--prod' {
			is_prod = true
		} else if arg == '--raw' || arg == '--bin-only' {
			bin_only = true
		} else if arg in ['--install-deps', '-i', '--deps'] {
			install_deps_flag = true
		} else if arg in ['--check-deps', '-c'] {
			check_deps_flag = true
		} else if arg == '-j' || arg == '--jobs' || arg == '-b' || arg == '--batch' {
			if i + 1 < os.args.len {
				i++
				batch_size = os.args[i].int()
				if batch_size < 1 {
					batch_size = 1
				}
			}
		} else if arg.starts_with('-j') {
			batch_size = arg.replace('-j', '').int()
			if batch_size < 1 {
				batch_size = 1
			}
		} else if arg == '-o' || arg == '--out' {
			if i + 1 < os.args.len {
				i++
				out_dir_arg = os.args[i]
			}
		} else if !arg.starts_with('-') {
			target_filter = arg
		}
		i++
	}

	if install_deps_flag || check_deps_flag {
		mut dep_script := os.join_path(os.getwd(), 'install_dependencies.vsh')
		if !os.exists(dep_script) {
			dep_script = os.join_path(os.getwd(), 'scripts', 'install_deps.vsh')
		}
		if os.exists(dep_script) {
			mode_opt := if check_deps_flag { '--check' } else { '-y' }
			println('🔍 Checking Homebrew dependencies before build...')
			exit_code := os.system('v run ${os.quoted_path(dep_script)} ${mode_opt}')
			if exit_code != 0 && !check_deps_flag {
				eprintln('⚠️ Dependency installation encountered errors.')
			}
			println('')
		}
	}

	cwd := os.getwd()
	app_dir := os.join_path(cwd, 'applications')
	out_dir := if os.is_abs_path(out_dir_arg) {
		out_dir_arg
	} else {
		os.join_path(cwd, out_dir_arg)
	}
	icon_cache_dir := os.join_path(out_dir, '.icon_cache')

	os.mkdir_all(out_dir) or {
		eprintln('Failed to create output directory: ${out_dir}')
		exit(1)
	}

	raw_files := os.ls(app_dir) or {
		eprintln('Failed to list applications directory: ${app_dir}')
		exit(1)
	}

	mut app_files := []string{}
	for f in raw_files {
		if f.ends_with('.v') && !f.ends_with('_test.v') {
			if target_filter.len == 0 || f.contains(target_filter) {
				app_files << f
			}
		}
	}
	app_files.sort()

	if app_files.len == 0 {
		println('No matching application files found in ${app_dir}')
		exit(0)
	}

	app_maps := get_app_maps()

	// Pre-generate / cache required icons
	mut icon_lookup := map[string]string{}
	if !bin_only {
		os.mkdir_all(icon_cache_dir) or {}
		println('🎨 Pre-generating and caching application .icns icons...')
		for f in app_files {
			config := app_maps[f] or {
				AppConfig{format_friendly_name(f.replace('.v', '')), 'icon.png', 'com.simplegui.${to_clean_identifier(f.replace('.v', ''))}'}
			}
			if config.icon_file !in icon_lookup {
				icns_path := ensure_icon_icns(config.icon_file, icon_cache_dir)
				icon_lookup[config.icon_file] = icns_path
			}
		}
		println('   Cached ${icon_lookup.len} unique .icns icon assets.')
	}

	mode_str := if is_prod { 'PRODUCTION (-prod)' } else { 'FAST BUILD' }
	target_type_str := if bin_only { 'CLI Binaries' } else { 'macOS Native .app Bundles' }

	println('Source Directory : ${app_dir}')
	println('Output Directory : ${out_dir}')
	println('Target Format    : ${target_type_str}')
	println('Build Mode       : ${mode_str}')
	println('Concurrent Batch : ${batch_size} parallel jobs')
	println('Total Targets    : ${app_files.len}')
	println('----------------------------------------------------------------------')

	mut tasks := []Task{}
	for idx, f in app_files {
		raw_id := f.replace('.v', '')
		config := app_maps[f] or {
			AppConfig{format_friendly_name(raw_id), 'icon.png', 'com.simplegui.${to_clean_identifier(raw_id)}'}
		}

		tasks << Task{
			index:        idx + 1
			total:        app_files.len
			app_id:       raw_id
			src_path:     os.join_path('applications', f)
			out_dir:      out_dir
			display_name: config.display_name
			icon_file:    config.icon_file
			bundle_id:    config.bundle_id
			is_prod:      is_prod
			bin_only:     bin_only
		}
	}

	mut success_count := 0
	mut fail_count := 0
	start_total := time.now()

	// Process in concurrent batches
	mut offset := 0
	for offset < tasks.len {
		end := if offset + batch_size > tasks.len { tasks.len } else { offset + batch_size }
		batch_tasks := tasks[offset..end].clone()
		batch_num := (offset / batch_size) + 1
		total_batches := ((tasks.len + batch_size - 1) / batch_size)

		println('--> Launching Batch ${batch_num}/${total_batches} (${batch_tasks.len} apps concurrent)...')

		mut threads := []thread TaskResult{}
		for t in batch_tasks {
			cached_icns := icon_lookup[t.icon_file] or { '' }
			threads << spawn compile_app(t, cached_icns)
		}

		results := threads.wait()
		for r in results {
			target_name := if bin_only { r.display_name } else { '${r.display_name}.app' }
			if r.success {
				println('   [${r.index:02d}/${r.total:02d}] OK   ${target_name:-30s} (${r.elapsed_ms}ms, ${r.size_mb:.2f} MB)')
				success_count++
			} else {
				println('   [${r.index:02d}/${r.total:02d}] FAIL ${target_name:-30s} (${r.elapsed_ms}ms)')
				if r.err_msg.len > 0 {
					eprintln('        Error: ${r.err_msg}')
				}
				fail_count++
			}
		}

		offset = end
	}

	total_elapsed := time.since(start_total)
	println('======================================================================')
	println('Build Summary:')
	println('  Success   : ${success_count} / ${app_files.len}')
	if fail_count > 0 {
		println('  Failed    : ${fail_count}')
	}
	println('  Total Time: ${total_elapsed.milliseconds()}ms (${total_elapsed.seconds():.2f}s)')
	println('  Bundles   : ${out_dir}/')
	println('======================================================================')

	if fail_count > 0 {
		exit(1)
	}
}
