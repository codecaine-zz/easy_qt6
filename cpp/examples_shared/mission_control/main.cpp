// Mission Control - a futuristic dashboard built only with SimpleGUI controls.
//
// Shows: neon theme, Timer, RadialGauge, RadarScope, TerminalView, LedIndicator,
// ToggleSwitch, NeonButton, GlassPanel, SegmentedControl and Sparkline.
#include "simplegui/simplegui.h"

#include <cmath>
#include <string>

using namespace simplegui;

int main(int argc, char* argv[]) {
    Application app(argc, argv);
    app.set_theme("neon");

    Window window("Mission Control", 1000, 640);

    // ---- Left column: gauges ------------------------------------------------
    auto speed = std::make_shared<RadialGauge>("VELOCITY", 0, 300);
    speed->set_units("km/s");
    auto power = std::make_shared<RadialGauge>("REACTOR", 0, 100);
    power->set_units("%");
    power->set_thresholds(75, 90);

    auto gauges = std::make_shared<GlassPanel>("Telemetry");
    gauges->add_child(speed, 1);
    gauges->add_child(power, 1);

    // ---- Middle column: radar + mode selector -------------------------------
    auto radar = std::make_shared<RadarScope>();
    radar->set_min_size(260, 260);
    const int ship = radar->add_blip(45, 0.7);
    radar->add_blip(200, 0.4, "#ff2d75");

    auto mode = std::make_shared<SegmentedControl>(std::vector<std::string>{"Cruise", "Scan", "Combat"});

    auto scanner = std::make_shared<GlassPanel>("Long range scanner");
    scanner->add_child(radar, 1);
    scanner->add_child(mode);

    // ---- Right column: systems ----------------------------------------------
    auto shields_led = std::make_shared<LedIndicator>("#22c55e", true);
    shields_led->set_label("Shields");
    auto alarm_led = std::make_shared<LedIndicator>("#ef4444", false);
    alarm_led->set_label("Proximity alarm");

    auto shields = std::make_shared<ToggleSwitch>(true, "Shields");
    auto autopilot = std::make_shared<ToggleSwitch>(false, "Autopilot");

    auto history = std::make_shared<Sparkline>("#00e5ff");
    history->set_max_samples(60);

    auto engage = std::make_shared<NeonButton>("ENGAGE");
    auto abort_btn = std::make_shared<NeonButton>("ABORT", "#ff2d75");
    auto buttons = std::make_shared<HBox>();
    buttons->add_child(engage, 1);
    buttons->add_child(abort_btn, 1);

    auto systems = std::make_shared<GlassPanel>("Systems");
    systems->add_child(shields_led);
    systems->add_child(alarm_led);
    systems->add_spacing(8);
    systems->add_child(shields);
    systems->add_child(autopilot);
    systems->add_spacing(8);
    systems->add_child(history);
    systems->add_stretch(1);
    systems->add_child(buttons);

    auto top = std::make_shared<HBox>();
    top->set_spacing(12);
    top->add_child(gauges, 1);
    top->add_child(scanner, 2);
    top->add_child(systems, 1);

    // ---- Bottom: ship computer ----------------------------------------------
    auto console = std::make_shared<TerminalView>();
    console->set_max_lines(500);
    console->set_min_size(200, 160);
    console->print_line("SHIP COMPUTER ONLINE. Type 'help'.");

    auto root = std::make_shared<VBox>();
    root->set_margins(16);
    root->set_spacing(12);
    root->add_child(top, 3);
    root->add_child(console, 1);
    window.set_content(root);

    // ---- Events -------------------------------------------------------------
    shields->on_toggle([shields_led, console](bool on) {
        shields_led->set_on(on);
        console->print_line(on ? "Shields raised." : "Shields lowered!", on ? "" : "#f59e0b");
    });
    autopilot->on_toggle([console](bool on) {
        console->print_line(std::string("Autopilot ") + (on ? "engaged." : "disengaged."));
    });
    mode->on_change([radar, console](int, const std::string& text) {
        radar->set_sweep_speed(text == "Scan" ? 240 : 90);
        console->print_line("Mode: " + text);
    });
    engage->on_click([power, console]() {
        power->set_value(95);
        console->print_line("Main engines engaged.", "#00e5ff");
    });
    abort_btn->on_click([power, alarm_led, console]() {
        power->set_value(10);
        alarm_led->set_blinking(false);
        alarm_led->set_on(false);
        console->print_line("ABORT - engines idle.", "#ff2d75");
    });
    console->on_command([console, alarm_led](const std::string& cmd) {
        console->print_line("> " + cmd, "#94a3b8");
        if (cmd == "help") {
            console->print_line("Commands: help, alarm, clear, quit");
        } else if (cmd == "alarm") {
            alarm_led->set_blinking(true, 300);
            console->print_line("Proximity alarm triggered!", "#ef4444");
        } else if (cmd == "clear") {
            console->clear();
        } else if (cmd == "quit") {
            Application::quit();
        } else {
            console->print_line("Unknown command: " + cmd, "#f59e0b");
        }
    });

    // ---- A Timer drives the simulation (Delphi: TTimer.OnTimer) -------------
    Timer tick(100);
    double t = 0.0;
    tick.on_tick([&t, speed, radar, history, ship]() {
        t += 0.1;
        const double v = 150.0 + 120.0 * std::sin(t * 0.4);
        speed->set_value(v);
        history->add_sample(v);
        radar->move_blip(ship, std::fmod(45.0 + t * 12.0, 360.0), 0.7);
    });
    tick.start();

    window.center();
    window.show();
    return app.run();
}
