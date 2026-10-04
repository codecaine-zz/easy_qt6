#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/button.h"
#include "simplegui/progress_indicator.h"
#include "simplegui/grid.h"
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("linux_adwaita");

    simplegui::Window window("GNOME System Monitor", 860, 560);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(20);
    root->set_spacing(16);

    // Header bar
    auto header = std::make_shared<simplegui::HBox>();
    header->set_spacing(12);

    auto title = std::make_shared<simplegui::Label>("Resource Usage & Processes");
    title->set_style("color: #ffffff; font-size: 20px; font-weight: 700;");

    auto refresh_btn = std::make_shared<simplegui::Button>("Refresh");
    refresh_btn->set_style("background-color: #3584e4; color: #ffffff; font-weight: 600; border-radius: 8px; padding: 6px 16px;");

    header->add_child(title);
    header->add_child(refresh_btn);
    root->add_child(header);

    // Cards Row (CPU, RAM, Swap/Disk)
    auto stats_row = std::make_shared<simplegui::HBox>();
    stats_row->set_spacing(14);

    // CPU Card
    auto cpu_card = std::make_shared<simplegui::VBox>();
    cpu_card->set_margins(14);
    cpu_card->set_spacing(8);
    cpu_card->set_style("background-color: #303030; border: 1px solid #3d3d3d; border-radius: 12px;");
    auto cpu_title = std::make_shared<simplegui::Label>("CPU (8 Cores)");
    cpu_title->set_style("color: #78aeed; font-size: 13px; font-weight: 700;");
    auto cpu_val = std::make_shared<simplegui::Label>("24% Utilization");
    cpu_val->set_style("color: #ffffff; font-size: 18px; font-weight: 700;");
    auto cpu_bar = std::make_shared<simplegui::ProgressIndicator>();
    cpu_bar->set_value(24);
    cpu_card->add_child(cpu_title);
    cpu_card->add_child(cpu_val);
    cpu_card->add_child(cpu_bar);
    stats_row->add_child(cpu_card);

    // Memory Card
    auto mem_card = std::make_shared<simplegui::VBox>();
    mem_card->set_margins(14);
    mem_card->set_spacing(8);
    mem_card->set_style("background-color: #303030; border: 1px solid #3d3d3d; border-radius: 12px;");
    auto mem_title = std::make_shared<simplegui::Label>("Memory");
    mem_title->set_style("color: #57e389; font-size: 13px; font-weight: 700;");
    auto mem_val = std::make_shared<simplegui::Label>("7.4 GiB of 16 GiB (46%)");
    mem_val->set_style("color: #ffffff; font-size: 18px; font-weight: 700;");
    auto mem_bar = std::make_shared<simplegui::ProgressIndicator>();
    mem_bar->set_value(46);
    mem_card->add_child(mem_title);
    mem_card->add_child(mem_val);
    mem_card->add_child(mem_bar);
    stats_row->add_child(mem_card);

    // NVMe Disk Card
    auto disk_card = std::make_shared<simplegui::VBox>();
    disk_card->set_margins(14);
    disk_card->set_spacing(8);
    disk_card->set_style("background-color: #303030; border: 1px solid #3d3d3d; border-radius: 12px;");
    auto disk_title = std::make_shared<simplegui::Label>("Root Disk (/dev/nvme0n1p2)");
    disk_title->set_style("color: #ffa348; font-size: 13px; font-weight: 700;");
    auto disk_val = std::make_shared<simplegui::Label>("218 GB free of 512 GB");
    disk_val->set_style("color: #ffffff; font-size: 18px; font-weight: 700;");
    auto disk_bar = std::make_shared<simplegui::ProgressIndicator>();
    disk_bar->set_value(57);
    disk_card->add_child(disk_title);
    disk_card->add_child(disk_val);
    disk_card->add_child(disk_bar);
    stats_row->add_child(disk_card);

    root->add_child(stats_row);

    // Process list section
    auto proc_header = std::make_shared<simplegui::Label>("Active Linux Processes");
    proc_header->set_style("color: #ffffff; font-size: 15px; font-weight: 700; margin-top: 6px;");
    root->add_child(proc_header);

    auto table = std::make_shared<simplegui::Grid>(7, 5, std::vector<std::string>{"PID", "Process Name", "User", "CPU %", "Memory (MB)"});

    struct ProcItem {
        std::string pid;
        std::string name;
        std::string user;
        std::string cpu;
        std::string mem;
    };

    std::vector<ProcItem> procs = {
        {"1042", "gnome-shell", "user", "4.2%", "342 MB"},
        {"1520", "pipewire", "user", "1.1%", "48 MB"},
        {"2311", "wayland-compositor", "user", "3.8%", "184 MB"},
        {"3480", "firefox-esr", "user", "11.6%", "980 MB"},
        {"4102", "systemd-journald", "root", "0.2%", "34 MB"},
        {"5620", "easy_qt6_app", "user", "0.5%", "42 MB"},
        {"6124", "kitty", "user", "0.8%", "64 MB"}
    };

    for (int r = 0; r < static_cast<int>(procs.size()); ++r) {
        table->set_cell(r, 0, procs[r].pid);
        table->set_cell(r, 1, procs[r].name);
        table->set_cell(r, 2, procs[r].user);
        table->set_cell(r, 3, procs[r].cpu);
        table->set_cell(r, 4, procs[r].mem);
    }
    root->add_child(table);

    refresh_btn->on_click([cpu_val, cpu_bar]() {
        cpu_val->set_text("31% Utilization");
        cpu_bar->set_value(31);
    });

    window.set_content(root);
    window.show();

    return app.run();
}
