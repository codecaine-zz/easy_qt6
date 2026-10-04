#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"
#include "simplegui/group_box.h"
#include "simplegui/grid.h"
#include "simplegui/sparkline.h"
#include "simplegui/vfd_meter.h"
#include "simplegui/stat_card.h"
#include "simplegui/composition_bar.h"
#include "simplegui/status_pill.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    simplegui::Window window("TMOG Inspired System Telemetry", 740, 560);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(16);
    root->set_spacing(12);

    // 1. Header Toolbar
    auto header = std::make_shared<simplegui::HBox>();
    header->set_spacing(10);

    auto title = std::make_shared<simplegui::Label>("TMOG / PRECISION TELEMETRY");
    title->set_style("font-weight: bold; font-size: 14px; letter-spacing: 1px; color: #f4f4f5;");

    auto pill_live = std::make_shared<simplegui::StatusPill>("LIVE 1.0.0", "#10b981");
    auto pill_sensors = std::make_shared<simplegui::StatusPill>("SENSORS ONLINE", "#06b6d4");

    header->add_child(title);
    header->add_child(pill_live);
    header->add_stretch();
    header->add_child(pill_sensors);

    // 2. Stat Cards Row
    auto stat_row = std::make_shared<simplegui::HBox>();
    stat_row->set_spacing(10);

    auto card_cpu = std::make_shared<simplegui::StatCard>("CPU LOAD", "34.8%", "16 Cores · 4.80 GHz", "#06b6d4");
    auto card_mem = std::make_shared<simplegui::StatCard>("MEMORY", "14.2 GB", "64 GB Total (22%)", "#10b981");
    auto card_io = std::make_shared<simplegui::StatCard>("DISK THROUGHPUT", "1.42 GB/s", "NVMe PCIe 4.0", "#f59e0b");
    auto card_pwr = std::make_shared<simplegui::StatCard>("PACKAGE POWER", "46.5 W", "Efficiency High", "#ec4899");

    stat_row->add_child(card_cpu);
    stat_row->add_child(card_mem);
    stat_row->add_child(card_io);
    stat_row->add_child(card_pwr);

    // 3. Middle Section: Sparkline History + VFD Meter Levels
    auto mid_row = std::make_shared<simplegui::HBox>();
    mid_row->set_spacing(12);

    // Left: Sparkline Telemetry Graph & Composition Bar
    auto grp_graph = std::make_shared<simplegui::GroupBox>("CPU Utilization History (Rolling Telemetry)");
    auto graph_vbox = std::make_shared<simplegui::VBox>();
    graph_vbox->set_margins(12);
    graph_vbox->set_spacing(10);

    auto sparkline = std::make_shared<simplegui::Sparkline>("#06b6d4");
    sparkline->set_range(0.0, 100.0);
    // Add realistic CPU samples
    const std::vector<double> history = {
        18, 22, 25, 20, 19, 35, 48, 62, 58, 45, 
        30, 28, 32, 40, 55, 78, 85, 92, 70, 52, 
        41, 38, 35, 33, 30, 36, 42, 50, 48, 35
    };
    sparkline->set_samples(history);

    auto comp_label = std::make_shared<simplegui::Label>("Memory Composition: Apps (14GB) | Wired (6GB) | Cache (8GB) | Free (36GB)");
    comp_label->set_style("color: #a1a1aa; font-size: 11px;");

    auto comp_bar = std::make_shared<simplegui::CompositionBar>();
    comp_bar->add_segment("Apps", 14.0, "#2563eb");
    comp_bar->add_segment("Wired", 6.0, "#06b6d4");
    comp_bar->add_segment("Cached", 8.0, "#f59e0b");
    comp_bar->add_segment("Free", 36.0, "#27272a");

    graph_vbox->add_child(sparkline);
    graph_vbox->add_child(comp_label);
    graph_vbox->add_child(comp_bar);
    grp_graph->add_child(graph_vbox);

    // Right: VFD Segmented Meter Core Levels
    auto grp_vfd = std::make_shared<simplegui::GroupBox>("Core VU Levels");
    auto vfd_hbox = std::make_shared<simplegui::HBox>();
    vfd_hbox->set_margins(12);
    vfd_hbox->set_spacing(10);

    auto vfd1 = std::make_shared<simplegui::VfdMeter>(18, true);
    vfd1->set_value(88.0);
    auto vfd2 = std::make_shared<simplegui::VfdMeter>(18, true);
    vfd2->set_value(45.0);
    auto vfd3 = std::make_shared<simplegui::VfdMeter>(18, true);
    vfd3->set_value(95.0);
    auto vfd4 = std::make_shared<simplegui::VfdMeter>(18, true);
    vfd4->set_value(32.0);

    vfd_hbox->add_child(vfd1);
    vfd_hbox->add_child(vfd2);
    vfd_hbox->add_child(vfd3);
    vfd_hbox->add_child(vfd4);
    grp_vfd->add_child(vfd_hbox);

    mid_row->add_child(grp_graph);
    mid_row->add_child(grp_vfd);

    // 4. Bottom Section: Heavy Subsystems Process Table
    auto grp_procs = std::make_shared<simplegui::GroupBox>("Top Resource Processes");
    auto procs_vbox = std::make_shared<simplegui::VBox>();
    procs_vbox->set_margins(10);

    auto grid = std::make_shared<simplegui::Grid>(5, 5, std::vector<std::string>{"PID", "Process Name", "CPU %", "Memory", "Energy"});
    const std::vector<std::vector<std::string>> proc_data = {
        {"1082", "clang++ (EasyQt6 Build)", "68.4%", "1.24 GB", "Very High"},
        {"429", "WindowServer", "12.8%", "680 MB", "Medium"},
        {"8891", "QtWebEngineProcess", "8.2%", "512 MB", "Low"},
        {"1", "launchd (init)", "0.1%", "28 MB", "Very Low"},
        {"7420", "tmog_telemetry_core", "0.4%", "18 MB", "Very Low"}
    };

    for (int r = 0; r < 5; ++r) {
        for (int c = 0; c < 5; ++c) {
            grid->set_cell(r, c, proc_data[r][c]);
        }
    }
    procs_vbox->add_child(grid);
    grp_procs->add_child(procs_vbox);

    root->add_child(header);
    root->add_child(stat_row);
    root->add_child(mid_row);
    root->add_child(grp_procs);

    window.set_content(root);
    window.show();

    return app.run();
}
