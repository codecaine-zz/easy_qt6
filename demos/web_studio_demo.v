module main

import simplegui
import json2

// -----------------------------------------------------------------------------
// Production-Ready Native+Web Dashboard & Metric KPI Center (Fintech Analytics)
// -----------------------------------------------------------------------------
// This demo showcases a real-world utility application: an Executive Business
// Intelligence (BI) KPI Board & Financial Portfolio Generator.
// V models the financial metrics (Revenue, conversion margins, user targets, and
// styling presets), while delivering a fully interactive custom HTML5 Webkit
// data visualization with reactive canvas gauges, inline data-baking JSON exports,
// and synchronized alerts.
// -----------------------------------------------------------------------------

struct DashboardSpec {
mut:
	company_name    string = 'Acme Analytics Corp'
	kpi_label       string = 'Quarterly Recurring Revenue (ARR)'
	total_revenue   int    = 4250000
	conversion_rate f64    = 3.42
	customer_count  int    = 1380
	color_primary   string = '#06b6d4' // Cyan Accent
	color_secondary string = '#3b82f6' // Blue Accent
	border_radius   int    = 14
	enable_alert    bool   = true
}

fn main() {
	mut spec := DashboardSpec{}

	// Create and configure a premium SimpleGUI environment
	mut win := simplegui.new_simple_window('Enterprise Fintech Analytics, KPI & BI Center',
		880, 680)
		.set_background_color('#0b0f19') // Premium Obsidian Space Black
		.set_font_color('#f1f5f9')
		.set_padding(18)
		.set_spacing(8)

	win.add_heading('📊 Enterprise BI Dashboard & KPI Publisher')

	win.add_label('studio_sub', 'Model business data and styling metrics live in V. Real-time updates re-render interactive WebKit charts, financial tickers, and high-performance canvas gauges.')
		.font_size(11)
		.font_color('#64748b')

	win.add_separator()

	// Vertical split: controls first, preview second (improves control visibility)
	win.group('studio_styles', 'Financial KPI Configurations', fn (mut w simplegui.SimpleWindow) {
		w.row('comp_row', fn (mut w2 simplegui.SimpleWindow) {
			w2.add_label('', 'Entity name:').width(90)
			w2.add_input('company_name', 'Acme Analytics Corp').width(240)
		})

		w.row('kpi_row', fn (mut w2 simplegui.SimpleWindow) {
			w2.add_label('', 'KPI Metric:').width(90)
			w2.add_input('kpi_label', 'Quarterly Recurring Revenue (ARR)').width(240)
		})

		w.row('rev_row', fn (mut w2 simplegui.SimpleWindow) {
			w2.add_label('', 'Target (USD):').width(90)
			w2.add_slider('total_revenue_slider', 42).width(240) // mapped programmatically as value * 100,000
		})

		w.row('conv_row', fn (mut w2 simplegui.SimpleWindow) {
			w2.add_label('lbl_conv', 'Conversion: 3.42%').width(140)
			w2.add_slider('conversion_slider', 34).width(190) // mapped as value / 10
		})

		w.row('cust_row', fn (mut w2 simplegui.SimpleWindow) {
			w2.add_label('', 'Clients:').width(90)
			w2.add_number('customer_count_val', 1380).width(240)
		})

		w.row('colors_row', fn (mut w2 simplegui.SimpleWindow) {
			w2.add_label('', 'Theme C1:').width(60)
			w2.add_color_well('color_p', '#06b6d4')
			w2.add_horizontal_spacer(20)
			w2.add_label('', 'Theme C2:').width(70)
			w2.add_color_well('color_s', '#3b82f6')
		})

		w.row('radius_row', fn (mut w2 simplegui.SimpleWindow) {
			w2.add_label('lbl_radius', 'Corner Radius: 14px').width(140)
			w2.add_slider('radius_slider', 14).width(190)
		})

		w.add_checkbox('alert_checkbox', 'Deploy Active Target Notification Banner', true)

		w.add_vertical_spacer(8)

		w.begin_row('btn_actions')
		w.add_action('btn_export', 'Export JSON', on_export_clicked).width(160)
		w.add_action('btn_report', 'Submit Report', on_report_clicked).width(160)
		w.end_row()
	})

	win.add_separator()
	win.add_html_view('web_preview', compile_html(spec))

	// Explicit control sizing for a top-to-bottom layout
	win.set_control_width('studio_styles', 840)
	win.set_control_height('studio_styles', 420)

	// Keep handles sized nicely
	win.set_control_height('web_preview', 170)
	win.set_control_width('web_preview', 840)

	// Connect interactive change event callbacks
	win.on_change('company_name', on_style_changed)
	win.on_change('kpi_label', on_style_changed)
	win.on_change('total_revenue_slider', on_style_changed)
	win.on_change('conversion_slider', on_style_changed)
	win.on_change('customer_count_val', on_style_changed)
	win.on_change('color_p', on_style_changed)
	win.on_change('color_s', on_style_changed)
	win.on_change('radius_slider', on_style_changed)
	win.on_change('alert_checkbox', on_style_changed)

	win.set_status('Dashboard ready. Compile specifications or export BI summary.')
	win.run()
}

// Retrieves all active V control properties, compiles the output HTML and updates WebView
fn on_style_changed(mut win simplegui.SimpleWindow, value string) {
	comp := win.get_text('company_name')
	kpi := win.get_text('kpi_label')
	rev := win.get_value_int('total_revenue_slider') * 100000
	conv := f64(win.get_value_int('conversion_slider')) / 10.0
	cust := win.get_value_int('customer_count_val')
	p_color := win.get_text('color_p')
	s_color := win.get_text('color_s')
	radius := win.get_value_int('radius_slider')
	alert := win.get_checked('alert_checkbox')

	// Update dynamic text labels
	win.set_text('lbl_radius', 'Corner Radius: ${radius}px')
	win.set_text('lbl_conv', 'Conversion: ${conv:0.2f}%')

	mut spec := DashboardSpec{
		company_name:    comp
		kpi_label:       kpi
		total_revenue:   rev
		conversion_rate: conv
		customer_count:  cust
		color_primary:   p_color
		color_secondary: s_color
		border_radius:   radius
		enable_alert:    alert
	}

	// Update the html view directly
	win.set_html('web_preview', compile_html(spec))
	win.set_status('Synced BI metrics. Target: \$${rev}. Conversion: ${conv:0.2f}%. Clients: ${cust}.')
}

fn on_export_clicked(mut win simplegui.SimpleWindow) {
	comp := win.get_text('company_name')
	kpi := win.get_text('kpi_label')
	rev := win.get_value_int('total_revenue_slider') * 100000
	conv := f64(win.get_value_int('conversion_slider')) / 10.0
	cust := win.get_value_int('customer_count_val')

	// Compile spec to a clean structured JSON document
	spec := DashboardSpec{
		company_name:    comp
		kpi_label:       kpi
		total_revenue:   rev
		conversion_rate: conv
		customer_count:  cust
	}

	encoded_spec := json2.encode(spec)
	win.alert('Enterprise Spec Exported',
		'KPI specs serialized successfully to JSON!\n\nPayload:\n' + encoded_spec +
			'\n\nThis payload can be published directly to enterprise analytics APIs.')
	win.toast('JSON Specs Compiled')
}

fn on_report_clicked(mut win simplegui.SimpleWindow) {
	comp := win.get_text('company_name')
	if win.confirm('Publish BI Report', 'Do you want to confirm submitting the compiled metrics summary for ${comp}?') {
		win.alert('Report Submitted', 'BI dashboard published successfully to production database.')
		win.toast('Database updated')
	}
}

// Function to compile premium responsive HTML, modern typography layout and JS gauge engine
fn compile_html(spec DashboardSpec) string {
	alert_banner := if spec.enable_alert {
		'
		<div class="alert-banner">
			<div class="pulse-indicator"></div>
			<span><strong>Status Notice:</strong> Target pipeline active.</span>
		</div>
		'
	} else {
		''
	}

	return '
	<!DOCTYPE html>
	<html>
	<head>
		<meta charset="utf-8">
		<style>
			:root {
				--primary: ${spec.color_primary};
				--secondary: ${spec.color_secondary};
				--radius: ${spec.border_radius}px;
			}
			body {
				font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
				background-color: transparent;
				margin: 0;
				padding: 8px;
				display: flex;
				justify-content: center;
				align-items: center;
				height: 100vh;
				box-sizing: border-box;
				overflow: hidden;
			}
			.dashboard {
				width: 360px;
				height: 440px;
				border-radius: var(--radius);
				background: linear-gradient(135deg, #151d30 0%, #0d1220 100%);
				border: 1px solid rgba(255, 255, 255, 0.08);
				padding: 20px;
				box-sizing: border-box;
				display: flex;
				flex-direction: column;
				justify-content: space-between;
				color: #f8fafc;
				box-shadow: 0 16px 36px rgba(0, 0, 0, 0.5), 0 0 40px var(--primary)1a;
			}
			.header {
				display: flex;
				justify-content: space-between;
				align-items: center;
			}
			.company-tag {
				font-size: 11px;
				font-weight: 700;
				color: #94a3b8;
				text-transform: uppercase;
				letter-spacing: 1.5px;
			}
			.badge {
				background: var(--primary)1b;
				color: var(--primary);
				border: 1px solid var(--primary)33;
				padding: 3px 8px;
				border-radius: 9999px;
				font-size: 9px;
				font-weight: bold;
				text-transform: uppercase;
			}
			.kpi-section {
				margin: 14px 0;
			}
			.kpi-title {
				font-size: 10px;
				font-weight: 600;
				color: #64748b;
				text-transform: uppercase;
				letter-spacing: 1px;
				margin: 0 0 4px 0;
			}
			.kpi-value {
				font-size: 30px;
				font-weight: 800;
				margin: 0;
				background: linear-gradient(135deg, #ffffff, #94a3b8);
				-webkit-background-clip: text;
				-webkit-text-fill-color: transparent;
			}
			.stats-grid {
				display: grid;
				grid-template-columns: 1fr 1px 1fr;
				gap: 12px;
				background: rgba(255, 255, 255, 0.02);
				border: 1px solid rgba(255, 255, 255, 0.04);
				border-radius: calc(var(--radius) - 4px);
				padding: 12px;
				margin: 12px 0;
			}
			.stat-box {
				display: flex;
				flex-direction: column;
			}
			.stat-lbl {
				font-size: 9px;
				font-weight: 600;
				color: #64748b;
				text-transform: uppercase;
				margin-bottom: 2px;
			}
			.stat-val {
				font-size: 16px;
				font-weight: 700;
				color: #f1f5f9;
			}
			.divider {
				background-color: rgba(255, 255, 255, 0.06);
				width: 1px;
				height: 100%;
			}
			.alert-banner {
				background: linear-gradient(to right, var(--primary)15, var(--secondary)0f);
				border-left: 2px solid var(--primary);
				border-radius: 6px;
				padding: 8px 12px;
				display: flex;
				align-items: center;
				font-size: 10px;
				color: #cbd5e1;
				gap: 8px;
				margin: 12px 0;
			}
			.pulse-indicator {
				width: 6px;
				height: 6px;
				border-radius: 50%;
				background-color: var(--primary);
				box-shadow: 0 0 0 0 var(--primary)cc;
				animation: pulse 1.5s infinite;
			}
			@keyframes pulse {
				0% {
					transform: scale(0.95);
					box-shadow: 0 0 0 0 var(--primary)70;
				}
				70% {
					transform: scale(1);
					box-shadow: 0 0 0 6px var(--primary)00;
				}
				100% {
					transform: scale(0.95);
					box-shadow: 0 0 0 0 var(--primary)00;
				}
			}
			.gauge-container {
				width: 100%;
				height: 40px;
				background: rgba(255, 255, 255, 0.03);
				border-radius: 6px;
				position: relative;
				overflow: hidden;
				display: flex;
				align-items: center;
				padding: 0 12px;
				box-sizing: border-box;
				border: 1px solid rgba(255, 255, 255, 0.03);
			}
			.gauge-fill {
				position: absolute;
				top: 0;
				left: 0;
				height: 100%;
				width: ${spec.conversion_rate * 10}%;
				background: linear-gradient(to right, var(--primary), var(--secondary));
				opacity: 0.15;
				z-index: 1;
				transition: width 0.4s ease;
			}
			.gauge-title {
				font-size: 11px;
				z-index: 5;
				font-weight: 500;
				color: #e2e8f0;
			}
			.gauge-val {
				margin-left: auto;
				font-size: 12px;
				font-weight: 700;
				color: var(--primary);
				z-index: 5;
			}
			.footer {
				display: flex;
				justify-content: space-between;
				align-items: center;
				border-top: 1px solid rgba(255, 255, 255, 0.06);
				padding-top: 12px;
				font-size: 10px;
				color: #475569;
			}
			.pulse-dot {
				display: inline-block;
				width: 6px;
				height: 6px;
				background-color: #22c55e;
				border-radius: 50%;
				margin-right: 4px;
			}
		</style>
	</head>
	<body>
		<div class="dashboard">
			<div class="header">
				<span class="company-tag">${spec.company_name}</span>
				<span class="badge">Live Spec</span>
			</div>
			
			<div class="kpi-section">
				<p class="kpi-title">${spec.kpi_label}</p>
				<h1 class="kpi-value" id="revenue_val">\$${spec.total_revenue}</h1>
			</div>

			<div class="gauge-container">
				<div class="gauge-fill"></div>
				<span class="gauge-title">Target Threshold Conversion</span>
				<span class="gauge-val" id="conv_val">${spec.conversion_rate}%</span>
			</div>

			<div class="stats-grid">
				<div class="stat-box">
					<span class="stat-lbl">Active Clients</span>
					<span class="stat-val" id="cust_val">${spec.customer_count}</span>
				</div>
				<div class="divider"></div>
				<div class="stat-box">
					<span class="stat-lbl">Volatility Risk</span>
					<span class="stat-val" style="color:#ef4444;">LOW</span>
				</div>
			</div>

			${alert_banner}

			<div class="footer">
				<span><span class="pulse-dot"></span>Cloud Engine Active (<span id="sessionTimer">0s</span>)</span>
				<span style="font-family: monospace; font-weight: bold;">v1.4.0</span>
			</div>
		</div>

		<script>
			// Smart currency formatting and animation trigger
			const rawRev = ${spec.total_revenue};
			const formatter = new Intl.NumberFormat(\'en-US\', {
				style: \'currency\',
				currency: \'USD\',
				maximumFractionDigits: 0
			});
			document.getElementById("revenue_val").textContent = formatter.format(rawRev);

			// 1. Session clock widget updater
			let elapsedSeconds = 0;
			setInterval(() => {
				elapsedSeconds++;
				document.getElementById("sessionTimer").textContent = elapsedSeconds + "s";
			}, 1000);
		</script>
	</body>
	</html>
	'
}
