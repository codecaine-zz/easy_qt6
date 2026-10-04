#include "simplegui/application.h"
#include "simplegui/window.h"
#include "simplegui/vbox.h"
#include "simplegui/hbox.h"
#include "simplegui/label.h"
#include "simplegui/text_input.h"
#include "simplegui/search_field.h"
#include "simplegui/button.h"
#include "simplegui/checkbox.h"
#include "simplegui/dropdown.h"
#include "simplegui/progress_indicator.h"
#include "simplegui/circular_progress.h"
#include "simplegui/grid.h"
#include "simplegui/split_view.h"
#include "simplegui/breadcrumbs.h"
#include "simplegui/group_box.h"

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    simplegui::Window window("Enterprise Data Explorer", 680, 480);

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(16);
    root->set_spacing(10);

    // 1. Breadcrumbs Header
    auto crumbs = std::make_shared<simplegui::Breadcrumbs>(std::vector<std::string>{"Organization", "Analytics", "Live Transactions"});

    // 2. Toolbar Row
    auto toolbar = std::make_shared<simplegui::HBox>();
    toolbar->set_spacing(8);

    auto search = std::make_shared<simplegui::SearchField>("Search customers or IDs...");
    auto status_filter = std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"All Statuses", "Paid", "Pending", "Failed"});
    auto btn_add = std::make_shared<simplegui::Button>("+ New Record");
    btn_add->set_style("background-color: #2563eb; color: #ffffff; font-weight: 600; border: 1px solid #3b82f6;");
    auto btn_refresh = std::make_shared<simplegui::Button>("Refresh");

    toolbar->add_child(search);
    toolbar->add_child(status_filter);
    toolbar->add_child(btn_add);
    toolbar->add_child(btn_refresh);

    // 3. Central SplitView (Sidebar + Main Grid)
    auto split = std::make_shared<simplegui::SplitView>(true);

    // Sidebar
    auto sidebar = std::make_shared<simplegui::VBox>();
    sidebar->set_margins(8);
    sidebar->set_spacing(10);

    auto grp_summary = std::make_shared<simplegui::GroupBox>("Overview");
    auto summary_vbox = std::make_shared<simplegui::VBox>();
    summary_vbox->set_spacing(6);
    summary_vbox->add_child(std::make_shared<simplegui::Label>("Total Revenue: $148,200"));
    summary_vbox->add_child(std::make_shared<simplegui::Label>("Active Subscriptions: 1,420"));

    auto circ = std::make_shared<simplegui::CircularProgress>();
    circ->set_value(84);
    summary_vbox->add_child(circ);
    summary_vbox->add_child(std::make_shared<simplegui::Label>("Target: 84% Met"));
    grp_summary->add_child(summary_vbox);

    auto grp_filter = std::make_shared<simplegui::GroupBox>("Quick Filters");
    auto filter_vbox = std::make_shared<simplegui::VBox>();
    filter_vbox->set_spacing(6);
    filter_vbox->add_child(std::make_shared<simplegui::Checkbox>("VIP Customers Only", false));
    filter_vbox->add_child(std::make_shared<simplegui::Checkbox>("High Value (> $1,000)", true));
    filter_vbox->add_child(std::make_shared<simplegui::Checkbox>("Auto-Renew Enabled", true));
    grp_filter->add_child(filter_vbox);

    sidebar->add_child(grp_summary);
    sidebar->add_child(grp_filter);
    sidebar->add_stretch();

    // Data Table Grid
    auto grid = std::make_shared<simplegui::Grid>(6, 5, std::vector<std::string>{"ID", "Customer", "Tier", "Amount", "Status"});
    const std::vector<std::vector<std::string>> data = {
        {"#401", "Acme Corporation", "Enterprise", "$12,450", "Paid"},
        {"#402", "Nova Solutions LLC", "Pro", "$3,200", "Paid"},
        {"#403", "Apex Systems Inc", "Basic", "$850", "Pending"},
        {"#404", "Quantum Dynamics", "Enterprise", "$24,000", "Paid"},
        {"#405", "Vanguard Media", "Pro", "$4,100", "Failed"},
        {"#406", "Horizon Robotics", "Enterprise", "$18,900", "Paid"}
    };

    for (int r = 0; r < static_cast<int>(data.size()); ++r) {
        for (int c = 0; c < 5; ++c) {
            grid->set_cell(r, c, data[r][c]);
        }
    }

    split->add_child(sidebar);
    split->add_child(grid);

    // 4. Bottom Status Bar
    auto status_bar = std::make_shared<simplegui::HBox>();
    status_bar->set_spacing(12);

    auto lbl_status = std::make_shared<simplegui::Label>("Displaying 6 of 1,420 records | System: Synced");
    auto sync_progress = std::make_shared<simplegui::ProgressIndicator>();
    sync_progress->set_value(100);
    auto btn_export = std::make_shared<simplegui::Button>("Export CSV");

    status_bar->add_child(lbl_status);
    status_bar->add_stretch();
    status_bar->add_child(sync_progress);
    status_bar->add_child(btn_export);

    // Assemble Root
    root->add_child(crumbs);
    root->add_child(toolbar);
    root->add_child(split);
    root->add_child(status_bar);

    window.set_content(root);
    window.show();

    return app.run();
}
