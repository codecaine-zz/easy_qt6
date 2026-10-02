module main

import simplegui
import os

// ----------------------------------------------------
// Production-Ready Markdown Studio Demo
// ----------------------------------------------------
// A complete, highly functional live Markdown editor
// utilizing WebView rendering, responsive rows/columns,
// native Cocoa file and clipboard dialogue operations,
// and dynamic custom CSS theme synchronization.
// ----------------------------------------------------

struct AppState {
mut:
	current_file string = 'Untitled.md'
	theme_name   string = 'dracula'
}

fn main() {
	mut state := AppState{}

	// Create window
	mut win := simplegui.new_simple_window('Production Markdown Studio', 840, 680)
	win.set_theme(state.theme_name)
	win.set_padding(18)
	win.set_spacing(10)

	// Top Section: Title & Controls

	win.add_label('studio_title', 'Markdown Live Studio')
		.bold(true)
		.font_size(20)
		.font_color('#8be9fd') // Dracula cyan

	win.add_label('subtitle', 'Write document content using standard Markdown syntax and view interactive parsed elements live.')
		.font_size(11)
		.font_color('#6272a4')

	win.add_separator()

	// Toolbar Pane
	win.begin_row('toolbar')
	win.add_label('lbl_tmpl', 'Template:')
	win.add_dropdown('template', ['Features Guide', 'Code Showcase', 'Project Planner'],
		'Features Guide')

	win.add_button('btn_open', 'Open File...')
	win.add_button('btn_save', 'Save File...')
	win.add_button('btn_copy', 'Copy HTML')

	win.add_label('lbl_theme', 'Theme:')
	win.add_theme_menu('editor_theme', 'Dracula')
	win.end_row()

	win.add_separator()

	// Initial text setup
	initial_text := get_template('Features Guide')
	initial_html := markdown_to_html(initial_text, state.theme_name)

	// Main Editor / Preview Split Pane
	win.begin_row('studio_split')
	win.add_textarea('editor', initial_text)
	win.add_html_view('preview', initial_html)
	win.end_row()

	// Explicit dimensional sizing
	win.set_control_width('editor', 395)
	win.set_control_height('editor', 450)
	win.set_control_width('preview', 395)
	win.set_control_height('preview', 450)

	// Add file drag & drop event listener
	win.on_file_drop(fn [mut state] (mut w simplegui.SimpleWindow, files []string) {
		if files.len > 0 {
			target := files[0]
			if target.ends_with('.md') || target.ends_with('.txt') {
				content := os.read_file(target) or {
					w.alert('Error', 'Unable to read dropped file.')
					return
				}
				state.current_file = target
				w.set_text('editor', content)

				// Force render update
				rendered_html := markdown_to_html(content, state.theme_name)
				w.set_html('preview', rendered_html)

				update_status(mut w, state.current_file, content)
				w.toast('Imported ${os.file_name(target)}')
			} else {
				w.alert('Unsupported File', 'Please drop a .md or .txt file.')
			}
		}
	})

	// Connect interactive element callbacks
	win.on_change('editor', fn [mut state] (mut w simplegui.SimpleWindow, value string) {
		rendered_html := markdown_to_html(value, state.theme_name)
		w.set_html('preview', rendered_html)
		update_status(mut w, state.current_file, value)
	})

	win.on_change('template', fn [mut state] (mut w simplegui.SimpleWindow, value string) {
		txt := get_template(value)
		w.set_text('editor', txt)
		rendered_html := markdown_to_html(txt, state.theme_name)
		w.set_html('preview', rendered_html)
		update_status(mut w, state.current_file, txt)
		w.set_status('Loaded ${value} Template')
	})

	win.on_change('editor_theme', fn [mut state] (mut w simplegui.SimpleWindow, value string) {
		state.theme_name = value.to_lower()
		w.set_theme(state.theme_name)

		// Re-render HTML with new corresponding CSS palette
		editor_val := w.get_text('editor')
		rendered_html := markdown_to_html(editor_val, state.theme_name)
		w.set_html('preview', rendered_html)

		// Dynamically color title bar and subtitle descriptions
		title_color := match state.theme_name {
			'dracula' { '#8be9fd' }
			'nord' { '#88c0d0' }
			'dark' { '#ff6b6b' }
			else { '#0066cc' } // Light
		}
		subtitle_color := match state.theme_name {
			'dracula' { '#6272a4' }
			'nord' { '#a3be8c' }
			'dark' { '#e0e0e0' }
			else { '#4a5568' } // Light - dark grey, very readable
		}
		w.set_control_font_color('studio_title', title_color)
		w.set_control_font_color('subtitle', subtitle_color)
		w.set_status('Editor theme changed to ${value}')
	})

	win.on_click('btn_open', fn [mut state] (mut w simplegui.SimpleWindow) {
		file_path := w.select_file()
		if file_path != '' {
			content := os.read_file(file_path) or {
				w.alert('File Error', 'Could not read file from path.')
				return
			}
			state.current_file = file_path
			w.set_text('editor', content)

			rendered_html := markdown_to_html(content, state.theme_name)
			w.set_html('preview', rendered_html)

			update_status(mut w, state.current_file, content)
			w.toast('Opened file successfully.')
		}
	})

	win.on_click('btn_save', fn [mut state] (mut w simplegui.SimpleWindow) {
		save_path := w.save_file_picker()
		if save_path != '' {
			mut real_path := save_path
			if !real_path.ends_with('.md') && !real_path.ends_with('.txt') {
				real_path += '.md'
			}
			content := w.get_text('editor')
			os.write_file(real_path, content) or {
				w.alert('Save Error', 'Failed to save markdown file.')
				return
			}
			state.current_file = real_path
			update_status(mut w, state.current_file, content)
			w.toast('Saved file successfully!')
			w.alert('Success', 'File successfully saved to:\n${real_path}')
		}
	})

	win.on_click('btn_copy', fn [mut state] (mut w simplegui.SimpleWindow) {
		content := w.get_text('editor')
		rendered_html := markdown_to_html(content, state.theme_name)
		w.copy_to_clipboard(rendered_html)
		w.toast('HTML copied to clipboard!')
		w.set_status('HTML structure copied successfully.')
	})

	update_status(mut win, state.current_file, initial_text)
	win.run()
}

// ----------------------------------------------------
// UI Update Helpers
// ----------------------------------------------------

fn update_status(mut win simplegui.SimpleWindow, file_name string, content string) {
	short_name := os.file_name(file_name)
	char_count := content.len
	word_count := content.split_any(' \n\t').filter(it != '').len
	line_count := content.split_into_lines().len

	win.set_status('Doc: ${short_name} | Words: ${word_count} | Chars: ${char_count} | Lines: ${line_count}')
}

// ----------------------------------------------------
// Compact Regex-Free Markdown Compiler
// ----------------------------------------------------

fn markdown_to_html(md string, theme string) string {
	lines := md.split_into_lines()
	mut html := ''
	mut in_list := false
	mut in_code_block := false

	// Define styles based on theme
	mut body_bg := '#ffffff'
	mut body_fg := '#333333'
	mut code_bg := '#f4f4f4'
	mut code_fg := '#d63384'
	mut hr_color := '#cccccc'

	if theme == 'dark' || theme == 'dracula' || theme == 'nord' {
		if theme == 'dracula' {
			body_bg = '#282a36'
			body_fg = '#f8f8f2'
			code_bg = '#44475a'
			code_fg = '#ff79c6'
			hr_color = '#6272a4'
		} else if theme == 'nord' {
			body_bg = '#2e3440'
			body_fg = '#d8dee9'
			code_bg = '#3b4252'
			code_fg = '#88c0d0'
			hr_color = '#4c566a'
		} else { // dark
			body_bg = '#1e1e1e'
			body_fg = '#ffffff'
			code_bg = '#2d2d2d'
			code_fg = '#ff6b6b'
			hr_color = '#444444'
		}
	}

	for line in lines {
		mut trimmed := line.trim_space()

		// 1. Code block handling
		if trimmed.starts_with('```') {
			if in_code_block {
				html += '</code></pre>\n'
				in_code_block = false
			} else {
				html += '<pre style="background: ' + code_bg + '; color: ' + body_fg +
					'; padding: 10px; border-radius: 4px; overflow-x: auto;"><code>'
				in_code_block = true
			}
			continue
		}

		if in_code_block {
			// Simple HTML escape for code blocks
			mut escaped := line.replace('&', '&amp;').replace('<', '&lt;').replace('>',
				'&gt;')
			html += escaped + '\n'
			continue
		}

		// 2. Wrap-up lists if we exited them
		is_bullet := trimmed.starts_with('* ') || trimmed.starts_with('- ')
		if in_list && !is_bullet {
			html += '</ul>\n'
			in_list = false
		}

		// 3. Horizontal Rule
		if trimmed == '---' {
			html += '<hr style="border: none; border-top: 1px solid ' + hr_color +
				'; margin: 20px 0;"/>\n'
			continue
		}

		// 4. Headers
		if trimmed.starts_with('# ') {
			html += '<h1 style="color: ' + (if theme == 'light' { '#0066cc' } else { '#50fa7b' }) +
				'; margin-top: 20px; font-weight: bold;">' +
				parse_inline(trimmed[2..], code_bg, code_fg) + '</h1>\n'
			continue
		} else if trimmed.starts_with('## ') {
			html += '<h2 style="color: ' + (if theme == 'light' { '#333333' } else { '#ffb86c' }) +
				'; margin-top: 16px; font-weight: bold;">' +
				parse_inline(trimmed[3..], code_bg, code_fg) + '</h2>\n'
			continue
		} else if trimmed.starts_with('### ') {
			html += '<h3 style="color: ' + (if theme == 'light' { '#555555' } else { '#ff79c6' }) +
				'; margin-top: 12px; font-weight: bold;">' +
				parse_inline(trimmed[4..], code_bg, code_fg) + '</h3>\n'
			continue
		}

		// 5. Lists
		if is_bullet {
			if !in_list {
				html += '<ul style="padding-left: 20px; line-height: 1.6;">\n'
				in_list = true
			}
			content := trimmed[2..]
			html += '<li style="margin-bottom: 4px;">' + parse_inline(content, code_bg, code_fg) +
				'</li>\n'
			continue
		}

		// 6. Normal line (skip empty lines or make paragraphs)
		if trimmed == '' {
			continue
		}

		html += '<p style="line-height: 1.6; margin-bottom: 12px;">' +
			parse_inline(line, code_bg, code_fg) + '</p>\n'
	}

	if in_list {
		html += '</ul>\n'
	}
	if in_code_block {
		html += '</code></pre>\n'
	}

	// Dynamic, beautiful wrap with system font stylesheet matching theme
	return '<html><head><meta charset="utf-8"></head><body style="font-family: -apple-system, BlinkMacSystemFont, \'Segoe UI\', Helvetica, Arial, sans-serif; font-size: 14px; background-color: ' +
		body_bg + '; color: ' + body_fg + '; padding: 18px; margin: 0; line-height: 1.6;">' + html +
		'</body></html>'
}

fn parse_inline(text string, code_bg string, code_fg string) string {
	mut res := text
	// Replace inline entities
	res = res.replace('&', '&amp;').replace('<', '&lt;').replace('>', '&gt;')

	// Parse bold: **text** -> <strong>text</strong>
	for {
		start := res.index('**') or { break }
		rest := res[start + 2..]
		end := rest.index('**') or { break }
		bold_text := rest[..end]
		res = res[..start] + '<strong>' + bold_text + '</strong>' + rest[end + 2..]
	}

	// Parse italics: *text* -> <em>text</em>
	for {
		start := res.index('*') or { break }
		rest := res[start + 1..]
		end := rest.index('*') or { break }
		italic_text := rest[..end]
		res = res[..start] + '<em>' + italic_text + '</em>' + rest[end + 1..]
	}

	// Parse inline code: `code` -> <code style="...">code</code>
	for {
		start := res.index('`') or { break }
		rest := res[start + 1..]
		end := rest.index('`') or { break }
		code_text := rest[..end]
		res = res[..start] + '<code style="background: ' + code_bg + '; color: ' + code_fg +
			'; border-radius: 3px; padding: 2px 4px; font-family: monospace; font-size: 0.9em;">' +
			code_text + '</code>' + rest[end + 1..]
	}

	return res
}

// ----------------------------------------------------
// Templates Repository
// ----------------------------------------------------

fn get_template(name string) string {
	match name {
		'Features Guide' {
			return '# Markdown Studio Guide

Welcome to the **SimpleGUI Markdown Studio** built with *V-Lang*!

This editor shows off many highly advanced features:
1. **Live Editing & Preview**: Type anywhere below.
2. **Four Themes Supported**: Dracula, Nord, Dark, Light.
3. **Native macOS Dialogues**: Saving, Opening, and Alerts.
4. **Drag & Drop Support**: Drag a *.md* file on the window!

---

## Formatting Cheat Sheet

You can use the following standard markdowns:
* **Bold Text** using double asterisks.
* *Italics Text* using single asterisks.
* Inline `code blocks` using backticks.

```v
// Example V Code Snippet
fn render() {
    println("Live Markdown View")
}
```

Give it a spin by writing or copying your own text!
'
		}
		'Code Showcase' {
			return '# V-Lang Control Demonstration

In this template, we display pre-formatted codebase references.

## SimpleGUI API

The Cocoa wrapper allows full fluent chaining:

```v
import simplegui

fn main() {
    mut win := simplegui.new_simple_window("Demo", 800, 600)
    win.set_theme("nord")
    win.add_button("btn", "Click me")
    win.run()
}
```

---

### Core Native Modifiers
* `win.set_theme(...)` - sets background and text colors.
* `win.set_control_background_color(...)` - updates control specific styles.
* `win.copy_to_clipboard(...)` - copies data safely.
'
		}
		'Project Planner' {
			return "# Personal Task & Sprint Planner

Keep track of your milestones and logs easily.

---

## Today's Priorities:
* **Sprint 3 Cleanup**: Finalize all native Cocoa view constraints.
* **Theme Styling**: Connect color well callbacks to button themes.
* **Testing suite**: Check validation scenarios with list selectors.

* **Documentation**: Write the `markdown_editor.v` developer instructions.

---

*Enjoy coding with SimpleGUI!*
"
		}
		else {
			return ''
		}
	}
}
