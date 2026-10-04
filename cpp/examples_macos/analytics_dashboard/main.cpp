#include "simplegui/simplegui.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    simplegui::Window window("Global Telemetry & Business Analytics", 960, 680);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(16);
    root->set_spacing(12);

    // --- Header Bar ---
    auto header = std::make_shared<simplegui::HBox>();
    header->set_spacing(12);

    auto title = std::make_shared<simplegui::Label>("Enterprise Analytics Cloud");
    title->set_style("font-size: 20px; font-weight: bold; color: #f8fafc;");
    header->add_child(title);

    auto live_pill = std::make_shared<simplegui::StatusPill>("REALTIME STREAM", "#10b981");
    header->add_child(live_pill);

    header->add_stretch();

    auto date_range = std::make_shared<simplegui::DateRangePicker>("2026-10-01", "2026-10-31");
    header->add_child(date_range);

    auto export_btn = std::make_shared<simplegui::Button>("Export Report");
    export_btn->set_style("background-color: #2563eb; color: #ffffff; font-weight: 600; padding: 6px 14px; border-radius: 6px;");
    header->add_child(export_btn);
    root->add_child(header);

    // --- Top KPI StatGrid ---
    auto stats = std::make_shared<simplegui::StatGrid>();
    stats->add_stat("Active Connections", "28,492", "+14.8%", true);
    stats->add_stat("Monthly Recurring", "$342,100", "+9.2%", true);
    stats->add_stat("Avg Latency (p99)", "14.2 ms", "-6.5%", true);
    stats->add_stat("Node Health", "99.98%", "Optimal", true);
    root->add_child(stats);

    // --- Mid Row: LineChart & DonutChart ---
    auto mid_row = std::make_shared<simplegui::HBox>();
    mid_row->set_spacing(12);

    auto line_chart = std::make_shared<simplegui::LineChart>("Network Bandwidth (Gbps)");
    line_chart->set_x_labels({"00:00", "04:00", "08:00", "12:00", "16:00", "20:00", "24:00"});
    line_chart->add_series("Inbound", {24, 32, 68, 120, 145, 110, 48}, "#3b82f6", true);
    line_chart->add_series("Outbound", {18, 22, 45, 85, 96, 72, 35}, "#10b981", true);
    mid_row->add_child(line_chart);

    auto donut = std::make_shared<simplegui::DonutChart>("84%", "CAPACITY");
    donut->add_segment("Database", 40.0, "#3b82f6");
    donut->add_segment("Application", 26.0, "#8b5cf6");
    donut->add_segment("Caching", 18.0, "#06b6d4");
    donut->add_segment("Headroom", 16.0, "#334155");
    mid_row->add_child(donut);

    root->add_child(mid_row);

    // --- Bottom Row: BarChart & RadarChart ---
    auto bot_row = std::make_shared<simplegui::HBox>();
    bot_row->set_spacing(12);

    auto bar_chart = std::make_shared<simplegui::BarChart>("Monthly Revenue Growth ($k)");
    bar_chart->add_bar("May", 140.0, "#3b82f6");
    bar_chart->add_bar("Jun", 175.0, "#3b82f6");
    bar_chart->add_bar("Jul", 210.0, "#06b6d4");
    bar_chart->add_bar("Aug", 245.0, "#3b82f6");
    bar_chart->add_bar("Sep", 290.0, "#10b981");
    bar_chart->add_bar("Oct", 342.0, "#10b981");
    bot_row->add_child(bar_chart);

    auto radar = std::make_shared<simplegui::RadarChart>("Global Infrastructure Benchmark");
    radar->set_dimensions({"Throughput", "Latency", "Resilience", "Security", "Scalability", "Efficiency"});
    radar->add_dataset("Production", {92, 88, 96, 84, 90, 94}, "#3b82f6");
    radar->add_dataset("Benchmark", {75, 70, 80, 85, 65, 70}, "#f59e0b");
    bot_row->add_child(radar);

    root->add_child(bot_row);

    window.set_content(root);
    window.show();

    return app.run();
}
