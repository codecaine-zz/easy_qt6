module simplegui

import os
import json2
import time
import net
import net.http
import crypto.sha256
import crypto.md5

#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/time.h>

// C function declarations for direct POSIX/BSD system calls used in this file.
fn C.getloadavg(loadavg &f64, nelem int) int
fn C.sysctl(name &int, namelen u32, oldp voidptr, oldlenp &usize, newp voidptr, newlen usize) int

// sys.v - Neutralino-inspired System Call and OS API Extensions for SimpleGUI
// Extends the `SimpleWindow` struct with direct, fluent methods for executing
// commands, environment variables, filesystem actions, native notifications, and hardware inspection.

// ==========================================
// 1. Operating System & Execution (Neutralino NL_OS)
// ==========================================

// exec runs a system command synchronously, returning its stdout/stderr output and exit code.
// Equivalent to Neutralino's `os.execCommand`.
pub fn (win &SimpleWindow) exec(command string) (string, int) {
	if win.debug_mode {
		println('[simplegui SYSTEM] Executing sync command: "${command}"')
	}
	res := os.execute(command)
	return res.output.trim_space(), res.exit_code
}

// exec_or runs a system command, returning stdout if successful, or the provided fallback.
pub fn (win &SimpleWindow) exec_or(command string, fallback string) string {
	output, code := win.exec(command)
	if code == 0 {
		return output
	}
	return fallback
}

// exec_bg runs a system command asynchronously (non-blocking in a V task thread).
// Helpful because it prevents the Cocoa UI event loop from freezing during long-running tasks.
pub fn (win &SimpleWindow) exec_bg(command string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Spawning background command: "${command}"')
	}
	// Spawn a concurrent thread using V's lightweight routines
	spawn fn (cmd string, debug bool) {
		res := os.execute(cmd)
		if debug {
			println('[simplegui SYSTEM] Background command finished (code: ${res.exit_code}): "${cmd}"')
			if res.output.trim_space().len > 0 {
				println('[simplegui SYSTEM] Output: ${res.output.trim_space()}')
			}
		}
	}(command, win.debug_mode)

	return win
}

// get_env retrieves the value of a system environment variable.
// Equivalent to Neutralino's `os.getEnv`.
pub fn (win &SimpleWindow) get_env(key string) string {
	return os.getenv(key)
}

// set_env sets a system environment variable for the application process.
// Equivalent to Neutralino's `os.setEnv`.
pub fn (win &SimpleWindow) set_env(key string, val string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Setting environment: ${key} = ${val}')
	}
	os.setenv(key, val, true)
	return win
}

// unset_env clears a system environment variable.
pub fn (win &SimpleWindow) unset_env(key string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Unsetting environment: ${key}')
	}
	os.unsetenv(key)
	return win
}

// show_system_notification triggers a native macOS system notification banner.
// Equivalent to Neutralino's `os.showNotification`. Uses AppleScript under the hood.
pub fn (win &SimpleWindow) show_system_notification(title string, message string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Triggering system notification: "${title}" - "${message}"')
	}
	// Escape double quotes for execution safety
	title_escaped := title.replace('"', '\\"')
	msg_escaped := message.replace('"', '\\"')
	cmd := 'osascript -e \'display notification "${msg_escaped}" with title "${title_escaped}"\''
	win.exec_bg(cmd)
	return win
}

// ==========================================
// 2. Hardware and Computer Info (Neutralino NL_COMPUTER)
// ==========================================

// get_cpu_info retrieves the local computer's processor information.
// Equivalent to Neutralino's `computer.getCPUInfo`.
pub fn (win &SimpleWindow) get_cpu_info() string {
	return win.exec_or('sysctl -n machdep.cpu.brand_string', 'Unknown macOS CPU')
}

// get_cpu_cores retrieves the total CPU core count (physical + virtual).
pub fn (win &SimpleWindow) get_cpu_cores() int {
	cores_str := win.exec_or('sysctl -n hw.ncpu', '0')
	return cores_str.int()
}

// get_memory_info retrieves physical RAM details on the machine.
// Equivalent to Neutralino's `computer.getMemoryInfo`.
pub fn (win &SimpleWindow) get_memory_info() string {
	bytes_str := win.exec_or('sysctl -n hw.memsize', '')
	if bytes_str.len > 0 {
		bytes := bytes_str.u64()
		gb := f64(bytes) / (1024.0 * 1024.0 * 1024.0)
		return '${gb:.1f} GB RAM'
	}
	return 'Unknown RAM'
}

// ==========================================
// 3. System Directory Lookup (Neutralino os.getPath/NL_PATH)
// ==========================================

// get_system_path resolves paths to standard environment folders on macOS.
pub fn (win &SimpleWindow) get_system_path(name string) string {
	home := os.home_dir()
	return match name.to_lower() {
		'home' { home }
		'temp', 'tmp' { os.temp_dir() }
		'desktop' { os.join_path(home, 'Desktop') }
		'documents' { os.join_path(home, 'Documents') }
		'downloads' { os.join_path(home, 'Downloads') }
		'cache' { os.cache_dir() }
		'config' { os.config_dir() or { '' } }
		'data' { os.data_dir() }
		'app' { os.dir(os.executable()) }
		else { home }
	}
}

// ==========================================
// 4. File System Utilities (Neutralino NL_FILESYSTEM)
// ==========================================

// file_exists checks if a file or folder exists at the specified path.
pub fn (win &SimpleWindow) file_exists(path string) bool {
	return os.exists(path)
}

// is_dir checks if the given path is a directory.
pub fn (win &SimpleWindow) is_dir(path string) bool {
	return os.is_dir(path)
}

// read_file_opt reads file contents, returning a standard V Result string.
// Forces robust error propagation and aligns with V's Option & Result practices.
// If the path is relative and not found in current working directory, checks the recommended app storage location.
pub fn (win &SimpleWindow) read_file_opt(path string) !string {
	mut target_path := path
	if !os.exists(target_path) {
		if !path.starts_with('/') && !path.starts_with('~') {
			resolved := win.resolve_storage_path(path)
			if os.exists(resolved) {
				target_path = resolved
			} else {
				return error('File does not exist: ' + path)
			}
		} else if path.starts_with('~') {
			target_path = path.replace('~', os.home_dir())
			if !os.exists(target_path) {
				return error('File does not exist: ' + target_path)
			}
		} else {
			return error('File does not exist: ' + path)
		}
	}
	content := os.read_file(target_path) or { return error(err.msg()) }
	return content
}

// read_file reads file contents, returning an empty string if reading fails.
pub fn (win &SimpleWindow) read_file(path string) string {
	res := win.read_file_opt(path) or { return '' }
	return res
}

// write_file_opt writes dynamic content to a file, returning a Result flag.
// Automatically ensures parent directory exists and expands ~ paths.
pub fn (win &SimpleWindow) write_file_opt(path string, content string) !&SimpleWindow {
	target_path := if path.starts_with('~') {
		path.replace('~', os.home_dir())
	} else {
		path
	}
	dir := os.dir(target_path)
	if dir != '' && !os.exists(dir) {
		os.mkdir_all(dir) or {}
	}
	os.write_file(target_path, content) or { return error(err.msg()) }
	return win
}

// write_file writes dynamic content to a file. Silent if failed (errors out if debugging is enabled).
pub fn (win &SimpleWindow) write_file(path string, content string) &SimpleWindow {
	win.write_file_opt(path, content) or {
		if win.debug_mode {
			println('[simplegui ERROR] Failed to write to file "${path}": ${err}')
		}
	}
	return win
}

// delete_file deletes a file or directory path.
pub fn (win &SimpleWindow) delete_file(path string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Deleting path: "${path}"')
	}
	os.rm(path) or {
		if win.debug_mode {
			println('[simplegui ERROR] Failed to delete path: ${err}')
		}
	}
	return win
}

// create_directory recursively creates a nested directory structure/folder path.
pub fn (win &SimpleWindow) create_directory(path string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Creating directory tree: "${path}"')
	}
	os.mkdir_all(path) or {
		if win.debug_mode {
			println('[simplegui ERROR] Failed to create directories: ${err}')
		}
	}
	return win
}

// read_dir lists all filenames inside any target folder.
pub fn (win &SimpleWindow) read_dir(path string) []string {
	return os.ls(path) or { []string{} }
}

// ==========================================
// 5. New System Call & OS API Extensions (V standard library os/process/stat details)
// ==========================================

// DiskStats represents disk space utilization statistics.
pub struct DiskStats {
pub:
	total     u64
	available u64
	used      u64
}

// FileMetadata represents comprehensive file information and permissions.
pub struct FileMetadata {
pub:
	size         i64
	inode        u64
	nlink        u64
	dev          u64
	uid          u32
	gid          u32
	atime        i64
	mtime        i64
	ctime        i64
	file_type    string
	mode_bitmask u32
	owner_r      bool
	owner_w      bool
	owner_x      bool
	group_r      bool
	group_w      bool
	group_x      bool
	others_r     bool
	others_w     bool
	others_x     bool
}

// SimpleProcess provides a high-level wrapper to control a background subprocess.
pub struct SimpleProcess {
pub mut:
	proc &os.Process = unsafe { nil }
}

// CommandResult provides structured command execution details for production diagnostics.
pub struct CommandResult {
pub:
	command     string
	output      string
	exit_code   int
	timed_out   bool
	duration_ms i64
	attempts    int
}

// success reports whether command execution completed with exit code 0 and no timeout.
pub fn (r CommandResult) success() bool {
	return !r.timed_out && r.exit_code == 0
}

// get_env_opt retrieves an environment variable value, returning none if it's not set.
pub fn (win &SimpleWindow) get_env_opt(key string) ?string {
	return os.getenv_opt(key)
}

// get_envs retrieves all system environment variables as a key-value map.
pub fn (win &SimpleWindow) get_envs() map[string]string {
	return os.environ()
}

// get_hostname retrieves the network hostname of the current machine.
pub fn (win &SimpleWindow) get_hostname() string {
	return os.hostname() or { 'unknown' }
}

// get_username retrieves the active username running the application.
pub fn (win &SimpleWindow) get_username() string {
	return os.loginname() or { 'unknown' }
}

// get_user_os returns a string identifying the host OS.
pub fn (win &SimpleWindow) get_user_os() string {
	return os.user_os()
}

// get_pid returns the current Process ID (PID).
pub fn (win &SimpleWindow) get_pid() int {
	return os.getpid()
}

// get_ppid returns the Parent Process ID (PPID).
pub fn (win &SimpleWindow) get_ppid() int {
	return os.getppid()
}

// get_uid returns the real User ID (UID).
pub fn (win &SimpleWindow) get_uid() int {
	return os.getuid()
}

// get_gid returns the real Group ID (GID).
pub fn (win &SimpleWindow) get_gid() int {
	return os.getgid()
}

// get_euid returns the effective User ID (EUID).
pub fn (win &SimpleWindow) get_euid() int {
	return os.geteuid()
}

// get_egid returns the effective Group ID (EGID).
pub fn (win &SimpleWindow) get_egid() int {
	return os.getegid()
}

// exists_in_path checks if a given executable/command binary is present in the system's PATH.
pub fn (win &SimpleWindow) exists_in_path(cmd string) bool {
	return os.exists_in_system_path(cmd)
}

// find_executable returns the absolute path of the specified command binary if it exists in the system's PATH.
pub fn (win &SimpleWindow) find_executable(cmd string) string {
	return os.find_abs_path_of_executable(cmd) or { '' }
}

// command_exists checks if an executable command exists in the current PATH.
pub fn (win &SimpleWindow) command_exists(cmd string) bool {
	return win.exists_in_path(cmd)
}

// require_command returns the absolute executable path or an error if the command is missing.
pub fn (win &SimpleWindow) require_command(cmd string) !string {
	path := win.find_executable(cmd)
	if path.len == 0 {
		return error('Required command not found in PATH: ${cmd}')
	}
	return path
}

// get_executable_path returns the absolute path of the current running executable.
pub fn (win &SimpleWindow) get_executable_path() string {
	return os.executable()
}

// get_disk_usage retrieves disk space usage statistics for the given folder/path.
pub fn (win &SimpleWindow) get_disk_usage(path string) !DiskStats {
	du := os.disk_usage(path)!
	return DiskStats{
		total:     du.total
		available: du.available
		used:      du.used
	}
}

// write_lines writes an array of strings to a file, separating them with newlines.
pub fn (win &SimpleWindow) write_lines(path string, lines []string) !&SimpleWindow {
	os.write_lines(path, lines)!
	return win
}

// read_lines reads a file line-by-line and returns an array of strings.
pub fn (win &SimpleWindow) read_lines(path string) ![]string {
	return os.read_lines(path)!
}

// write_bytes writes a byte array to a file.
pub fn (win &SimpleWindow) write_bytes(path string, bytes []u8) !&SimpleWindow {
	os.write_bytes(path, bytes)!
	return win
}

// read_bytes reads a file's content as a byte array.
pub fn (win &SimpleWindow) read_bytes(path string) ![]u8 {
	return os.read_bytes(path)!
}

// is_file returns true if the path points to a regular file.
pub fn (win &SimpleWindow) is_file(path string) bool {
	return os.is_file(path)
}

// is_dir_empty checks if the directory has no files or subfolders.
pub fn (win &SimpleWindow) is_dir_empty(path string) bool {
	return os.is_dir_empty(path)
}

// create_single_directory creates a single new directory (non-recursive).
pub fn (win &SimpleWindow) create_single_directory(path string) !&SimpleWindow {
	os.mkdir(path)!
	return win
}

// delete_single_directory deletes a single empty directory.
pub fn (win &SimpleWindow) delete_single_directory(path string) !&SimpleWindow {
	os.rmdir(path)!
	return win
}

// delete_directory recursively removes a directory and all of its contents (rmdir_all).
pub fn (win &SimpleWindow) delete_directory(path string) !&SimpleWindow {
	os.rmdir_all(path)!
	return win
}

// copy_file copies a file from a source path to a destination path.
pub fn (win &SimpleWindow) copy_file(src string, dest string) !&SimpleWindow {
	os.cp(src, dest)!
	return win
}

// move_file moves or renames a file.
pub fn (win &SimpleWindow) move_file(src string, dest string) !&SimpleWindow {
	os.mv(src, dest)!
	return win
}

// create_symlink creates a symbolic link pointing to a target path.
pub fn (win &SimpleWindow) create_symlink(target string, linkpath string) !&SimpleWindow {
	os.symlink(target, linkpath)!
	return win
}

// is_symlink checks if a path points to a symbolic link.
pub fn (win &SimpleWindow) is_symlink(path string) bool {
	return os.is_link(path)
}

// get_working_directory returns the current active working directory.
pub fn (win &SimpleWindow) get_working_directory() string {
	return os.getwd()
}

// set_working_directory changes the current active working directory.
pub fn (win &SimpleWindow) set_working_directory(path string) !&SimpleWindow {
	os.chdir(path)!
	return win
}

// get_file_size returns the size of a file in bytes.
pub fn (win &SimpleWindow) get_file_size(path string) !i64 {
	st := os.stat(path)!
	return st.size
}

// get_last_modified returns the Unix timestamp when the file was last modified.
pub fn (win &SimpleWindow) get_last_modified(path string) i64 {
	return os.file_last_mod_unix(path)
}

// glob finds all files matching a wildcard pattern (e.g. *.txt).
pub fn (win &SimpleWindow) glob(pattern string) ![]string {
	return os.glob(pattern)!
}

// walk recursively traverses a directory, executing a callback function for each file found.
pub fn (win &SimpleWindow) walk(path string, callback fn (string)) {
	os.walk(path, callback)
}

// walk_ext traverses a directory, returning a list of files that match a file extension filter (e.g. ".txt").
pub fn (win &SimpleWindow) walk_ext(path string, ext string) []string {
	return os.walk_ext(path, ext, os.WalkParams{})
}

// set_permissions changes the permission bits on a file.
pub fn (win &SimpleWindow) set_permissions(path string, mode int) !&SimpleWindow {
	os.chmod(path, mode)!
	return win
}

// set_ownership changes the owner user ID (UID) and group ID (GID) of a file.
pub fn (win &SimpleWindow) set_ownership(path string, uid int, gid int) !&SimpleWindow {
	os.chown(path, uid, gid)!
	return win
}

// is_readable checks if a path is readable.
pub fn (win &SimpleWindow) is_readable(path string) bool {
	return os.is_readable(path)
}

// is_writable checks if a path is writable.
pub fn (win &SimpleWindow) is_writable(path string) bool {
	return os.is_writable(path)
}

// is_executable checks if a path is executable.
pub fn (win &SimpleWindow) is_executable(path string) bool {
	return os.is_executable(path)
}

// path_dir returns the parent directory of the path.
pub fn (win &SimpleWindow) path_dir(path string) string {
	return os.dir(path)
}

// path_base returns the last element of the path.
pub fn (win &SimpleWindow) path_base(path string) string {
	return os.base(path)
}

// path_ext returns the file extension of the path (including the dot).
pub fn (win &SimpleWindow) path_ext(path string) string {
	return os.file_ext(path)
}

// path_name returns the filename without the path.
pub fn (win &SimpleWindow) path_name(path string) string {
	return os.file_name(path)
}

// path_is_abs checks if the path is an absolute path.
pub fn (win &SimpleWindow) path_is_abs(path string) bool {
	return os.is_abs_path(path)
}

// path_real resolves all symbolic links and relative references to return an absolute path.
pub fn (win &SimpleWindow) path_real(path string) string {
	return os.real_path(path)
}

// path_norm normalizes path separators.
pub fn (win &SimpleWindow) path_norm(path string) string {
	return os.norm_path(path)
}

// path_split splits a path into (directory, filename, extension).
pub fn (win &SimpleWindow) path_split(path string) (string, string, string) {
	return os.split_path(path)
}

// get_file_metadata retrieves detailed metadata (size, timestamps, owners, permissions) for the file at the given path.
pub fn (win &SimpleWindow) get_file_metadata(path string) !FileMetadata {
	st := os.stat(path)!
	fm := st.get_mode()
	return FileMetadata{
		size:         st.size
		inode:        st.inode
		nlink:        st.nlink
		dev:          st.dev
		uid:          st.uid
		gid:          st.gid
		atime:        st.atime
		mtime:        st.mtime
		ctime:        st.ctime
		file_type:    st.get_filetype().str()
		mode_bitmask: fm.bitmask()
		owner_r:      fm.owner.read
		owner_w:      fm.owner.write
		owner_x:      fm.owner.execute
		group_r:      fm.group.read
		group_w:      fm.group.write
		group_x:      fm.group.execute
		others_r:     fm.others.read
		others_w:     fm.others.write
		others_x:     fm.others.execute
	}
}

// spawn_process starts a background subprocess with stdout/stderr redirected.
pub fn (win &SimpleWindow) spawn_process(path string, args []string, env map[string]string) !&SimpleProcess {
	mut p := os.new_process(path)
	p.set_args(args)
	p.set_environment(env)
	p.set_redirect_stdio()
	p.use_stdio_ctl = true
	p.run()
	return &SimpleProcess{
		proc: p
	}
}

// is_alive checks if the child process is still running.
pub fn (mut sp SimpleProcess) is_alive() bool {
	if sp.proc == unsafe { nil } {
		return false
	}
	return sp.proc.is_alive()
}

// write sends input data to the process's standard input.
pub fn (mut sp SimpleProcess) write(data string) {
	if sp.proc == unsafe { nil } {
		return
	}
	sp.proc.stdin_write(data)
}

// read reads any output currently available in the process's stdout pipe.
pub fn (mut sp SimpleProcess) read() string {
	if sp.proc == unsafe { nil } {
		return ''
	}
	out := sp.proc.stdout_read()
	if out == '' {
		return sp.proc.stdout_slurp()
	}
	return out
}

// stop suspends the process using a POSIX SIGSTOP signal.
pub fn (mut sp SimpleProcess) stop() {
	if sp.proc != unsafe { nil } {
		sp.proc.signal_stop()
	}
}

// resume resumes a suspended process using a POSIX SIGCONT signal.
pub fn (mut sp SimpleProcess) resume() {
	if sp.proc != unsafe { nil } {
		sp.proc.signal_continue()
	}
}

// terminate terminates the process using a POSIX SIGTERM signal.
pub fn (mut sp SimpleProcess) terminate() {
	if sp.proc != unsafe { nil } {
		sp.proc.signal_term()
	}
}

// wait waits for the subprocess to exit and blocks until completion.
pub fn (mut sp SimpleProcess) wait() {
	if sp.proc != unsafe { nil } {
		sp.proc.wait()
	}
}

// close cleans up and releases process resources.
pub fn (mut sp SimpleProcess) close() {
	if sp.proc != unsafe { nil } {
		sp.proc.close()
	}
}

// Uname represents kernel and operating system description details.
pub struct Uname {
pub:
	sysname  string
	nodename string
	release  string
	version  string
	machine  string
}

// get_uname retrieves system operating system and kernel architecture details.
pub fn (win &SimpleWindow) get_uname() Uname {
	u := os.uname()
	return Uname{
		sysname:  u.sysname
		nodename: u.nodename
		release:  u.release
		version:  u.version
		machine:  u.machine
	}
}

// ==========================================
// 6. System Clock & Time
// ==========================================

// SystemTime holds a structured representation of the current local time.
pub struct SystemTime {
pub:
	year       int
	month      int
	day        int
	hour       int
	minute     int
	second     int
	unix_epoch i64
	unix_milli i64
	rfc3339    string
	weekday    string
}

// get_time returns the current local date and time packed into a SystemTime struct.
pub fn (win &SimpleWindow) get_time() SystemTime {
	now := time.now()
	return SystemTime{
		year:       now.year
		month:      now.month
		day:        now.day
		hour:       now.hour
		minute:     now.minute
		second:     now.second
		unix_epoch: now.unix()
		unix_milli: now.unix_milli()
		rfc3339:    now.format_rfc3339()
		weekday:    now.weekday_str()
	}
}

// get_unix_epoch returns the current time as a Unix epoch timestamp (seconds since 1970-01-01).
pub fn (win &SimpleWindow) get_unix_epoch() i64 {
	return time.now().unix()
}

// get_unix_milli returns the current time as a Unix epoch in milliseconds.
pub fn (win &SimpleWindow) get_unix_milli() i64 {
	return time.now().unix_milli()
}

// get_time_formatted returns the current local time as a formatted string.
// format uses V's time.Time.custom_format(), e.g. "YYYY-MM-DD HH:mm:ss".
pub fn (win &SimpleWindow) get_time_formatted(format string) string {
	return time.now().custom_format(format)
}

// sleep_ms pauses execution for the specified number of milliseconds (blocking).
// Use exec_bg/spawn to avoid freezing the Cocoa UI event loop.
pub fn (win &SimpleWindow) sleep_ms(ms u64) &SimpleWindow {
	time.sleep(ms * time.millisecond)
	return win
}

// get_uptime_seconds returns how long the macOS system has been running (in seconds).
// Uses C sysctl() directly via V's C interop — no subprocess required.
pub fn (win &SimpleWindow) get_uptime_seconds() i64 {
	// kern.boottime is a struct timeval { tv_sec, tv_usec }
	$if macos || freebsd {
		mib := [C.CTL_KERN, C.KERN_BOOTTIME]!
		tv := C.timeval{}
		sz := usize(sizeof(C.timeval))
		if unsafe { C.sysctl(&mib[0], 2, &tv, &sz, nil, 0) } == 0 {
			return time.now().unix() - i64(tv.tv_sec)
		}
	}
	// Fallback: parse sysctl text output on other POSIX systems
	raw := win.exec_or('sysctl -n kern.boottime', '')
	if raw.len > 0 {
		parts := raw.split('=')
		if parts.len >= 2 {
			boot_sec := parts[1].trim_space().split(',')[0].trim_space().i64()
			return time.now().unix() - boot_sec
		}
	}
	return 0
}

// ==========================================
// 7. Clipboard
// NOTE: clipboard_copy(text) and clipboard_read() are implemented in stdlib.v
// using the native V `clipboard` module. Use those methods on SimpleWindow.
// ==========================================

// ==========================================
// 8. macOS Open / Reveal Commands
// ==========================================

// NOTE: open_url(url) is already implemented in simplegui.v via the native Cocoa C bridge.
// Use win.open_url(url) directly — it calls window_open_url() in the Objective-C layer.

// reveal_in_finder opens and highlights a file or folder in macOS Finder.
pub fn (win &SimpleWindow) reveal_in_finder(path string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Revealing in Finder: ${path}')
	}
	win.exec_bg('open -R "${path}"')
	return win
}

// open_in_default_app opens a file with its default associated application.
pub fn (win &SimpleWindow) open_in_default_app(path string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Opening file with default app: ${path}')
	}
	win.exec_bg('open "${path}"')
	return win
}

// open_with_app opens a file using a specific application by bundle ID or app name.
// Example: win.open_with_app('/path/to/file.txt', 'com.apple.TextEdit')
pub fn (win &SimpleWindow) open_with_app(path string, app_id string) &SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Opening "${path}" with app: ${app_id}')
	}
	win.exec_bg('open -a "${app_id}" "${path}"')
	return win
}

// open_terminal opens a new macOS Terminal window.
pub fn (win &SimpleWindow) open_terminal() &SimpleWindow {
	win.exec_bg('open -a Terminal')
	return win
}

// ==========================================
// 9. Network Utilities
// ==========================================

// get_local_ip returns the primary local IP address of this machine.
// Uses os.hostname() + net.resolve_ipaddrs() — no subprocess required.
pub fn (win &SimpleWindow) get_local_ip() string {
	// Resolve the machine's own hostname to get its primary IP
	host := os.hostname() or { return 'unavailable' }
	addrs := net.resolve_ipaddrs(host, .ip, .tcp) or {
		// Fallback: if hostname resolution fails, return unavailable
		return 'unavailable'
	}
	for addr in addrs {
		s := addr.str()
		// Skip loopback addresses (127.x.x.x)
		if !s.starts_with('127.') && !s.starts_with('::1') {
			// strip port suffix if present
			return s.split(':')[0]
		}
	}
	// Return loopback as last resort
	if addrs.len > 0 {
		return addrs[0].str().split(':')[0]
	}
	return 'unavailable'
}

// get_external_ip fetches the machine's public/external IP address using a fast curl request with fallback.
// Uses a 2-second timeout to prevent UI freezes.
pub fn (win &SimpleWindow) get_external_ip() string {
	res := win.exec_or('curl -s --max-time 2 https://api.ipify.org', '')
	if res.len > 0 && !res.contains('html') && !res.contains('error') {
		return res.trim_space()
	}
	resp := http.get('https://api.ipify.org') or { return 'unavailable' }
	if resp.status_code == 200 {
		return resp.body.trim_space()
	}
	return 'unavailable'
}

// ping tests network connectivity to a host. Returns true if the host is reachable.
// Uses macOS netcat with a 1-second connection timeout — non-blocking and fast.
pub fn (win &SimpleWindow) ping(host string, count int) bool {
	iterations := if count < 1 { 1 } else { count }
	for _ in 0 .. iterations {
		_, code80 := win.exec('nc -z -G 1 "${host}" 80 2>/dev/null')
		if code80 == 0 {
			return true
		}
		_, code443 := win.exec('nc -z -G 1 "${host}" 443 2>/dev/null')
		if code443 == 0 {
			return true
		}
	}
	return false
}

// dns_lookup performs a DNS forward lookup, returning the resolved IP address.
// Uses net.resolve_ipaddrs() — direct libc getaddrinfo(), no subprocess.
pub fn (win &SimpleWindow) dns_lookup(hostname string) string {
	addrs := net.resolve_ipaddrs(hostname, .ip, .tcp) or { return '' }
	if addrs.len > 0 {
		// addr.str() gives "IP:port" — return just the IP part
		return addrs[0].str().split(':')[0]
	}
	return ''
}

// get_wifi_ssid returns the SSID of the currently connected Wi-Fi network (macOS).
// Uses fast ipconfig getsummary, falling back to the airport tool.
pub fn (win &SimpleWindow) get_wifi_ssid() string {
	ssid_ip := win.exec_or("ipconfig getsummary en0 2>/dev/null | awk -F ' : ' '/ SSID /{print $2}'",
		'').trim_space()
	if ssid_ip.len > 0 {
		return ssid_ip
	}
	return win.exec_or("/System/Library/PrivateFrameworks/Apple80211.framework/Versions/Current/Resources/airport -I 2>/dev/null | awk '/ SSID:/{print $2}'",
		'').trim_space()
}

// get_network_interfaces lists all active network interface names on the machine.
// No V stdlib API exists for enumerating interfaces; uses ifconfig.
pub fn (win &SimpleWindow) get_network_interfaces() []string {
	raw := win.exec_or('ifconfig -l', '')
	return raw.trim_space().split(' ').filter(it.len > 0)
}

// ==========================================
// 10. System Resource Monitoring
// ==========================================

// get_cpu_usage_percent returns an instantaneous CPU usage percentage estimate (macOS).
// Uses `ps` to aggregate all process CPU usage — a quick approximation.
pub fn (win &SimpleWindow) get_cpu_usage_percent() f64 {
	raw := win.exec_or("ps -A -o %cpu | awk '{s+=$1} END {print s}'", '0')
	return raw.trim_space().f64()
}

// get_load_average returns the 1, 5, and 15 minute system load averages.
// Uses C getloadavg() via V C interop on POSIX systems — no subprocess.
pub fn (win &SimpleWindow) get_load_average() (f64, f64, f64) {
	$if macos || linux || freebsd {
		loadavg := [f64(0), f64(0), f64(0)]!
		if C.getloadavg(&loadavg[0], 3) == 3 {
			return loadavg[0], loadavg[1], loadavg[2]
		}
	}
	// Fallback: parse sysctl text on other systems
	raw := win.exec_or('sysctl -n vm.loadavg', '{ 0.00 0.00 0.00 }')
	clean := raw.trim_space().trim('{').trim('}').trim_space()
	parts := clean.split(' ').filter(it.len > 0)
	if parts.len >= 3 {
		return parts[0].f64(), parts[1].f64(), parts[2].f64()
	}
	return 0.0, 0.0, 0.0
}

// get_memory_pressure returns a macOS memory pressure string: "normal", "warn", or "critical".
// Uses sysctl for sub-millisecond kernel lookup.
pub fn (win &SimpleWindow) get_memory_pressure() string {
	level := win.exec_or('sysctl -n kern.memorystatus_vm_pressure_level 2>/dev/null',
		'').trim_space().int()
	if level >= 4 {
		return 'critical'
	} else if level >= 2 {
		return 'warn'
	} else if level == 1 {
		return 'normal'
	}
	raw := win.exec_or('sysctl -n vm.memory_pressure 2>/dev/null', '')
	lower := raw.to_lower()
	if lower.contains('critical') {
		return 'critical'
	} else if lower.contains('warn') {
		return 'warn'
	}
	return 'normal'
}

// get_running_process_count returns the total number of running processes.
pub fn (win &SimpleWindow) get_running_process_count() int {
	raw := win.exec_or('ps -A | wc -l', '0')
	return raw.trim_space().int()
}

// get_open_file_count returns the total count of open file descriptors in the system.
pub fn (win &SimpleWindow) get_open_file_count() int {
	raw := win.exec_or('sysctl -n kern.num_files', '0')
	return raw.trim_space().int()
}

// get_swap_usage returns a human-readable description of current macOS swap usage.
pub fn (win &SimpleWindow) get_swap_usage() string {
	return win.exec_or('sysctl -n vm.swapusage', 'unknown').trim_space()
}

// ==========================================
// 11. Terminal / Shell Utilities
// ==========================================

// beep plays the macOS system alert sound (NSBeep) using osascript.
pub fn (win &SimpleWindow) beep() &SimpleWindow {
	win.exec_bg("osascript -e 'beep'")
	return win
}

// beep_n plays the macOS system alert sound n times.
pub fn (win &SimpleWindow) beep_n(n int) &SimpleWindow {
	count := if n < 1 { 1 } else { n }
	win.exec_bg("osascript -e 'beep ${count}'")
	return win
}

// osascript_dialog shows a native macOS input dialog box and returns what the user typed.
// Returns an empty string if the user cancels.
pub fn (win &SimpleWindow) osascript_dialog(prompt string, default_value string) string {
	prompt_esc := prompt.replace('"', '\\"')
	default_esc := default_value.replace('"', '\\"')
	script := 'osascript -e \'set ans to text returned of (display dialog "${prompt_esc}" default answer "${default_esc}" buttons {"Cancel","OK"} default button "OK")\''
	output, code := win.exec(script)
	if code == 0 {
		return output.trim_space()
	}
	return ''
}

// osascript_alert shows a native macOS alert dialog with a message.
// Blocks until the user clicks OK or Cancel. Returns true if OK was clicked.
pub fn (win &SimpleWindow) osascript_alert(title string, message string) bool {
	title_esc := title.replace('"', '\\"')
	msg_esc := message.replace('"', '\\"')
	script := 'osascript -e \'button returned of (display alert "${title_esc}" message "${msg_esc}" buttons {"Cancel","OK"} default button "OK")\''
	output, code := win.exec(script)
	if code == 0 {
		return output.trim_space() == 'OK'
	}
	return false
}

// osascript_choose_file shows a native macOS file-picker dialog and returns the chosen path.
// Returns an empty string if cancelled.
pub fn (win &SimpleWindow) osascript_choose_file() string {
	script := "osascript -e 'POSIX path of (choose file)'"
	output, code := win.exec(script)
	if code == 0 {
		return output.trim_space()
	}
	return ''
}

// osascript_choose_folder shows a native macOS folder-picker dialog and returns the chosen path.
// Returns an empty string if cancelled.
pub fn (win &SimpleWindow) osascript_choose_folder() string {
	script := "osascript -e 'POSIX path of (choose folder)'"
	output, code := win.exec(script)
	if code == 0 {
		return output.trim_space()
	}
	return ''
}

// say uses macOS text-to-speech to speak a message out loud.
pub fn (win &SimpleWindow) say(text string) &SimpleWindow {
	escaped := text.replace('"', '\\"')
	win.exec_bg('say "${escaped}"')
	return win
}

// ==========================================
// 12. macOS-Specific System Information
// ==========================================

// system_version_plist_value reads a key from the macOS SystemVersion.plist file
// using pure V file I/O — no sw_vers subprocess required.
fn system_version_plist_value(key string) string {
	plist_path := '/System/Library/CoreServices/SystemVersion.plist'
	content := os.read_file(plist_path) or { return 'unknown' }
	// Simple linear search for the key/value pair in plist XML:
	// <key>ProductVersion</key><string>14.5</string>
	key_tag := '<key>${key}</key>'
	key_idx := content.index(key_tag) or { return 'unknown' }
	after_key := content[key_idx + key_tag.len..]
	val_start_tag := '<string>'
	val_end_tag := '</string>'
	val_start := after_key.index(val_start_tag) or { return 'unknown' }
	after_start := after_key[val_start + val_start_tag.len..]
	val_end := after_start.index(val_end_tag) or { return 'unknown' }
	return after_start[..val_end].trim_space()
}

// get_macos_version returns the human-readable macOS version string (e.g. "14.5").
// Reads /System/Library/CoreServices/SystemVersion.plist directly — no sw_vers subprocess.
pub fn (win &SimpleWindow) get_macos_version() string {
	return system_version_plist_value('ProductVersion')
}

// get_macos_build returns the macOS build number (e.g. "23F79").
// Reads /System/Library/CoreServices/SystemVersion.plist directly — no sw_vers subprocess.
pub fn (win &SimpleWindow) get_macos_build() string {
	return system_version_plist_value('ProductBuildVersion')
}

// get_macos_product_name returns the product name (e.g. "macOS", "Mac OS X").
// Reads /System/Library/CoreServices/SystemVersion.plist directly — no sw_vers subprocess.
pub fn (win &SimpleWindow) get_macos_product_name() string {
	return system_version_plist_value('ProductName')
}

// get_device_model returns the hardware model identifier (e.g. "MacBookPro18,3").
pub fn (win &SimpleWindow) get_device_model() string {
	return win.exec_or('sysctl -n hw.model', 'unknown').trim_space()
}

// get_serial_number returns the device serial number.
// Uses targeted ioreg query for sub-millisecond resolution.
pub fn (win &SimpleWindow) get_serial_number() string {
	return win.exec_or('ioreg -c IOPlatformExpertDevice 2>/dev/null | awk -F \'"\' \'/IOPlatformSerialNumber/{print $4}\'',
		'unavailable').trim_space()
}

// get_screen_resolution returns the primary display resolution (e.g. "2560 x 1600").
// Uses fast AppleScript desktop query with fallback.
pub fn (win &SimpleWindow) get_screen_resolution() string {
	raw := win.exec_or('osascript -e \'tell application "Finder" to get bounds of window of desktop\' 2>/dev/null',
		'')
	if raw.len > 0 {
		parts := raw.split(',').map(it.trim_space())
		if parts.len >= 4 {
			return '${parts[2]} x ${parts[3]}'
		}
	}
	return win.exec_or('system_profiler SPDisplaysDataType 2>/dev/null | awk \'/Resolution:/{print $2" x "$4; exit}\'',
		'unknown').trim_space()
}

// get_gpu_info returns the GPU model string of the primary graphics adapter.
// Uses fast IOPCIDevice ioreg lookup with fallback.
pub fn (win &SimpleWindow) get_gpu_info() string {
	ioreg_gpu := win.exec_or('ioreg -rc IOPCIDevice 2>/dev/null | awk -F \'"\' \'/"model" = /{print $4}\' | head -1',
		'').trim_space()
	if ioreg_gpu.len > 0 {
		return ioreg_gpu
	}
	return win.exec_or('system_profiler SPDisplaysDataType 2>/dev/null | awk \'/Chipset Model:/{sub(/.*Chipset Model: /,""); print; exit}\'',
		'unknown').trim_space()
}

// get_battery_percent returns the battery charge percentage, or -1 if no battery is present.
pub fn (win &SimpleWindow) get_battery_percent() int {
	raw := win.exec_or("pmset -g batt 2>/dev/null | grep -oE '[0-9]+%' | head -1 | tr -d '%'",
		'')
	pct := raw.trim_space()
	if pct.len == 0 {
		return -1
	}
	return pct.int()
}

// is_on_ac_power returns true if the machine is currently plugged into AC power.
pub fn (win &SimpleWindow) is_on_ac_power() bool {
	raw := win.exec_or('pmset -g batt 2>/dev/null | head -1', '')
	return raw.contains('AC Power')
}

// get_app_bundle_id returns the bundle identifier of the running app (macOS bundles only).
// Reads Info.plist directly with os.read_file() — no defaults subprocess.
pub fn (win &SimpleWindow) get_app_bundle_id() string {
	exe := os.executable()
	// In a .app bundle the plist is at Contents/Info.plist relative to the binary
	plist_path := os.join_path(os.dir(exe), '..', 'Info.plist')
	if os.exists(plist_path) {
		content := os.read_file(plist_path) or { return '' }
		key_tag := '<key>CFBundleIdentifier</key>'
		key_idx := content.index(key_tag) or { return '' }
		after_key := content[key_idx + key_tag.len..]
		val_start_tag := '<string>'
		val_end_tag := '</string>'
		val_start := after_key.index(val_start_tag) or { return '' }
		after_start := after_key[val_start + val_start_tag.len..]
		val_end := after_start.index(val_end_tag) or { return '' }
		return after_start[..val_end].trim_space()
	}
	return ''
}

// get_system_locale returns the primary locale string configured on the system (e.g. "en_US").
// Reads standard POSIX locale environment variables via os.getenv() — no subprocess.
pub fn (win &SimpleWindow) get_system_locale() string {
	// Check standard POSIX locale env vars in priority order
	for key in ['LC_ALL', 'LC_MESSAGES', 'LANG'] {
		val := os.getenv(key)
		if val.len > 0 {
			// Strip encoding suffix: "en_US.UTF-8" -> "en_US"
			return val.split('.')[0]
		}
	}
	// macOS fallback: read AppleLocale from the user defaults plist directly
	plist := os.join_path(os.home_dir(), 'Library', 'Preferences', '.GlobalPreferences.plist')
	if os.exists(plist) {
		raw := win.exec_or('defaults read NSGlobalDomain AppleLocale 2>/dev/null', '')
		if raw.len > 0 {
			return raw.trim_space()
		}
	}
	return 'unknown'
}

// get_timezone returns the current system timezone string (e.g. "America/Chicago").
// Uses os.real_path() on /etc/localtime — pure V, no readlink/sed subprocess.
pub fn (win &SimpleWindow) get_timezone() string {
	// /etc/localtime is a symlink to the active zoneinfo file
	resolved := os.real_path('/etc/localtime')
	// Strip the zoneinfo prefix to get just the timezone name
	for prefix in ['/var/db/timezone/zoneinfo/', '/usr/share/zoneinfo/', '/usr/lib/zoneinfo/'] {
		if resolved.starts_with(prefix) {
			return resolved[prefix.len..]
		}
	}
	// Final fallback: check the TZ environment variable
	tz := os.getenv('TZ')
	if tz.len > 0 {
		return tz
	}
	return 'unknown'
}

// launch_at_login_add registers the current application with macOS Login Items so it
// auto-launches when the user logs in. Uses osascript/launchctl (best-effort).
pub fn (win &SimpleWindow) launch_at_login_add(app_name_or_path string) &SimpleWindow {
	escaped := app_name_or_path.replace('"', '\\"')
	script := 'osascript -e \'tell application "System Events" to make login item at end with properties {path:"${escaped}", hidden:false}\''
	win.exec_bg(script)
	return win
}

// launch_at_login_remove removes the application from macOS Login Items.
pub fn (win &SimpleWindow) launch_at_login_remove(app_name string) &SimpleWindow {
	escaped := app_name.replace('"', '\\"')
	script := 'osascript -e \'tell application "System Events" to delete login item "${escaped}"\''
	win.exec_bg(script)
	return win
}

// set_dock_badge sets the macOS Dock badge count for the current application.
// Pass 0 to clear the badge.
pub fn (win &SimpleWindow) set_dock_badge(count int) &SimpleWindow {
	if count <= 0 {
		win.exec_bg('osascript -e \'tell application "System Events" to set the dock badge of the front application to 0\'')
	} else {
		win.exec_bg('osascript -e \'tell application "System Events" to set the dock badge of the front application to ${count}\'')
	}
	return win
}

// ==========================================
// 13. System Utilities, Theme, Audio & Helpers
// ==========================================

// is_dark_mode returns true if macOS global appearance is set to Dark Mode.
pub fn (win &SimpleWindow) is_dark_mode() bool {
	style := win.exec_or('defaults read -g AppleInterfaceStyle 2>/dev/null', '').trim_space()
	return style.to_lower() == 'dark'
}

// get_system_theme returns "dark" if macOS is in Dark Mode, otherwise "light".
pub fn (win &SimpleWindow) get_system_theme() string {
	if win.is_dark_mode() {
		return 'dark'
	}
	return 'light'
}

// set_system_dark_mode toggles macOS global Dark Mode on/off.
// Note: this is best-effort and may require Automation permissions for System Events.
pub fn (win &SimpleWindow) set_system_dark_mode(enabled bool) &SimpleWindow {
	value := if enabled { 'true' } else { 'false' }
	win.exec_bg('osascript -e \'tell application "System Events" to tell appearance preferences to set dark mode to ${value}\'')
	return win
}

// set_system_theme sets the macOS global appearance theme to "dark" or "light".
// Returns an error for unsupported values or if execution fails (e.g. missing Automation permissions).
pub fn (win &SimpleWindow) set_system_theme(theme string) !&SimpleWindow {
	mode := theme.trim_space().to_lower()
	if mode != 'dark' && mode != 'light' {
		return error('Invalid theme "${theme}". Use "dark" or "light".')
	}
	enabled := if mode == 'dark' { 'true' } else { 'false' }
	cmd := 'osascript -e \'tell application "System Events" to tell appearance preferences to set dark mode to ${enabled}\''
	res := os.execute(cmd)
	if res.exit_code != 0 {
		return error('Failed to set macOS system theme: ${res.output.trim_space()}')
	}
	return win
}

// sleep_display turns off/puts displays to sleep immediately.
pub fn (win &SimpleWindow) sleep_display() &SimpleWindow {
	win.exec_bg('pmset displaysleepnow')
	return win
}

// sleep_computer puts the Mac to sleep.
pub fn (win &SimpleWindow) sleep_computer() &SimpleWindow {
	win.exec_bg('osascript -e \'tell application "System Events" to sleep\'')
	return win
}

// lock_screen locks the current user session.
pub fn (win &SimpleWindow) lock_screen() &SimpleWindow {
	win.exec_bg('/System/Library/CoreServices/Menu\\ Extras/User.menu/Contents/Resources/CGSession -suspend')
	return win
}

// start_screen_saver launches the built-in macOS screen saver engine.
pub fn (win &SimpleWindow) start_screen_saver() &SimpleWindow {
	win.exec_bg('open -a ScreenSaverEngine')
	return win
}

// log_out_user logs out the current macOS user session.
pub fn (win &SimpleWindow) log_out_user() &SimpleWindow {
	win.exec_bg('osascript -e \'tell application "System Events" to log out\'')
	return win
}

// restart_computer restarts the Mac.
pub fn (win &SimpleWindow) restart_computer() &SimpleWindow {
	win.exec_bg('osascript -e \'tell application "System Events" to restart\'')
	return win
}

// shut_down_computer powers off the Mac.
pub fn (win &SimpleWindow) shut_down_computer() &SimpleWindow {
	win.exec_bg('osascript -e \'tell application "System Events" to shut down\'')
	return win
}

// get_volume returns the system output volume level (0 to 100).
pub fn (win &SimpleWindow) get_volume() int {
	vol_str := win.exec_or("osascript -e 'output volume of (get volume settings)' 2>/dev/null",
		'0').trim_space()
	return vol_str.int()
}

// set_volume sets the system output volume level (0 to 100).
pub fn (win &SimpleWindow) set_volume(level int) &SimpleWindow {
	clamped := if level < 0 {
		0
	} else if level > 100 {
		100
	} else {
		level
	}
	win.exec_bg("osascript -e 'set volume output volume ${clamped}'")
	return win
}

// is_muted returns true if the system output volume is muted.
pub fn (win &SimpleWindow) is_muted() bool {
	muted_str := win.exec_or("osascript -e 'output muted of (get volume settings)' 2>/dev/null",
		'false').trim_space()
	return muted_str.to_lower() == 'true'
}

// set_muted mutes or unmutes system output audio.
pub fn (win &SimpleWindow) set_muted(mute bool) &SimpleWindow {
	val := if mute { 'true' } else { 'false' }
	win.exec_bg("osascript -e 'set volume output muted ${val}'")
	return win
}

// trash_file safely moves a file or directory to the macOS Trash bin instead of permanently deleting it.
pub fn (win &SimpleWindow) trash_file(path string) !&SimpleWindow {
	abs_path := os.real_path(path)
	if !os.exists(abs_path) {
		return error('File does not exist: ${path}')
	}
	escaped := abs_path.replace('"', '\\"')
	script := 'osascript -e \'tell application "Finder" to delete POSIX file "${escaped}"\''
	output, code := win.exec(script)
	if code != 0 {
		return error('Failed to move to Trash: ${output}')
	}
	return win
}

// zip_directory compresses a directory into a .zip archive file.
pub fn (win &SimpleWindow) zip_directory(dir_path string, zip_path string) !&SimpleWindow {
	abs_dir := os.real_path(dir_path)
	if !os.exists(abs_dir) {
		return error('Source directory does not exist: ${dir_path}')
	}
	parent := os.dir(abs_dir)
	base := os.base(abs_dir)
	target_zip := os.real_path(zip_path)
	output, code := win.exec('cd "${parent}" && zip -r "${target_zip}" "${base}"')
	if code != 0 {
		return error('Failed to create zip archive: ${output}')
	}
	return win
}

// unzip_archive extracts a .zip archive file into a target destination directory.
pub fn (win &SimpleWindow) unzip_archive(zip_path string, dest_dir string) !&SimpleWindow {
	abs_zip := os.real_path(zip_path)
	if !os.exists(abs_zip) {
		return error('Zip archive does not exist: ${zip_path}')
	}
	os.mkdir_all(dest_dir)!
	abs_dest := os.real_path(dest_dir)
	output, code := win.exec('unzip -o "${abs_zip}" -d "${abs_dest}"')
	if code != 0 {
		return error('Failed to extract zip archive: ${output}')
	}
	return win
}

// create_temp_file generates and creates a unique temporary file with given prefix and suffix.
pub fn (win &SimpleWindow) create_temp_file(prefix string, suffix string) !string {
	rand_num := time.now().unix_milli()
	file_name := '${prefix}_${rand_num}${suffix}'
	temp_path := os.join_path(os.temp_dir(), file_name)
	os.write_file(temp_path, '')!
	return temp_path
}

// create_temp_dir generates and creates a unique temporary directory with given prefix.
pub fn (win &SimpleWindow) create_temp_dir(prefix string) !string {
	rand_num := time.now().unix_milli()
	dir_name := '${prefix}_${rand_num}'
	temp_path := os.join_path(os.temp_dir(), dir_name)
	os.mkdir_all(temp_path)!
	return temp_path
}

// sha256_file calculates the SHA256 hexadecimal hash digest of a file.
pub fn (win &SimpleWindow) sha256_file(path string) !string {
	bytes := os.read_bytes(path)!
	return sha256.hexhash(bytes.bytestr())
}

// md5_file calculates the MD5 hexadecimal hash digest of a file.
pub fn (win &SimpleWindow) md5_file(path string) !string {
	bytes := os.read_bytes(path)!
	return md5.hexhash(bytes.bytestr())
}

// get_screen_count returns the number of active displays connected to the machine.
pub fn (win &SimpleWindow) get_screen_count() int {
	raw := win.exec_or("system_profiler SPDisplaysDataType 2>/dev/null | grep -c 'Resolution:'",
		'1')
	cnt := raw.trim_space().int()
	return if cnt > 0 { cnt } else { 1 }
}

// is_retina_display checks if the primary display is a high-DPI (Retina) display.
pub fn (win &SimpleWindow) is_retina_display() bool {
	raw := win.exec_or("system_profiler SPDisplaysDataType 2>/dev/null | grep -i 'retina'",
		'')
	if raw.trim_space().len > 0 {
		return true
	}
	res := win.get_screen_resolution()
	parts := res.split('x').map(it.trim_space().int())
	if parts.len >= 2 && parts[0] >= 2560 {
		return true
	}
	return false
}

// is_port_open checks if a TCP port on a given host is accepting connections.
pub fn (win &SimpleWindow) is_port_open(host string, port int) bool {
	_, code := win.exec('nc -z -G 1 "${host}" ${port} 2>/dev/null')
	return code == 0
}

// find_available_port scans ports starting from start_port up to start_port + 100 to find an unused local TCP port.
pub fn (win &SimpleWindow) find_available_port(start_port int) int {
	mut port := start_port
	for port < start_port + 100 {
		if !win.is_port_open('127.0.0.1', port) {
			return port
		}
		port++
	}
	return start_port
}

// prevent_sleep_bg prevents macOS display and system sleep for a given duration in seconds.
pub fn (win &SimpleWindow) prevent_sleep_bg(duration_sec int) &SimpleWindow {
	dur := if duration_sec <= 0 { 60 } else { duration_sec }
	win.exec_bg('caffeinate -t ${dur}')
	return win
}

// get_power_source returns the active macOS power source: "ac", "battery", "ups", or "unknown".
pub fn (win &SimpleWindow) get_power_source() string {
	raw := win.exec_or('pmset -g batt 2>/dev/null | head -n 1', '').trim_space()
	raw_lower := raw.to_lower()
	if raw_lower.contains('ac power') {
		return 'ac'
	}
	if raw_lower.contains('battery power') {
		return 'battery'
	}
	if raw_lower.contains('ups power') {
		return 'ups'
	}
	return 'unknown'
}

// get_battery_charge_percent returns battery percentage (0..100), or -1 when unavailable.
pub fn (win &SimpleWindow) get_battery_charge_percent() int {
	raw := win.exec_or("pmset -g batt 2>/dev/null | grep -Eo '[0-9]+%' | head -n 1 | tr -d '%'",
		'').trim_space()
	if raw != '' {
		return raw.int()
	}
	return win.get_battery_percent()
}

// get_battery_charging_status returns one of: "charging", "discharging", "charged", "not_charging", or "unknown".
pub fn (win &SimpleWindow) get_battery_charging_status() string {
	raw := win.exec_or('pmset -g batt 2>/dev/null | tail -n 1', '').trim_space().to_lower()
	if raw.contains('discharging') {
		return 'discharging'
	}
	if raw.contains('not charging') {
		return 'not_charging'
	}
	if raw.contains('charging') {
		return 'charging'
	}
	if raw.contains('charged') || raw.contains('finishing charge') {
		return 'charged'
	}
	return 'unknown'
}

fn power_caffeinate_pid_file_for_process(pid int) string {
	return os.join_path(os.temp_dir(), 'simplegui_caffeinate_${pid}.pid')
}

// start_prevent_sleep starts an indefinite caffeinate process and tracks its PID for later control.
pub fn (win &SimpleWindow) start_prevent_sleep() &SimpleWindow {
	win.stop_prevent_sleep()
	pid_file := power_caffeinate_pid_file_for_process(win.get_pid())
	win.exec_bg('sh -c \'caffeinate -dimsu >/dev/null 2>&1 & echo $! > "${pid_file}"\'')
	return win
}

// stop_prevent_sleep stops the tracked caffeinate process started by start_prevent_sleep().
pub fn (win &SimpleWindow) stop_prevent_sleep() &SimpleWindow {
	pid_file := power_caffeinate_pid_file_for_process(win.get_pid())
	if !os.exists(pid_file) {
		return win
	}
	pid_raw := os.read_file(pid_file) or { '' }
	pid := pid_raw.trim_space()
	if pid != '' {
		win.exec('kill ${pid} >/dev/null 2>&1')
	}
	os.rm(pid_file) or {}
	return win
}

// is_preventing_sleep reports whether a tracked caffeinate process is currently running.
pub fn (win &SimpleWindow) is_preventing_sleep() bool {
	pid_file := power_caffeinate_pid_file_for_process(win.get_pid())
	if !os.exists(pid_file) {
		return false
	}
	pid_raw := os.read_file(pid_file) or { '' }
	pid := pid_raw.trim_space()
	if pid == '' {
		os.rm(pid_file) or {}
		return false
	}
	_, code := win.exec('kill -0 ${pid} >/dev/null 2>&1')
	if code != 0 {
		os.rm(pid_file) or {}
		return false
	}
	return true
}

// prevent_sleep_while_process_running prevents sleep while target_pid remains alive.
pub fn (win &SimpleWindow) prevent_sleep_while_process_running(target_pid int) &SimpleWindow {
	if target_pid <= 0 {
		return win
	}
	win.exec_bg('caffeinate -dimsu -w ${target_pid}')
	return win
}

// ==========================================
// 14. Developer Productivity Extensions
// ==========================================

// download_file downloads a remote file from a URL and saves it to a target local file path.
pub fn (win &SimpleWindow) download_file(url string, dest_path string) !&SimpleWindow {
	if win.debug_mode {
		println('[simplegui SYSTEM] Downloading file from "${url}" to "${dest_path}"')
	}
	resp := http.get(url)!
	os.write_file(dest_path, resp.body)!
	return win
}

// append_file appends content text to a file (creates file if it does not exist).
pub fn (win &SimpleWindow) append_file(path string, content string) !&SimpleWindow {
	mut f := os.open_append(path)!
	defer { f.close() }
	f.write_string(content)!
	return win
}

// touch_file creates an empty file if it does not exist, or updates its modification time.
pub fn (win &SimpleWindow) touch_file(path string) !&SimpleWindow {
	if !os.exists(path) {
		os.write_file(path, '')!
	} else {
		win.exec('touch "${path}"')
	}
	return win
}

// get_directory_size calculates the total cumulative size of a directory and all contained files in bytes.
pub fn (win &SimpleWindow) get_directory_size(path string) u64 {
	if !os.exists(path) || !os.is_dir(path) {
		return 0
	}
	mut total_bytes := u64(0)
	files := os.walk_ext(path, '')
	for f in files {
		if os.is_file(f) {
			st := os.stat(f) or { continue }
			total_bytes += u64(st.size)
		}
	}
	return total_bytes
}

// exec_in_dir executes a command synchronously inside a specific working directory.
pub fn (win &SimpleWindow) exec_in_dir(dir_path string, command string) (string, int) {
	abs_dir := os.real_path(dir_path)
	return win.exec('cd "${abs_dir}" && ${command}')
}

// is_process_running checks if a process matching the given name is currently active.
pub fn (win &SimpleWindow) is_process_running(proc_name string) bool {
	_, code := win.exec('pgrep -f "${proc_name}" 2>/dev/null')
	return code == 0
}

// kill_process terminates processes matching the given name (`pkill -f`).
pub fn (win &SimpleWindow) kill_process(proc_name string) bool {
	_, code := win.exec('pkill -f "${proc_name}" 2>/dev/null')
	return code == 0
}

// get_machine_id returns the unique hardware UUID of the macOS machine (`IOPlatformUUID`).
pub fn (win &SimpleWindow) get_machine_id() string {
	return win.exec_or('ioreg -rd1 -c IOPlatformExpertDevice 2>/dev/null | awk -F \'"\' \'/IOPlatformUUID/{print $4}\'',
		'unknown').trim_space()
}

// ==========================================
// 15. Advanced macOS System Extensions
// ==========================================

// get_active_app_name returns the name of the active frontmost macOS application.
pub fn (win &SimpleWindow) get_active_app_name() string {
	return win.exec_or('osascript -e \'tell application "System Events" to get name of first application process whose frontmost is true\' 2>/dev/null',
		'unknown').trim_space()
}

// get_active_window_title returns the title of the frontmost focused window.
pub fn (win &SimpleWindow) get_active_window_title() string {
	return win.exec_or('osascript -e \'tell application "System Events" to tell (first application process whose frontmost is true) to get name of window 1\' 2>/dev/null',
		'unknown').trim_space()
}

// get_running_app_names returns a list of names of all active GUI applications running on macOS.
pub fn (win &SimpleWindow) get_running_app_names() []string {
	raw := win.exec_or('osascript -e \'tell application "System Events" to get name of every application process whose background only is false\' 2>/dev/null',
		'')
	if raw.len == 0 {
		return []string{}
	}
	return raw.split(',').map(it.trim_space())
}

// is_apple_silicon returns true if the machine CPU architecture is Apple Silicon (ARM64).
pub fn (win &SimpleWindow) is_apple_silicon() bool {
	arch := win.exec_or('uname -m', '').trim_space()
	return arch == 'arm64'
}

// is_rosetta_emulation returns true if the process is executing under Rosetta 2 binary translation.
pub fn (win &SimpleWindow) is_rosetta_emulation() bool {
	val := win.exec_or('sysctl -n sysctl.proc_translated 2>/dev/null', '0').trim_space()
	return val == '1'
}

// is_sip_enabled returns true if macOS System Integrity Protection (SIP) is active.
pub fn (win &SimpleWindow) is_sip_enabled() bool {
	status := win.exec_or('csrutil status 2>/dev/null', '').to_lower()
	return status.contains('enabled')
}

// get_battery_time_remaining returns estimated remaining battery operating time (e.g. "2:30").
pub fn (win &SimpleWindow) get_battery_time_remaining() string {
	raw := win.exec_or("pmset -g batt 2>/dev/null | grep -oE '[0-9]+:[0-9]+' | head -1",
		'N/A')
	return raw.trim_space()
}

// is_low_power_mode returns true if macOS Low Power Mode is currently enabled.
pub fn (win &SimpleWindow) is_low_power_mode() bool {
	raw := win.exec_or('pmset -g 2>/dev/null', '')
	return raw.contains('lowpowermode 1') || raw.contains('lowpowermode\t1')
}

// take_screenshot captures a screenshot of the entire primary display and saves it to a file.
pub fn (win &SimpleWindow) take_screenshot(target_path string) !&SimpleWindow {
	abs_path := os.real_path(target_path)
	output, code := win.exec('screencapture -x "${abs_path}"')
	if code != 0 {
		return error('screencapture failed: ${output}')
	}
	return win
}

// take_screenshot_window captures a screenshot of the active window and saves it to a file.
pub fn (win &SimpleWindow) take_screenshot_window(target_path string) !&SimpleWindow {
	abs_path := os.real_path(target_path)
	output, code := win.exec('screencapture -cw "${abs_path}"')
	if code != 0 {
		return error('screencapture window failed: ${output}')
	}
	return win
}

// defaults_read reads a preference value from a macOS defaults domain.
pub fn (win &SimpleWindow) defaults_read(domain string, key string) string {
	return win.exec_or('defaults read "${domain}" "${key}" 2>/dev/null', '').trim_space()
}

// defaults_write writes a preference key/value pair into a macOS defaults domain.
pub fn (win &SimpleWindow) defaults_write(domain string, key string, val string) &SimpleWindow {
	win.exec('defaults write "${domain}" "${key}" "${val}"')
	return win
}

// defaults_delete deletes a preference key from a macOS defaults domain.
pub fn (win &SimpleWindow) defaults_delete(domain string, key string) &SimpleWindow {
	win.exec('defaults delete "${domain}" "${key}"')
	return win
}

// open_in_finder opens a folder directly in macOS Finder.
pub fn (win &SimpleWindow) open_in_finder(folder_path string) &SimpleWindow {
	abs_path := os.real_path(folder_path)
	win.exec_bg('open "${abs_path}"')
	return win
}

// empty_trash empties the macOS Trash bin.
pub fn (win &SimpleWindow) empty_trash() &SimpleWindow {
	win.exec_bg('osascript -e \'tell application "Finder" to empty trash\'')
	return win
}

// ==========================================
// 16. Extended Useful System Calls
// ==========================================

// get_process_memory_mb returns the resident set size (RSS) memory usage of a process in Megabytes.
// If pid is 0, retrieves memory for the current process.
pub fn (win &SimpleWindow) get_process_memory_mb(pid int) f64 {
	target_pid := if pid <= 0 { os.getpid() } else { pid }
	raw := win.exec_or('ps -p ${target_pid} -o rss= 2>/dev/null', '0').trim_space()
	kb := raw.f64()
	return kb / 1024.0
}

// get_process_cpu_percent returns CPU usage percentage for a process (or current process if pid is 0).
pub fn (win &SimpleWindow) get_process_cpu_percent(pid int) f64 {
	target_pid := if pid <= 0 { os.getpid() } else { pid }
	raw := win.exec_or('ps -p ${target_pid} -o %cpu= 2>/dev/null', '0').trim_space()
	return raw.f64()
}

// get_parent_pid returns the parent process ID (PPID).
pub fn (win &SimpleWindow) get_parent_pid() int {
	return os.getppid()
}

// get_parent_process_name returns the executable name of the parent process.
pub fn (win &SimpleWindow) get_parent_process_name() string {
	ppid := os.getppid()
	return win.exec_or('ps -p ${ppid} -o comm= 2>/dev/null', 'unknown').trim_space()
}

// get_env_or retrieves an environment variable or returns the default fallback if unset or empty.
pub fn (win &SimpleWindow) get_env_or(key string, default_val string) string {
	val := os.getenv(key)
	if val.len > 0 {
		return val
	}
	return default_val
}

// get_uptime_formatted returns a human-readable system uptime string (e.g. "3 days, 4 hours, 12 mins").
pub fn (win &SimpleWindow) get_uptime_formatted() string {
	sec := win.get_uptime_seconds()
	if sec <= 0 {
		return 'unknown'
	}
	days := sec / 86400
	hours := (sec % 86400) / 3600
	mins := (sec % 3600) / 60
	secs := sec % 60

	if days > 0 {
		return '${days} days, ${hours} hours, ${mins} mins'
	} else if hours > 0 {
		return '${hours} hours, ${mins} mins'
	}
	return '${mins} mins, ${secs} secs'
}

// get_boot_timestamp returns the Unix epoch timestamp of system boot time.
pub fn (win &SimpleWindow) get_boot_timestamp() i64 {
	up := win.get_uptime_seconds()
	if up > 0 {
		return time.now().unix() - up
	}
	return 0
}

// exec_timeout executes a command synchronously with a maximum timeout in milliseconds.
// Returns (output, exit_code, timed_out).
pub fn (win &SimpleWindow) exec_timeout(command string, timeout_ms int) (string, int, bool) {
	ms := if timeout_ms <= 0 { 1000 } else { timeout_ms }
	sec := f64(ms) / 1000.0
	cmd := "perl -e 'alarm ${sec}; exec @ARGV' ${command}"
	res := os.execute(cmd)
	if res.exit_code == 142 || res.exit_code == 124 {
		return '', -1, true
	}
	return res.output.trim_space(), res.exit_code, false
}

// exec_result executes a command and returns a structured production-friendly result.
pub fn (win &SimpleWindow) exec_result(command string) CommandResult {
	start := time.now().unix_milli()
	output, code := win.exec(command)
	return CommandResult{
		command:     command
		output:      output
		exit_code:   code
		timed_out:   false
		duration_ms: time.now().unix_milli() - start
		attempts:    1
	}
}

// exec_timeout_result executes a command with timeout and returns a structured result.
pub fn (win &SimpleWindow) exec_timeout_result(command string, timeout_ms int) CommandResult {
	start := time.now().unix_milli()
	output, code, timed_out := win.exec_timeout(command, timeout_ms)
	return CommandResult{
		command:     command
		output:      output
		exit_code:   code
		timed_out:   timed_out
		duration_ms: time.now().unix_milli() - start
		attempts:    1
	}
}

// exec_retry executes a command with retry + exponential backoff.
// backoff_factor values below 1.0 are clamped to 1.0.
pub fn (win &SimpleWindow) exec_retry(command string, max_attempts int, initial_delay_ms int, backoff_factor f64) CommandResult {
	attempts := if max_attempts < 1 { 1 } else { max_attempts }
	mut delay_ms := if initial_delay_ms < 1 { 100 } else { initial_delay_ms }
	factor := if backoff_factor < 1.0 { 1.0 } else { backoff_factor }
	start := time.now().unix_milli()
	mut final_output := ''
	mut final_code := -1

	for attempt in 1 .. attempts + 1 {
		output, code := win.exec(command)
		final_output = output
		final_code = code
		if code == 0 {
			return CommandResult{
				command:     command
				output:      final_output
				exit_code:   final_code
				timed_out:   false
				duration_ms: time.now().unix_milli() - start
				attempts:    attempt
			}
		}
		if attempt < attempts {
			time.sleep(u64(delay_ms) * time.millisecond)
			delay_ms = int(f64(delay_ms) * factor)
		}
	}

	return CommandResult{
		command:     command
		output:      final_output
		exit_code:   final_code
		timed_out:   false
		duration_ms: time.now().unix_milli() - start
		attempts:    attempts
	}
}

// get_total_memory_bytes returns total physical RAM in bytes.
pub fn (win &SimpleWindow) get_total_memory_bytes() u64 {
	bytes_str := win.exec_or('sysctl -n hw.memsize 2>/dev/null', '0')
	return bytes_str.trim_space().u64()
}

// get_physical_cpu_cores returns the number of physical CPU cores.
pub fn (win &SimpleWindow) get_physical_cpu_cores() int {
	cores_str := win.exec_or('sysctl -n hw.physicalcpu 2>/dev/null', '0')
	return cores_str.trim_space().int()
}

// get_logical_cpu_cores returns the number of logical CPU cores.
pub fn (win &SimpleWindow) get_logical_cpu_cores() int {
	cores_str := win.exec_or('sysctl -n hw.logicalcpu 2>/dev/null', '0')
	return cores_str.trim_space().int()
}

// get_cpu_frequency_hz returns the CPU clock speed frequency in Hz (if available).
pub fn (win &SimpleWindow) get_cpu_frequency_hz() u64 {
	freq_str := win.exec_or('sysctl -n hw.cpufreq 2>/dev/null', '0')
	return freq_str.trim_space().u64()
}

// get_cpu_architecture returns the machine CPU architecture string (e.g. "arm64", "x86_64").
pub fn (win &SimpleWindow) get_cpu_architecture() string {
	return win.exec_or('uname -m 2>/dev/null', 'unknown').trim_space()
}

// get_main_display_bounds returns (x, y, width, height) of the primary display.
pub fn (win &SimpleWindow) get_main_display_bounds() (int, int, int, int) {
	raw := win.exec_or('osascript -e \'tell application "Finder" to get bounds of window of desktop\' 2>/dev/null',
		'')
	if raw.len > 0 {
		parts := raw.split(',').map(it.trim_space().int())
		if parts.len >= 4 {
			return parts[0], parts[1], parts[2] - parts[0], parts[3] - parts[1]
		}
	}
	return 0, 0, 1920, 1080
}

// get_display_scale_factor returns display scale factor (2.0 for Retina, 1.0 for Standard).
pub fn (win &SimpleWindow) get_display_scale_factor() f64 {
	if win.is_retina_display() {
		return 2.0
	}
	return 1.0
}

// get_system_accent_color returns the current macOS accent color name.
pub fn (win &SimpleWindow) get_system_accent_color() string {
	raw := win.exec_or('defaults read -g AppleAccentColor 2>/dev/null', '-1').trim_space()
	return match raw {
		'-1' { 'Multicolor / Default' }
		'0' { 'Red' }
		'1' { 'Orange' }
		'2' { 'Yellow' }
		'3' { 'Green' }
		'4' { 'Blue' }
		'5' { 'Purple' }
		'6' { 'Pink' }
		else { 'Custom (${raw})' }
	}
}

// is_do_not_disturb_enabled returns true if macOS Do Not Disturb / Focus Mode is active.
pub fn (win &SimpleWindow) is_do_not_disturb_enabled() bool {
	raw := win.exec_or("defaults read com.apple.controlcenter 'NSStatusItem Visible FocusModes' 2>/dev/null",
		'0').trim_space()
	if raw == '1' {
		return true
	}
	dnd := win.exec_or('defaults read com.apple.ncprefs dnd_enabled 2>/dev/null', '0').trim_space()
	return dnd == '1'
}

// get_mac_address returns the hardware MAC address of the primary interface (en0).
pub fn (win &SimpleWindow) get_mac_address() string {
	return win.exec_or("ifconfig en0 2>/dev/null | awk '/ether /{print $2}'", '00:00:00:00:00:00').trim_space()
}

// get_dns_servers returns a list of configured DNS server IP addresses.
pub fn (win &SimpleWindow) get_dns_servers() []string {
	raw := win.exec_or("scutil --dns 2>/dev/null | awk '/nameserver\\[[0-9]+\\]/{print $3}' | sort -u",
		'')
	if raw.len == 0 {
		return []string{}
	}
	return raw.split('\n').map(it.trim_space()).filter(it.len > 0)
}

// get_default_gateway returns the IP address of the default network gateway.
pub fn (win &SimpleWindow) get_default_gateway() string {
	return win.exec_or("route -n get default 2>/dev/null | awk '/gateway:/{print $2}'",
		'unknown').trim_space()
}

// is_internet_connected tests if the machine currently has active internet connectivity.
pub fn (win &SimpleWindow) is_internet_connected() bool {
	return win.ping('1.1.1.1', 1) || win.ping('8.8.8.8', 1)
}

// get_listening_ports returns a list of TCP ports currently listening for connections on the machine.
pub fn (win &SimpleWindow) get_listening_ports() []int {
	raw := win.exec_or("lsof -iTCP -sTCP:LISTEN -P -n 2>/dev/null | awk 'NR>1 {print \$9}' | awk -F: '{print \$NF}' | sort -n -u",
		'')
	if raw.len == 0 {
		return []int{}
	}
	lines := raw.split('\n')
	mut ports := []int{}
	for l in lines {
		p := l.trim_space().int()
		if p > 0 && p !in ports {
			ports << p
		}
	}
	return ports
}

// get_platform_label returns the human-readable platform name.
pub fn get_platform_label() string {
	$if macos {
		return 'macOS Cocoa'
	} $else $if linux {
		return 'Linux'
	} $else $if windows {
		return 'Windows'
	} $else {
		return 'Unknown'
	}
}

// get_os_name returns the operating system name ('macOS', 'Linux', 'Windows', or 'Unknown').
pub fn get_os_name() string {
	$if macos {
		return 'macOS'
	} $else $if linux {
		return 'Linux'
	} $else $if windows {
		return 'Windows'
	} $else {
		return 'Unknown'
	}
}

// get_platform_label returns the human-readable platform label for this window.
pub fn (win &SimpleWindow) get_platform_label() string {
	return get_platform_label()
}

// get_os_name returns the operating system name for this window.
pub fn (win &SimpleWindow) get_os_name() string {
	return get_os_name()
}

// get_app_name infers a clean, filesystem-safe application identifier from window title, bundle ID, or binary name.
pub fn (win &SimpleWindow) get_app_name() string {
	if win.title.trim_space() != '' {
		mut clean := ''
		for c in win.title.trim_space() {
			if c.is_alnum() || c == `_` || c == `-` {
				clean += c.ascii_str()
			} else if c == ` ` {
				clean += '_'
			}
		}
		clean = clean.trim('_')
		if clean != '' {
			return clean
		}
	}
	bundle := win.get_app_bundle_id()
	if bundle != '' && bundle != 'unknown' {
		parts := bundle.split('.')
		if parts.len > 0 && parts.last() != '' {
			return parts.last()
		}
		return bundle
	}
	exe := os.file_name(os.executable()).all_before_last('.')
	if exe != '' && exe != 'v' && exe != 'main' {
		return exe
	}
	return 'simplegui_app'
}

// get_app_storage_dir returns the recommended user storage directory path for the app (~/Library/Application Support/<app_name> on macOS).
// Automatically creates the directory if it does not exist.
pub fn (win &SimpleWindow) get_app_storage_dir(app_name ...string) string {
	target_app := if app_name.len > 0 && app_name[0].trim_space() != '' {
		app_name[0].trim_space()
	} else {
		win.get_app_name()
	}
	base := os.config_dir() or {
		os.join_path(os.home_dir(), 'Library', 'Application Support')
	}
	dir := os.join_path(base, target_app)
	if !os.exists(dir) {
		os.mkdir_all(dir) or {}
	}
	return dir
}

// get_app_cache_dir returns the recommended cache directory path for the app (~/Library/Caches/<app_name> on macOS).
// Automatically creates the directory if it does not exist.
pub fn (win &SimpleWindow) get_app_cache_dir(app_name ...string) string {
	target_app := if app_name.len > 0 && app_name[0].trim_space() != '' {
		app_name[0].trim_space()
	} else {
		win.get_app_name()
	}
	base := os.cache_dir()
	dir := os.join_path(base, target_app)
	if !os.exists(dir) {
		os.mkdir_all(dir) or {}
	}
	return dir
}

// get_app_storage_path returns the full path for a file inside the recommended user storage directory.
// Automatically creates any required parent subdirectories.
pub fn (win &SimpleWindow) get_app_storage_path(filename string, app_name ...string) string {
	if filename.starts_with('/') {
		dir := os.dir(filename)
		if !os.exists(dir) {
			os.mkdir_all(dir) or {}
		}
		return filename
	}
	if filename.starts_with('~') {
		expanded := filename.replace('~', os.home_dir())
		dir := os.dir(expanded)
		if !os.exists(dir) {
			os.mkdir_all(dir) or {}
		}
		return expanded
	}
	storage_dir := win.get_app_storage_dir(...app_name)
	target := os.join_path(storage_dir, filename)
	dir := os.dir(target)
	if !os.exists(dir) {
		os.mkdir_all(dir) or {}
	}
	return target
}

// resolve_storage_path resolves a path to an absolute path:
// - Absolute paths (starting with / or ~) have ~ expanded to home directory.
// - Relative filenames/paths automatically resolve into the recommended user application storage directory (~/Library/Application Support/<app_name>/).
// This ensures that apps moved to /Applications or installed globally persist their state properly.
pub fn (win &SimpleWindow) resolve_storage_path(path string, app_name ...string) string {
	return win.get_app_storage_path(path, ...app_name)
}

// get_app_data_dir returns the user application support directory path for app_name.
pub fn (win &SimpleWindow) get_app_data_dir(args ...string) string {
	if args.len > 0 && args[0].len > 0 {
		return get_app_data_dir(args[0])
	}
	return get_app_data_dir(win.get_app_id())
}

// get_app_data_file returns the full file path for an app data file.
pub fn (win &SimpleWindow) get_app_data_file(args ...string) string {
	if args.len >= 2 {
		return get_app_data_file(args[0], args[1])
	} else if args.len == 1 {
		return get_app_data_file(win.get_app_id(), args[0])
	}
	return get_app_data_file(win.get_app_id(), 'data.json')
}

// get_user_downloads_dir returns absolute path to Downloads folder.
pub fn (win &SimpleWindow) get_user_downloads_dir() string {
	return win.get_system_path('downloads')
}

// get_user_documents_dir returns absolute path to Documents folder.
pub fn (win &SimpleWindow) get_user_documents_dir() string {
	return win.get_system_path('documents')
}

// get_user_desktop_dir returns absolute path to Desktop folder.
pub fn (win &SimpleWindow) get_user_desktop_dir() string {
	return win.get_system_path('desktop')
}

// get_free_disk_space returns free bytes available on the volume hosting the given path.
pub fn (win &SimpleWindow) get_free_disk_space(path string) u64 {
	ds := win.get_disk_usage(path) or { return 0 }
	return ds.available
}

// copy_directory copies a directory and its contents recursively to dest.
pub fn (win &SimpleWindow) copy_directory(src string, dest string) !&SimpleWindow {
	abs_src := os.real_path(src)
	if !os.exists(abs_src) {
		return error('Source directory does not exist: ${src}')
	}
	os.mkdir_all(dest)!
	abs_dest := os.real_path(dest)
	output, code := win.exec('cp -R "${abs_src}/." "${abs_dest}/"')
	if code != 0 {
		return error('Failed to copy directory: ${output}')
	}
	return win
}

// write_file_atomic writes file contents using a temp file + rename strategy.
// This is best-effort atomic when source and destination are on the same volume.
pub fn (win &SimpleWindow) write_file_atomic(path string, content string) !&SimpleWindow {
	target_dir := os.dir(path)
	if target_dir.len == 0 {
		return error('Invalid target path: ${path}')
	}
	os.mkdir_all(target_dir)!
	tmp_name := '.${os.base(path)}.tmp_${os.getpid()}_${time.now().unix_milli()}'
	tmp_path := os.join_path(target_dir, tmp_name)
	os.write_file(tmp_path, content)!
	os.mv(tmp_path, path)!
	return win
}

// tail_file returns up to the last max_lines lines from a file.
pub fn (win &SimpleWindow) tail_file(path string, max_lines int) ![]string {
	lines := os.read_lines(path)!
	if max_lines <= 0 || lines.len <= max_lines {
		return lines
	}
	start := lines.len - max_lines
	return lines[start..]
}

// wait_for_file waits until a file exists or the timeout expires.
pub fn (win &SimpleWindow) wait_for_file(path string, timeout_ms int, poll_ms int) bool {
	deadline := time.now().unix_milli() + i64(if timeout_ms <= 0 { 1000 } else { timeout_ms })
	interval_ms := if poll_ms <= 0 { 50 } else { poll_ms }
	for time.now().unix_milli() <= deadline {
		if os.exists(path) {
			return true
		}
		time.sleep(u64(interval_ms) * time.millisecond)
	}
	return os.exists(path)
}

// wait_for_port waits until a TCP port becomes reachable or timeout expires.
pub fn (win &SimpleWindow) wait_for_port(host string, port int, timeout_ms int, poll_ms int) bool {
	if port <= 0 {
		return false
	}
	deadline := time.now().unix_milli() + i64(if timeout_ms <= 0 { 1000 } else { timeout_ms })
	interval_ms := if poll_ms <= 0 { 100 } else { poll_ms }
	for time.now().unix_milli() <= deadline {
		if win.is_port_open(host, port) {
			return true
		}
		time.sleep(u64(interval_ms) * time.millisecond)
	}
	return win.is_port_open(host, port)
}

// play_system_sound plays a macOS system alert sound (e.g. "Glass", "Ping", "Hero", "Pop", "Tink", "Submarine").
pub fn (win &SimpleWindow) play_system_sound(sound_name string) &SimpleWindow {
	sound_path := '/System/Library/Sounds/${sound_name}.aiff'
	if os.exists(sound_path) {
		win.exec_bg('afplay "${sound_path}"')
	} else {
		win.beep()
	}
	return win
}

// speak_with_voice speaks text out loud using a specific macOS voice name (e.g. "Samantha", "Alex", "Fred", "Victoria").
pub fn (win &SimpleWindow) speak_with_voice(text string, voice string) &SimpleWindow {
	escaped := text.replace('"', '\\"')
	v_esc := voice.replace('"', '\\"')
	win.exec_bg('say -v "${v_esc}" "${escaped}"')
	return win
}

// toggle_dark_mode toggles macOS appearance between Light Mode and Dark Mode.
pub fn (win &SimpleWindow) toggle_dark_mode() &SimpleWindow {
	win.exec_bg('osascript -e \'tell application "System Events" to tell appearance preferences to set dark mode to not dark mode\'')
	return win
}

// kill_process_by_pid terminates a process using its process ID (PID).
pub fn (win &SimpleWindow) kill_process_by_pid(pid int) bool {
	if pid <= 0 {
		return false
	}
	_, code := win.exec('kill -9 ${pid} 2>/dev/null')
	return code == 0
}

// kill_process_by_name terminates processes matching the given name (`pkill -f`). Alias for `kill_process`.
pub fn (win &SimpleWindow) kill_process_by_name(proc_name string) bool {
	return win.kill_process(proc_name)
}

// kill_process_exact terminates processes matching the exact name (`pkill -x`).
pub fn (win &SimpleWindow) kill_process_exact(proc_name string) bool {
	if proc_name.len == 0 {
		return false
	}
	_, code := win.exec('pkill -x "${proc_name}" 2>/dev/null')
	return code == 0
}

// ==========================================
// Standalone Package-Level Helpers
// ==========================================

// play_system_sound plays a native macOS system sound by name (e.g. "Glass", "Ping", "Hero", "Pop", "Tink", "Submarine").
pub fn play_system_sound(sound_name string) {
	sound_path := '/System/Library/Sounds/${sound_name}.aiff'
	if os.exists(sound_path) {
		os.execute_opt('afplay "${sound_path}" &') or {}
	} else {
		beep()
	}
}

// speak_with_voice speaks text out loud using a specific macOS voice (e.g. "Samantha", "Alex", "Fred").
pub fn speak_with_voice(text string, voice string) {
	escaped := text.replace('"', '\\"')
	v_esc := voice.replace('"', '\\"')
	os.execute_opt('say -v "${v_esc}" "${escaped}" &') or {}
}

// toggle_dark_mode toggles macOS system appearance mode between Light Mode and Dark Mode.
pub fn toggle_dark_mode() {
	os.execute_opt('osascript -e \'tell application "System Events" to tell appearance preferences to set dark mode to not dark mode\' &') or {}
}

// ==========================================
// Cross-Window Spy++ Application Registry & Remote Control
// ==========================================
struct WindowRegistry {
mut:
	windows map[string]&SimpleWindow
}

fn get_window_registry() &WindowRegistry {
	unsafe {
		mut static reg := &WindowRegistry(nil)
		if reg == nil {
			reg = &WindowRegistry{
				windows: map[string]&SimpleWindow{}
			}
		}
		return reg
	}
}

// sys_register_window registers a window in the global application registry for cross-window Spy++ control.
pub fn sys_register_window(win &SimpleWindow) {
	mut reg := get_window_registry()
	reg.windows[win.title] = win
}

// sys_unregister_window unregisters a window from the global application registry.
pub fn sys_unregister_window(title string) {
	mut reg := get_window_registry()
	reg.windows.delete(title)
}

// sys_list_app_windows returns titles of all registered application windows.
pub fn sys_list_app_windows() []string {
	reg := get_window_registry()
	mut titles := []string{cap: reg.windows.len}
	for title, _ in reg.windows {
		titles << title
	}
	return titles
}

// sys_get_window returns a registered window instance by its title.
pub fn sys_get_window(title string) ?&SimpleWindow {
	reg := get_window_registry()
	win := reg.windows[title] or { return none }
	return win
}

// sys_order_app_window_front brings a registered internal window to the front by title.
pub fn sys_order_app_window_front(title string) bool {
	win := sys_get_window(title) or { return false }
	win.order_front()
	return true
}

// sys_order_app_window_back sends a registered internal window behind others by title.
pub fn sys_order_app_window_back(title string) bool {
	win := sys_get_window(title) or { return false }
	win.order_back()
	return true
}

// sys_set_app_window_visible shows or hides a registered internal window by title.
pub fn sys_set_app_window_visible(title string, visible bool) bool {
	win := sys_get_window(title) or { return false }
	if visible {
		win.show_window()
	} else {
		win.hide()
	}
	return true
}

// sys_spy_window inspects all controls of a registered application window by title.
pub fn sys_spy_window(title string) ?[]ControlInfo {
	win := sys_get_window(title) or { return none }
	return win.spy_controls()
}

// sys_set_control_enabled sets the enabled state of a control in a target window by window title.
pub fn sys_set_control_enabled(win_title string, control_name string, enabled bool) bool {
	win := sys_get_window(win_title) or { return false }
	win.set_control_enabled(control_name, enabled)
	return true
}

// sys_set_control_visible sets the visible state of a control in a target window by window title.
pub fn sys_set_control_visible(win_title string, control_name string, visible bool) bool {
	win := sys_get_window(win_title) or { return false }
	win.set_control_visible(control_name, visible)
	return true
}

// sys_set_control_text sets the text/value of a control in a target window by window title.
pub fn sys_set_control_text(win_title string, control_name string, text string) bool {
	win := sys_get_window(win_title) or { return false }
	win.set_control_text(control_name, text)
	return true
}

// sys_get_control_text gets the text/value of a control in a target window by window title.
pub fn sys_get_control_text(win_title string, control_name string) string {
	win := sys_get_window(win_title) or { return '' }
	return win.get_control_text(control_name)
}

// sys_flash_control visually flashes a control outline in a target window by window title.
pub fn sys_flash_control(win_title string, control_name string) bool {
	win := sys_get_window(win_title) or { return false }
	win.flash_control(control_name)
	return true
}

// ==========================================
// Live Cross-Window Event Streaming Bus
// ==========================================

// SystemEventCallback is the function signature for cross-window event stream observers.
pub type SystemEventCallback = fn (win_title string, control_name string, event_name string, value string)

struct EventStreamBus {
mut:
	subscribers []SystemEventCallback
}

fn get_event_bus() &EventStreamBus {
	unsafe {
		mut static bus := &EventStreamBus(nil)
		if bus == nil {
			bus = &EventStreamBus{
				subscribers: []SystemEventCallback{}
			}
		}
		return bus
	}
}

// sys_subscribe_events registers a global callback to receive all live UI events triggered across all simplegui windows.
pub fn sys_subscribe_events(callback SystemEventCallback) {
	mut bus := get_event_bus()
	bus.subscribers << callback
}

// sys_broadcast_event dispatches an event to all global event stream subscribers.
pub fn sys_broadcast_event(win_title string, control_name string, event_name string, value string) {
	bus := get_event_bus()
	for cb in bus.subscribers {
		cb(win_title, control_name, event_name, value)
	}
}

// ==========================================
// External macOS Applications Accessibility Inspection (AXUIElement)
// ==========================================

// ExternalAppInfo represents metadata for a running GUI desktop application on macOS.
pub struct ExternalAppInfo {
pub mut:
	pid       int
	name      string
	bundle_id string
}

// ExternalControlInfo represents metadata for a control inside an external application window.
pub struct ExternalControlInfo {
pub mut:
	role    string
	title   string
	value   string
	enabled bool
}

// sys_list_external_apps lists all running GUI applications on macOS.
pub fn sys_list_external_apps() []ExternalAppInfo {
	unsafe {
		json_ptr := C.window_list_external_apps()
		json_str := json_ptr.vstring()
		if json_str.len == 0 || json_str == '[]' {
			return []ExternalAppInfo{}
		}
		res := json2.decode[[]ExternalAppInfo](json_str) or { return []ExternalAppInfo{} }
		return res
	}
}

// sys_spy_external_app inspects windows and controls of an external application by PID using macOS AXUIElement.
pub fn sys_spy_external_app(pid int) []ExternalControlInfo {
	unsafe {
		json_ptr := C.window_spy_external_app(pid)
		json_str := json_ptr.vstring()
		if json_str.len == 0 || json_str == '[]' {
			return []ExternalControlInfo{}
		}
		res := json2.decode[[]ExternalControlInfo](json_str) or { return []ExternalControlInfo{} }
		return res
	}
}

// sys_set_external_control_value sets text or value on a control of an external application by PID.
pub fn sys_set_external_control_value(pid int, control_title string, value string) bool {
	unsafe {
		return C.window_set_external_control_value(pid, control_title.str, value.str) == 1
	}
}

// sys_press_external_control triggers action/click on a control of an external application by PID.
pub fn sys_press_external_control(pid int, control_title string) bool {
	unsafe {
		return C.window_press_external_control(pid, control_title.str) == 1
	}
}

// sys_set_external_control_enabled enables or disables a control in an external application by PID.
pub fn sys_set_external_control_enabled(pid int, control_title string, enabled bool) bool {
	unsafe {
		en_val := if enabled { 1 } else { 0 }
		return C.window_set_external_control_enabled(pid, control_title.str, en_val) == 1
	}
}

// sys_set_external_control_visible shows or hides a control in an external application by PID.
pub fn sys_set_external_control_visible(pid int, control_title string, visible bool) bool {
	unsafe {
		vis_val := if visible { 1 } else { 0 }
		return C.window_set_external_control_visible(pid, control_title.str, vis_val) == 1
	}
}

// sys_flash_external_control draws a temporary visual highlight overlay on screen over an external app control by PID.
pub fn sys_flash_external_control(pid int, control_title string) bool {
	unsafe {
		return C.window_flash_external_control(pid, control_title.str) == 1
	}
}

// sys_set_external_app_frontmost requests that an external application process becomes frontmost.
pub fn sys_set_external_app_frontmost(pid int) bool {
	if pid <= 0 {
		return false
	}
	cmd := 'osascript -e \'tell application "System Events" to set frontmost of (first process whose unix id is ${pid}) to true\''
	res := os.execute(cmd)
	return res.exit_code == 0
}

// sys_set_external_app_visible shows or hides an external application process.
pub fn sys_set_external_app_visible(pid int, visible bool) bool {
	if pid <= 0 {
		return false
	}
	vis := if visible { 'true' } else { 'false' }
	cmd := 'osascript -e \'tell application "System Events" to set visible of (first process whose unix id is ${pid}) to ${vis}\''
	res := os.execute(cmd)
	return res.exit_code == 0
}

// clear_clipboard clears the system clipboard.
pub fn (win &SimpleWindow) clear_clipboard() &SimpleWindow {
	return win.copy_to_clipboard('')
}

// get_user_home_dir returns the current user's home directory.
pub fn get_user_home_dir() string {
	return os.home_dir()
}

// get_app_config_dir returns the standard directory for application configuration files.
pub fn get_app_config_dir(app_name string) string {
	$if macos {
		return os.join_path(os.home_dir(), 'Library', 'Application Support', app_name)
	} $else $if windows {
		app_data := os.getenv('APPDATA')
		if app_data.len > 0 {
			return os.join_path(app_data, app_name)
		}
		return os.join_path(os.home_dir(), 'AppData', 'Roaming', app_name)
	} $else {
		xdg := os.getenv('XDG_CONFIG_HOME')
		if xdg.len > 0 {
			return os.join_path(xdg, app_name)
		}
		return os.join_path(os.home_dir(), '.config', app_name)
	}
}

// get_app_config_file returns the full file path for an app's configuration file.
pub fn get_app_config_file(app_name string, filename string) string {
	return os.join_path(get_app_config_dir(app_name), filename)
}

// get_app_data_dir returns the standard directory for application data files.
pub fn get_app_data_dir(app_name string) string {
	$if macos {
		return os.join_path(os.home_dir(), 'Library', 'Application Support', app_name)
	} $else $if windows {
		app_data := os.getenv('APPDATA')
		if app_data.len > 0 {
			return os.join_path(app_data, app_name)
		}
		return os.join_path(os.home_dir(), 'AppData', 'Roaming', app_name)
	} $else {
		xdg := os.getenv('XDG_DATA_HOME')
		if xdg.len > 0 {
			return os.join_path(xdg, app_name)
		}
		return os.join_path(os.home_dir(), '.local', 'share', app_name)
	}
}

// get_app_data_file returns the full file path for an app data file.
pub fn get_app_data_file(app_name string, filename string) string {
	return os.join_path(get_app_data_dir(app_name), filename)
}

// get_app_state_dir returns the standard directory for runtime state files.
pub fn get_app_state_dir(app_name string) string {
	$if macos {
		return os.join_path(os.home_dir(), 'Library', 'Application Support', app_name, 'State')
	} $else $if windows {
		return os.join_path(get_app_data_dir(app_name), 'State')
	} $else {
		xdg := os.getenv('XDG_STATE_HOME')
		if xdg.len > 0 {
			return os.join_path(xdg, app_name)
		}
		return os.join_path(os.home_dir(), '.local', 'state', app_name)
	}
}

// get_app_state_file returns the full file path for an app state file.
pub fn get_app_state_file(app_name string, filename string) string {
	return os.join_path(get_app_state_dir(app_name), filename)
}

// get_app_cache_dir returns the standard directory for cache files.
pub fn get_app_cache_dir(app_name string) string {
	$if macos {
		return os.join_path(os.home_dir(), 'Library', 'Caches', app_name)
	} $else $if windows {
		return os.join_path(get_app_data_dir(app_name), 'Cache')
	} $else {
		xdg := os.getenv('XDG_CACHE_HOME')
		if xdg.len > 0 {
			return os.join_path(xdg, app_name)
		}
		return os.join_path(os.home_dir(), '.cache', app_name)
	}
}

// get_app_cache_file returns the full file path for an app cache file.
pub fn get_app_cache_file(app_name string, filename string) string {
	return os.join_path(get_app_cache_dir(app_name), filename)
}

// get_app_log_dir returns the standard directory for application logs.
pub fn get_app_log_dir(app_name string) string {
	$if macos {
		return os.join_path(os.home_dir(), 'Library', 'Logs', app_name)
	} $else $if windows {
		return os.join_path(get_app_data_dir(app_name), 'Logs')
	} $else {
		return os.join_path(get_app_state_dir(app_name), 'logs')
	}
}

// get_app_log_file returns the full file path for an app log file.
pub fn get_app_log_file(app_name string, filename string) string {
	return os.join_path(get_app_log_dir(app_name), filename)
}

// get_app_runtime_dir returns the runtime directory for temporary PID or socket files.
pub fn get_app_runtime_dir(app_name string) string {
	return os.join_path(os.temp_dir(), app_name)
}

// resolve_user_path expands user home tildes (~ and ~/ or ~\), environment variables ($VAR, ${VAR}, %VAR%),
// and normalizes directory separators for the current operating system.
pub fn resolve_user_path(raw_path string) string {
	trimmed := raw_path.trim_space()
	if trimmed == '' {
		return ''
	}

	mut p := trimmed

	// 1. Expand environment variables: ${VAR}, %VAR%, $VAR
	for p.contains(r'${') {
		start := p.index(r'${') or { break }
		end := p.index_after(r'}', start) or { break }
		var_name := p[start + 2..end]
		env_val := os.getenv(var_name)
		p = p[..start] + env_val + p[end + 1..]
	}

	if p.contains('%') {
		parts := p.split('%')
		if parts.len >= 3 {
			mut sb := []string{}
			mut i := 0
			for i < parts.len {
				if i + 1 < parts.len && i % 2 == 1 {
					var_name := parts[i]
					env_val := os.getenv(var_name)
					sb << env_val
				} else {
					sb << parts[i]
				}
				i++
			}
			p = sb.join('')
		}
	}

	if p.contains('$') {
		mut res := []u8{cap: p.len}
		mut i := 0
		bytes := p.bytes()
		for i < bytes.len {
			if bytes[i] == `$` && i + 1 < bytes.len && (bytes[i + 1].is_letter() || bytes[i + 1] == `_`) {
				mut j := i + 1
				for j < bytes.len && (bytes[j].is_letter() || bytes[j].is_digit() || bytes[j] == `_`) {
					j++
				}
				var_name := p[i + 1..j]
				env_val := os.getenv(var_name)
				for b in env_val.bytes() {
					res << b
				}
				i = j
			} else {
				res << bytes[i]
				i++
			}
		}
		p = res.bytestr()
	}

	// 2. Expand tildes (~ and ~/ or ~\ )
	home := get_user_home_dir()
	if p == '~' {
		p = home
	} else if p.starts_with('~/') || p.starts_with('~\\') {
		p = os.join_path(home, p[2..])
	}

	// 3. Normalize path separators per OS
	$if windows {
		p = p.replace('/', '\\')
	} $else {
		p = p.replace('\\', '/')
	}

	return p
}

// expand_user_path expands '~' and environment variables to user's paths.
pub fn expand_user_path(raw_path string) string {
	return resolve_user_path(raw_path)
}

// get_app_config_file returns the full path for a config file.
pub fn (win &SimpleWindow) get_app_config_file(args ...string) string {
	if args.len >= 2 {
		return get_app_config_file(args[0], args[1])
	} else if args.len == 1 {
		return get_app_config_file(win.get_app_id(), args[0])
	}
	return get_app_config_file(win.get_app_id(), 'config.json')
}

// get_app_state_dir returns the state directory for this app.
pub fn (win &SimpleWindow) get_app_state_dir(args ...string) string {
	app := if args.len > 0 && args[0].len > 0 { args[0] } else { win.get_app_id() }
	return get_app_state_dir(app)
}

// get_app_state_file returns the full path for a state file.
pub fn (win &SimpleWindow) get_app_state_file(args ...string) string {
	if args.len >= 2 {
		return get_app_state_file(args[0], args[1])
	} else if args.len == 1 {
		return get_app_state_file(win.get_app_id(), args[0])
	}
	return get_app_state_file(win.get_app_id(), 'state.json')
}

// get_app_cache_file returns the full path for a cache file.
pub fn (win &SimpleWindow) get_app_cache_file(args ...string) string {
	if args.len >= 2 {
		return get_app_cache_file(args[0], args[1])
	} else if args.len == 1 {
		return get_app_cache_file(win.get_app_id(), args[0])
	}
	return get_app_cache_file(win.get_app_id(), 'cache.bin')
}

// get_app_log_dir returns the log directory for this app.
pub fn (win &SimpleWindow) get_app_log_dir(args ...string) string {
	app := if args.len > 0 && args[0].len > 0 { args[0] } else { win.get_app_id() }
	return get_app_log_dir(app)
}

// get_app_log_file returns the full path for a log file.
pub fn (win &SimpleWindow) get_app_log_file(args ...string) string {
	if args.len >= 2 {
		return get_app_log_file(args[0], args[1])
	} else if args.len == 1 {
		return get_app_log_file(win.get_app_id(), args[0])
	}
	return get_app_log_file(win.get_app_id(), 'app.log')
}

// get_app_runtime_dir returns the runtime directory for this app.
pub fn (win &SimpleWindow) get_app_runtime_dir(args ...string) string {
	app := if args.len > 0 && args[0].len > 0 { args[0] } else { win.get_app_id() }
	return get_app_runtime_dir(app)
}

// resolve_user_path expands '~' and environment variables to paths.
pub fn (win &SimpleWindow) resolve_user_path(raw_path string) string {
	return resolve_user_path(raw_path)
}

// expand_user_path expands '~' and environment variables to paths.
pub fn (win &SimpleWindow) expand_user_path(raw_path string) string {
	return resolve_user_path(raw_path)
}

// ExecResult contains detailed results for process execution with retries or timeouts.
pub struct ExecResult {
pub:
	output      string
	exit_code   int
	duration_ms i64
	timed_out   bool
	attempts    int
}

// parallel_exec executes multiple commands concurrently using background threads.
pub fn (win &SimpleWindow) parallel_exec(commands []string) []ExecResult {
	mut results := []ExecResult{len: commands.len}
	mut threads := []thread ExecResult{}

	for cmd in commands {
		threads << spawn fn (c string) ExecResult {
			start := time.now()
			res := os.execute(c)
			return ExecResult{
				output:      res.output.trim_space()
				exit_code:   res.exit_code
				duration_ms: time.since(start).milliseconds()
				timed_out:   false
				attempts:    1
			}
		}(cmd)
	}

	for i, t in threads {
		results[i] = t.wait()
	}
	return results
}

// has_command checks whether an executable binary exists in the system PATH.
pub fn (win &SimpleWindow) has_command(cmd_name string) bool {
	return win.find_executable(cmd_name).len > 0
}

// get_command_path returns absolute path to executable binary in system PATH.
pub fn (win &SimpleWindow) get_command_path(cmd_name string) string {
	return win.find_executable(cmd_name)
}

// get_ip_address returns the local IPv4 address.
pub fn (win &SimpleWindow) get_ip_address() string {
	return win.get_local_ip()
}

// get_public_ip fetches the public IP address via external HTTP resolver.
pub fn (win &SimpleWindow) get_public_ip() string {
	res := http.get('https://api.ipify.org') or { return '' }
	return res.body.trim_space()
}

// sys_beep plays terminal/system bell beep.
pub fn sys_beep() {
	print('\x07')
	os.flush()
}

// sys_beep_cross plays system bell beep.
pub fn sys_beep_cross() {
	sys_beep()
}

// linux_font_candidates returns candidate paths for system TTF/OTF fonts on Linux.
pub fn linux_font_candidates() []string {
	return [
		'/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',
		'/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf',
		'/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf',
		'/usr/share/fonts/truetype/noto/NotoSans-Regular.ttf',
		'/usr/share/fonts/TTF/DejaVuSans.ttf',
	]
}

// macos_font_candidates returns candidate paths for system TTF/OTF fonts on macOS.
pub fn macos_font_candidates() []string {
	return [
		'/System/Library/Fonts/Supplemental/Arial.ttf',
		'/System/Library/Fonts/Supplemental/Helvetica.ttf',
		'/System/Library/Fonts/Supplemental/Times New Roman.ttf',
		'/System/Library/Fonts/Supplemental/Courier New.ttf',
		'/System/Library/Fonts/Supplemental/Georgia.ttf',
		'/System/Library/Fonts/Supplemental/Verdana.ttf',
		'/System/Library/Fonts/Monaco.ttf',
		'/Library/Fonts/Arial.ttf',
	]
}

// resolve_window_font_path resolves custom or system font path.
pub fn resolve_window_font_path() string {
	custom := os.getenv('SIMPLEGUI_FONT_PATH')
	if custom.len > 0 && os.exists(custom) {
		return custom
	}
	$if macos {
		for candidate in macos_font_candidates() {
			if os.exists(candidate) {
				return candidate
			}
		}
	} $else $if linux {
		for candidate in linux_font_candidates() {
			if os.exists(candidate) {
				return candidate
			}
		}
	}
	return ''
}
