module simplegui

import json2
import strings

// -----------------------------------------------------------------------------
// SimpleGUI RAD Visual UI Designer & Code Generator (Delphi/VB/Lazarus Inspired)
// Enhanced Drag & Drop, Multi-Column Auto-Layout, Undo/Redo & Studio Edition
// -----------------------------------------------------------------------------

// ControlSpec represents a single GUI control placed on the designer canvas.
pub struct ControlSpec {
pub mut:
	id               string
	control_type     string // 'button', 'label', 'input', 'password', 'textarea', 'checkbox', 'switch', 'slider', 'mode', 'number', 'date', 'color', 'progress', 'image', 'table', 'panel', 'radio', 'divider', 'badge', 'search'
	x                int
	y                int
	width            int
	height           int
	text             string
	font_size        int
	font_color       string
	background_color string
	hover_color      string
	hover_text_color string
	cursor           string
	placeholder      string
	tooltip          string
	min_val          int
	max_val          int
	value            int
	enabled          bool
	visible          bool
	checked          bool
	locked           bool
	tab_order        int
	event_handlers   map[string]string // e.g. {"onClick": "on_button_click", "onHover": "on_button_hover", "onHoverExit": "on_button_hover_exit"}
}

// with_event adds an event handler callback function to the ControlSpec.
pub fn (c ControlSpec) with_event(event string, handler string) ControlSpec {
	mut handlers := c.event_handlers.clone()
	handlers[event] = handler
	return ControlSpec{
		...c
		event_handlers: handlers
	}
}

// FormSpec represents the window form layout containing all design controls.
pub struct FormSpec {
pub mut:
	title            string
	width            int
	height           int
	background_color string
	font_color       string
	padding          int
	spacing          int
	controls         []ControlSpec
}

// Encodes FormSpec to JSON layout
pub fn (f FormSpec) to_json() string {
	return json2.encode(f)
}

// Decodes FormSpec from JSON layout
pub fn form_spec_from_json(js string) FormSpec {
	return json2.decode[FormSpec](js) or { FormSpec{} }
}

fn write_single_control_v_code(mut sb strings.Builder, c ControlSpec) {
	clean_text := c.text.replace("'", "\\'")
	match c.control_type {
		'button' {
			sb.write_string("\twin.add_button('${c.id}', '${clean_text}')\n")
		}
		'label' {
			sb.write_string("\twin.add_label('${c.id}', '${clean_text}')\n")
		}
		'input' {
			sb.write_string("\twin.add_input('${c.id}', '${clean_text}')\n")
		}
		'password' {
			sb.write_string("\twin.add_password('${c.id}', '${clean_text}')\n")
		}
		'textarea' {
			sb.write_string("\twin.add_textarea('${c.id}', '${clean_text}')\n")
		}
		'checkbox' {
			chk := if c.checked { 'true' } else { 'false' }
			sb.write_string("\twin.add_checkbox('${c.id}', '${clean_text}', ${chk})\n")
		}
		'switch' {
			chk := if c.checked { 'true' } else { 'false' }
			sb.write_string("\twin.add_switch('${c.id}', '${clean_text}', ${chk})\n")
		}
		'slider' {
			sb.write_string("\twin.add_slider('${c.id}', ${c.value})\n")
		}
		'mode' {
			sb.write_string("\twin.add_mode_control('${c.id}', '${clean_text}')\n")
		}
		'number' {
			sb.write_string("\twin.add_number('${c.id}', ${c.value})\n")
		}
		'date' {
			sb.write_string("\twin.add_date_picker('${c.id}', '${clean_text}')\n")
		}
		'color' {
			sb.write_string("\twin.add_color_well('${c.id}', '${clean_text}')\n")
		}
		'progress' {
			sb.write_string("\twin.add_progress_indicator('${c.id}', ${c.value})\n")
		}
		'image' {
			sb.write_string("\twin.add_image('${c.id}', '${clean_text}')\n")
		}
		'table' {
			sb.write_string("\twin.add_table('${c.id}', ['ID', 'Item Name', 'Status'])\n")
		}
		'panel' {
			sb.write_string("\twin.add_heading('${clean_text}')\n")
		}
		'radio' {
			chk := if c.checked { 'true' } else { 'false' }
			sb.write_string("\twin.add_radio('${c.id}', '${clean_text}', ${chk})\n")
		}
		'divider' {
			sb.write_string('\twin.add_vertical_spacer(6)\n')
		}
		'badge' {
			sb.write_string("\twin.add_label('${c.id}', '${clean_text}')\n")
		}
		'search' {
			sb.write_string("\twin.add_input('${c.id}', '${clean_text}')\n")
		}
		// Integrated Form Controls (Label + Input/Control)
		'form_field' {
			sb.write_string("\twin.add_form_field('${clean_text}', '${c.id}', 'Sample Text')\n")
		}
		'form_textarea' {
			sb.write_string("\twin.add_form_textarea('${clean_text}', '${c.id}', 'Multi-line content...')\n")
		}
		'form_password' {
			sb.write_string("\twin.add_form_password('${clean_text}', '${c.id}', 'secret123')\n")
		}
		'form_number' {
			sb.write_string("\twin.add_form_number('${clean_text}', '${c.id}', ${c.value})\n")
		}
		'form_slider' {
			sb.write_string("\twin.add_form_slider('${clean_text}', '${c.id}', ${c.value})\n")
		}
		'form_dropdown' {
			sb.write_string("\twin.add_form_dropdown('${clean_text}', '${c.id}', ['Option 1', 'Option 2', 'Option 3'], 'Option 1')\n")
		}
		'form_date' {
			sb.write_string("\twin.add_form_date_picker('${clean_text}', '${c.id}', '2026-07-22')\n")
		}
		'form_progress' {
			sb.write_string("\twin.add_form_progress('${clean_text}', '${c.id}', ${c.value})\n")
		}
		'form_switch' {
			chk := if c.checked { 'true' } else { 'false' }
			sb.write_string("\twin.add_form_switch('${clean_text}', '${c.id}', 'Enable', ${chk})\n")
		}
		'form_link' {
			sb.write_string("\twin.add_form_link('${clean_text}', '${c.id}', 'View Guide', 'https://github.com')\n")
		}
		// Demo Controls & Widgets
		'rating' {
			sb.write_string("\twin.add_rating('${c.id}', ${c.value / 20})\n")
		}
		'stepper' {
			sb.write_string("\twin.add_stepper('${c.id}', 0, 100, 1, ${c.value})\n")
		}
		'tag' {
			sb.write_string("\twin.add_token_field('${c.id}', 'vlang,gui,simplegui')\n")
		}
		'path' {
			sb.write_string("\twin.add_path_control('${c.id}', '/Users/developer/Projects/vlang_simplegui')\n")
		}
		'drop_zone' {
			sb.write_string("\twin.add_drop_zone('${c.id}', '${clean_text}')\n")
		}
		'circular_progress' {
			sb.write_string("\twin.add_circular_progress('${c.id}', ${c.value}, 0, 100)\n")
		}
		'metric_meter' {
			sb.write_string("\twin.add_metric_meter('${c.id}', '${clean_text}', ${c.value}, 0, 100, '%')\n")
		}
		'status_indicator' {
			sb.write_string("\twin.add_status_indicator('${c.id}', '${clean_text}', 'online')\n")
		}
		'metric_card' {
			sb.write_string('\twin.add_metric_card(\'${c.id}\', \'${clean_text}\', \'$48.2K\', \'+12%\', \'vs previous month\')\n')
		}
		'alert_banner' {
			sb.write_string("\twin.add_alert_banner('${c.id}', '${clean_text}', 'System update completed successfully.', 'info')\n")
		}
		'code_view' {
			sb.write_string('\twin.add_code_view(\'${c.id}\', \'v\', \'fn main() { println("Hello SimpleGUI") }\', 100)\n')
		}
		else {
			sb.write_string("\twin.add_button('${c.id}', '${clean_text}')\n")
		}
	}

	if c.width > 0 {
		sb.write_string("\twin.set_control_width('${c.id}', ${c.width})\n")
	}
	if c.height > 0 {
		sb.write_string("\twin.set_control_height('${c.id}', ${c.height})\n")
	}
	if c.font_size > 0 && c.font_size != 13 {
		sb.write_string("\twin.set_control_font_size('${c.id}', ${c.font_size})\n")
	}
	if c.font_color.len > 0 && c.font_color != '#ffffff' {
		sb.write_string("\twin.set_control_font_color('${c.id}', '${c.font_color}')\n")
	}
	if c.background_color.len > 0 && c.background_color != '#1e293b' {
		sb.write_string("\twin.set_control_background_color('${c.id}', '${c.background_color}')\n")
	}
	if !c.enabled {
		sb.write_string("\twin.set_control_enabled('${c.id}', false)\n")
	}
	if !c.visible {
		sb.write_string("\twin.set_control_visible('${c.id}', false)\n")
	}
	if c.tooltip.len > 0 {
		clean_tooltip := c.tooltip.replace("'", "\\'")
		sb.write_string("\twin.set_tooltip('${c.id}', '${clean_tooltip}')\n")
	}
	if c.cursor.len > 0 && c.cursor != 'default' {
		sb.write_string("\twin.set_control_cursor('${c.id}', '${c.cursor}')\n")
	}
}

// Generate complete idiomatic V simplegui code with RAD event handler placeholders
pub fn generate_v_code(spec FormSpec) string {
	mut sb := strings.new_builder(2048)
	sb.write_string('module main\n\nimport simplegui\n\nfn main() {\n')
	sb.write_string('\t// Create main application window\n')
	sb.write_string("\tmut win := simplegui.new_simple_window('${spec.title}', ${spec.width}, ${spec.height})\n")
	sb.write_string("\twin.set_background_color('${spec.background_color}')\n")
	sb.write_string("\t\t.set_font_color('${spec.font_color}')\n")
	sb.write_string('\t\t.set_padding(${spec.padding})\n')
	sb.write_string('\t\t.set_spacing(${spec.spacing})\n\n')

	mut non_panels := []ControlSpec{}
	mut panels := []ControlSpec{}
	for c in spec.controls {
		if !c.visible {
			continue
		}
		if c.control_type == 'panel' {
			panels << c
		} else {
			non_panels << c
		}
	}

	mut has_col2 := false
	for c in non_panels {
		if c.x >= 420 && c.width < 550 {
			has_col2 = true
			break
		}
	}

	mut sorted_controls := non_panels.clone()
	sorted_controls.sort_with_compare(fn (a &ControlSpec, b &ControlSpec) int {
		dy := if a.y >= b.y { a.y - b.y } else { b.y - a.y }
		if dy > 25 {
			return if a.y < b.y { -1 } else { 1 }
		}
		return if a.x < b.x { -1 } else { 1 }
	})

	mut rows := [][]ControlSpec{}
	mut current_row := []ControlSpec{}
	mut current_y := -1000

	for c in sorted_controls {
		if current_row.len == 0 {
			current_row << c
			current_y = c.y
		} else {
			dy := if c.y >= current_y { c.y - current_y } else { current_y - c.y }
			if dy <= 25 {
				current_row << c
			} else {
				rows << current_row.clone()
				current_row = [c]
				current_y = c.y
			}
		}
	}
	if current_row.len > 0 {
		rows << current_row
	}

	sb.write_string('\t// Define visual components\n')

	if panels.len > 0 {
		if panels.len > 1 {
			sb.write_string("\twin.begin_row('panel_headers_row')\n")
			for p in panels {
				clean_text := p.text.replace("'", "\\'")
				sb.write_string("\twin.add_heading('📦 ${clean_text}')\n")
			}
			sb.write_string('\twin.end_row()\n')
		} else {
			clean_text := panels[0].text.replace("'", "\\'")
			sb.write_string("\twin.add_heading('📦 ${clean_text}')\n")
		}
	}

	mut spacer_idx := 0
	for r_idx, row in rows {
		if has_col2 {
			mut col1 := []ControlSpec{}
			mut col2 := []ControlSpec{}

			for c in row {
				if c.x >= 420 && c.width < 550 {
					col2 << c
				} else {
					col1 << c
				}
			}

			sb.write_string("\twin.begin_row('row_${r_idx + 1}')\n")
			if col1.len > 0 {
				for c in col1 {
					write_single_control_v_code(mut sb, c)
				}
			} else {
				spacer_idx++
				sb.write_string("\twin.add_label('spc_${spacer_idx}', '')\n")
			}

			if col2.len > 0 {
				for c in col2 {
					write_single_control_v_code(mut sb, c)
				}
			} else {
				spacer_idx++
				sb.write_string("\twin.add_label('spc_${spacer_idx}', '')\n")
			}
			sb.write_string('\twin.end_row()\n')
		} else {
			is_multi := row.len > 1
			if is_multi {
				sb.write_string("\twin.begin_row('row_${r_idx + 1}')\n")
			}
			for c in row {
				write_single_control_v_code(mut sb, c)
			}
			if is_multi {
				sb.write_string('\twin.end_row()\n')
			}
		}
	}

	sb.write_string('\n\t// Register event handler callbacks\n')
	mut handlers_to_generate := map[string]string{}
	for c in spec.controls {
		for event_type, handler_name in c.event_handlers {
			if handler_name.trim_space() == '' {
				continue
			}
			h_clean := handler_name.trim_space()
			match event_type {
				'onClick' {
					sb.write_string("\twin.on_click('${c.id}', ${h_clean})\n")
					handlers_to_generate[h_clean] = 'onClick for control `${c.id}`'
				}
				'onChange' {
					sb.write_string("\twin.on_change('${c.id}', ${h_clean})\n")
					handlers_to_generate[h_clean] = 'onChange for control `${c.id}`'
				}
				'onHover', 'onMouseEnter' {
					sb.write_string("\twin.on_hover('${c.id}', ${h_clean})\n")
					handlers_to_generate[h_clean] = 'onHover for control `${c.id}`'
				}
				'onHoverExit', 'onMouseLeave' {
					sb.write_string("\twin.on_hover_exit('${c.id}', ${h_clean})\n")
					handlers_to_generate[h_clean] = 'onHoverExit for control `${c.id}`'
				}
				'onDoubleClick' {
					sb.write_string("\twin.add_context_menu_item('${c.id}', 'Double Click', ${h_clean})\n")
					handlers_to_generate[h_clean] = 'onDoubleClick for control `${c.id}`'
				}
				else {
					sb.write_string("\twin.on_change('${c.id}', ${h_clean})\n")
					handlers_to_generate[h_clean] = '${event_type} for control `${c.id}`'
				}
			}
		}
	}

	sb.write_string("\n\twin.set_status('SimpleGUI Form initialized and ready.')\n")
	sb.write_string('\twin.run()\n}\n\n')

	sb.write_string('// ============================================================================\n')
	sb.write_string('// Event Handler Callbacks (Delphi / VB / Lazarus RAD Event Placeholders)\n')
	sb.write_string('// ============================================================================\n\n')

	if handlers_to_generate.len == 0 {
		sb.write_string('// No event placeholders generated yet.\n')
	} else {
		for handler_name, desc in handlers_to_generate {
			if desc.starts_with('onClick') || desc.starts_with('onHover')
				|| desc.starts_with('onHoverExit') {
				sb.write_string('// Handler: ${desc}\n')
				sb.write_string('fn ${handler_name}(mut win simplegui.SimpleWindow) {\n')
				sb.write_string("\twin.set_status('Event triggered: ${handler_name}')\n")
				sb.write_string("\twin.toast('${handler_name} executed successfully!')\n")
				sb.write_string('\t// TODO: Implement custom Delphi/VB event logic\n')
				sb.write_string('}\n\n')
			} else {
				sb.write_string('// Handler: ${desc}\n')
				sb.write_string('fn ${handler_name}(mut win simplegui.SimpleWindow, value string) {\n')
				sb.write_string("\twin.set_status('Event triggered: ${handler_name} (Value: \${value})')\n")
				sb.write_string('\t// TODO: Implement custom Delphi/VB event logic\n')
				sb.write_string('}\n\n')
			}
		}
	}

	return sb.str()
}

// Generate standalone semantic HTML5 and modern CSS webpage from FormSpec layout
pub fn generate_html_code(spec FormSpec) string {
	mut sb := strings.new_builder(4096)
	bg_col := if spec.background_color.len > 0 { spec.background_color } else { '#0f172a' }
	fg_col := if spec.font_color.len > 0 { spec.font_color } else { '#f8fafc' }

	sb.write_string('<!DOCTYPE html>\n<html lang="en">\n<head>\n')
	sb.write_string('\t<meta charset="utf-8">\n')
	sb.write_string('\t<meta name="viewport" content="width=device-width, initial-scale=1.0">\n')
	sb.write_string('\t<title>${spec.title}</title>\n')
	sb.write_string('\t<style>\n')
	sb.write_string('\t\t* { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif; }\n')
	sb.write_string('\t\tbody { background-color: ${bg_col}; color: ${fg_col}; display: flex; justify-content: center; align-items: center; min-height: 100vh; padding: 20px; }\n')
	sb.write_string('\t\t.form-window { position: relative; width: ${spec.width}px; min-height: ${spec.height}px; background-color: ${bg_col}; border: 1px solid rgba(255, 255, 255, 0.1); border-radius: 12px; box-shadow: 0 20px 40px rgba(0, 0, 0, 0.5); padding: ${spec.padding}px; overflow: hidden; }\n')
	sb.write_string('\t\t.form-title { font-size: 20px; font-weight: 700; margin-bottom: 16px; color: ${fg_col}; border-bottom: 1px solid rgba(255, 255, 255, 0.08); padding-bottom: 10px; }\n')
	sb.write_string('\t\t.control-item { position: absolute; box-sizing: border-box; display: flex; align-items: center; transition: all 0.15s ease; }\n')
	sb.write_string('\t\t.control-item.panel-ctrl { align-items: flex-start; justify-content: flex-start; }\n')
	sb.write_string('\t\t.btn-ctrl { display: inline-flex; align-items: center; justify-content: center; font-weight: 600; border-radius: 6px; border: 1px solid rgba(255, 255, 255, 0.15); cursor: pointer; text-decoration: none; }\n')
	sb.write_string('\t\t.btn-ctrl:hover { filter: brightness(1.15); transform: translateY(-1px); }\n')
	sb.write_string('\t\t.input-ctrl { background: rgba(255, 255, 255, 0.05); border: 1px solid rgba(255, 255, 255, 0.15); border-radius: 6px; color: inherit; padding: 6px 12px; outline: none; width: 100%; height: 100%; }\n')
	sb.write_string('\t\t.input-ctrl:focus { border-color: #38bdf8; box-shadow: 0 0 0 2px rgba(56, 189, 248, 0.25); }\n')
	sb.write_string('\t\t.panel-ctrl { border: 1px solid rgba(255, 255, 255, 0.1); border-radius: 8px; background: rgba(255, 255, 255, 0.03); padding: 10px 14px; font-size: 14px; font-weight: 700; align-items: flex-start; justify-content: flex-start; }\n')
	sb.write_string('\t\t.badge-ctrl { display: inline-flex; align-items: center; justify-content: center; padding: 4px 10px; border-radius: 9999px; font-size: 11px; font-weight: 700; }\n')
	sb.write_string('\t</style>\n</head>\n<body>\n\n')

	sb.write_string('<main class="form-window">\n')
	sb.write_string('\t<h1 class="form-title">${spec.title}</h1>\n')

	for c in spec.controls {
		if !c.visible {
			continue
		}
		c_bg := if c.background_color.len > 0 { c.background_color } else { 'transparent' }
		c_fg := if c.font_color.len > 0 { c.font_color } else { fg_col }
		c_fs := if c.font_size > 0 { c.font_size } else { 13 }
		clean_text := c.text.replace('<', '&lt;').replace('>', '&gt;')

		pos_style := 'left: ${c.x}px; top: ${c.y + 40}px; width: ${c.width}px; height: ${c.height}px; color: ${c_fg}; font-size: ${c_fs}px;'

		match c.control_type {
			'button' {
				sb.write_string('\t<div class="control-item" style="${pos_style}">\n')
				sb.write_string('\t\t<button class="btn-ctrl" style="width:100%; height:100%; background-color:${c_bg}; color:${c_fg}; font-size:${c_fs}px;" id="${c.id}">${clean_text}</button>\n')
				sb.write_string('\t</div>\n')
			}
			'label' {
				sb.write_string('\t<div class="control-item" style="${pos_style} font-weight: 600;">\n')
				sb.write_string('\t\t<span id="${c.id}">${clean_text}</span>\n')
				sb.write_string('\t</div>\n')
			}
			'input' {
				sb.write_string('\t<div class="control-item" style="${pos_style}">\n')
				sb.write_string('\t\t<input type="text" class="input-ctrl" id="${c.id}" value="${clean_text}" placeholder="${c.placeholder}">\n')
				sb.write_string('\t</div>\n')
			}
			'password' {
				sb.write_string('\t<div class="control-item" style="${pos_style}">\n')
				sb.write_string('\t\t<input type="password" class="input-ctrl" id="${c.id}" value="${clean_text}" placeholder="${c.placeholder}">\n')
				sb.write_string('\t</div>\n')
			}
			'textarea' {
				sb.write_string('\t<div class="control-item" style="${pos_style}">\n')
				sb.write_string('\t\t<textarea class="input-ctrl" id="${c.id}" style="resize:none;">${clean_text}</textarea>\n')
				sb.write_string('\t</div>\n')
			}
			'checkbox' {
				chk := if c.checked { 'checked' } else { '' }
				sb.write_string('\t<div class="control-item" style="${pos_style} gap:8px;">\n')
				sb.write_string('\t\t<input type="checkbox" id="${c.id}" ${chk}>\n')
				sb.write_string('\t\t<label for="${c.id}">${clean_text}</label>\n')
				sb.write_string('\t</div>\n')
			}
			'radio' {
				chk := if c.checked { 'checked' } else { '' }
				sb.write_string('\t<div class="control-item" style="${pos_style} gap:8px;">\n')
				sb.write_string('\t\t<input type="radio" name="form_radios" id="${c.id}" ${chk}>\n')
				sb.write_string('\t\t<label for="${c.id}">${clean_text}</label>\n')
				sb.write_string('\t</div>\n')
			}
			'switch' {
				chk := if c.checked { 'checked' } else { '' }
				sb.write_string('\t<div class="control-item" style="${pos_style} gap:8px;">\n')
				sb.write_string('\t\t<input type="checkbox" role="switch" id="${c.id}" ${chk}>\n')
				sb.write_string('\t\t<label for="${c.id}">${clean_text}</label>\n')
				sb.write_string('\t</div>\n')
			}
			'slider' {
				sb.write_string('\t<div class="control-item" style="${pos_style}">\n')
				sb.write_string('\t\t<input type="range" id="${c.id}" min="${c.min_val}" max="${c.max_val}" value="${c.value}" style="width:100%;">\n')
				sb.write_string('\t</div>\n')
			}
			'badge' {
				sb.write_string('\t<div class="control-item" style="${pos_style}">\n')
				sb.write_string('\t\t<span class="badge-ctrl" style="background-color:${c_bg}; color:${c_fg};" id="${c.id}">${clean_text}</span>\n')
				sb.write_string('\t</div>\n')
			}
			'panel' {
				sb.write_string('\t<div class="control-item panel-ctrl" style="${pos_style} background-color:${c_bg}; color:${c_fg};" id="${c.id}">\n')
				sb.write_string('\t\t📦 ${clean_text}\n')
				sb.write_string('\t</div>\n')
			}
			'progress' {
				sb.write_string('\t<div class="control-item" style="${pos_style}">\n')
				sb.write_string('\t\t<progress id="${c.id}" value="${c.value}" max="100" style="width:100%; height:100%;"></progress>\n')
				sb.write_string('\t</div>\n')
			}
			else {
				sb.write_string('\t<div class="control-item" style="${pos_style}">\n')
				sb.write_string('\t\t<div id="${c.id}" style="width:100%; height:100%; background-color:${c_bg}; padding:6px; border-radius:6px;">${clean_text}</div>\n')
				sb.write_string('\t</div>\n')
			}
		}
	}

	sb.write_string('</main>\n\n</body>\n</html>')
	return sb.str()
}

// Starter Form Templates
pub fn get_default_form_spec() FormSpec {
	return FormSpec{
		title:            'Delphi/VB RAD Enterprise Portal'
		width:            940
		height:           620
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          20
		spacing:          12
		controls:         [
			ControlSpec{
				id:           'heading_title'
				control_type: 'label'
				x:            24
				y:            20
				width:        460
				height:       32
				text:         '📋 Customer Account & Portal Registration'
				font_size:    18
				font_color:   '#38bdf8'
			},
			ControlSpec{
				id:               'panel_profile'
				control_type:     'panel'
				x:                24
				y:                64
				width:            420
				height:           440
				text:             'Profile Information'
				background_color: '#1e293b'
			},
			ControlSpec{
				id:               'panel_preferences'
				control_type:     'panel'
				x:                470
				y:                64
				width:            440
				height:           440
				text:             'Security & Priority Settings'
				background_color: '#1e293b'
			},
			ControlSpec{
				id:           'input_fullname'
				control_type: 'input'
				x:            40
				y:            104
				width:        388
				height:       36
				text:         'Ada Lovelace'
				placeholder:  'Full Name'
			},
			ControlSpec{
				id:           'slider_priority'
				control_type: 'slider'
				x:            486
				y:            104
				width:        408
				height:       28
				value:        75
			},
			ControlSpec{
				id:           'input_email'
				control_type: 'input'
				x:            40
				y:            152
				width:        388
				height:       36
				text:         'ada@lovelace.org'
				placeholder:  'Email Address'
			},
			ControlSpec{
				id:           'sw_two_factor'
				control_type: 'switch'
				x:            486
				y:            148
				width:        300
				height:       28
				text:         'Enable 2FA Authentication'
				checked:      true
			},
			ControlSpec{
				id:           'inp_phone'
				control_type: 'input'
				x:            40
				y:            200
				width:        388
				height:       36
				text:         '+1 (555) 019-2834'
				placeholder:  'Phone Number'
			},
			ControlSpec{
				id:               'badge_account_tier'
				control_type:     'badge'
				x:                486
				y:                192
				width:            160
				height:           28
				text:             'ENTERPRISE TIER'
				background_color: '#059669'
			},
			ControlSpec{
				id:           'chk_newsletter'
				control_type: 'checkbox'
				x:            40
				y:            248
				width:        320
				height:       26
				text:         'Subscribe to Developer Updates'
				checked:      true
			},
			ControlSpec{
				id:           'radio_plan_std'
				control_type: 'radio'
				x:            486
				y:            230
				width:        180
				height:       26
				text:         'Standard Account'
				checked:      false
			},
			ControlSpec{
				id:           'radio_plan_pro'
				control_type: 'radio'
				x:            486
				y:            260
				width:        180
				height:       26
				text:         'Pro Developer Tier'
				checked:      true
			},
			ControlSpec{
				id:           'radio_plan_ent'
				control_type: 'radio'
				x:            486
				y:            290
				width:        180
				height:       26
				text:         'Enterprise Studio'
				checked:      false
			},
			ControlSpec{
				id:               'btn_submit'
				control_type:     'button'
				x:                24
				y:                520
				width:            180
				height:           42
				text:             '💾 Save Account'
				font_size:        14
				background_color: '#0284c7'
			}.with_event('onClick', 'on_btn_submit_click'),
			ControlSpec{
				id:               'btn_reset'
				control_type:     'button'
				x:                216
				y:                520
				width:            140
				height:           42
				text:             '↺ Reset Form'
				font_size:        14
				background_color: '#334155'
			}.with_event('onClick', 'on_place_order_click'),
		]
	}
}

pub fn get_profile_form_spec() FormSpec {
	return FormSpec{
		title:            'User Profile & Account Settings'
		width:            720
		height:           560
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          20
		spacing:          12
		controls:         [
			ControlSpec{
				id:           'lbl_profile_header'
				control_type: 'label'
				x:            24
				y:            20
				width:        400
				height:       32
				text:         '👤 User Profile & Account'
				font_size:    20
				font_color:   '#38bdf8'
			},
			ControlSpec{
				id:           'field_name'
				control_type: 'form_field'
				x:            24
				y:            64
				width:        300
				height:       44
				text:         'Full Name: Alex Mercer'
			},
			ControlSpec{
				id:           'field_username'
				control_type: 'form_field'
				x:            344
				y:            64
				width:        300
				height:       44
				text:         'Username: @alexmercer'
			},
			ControlSpec{
				id:           'field_bio'
				control_type: 'form_textarea'
				x:            24
				y:            120
				width:        620
				height:       100
				text:         'Senior Systems Engineer building desktop applications with Vlang and SimpleGUI.'
			},
			ControlSpec{
				id:           'switch_notif'
				control_type: 'form_switch'
				x:            24
				y:            230
				width:        280
				height:       28
				text:         'System Push Notifications'
				checked:      true
			},
			ControlSpec{
				id:           'dropdown_role'
				control_type: 'form_dropdown'
				x:            344
				y:            230
				width:        300
				height:       44
				text:         'Account Role'
			},
			ControlSpec{
				id:               'btn_save_profile'
				control_type:     'button'
				x:                24
				y:                290
				width:            180
				height:           42
				text:             '💾 Save Profile'
				background_color: '#0284c7'
			},
			ControlSpec{
				id:               'btn_cancel_profile'
				control_type:     'button'
				x:                216
				y:                290
				width:            140
				height:           42
				text:             'Cancel'
				background_color: '#334155'
			},
		]
	}
}

pub fn get_crud_form_spec() FormSpec {
	return FormSpec{
		title:            'Enterprise Product Data Grid & CRUD'
		width:            920
		height:           620
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          20
		spacing:          12
		controls:         [
			ControlSpec{
				id:           'lbl_crud_header'
				control_type: 'label'
				x:            24
				y:            20
				width:        420
				height:       32
				text:         '🗄️ Database Record Manager'
				font_size:    20
				font_color:   '#10b981'
			},
			ControlSpec{
				id:           'inp_search'
				control_type: 'search'
				x:            24
				y:            64
				width:        320
				height:       36
				text:         'Search products...'
			},
			ControlSpec{
				id:               'btn_add_record'
				control_type:     'button'
				x:                360
				y:                64
				width:            140
				height:           36
				text:             '➕ Add Record'
				background_color: '#10b981'
			},
			ControlSpec{
				id:               'btn_refresh_table'
				control_type:     'button'
				x:                510
				y:                64
				width:            120
				height:           36
				text:             '🔄 Refresh'
				background_color: '#0284c7'
			},
			ControlSpec{
				id:           'table_records'
				control_type: 'table'
				x:            24
				y:            116
				width:        860
				height:       360
				text:         'Product Database Grid'
			},
			ControlSpec{
				id:               'badge_count'
				control_type:     'badge'
				x:                24
				y:                490
				width:            160
				height:           28
				text:             '148 Records Total'
				background_color: '#38bdf8'
			},
			ControlSpec{
				id:           'status_db'
				control_type: 'status_indicator'
				x:            200
				y:            490
				width:        180
				height:       28
				text:         'PostgreSQL Online'
			},
		]
	}
}

pub fn get_ticket_form_spec() FormSpec {
	return FormSpec{
		title:            'Help Desk & Bug Ticket Reporter'
		width:            760
		height:           600
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          20
		spacing:          12
		controls:         [
			ControlSpec{
				id:           'lbl_ticket_header'
				control_type: 'label'
				x:            24
				y:            20
				width:        420
				height:       32
				text:         '🎫 Submit Support & Bug Ticket'
				font_size:    20
				font_color:   '#38bdf8'
			},
			ControlSpec{
				id:           'field_ticket_title'
				control_type: 'form_field'
				x:            24
				y:            64
				width:        680
				height:       44
				text:         'Issue Title: Canvas element overlap on high-DPI displays'
			},
			ControlSpec{
				id:           'dropdown_severity'
				control_type: 'form_dropdown'
				x:            24
				y:            120
				width:        320
				height:       44
				text:         'Severity Level'
			},
			ControlSpec{
				id:           'field_ticket_desc'
				control_type: 'form_textarea'
				x:            24
				y:            176
				width:        680
				height:       110
				text:         'Steps to reproduce:\n1. Open UI Designer\n2. Zoom to 150%\n3. Observe snap guidelines alignment.'
			},
			ControlSpec{
				id:           'drop_attachment'
				control_type: 'drop_zone'
				x:            24
				y:            296
				width:        680
				height:       80
				text:         'Drag & drop log files or screenshots here'
			},
			ControlSpec{
				id:           'rating_priority'
				control_type: 'rating'
				x:            24
				y:            390
				width:        140
				height:       32
				text:         'Urgency Rating'
			},
			ControlSpec{
				id:               'btn_send_ticket'
				control_type:     'button'
				x:                24
				y:                435
				width:            220
				height:           44
				text:             '🚀 Submit Support Ticket'
				background_color: '#0284c7'
			},
		]
	}
}

pub fn get_api_form_spec() FormSpec {
	return FormSpec{
		title:            'REST API Request & Response Tester'
		width:            860
		height:           600
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          20
		spacing:          12
		controls:         [
			ControlSpec{
				id:           'lbl_api_header'
				control_type: 'label'
				x:            24
				y:            20
				width:        480
				height:       32
				text:         '⚡ REST API Client & Endpoint Tester'
				font_size:    20
				font_color:   '#10b981'
			},
			ControlSpec{
				id:           'dropdown_method'
				control_type: 'form_dropdown'
				x:            24
				y:            64
				width:        140
				height:       44
				text:         'HTTP Method'
			},
			ControlSpec{
				id:           'field_url'
				control_type: 'form_field'
				x:            176
				y:            64
				width:        480
				height:       44
				text:         'Endpoint: https://api.vlang.org/v1/status'
			},
			ControlSpec{
				id:               'btn_send_api'
				control_type:     'button'
				x:                668
				y:                64
				width:            140
				height:           44
				text:             '▶ Send'
				background_color: '#10b981'
			},
			ControlSpec{
				id:           'status_api'
				control_type: 'status_indicator'
				x:            24
				y:            120
				width:        220
				height:       28
				text:         '200 OK (38ms)'
			},
			ControlSpec{
				id:           'code_response'
				control_type: 'code_view'
				x:            24
				y:            160
				width:        784
				height:       340
				text:         'JSON Response Payload'
			},
		]
	}
}

pub fn get_media_form_spec() FormSpec {
	return FormSpec{
		title:            'Hi-Fi Audio & Media Control Panel'
		width:            640
		height:           520
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          20
		spacing:          12
		controls:         [
			ControlSpec{
				id:           'lbl_media_header'
				control_type: 'label'
				x:            24
				y:            20
				width:        400
				height:       32
				text:         '🎵 Hi-Fi Audio Player'
				font_size:    20
				font_color:   '#38bdf8'
			},
			ControlSpec{
				id:           'lbl_track_name'
				control_type: 'label'
				x:            24
				y:            64
				width:        400
				height:       28
				text:         'Track: Vlang Symphony No. 9 in D Minor'
				font_size:    15
			},
			ControlSpec{
				id:           'progress_playback'
				control_type: 'progress'
				x:            24
				y:            100
				width:        560
				height:       32
				value:        64
				text:         'Playback Progress'
			},
			ControlSpec{
				id:               'btn_prev'
				control_type:     'button'
				x:                24
				y:                145
				width:            100
				height:           42
				text:             '⏮ Prev'
				background_color: '#334155'
			},
			ControlSpec{
				id:               'btn_play'
				control_type:     'button'
				x:                136
				y:                145
				width:            140
				height:           42
				text:             '⏸ Pause'
				background_color: '#0284c7'
			},
			ControlSpec{
				id:               'btn_next'
				control_type:     'button'
				x:                288
				y:                145
				width:            100
				height:           42
				text:             '⏭ Next'
				background_color: '#334155'
			},
			ControlSpec{
				id:           'slider_volume'
				control_type: 'slider'
				x:            24
				y:            205
				width:        360
				height:       28
				value:        80
				text:         'Volume Level'
			},
			ControlSpec{
				id:           'rating_track'
				control_type: 'rating'
				x:            24
				y:            245
				width:        140
				height:       32
				text:         'Favorite Rating'
			},
		]
	}
}

pub fn get_login_form_spec() FormSpec {
	return FormSpec{
		title:            'User Authentication Dialog'
		width:            480
		height:           420
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          24
		spacing:          14
		controls:         [
			ControlSpec{
				id:               'lbl_auth_header'
				control_type:     'label'
				x:                24
				y:                24
				width:            360
				height:           32
				text:             '🔐 Account Sign In'
				font_size:        20
				font_color:       '#38bdf8'
				background_color: ''
			},
			ControlSpec{
				id:           'inp_user'
				control_type: 'input'
				x:            24
				y:            72
				width:        360
				height:       36
				text:         'admin@developer.com'
				placeholder:  'Email or Username'
			}.with_event('onChange', 'on_user_change'),
			ControlSpec{
				id:           'inp_pass'
				control_type: 'password'
				x:            24
				y:            120
				width:        360
				height:       36
				text:         'secretpass'
				placeholder:  'Password'
			}.with_event('onChange', 'on_pass_change'),
			ControlSpec{
				id:           'chk_remember'
				control_type: 'checkbox'
				x:            24
				y:            168
				width:        240
				height:       24
				text:         'Remember login session'
				checked:      true
			},
			ControlSpec{
				id:               'btn_signin'
				control_type:     'button'
				x:                24
				y:                208
				width:            360
				height:           42
				text:             'Sign In Now'
				font_size:        14
				font_color:       '#ffffff'
				background_color: '#0284c7'
			}.with_event('onClick', 'on_login_click'),
		]
	}
}

pub fn get_dashboard_form_spec() FormSpec {
	return FormSpec{
		title:            'Fintech Analytics KPI Dashboard'
		width:            960
		height:           580
		background_color: '#0b0f19'
		font_color:       '#f1f5f9'
		padding:          16
		spacing:          10
		controls:         [
			ControlSpec{
				id:               'lbl_dash_title'
				control_type:     'label'
				x:                20
				y:                20
				width:            460
				height:           30
				text:             '📊 Executive Performance Dashboard'
				font_size:        18
				font_color:       '#10b981'
				background_color: ''
			},
			ControlSpec{
				id:           'inp_kpi_target'
				control_type: 'input'
				x:            20
				y:            60
				width:        280
				height:       34
				text:         'Q3 ARR Target: $4.2M'
			}.with_event('onChange', 'on_target_change'),
			ControlSpec{
				id:           'progress_revenue'
				control_type: 'progress'
				x:            320
				y:            60
				width:        360
				height:       34
				value:        85
			}.with_event('onChange', 'on_progress_change'),
			ControlSpec{
				id:           'slider_volatility'
				control_type: 'slider'
				x:            20
				y:            110
				width:        280
				height:       30
				value:        45
			}.with_event('onChange', 'on_volatility_change'),
			ControlSpec{
				id:               'btn_refresh'
				control_type:     'button'
				x:                320
				y:                110
				width:            160
				height:           34
				text:             'Refresh KPI'
				font_size:        13
				font_color:       '#ffffff'
				background_color: '#10b981'
			}.with_event('onClick', 'on_refresh_click'),
		]
	}
}

pub fn get_settings_form_spec() FormSpec {
	return FormSpec{
		title:            'Application Settings Studio'
		width:            860
		height:           580
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          20
		spacing:          12
		controls:         [
			ControlSpec{
				id:           'lbl_settings_header'
				control_type: 'label'
				x:            24
				y:            24
				width:        420
				height:       32
				text:         '⚙️ System Settings & Preferences'
				font_size:    18
				font_color:   '#38bdf8'
			},
			ControlSpec{
				id:               'panel_general'
				control_type:     'panel'
				x:                24
				y:                70
				width:            380
				height:           220
				text:             'General Settings'
				background_color: '#1e293b'
			},
			ControlSpec{
				id:               'panel_security'
				control_type:     'panel'
				x:                430
				y:                70
				width:            400
				height:           220
				text:             'Security & Telemetry'
				background_color: '#1e293b'
			},
			ControlSpec{
				id:           'inp_app_name'
				control_type: 'input'
				x:            40
				y:            110
				width:        340
				height:       36
				text:         'Vlang Studio App'
				placeholder:  'App Name'
			},
			ControlSpec{
				id:           'slider_security_level'
				control_type: 'slider'
				x:            446
				y:            110
				width:        360
				height:       28
				value:        80
			},
			ControlSpec{
				id:           'sw_dark_mode'
				control_type: 'switch'
				x:            40
				y:            160
				width:        260
				height:       28
				text:         'Enable GPU Acceleration'
				checked:      true
			},
			ControlSpec{
				id:               'badge_status'
				control_type:     'badge'
				x:                446
				y:                160
				width:            140
				height:           28
				text:             'VERIFIED SECURE'
				background_color: '#059669'
			},
			ControlSpec{
				id:           'sw_auto_save'
				control_type: 'switch'
				x:            40
				y:            200
				width:        260
				height:       28
				text:         'Auto-save Session State'
				checked:      true
			},
			ControlSpec{
				id:               'btn_save_settings'
				control_type:     'button'
				x:                24
				y:                310
				width:            200
				height:           42
				text:             'Save Preferences'
				font_size:        14
				background_color: '#0284c7'
			}.with_event('onClick', 'on_save_settings_click'),
		]
	}
}

pub fn get_checkout_form_spec() FormSpec {
	return FormSpec{
		title:            'E-Commerce Multi-Column Checkout'
		width:            960
		height:           640
		background_color: '#0f172a'
		font_color:       '#f8fafc'
		padding:          20
		spacing:          12
		controls:         [
			ControlSpec{
				id:           'lbl_checkout_header'
				control_type: 'label'
				x:            24
				y:            20
				width:        420
				height:       32
				text:         '🛒 Order Checkout & Payment'
				font_size:    20
				font_color:   '#38bdf8'
			},
			ControlSpec{
				id:               'panel_billing'
				control_type:     'panel'
				x:                24
				y:                64
				width:            440
				height:           450
				text:             'Billing & Shipping Details'
				background_color: '#1e293b'
			},
			ControlSpec{
				id:               'panel_payment'
				control_type:     'panel'
				x:                490
				y:                64
				width:            440
				height:           450
				text:             'Payment Method & Summary'
				background_color: '#1e293b'
			},
			ControlSpec{
				id:           'inp_full_name'
				control_type: 'input'
				x:            40
				y:            104
				width:        408
				height:       36
				text:         'Ada Lovelace'
				placeholder:  'Full Name'
			},
			ControlSpec{
				id:           'inp_card_number'
				control_type: 'input'
				x:            506
				y:            104
				width:        408
				height:       36
				text:         '4532 •••• •••• 8892'
				placeholder:  'Credit Card Number'
			},
			ControlSpec{
				id:           'inp_email_addr'
				control_type: 'input'
				x:            40
				y:            152
				width:        408
				height:       36
				text:         'ada@lovelace.org'
				placeholder:  'Email Address'
			},
			ControlSpec{
				id:               'badge_total'
				control_type:     'badge'
				x:                506
				y:                160
				width:            408
				height:           38
				text:             'TOTAL DUE: $299.00 USD'
				background_color: '#059669'
			},
			ControlSpec{
				id:           'inp_address_line'
				control_type: 'input'
				x:            40
				y:            200
				width:        408
				height:       36
				text:         '100 Innovation Way, Suite 400'
				placeholder:  'Street Address'
			},
			ControlSpec{
				id:               'btn_place_order'
				control_type:     'button'
				x:                506
				y:                216
				width:            408
				height:           44
				text:             '💳 Complete Payment & Order'
				background_color: '#0284c7'
			}.with_event('onClick', 'on_pay_click'),
		]
	}
}

// Compiles full interactive HTML/CSS/JS Delphi & VB RAD Studio web interface
pub fn compile_designer_html(spec FormSpec) string {
	spec_json := json2.encode(spec)
	html := $embed_file('designer.html').to_string()
	return html.replace('__SPEC_JSON__', spec_json)
}
