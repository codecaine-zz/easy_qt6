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
    app.set_theme("windows_fluent");

    simplegui::Window window("Task Manager", 880, 580);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(20);
    root->set_spacing(16);

    // Header bar
    auto header = std::make_shared<simplegui::HBox>();
    header->set_spacing(12);

    auto title = std::make_shared<simplegui::Label>("Processes & Performance");
    title->set_style("color: #ffffff; font-size: 20px; font-weight: 600;");

    auto end_task_btn = std::make_shared<simplegui::Button>("End task");
    end_task_btn->set_style("background-color: #323232; color: #ffffff; border: 1px solid #404040; border-radius: 4px; padding: 6px 16px; font-weight: 600;");

    auto run_new_btn = std::make_shared<simplegui::Button>("Run new task");
    run_new_btn->set_style("background-color: #60cdff; color: #000000; border-radius: 4px; padding: 6px 16px; font-weight: 600;");

    header->add_child(title);
    header->add_child(run_new_btn);
    header->add_child(end_task_btn);
    root->add_child(header);

    // Performance Summary Cards Row
    auto cards_row = std::make_shared<simplegui::HBox>();
    cards_row->set_spacing(12);

    struct MetricCard {
        std::string title;
        std::string val;
        int pct;
        std::string color;
    };

    std::vector<MetricCard> metrics = {
        {"CPU", "18% 3.42 GHz", 18, "#60cdff"},
        {"Memory", "8.2/16.0 GB (51%)", 51, "#4cc2ff"},
        {"Disk (C:)", "12% SSD", 12, "#00b7c3"},
        {"GPU", "22% RTX 4070", 22, "#107c41"}
    };

    for (const auto& m : metrics) {
        auto card = std::make_shared<simplegui::VBox>();
        card->set_margins(12);
        card->set_spacing(6);
        card->set_style("background-color: #2b2b2b; border: 1px solid #363636; border-radius: 6px;");

        auto t = std::make_shared<simplegui::Label>(m.title);
        t->set_style("color: " + m.color + "; font-size: 13px; font-weight: 600;");

        auto v = std::make_shared<simplegui::Label>(m.val);
        v->set_style("color: #ffffff; font-size: 16px; font-weight: 600;");

        auto p = std::make_shared<simplegui::ProgressIndicator>();
        p->set_value(m.pct);

        card->add_child(t);
        card->add_child(v);
        card->add_child(p);
        cards_row->add_child(card);
    }
    root->add_child(cards_row);

    // Process Table
    auto proc_header = std::make_shared<simplegui::Label>("Apps and Background Processes");
    proc_header->set_style("color: #ffffff; font-size: 15px; font-weight: 600; margin-top: 6px;");
    root->add_child(proc_header);

    auto table = std::make_shared<simplegui::Grid>(7, 5, std::vector<std::string>{"Name", "Status", "CPU", "Memory", "Disk"});

    struct WinProc {
        std::string name;
        std::string status;
        std::string cpu;
        std::string mem;
        std::string disk;
    };

    std::vector<WinProc> win_procs = {
        {"Windows Explorer", "Running", "1.2%", "84.2 MB", "0.1 MB/s"},
        {"Microsoft Edge (14)", "Running", "4.8%", "682.0 MB", "0.4 MB/s"},
        {"Visual Studio Code", "Running", "2.1%", "412.5 MB", "0.0 MB/s"},
        {"EasyQt6 Application", "Running", "0.2%", "38.6 MB", "0.0 MB/s"},
        {"Desktop Window Manager", "Running", "3.4%", "124.0 MB", "0.0 MB/s"},
        {"Windows Terminal", "Running", "0.5%", "64.1 MB", "0.0 MB/s"},
        {"System", "Running", "0.9%", "12.8 MB", "1.8 MB/s"}
    };

    for (int r = 0; r < static_cast<int>(win_procs.size()); ++r) {
        table->set_cell(r, 0, win_procs[r].name);
        table->set_cell(r, 1, win_procs[r].status);
        table->set_cell(r, 2, win_procs[r].cpu);
        table->set_cell(r, 3, win_procs[r].mem);
        table->set_cell(r, 4, win_procs[r].disk);
    }
    root->add_child(table);

    window.set_content(root);
    window.show();

    return app.run();
}
