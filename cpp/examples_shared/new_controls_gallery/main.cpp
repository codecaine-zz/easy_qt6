#include "simplegui/simplegui.h"

using namespace simplegui;

int main(int argc, char* argv[]) {
    Application app(argc, argv);
    app.set_theme("light");
    Window window("New Controls Gallery", 1100, 900);
    
    auto main_layout = std::make_shared<VBox>();
    main_layout->set_margins(20);
    main_layout->set_spacing(20);
    
    auto title = std::make_shared<Label>("New Controls (v0.2)");
    title->set_font_size(24);
    title->set_bold(true);
    main_layout->add_child(title);
    
    auto row1 = std::make_shared<HBox>();
    row1->set_spacing(20);
    
    auto group1 = std::make_shared<GroupBox>("DataForm & PathPicker");
    auto form = std::make_shared<DataForm>();
    form->set_fields({
        {"name", "Name", "John Doe", FormField::Type::Text},
        {"age", "Age", "30", FormField::Type::Number},
        {"admin", "Is Admin", "true", FormField::Type::Checkbox}
    });
    auto picker = std::make_shared<PathPicker>();
    group1->add_child(form);
    group1->add_spacing(10);
    group1->add_child(std::make_shared<Label>("PathPicker:"));
    group1->add_child(picker);
    row1->add_child(group1, 1);
    
    auto group2 = std::make_shared<GroupBox>("MarkdownViewer");
    auto md = std::make_shared<MarkdownViewer>("# Hello\n* Item 1\n* Item 2\n\n**Bold text**");
    group2->add_child(md, 1);
    row1->add_child(group2, 1);
    
    auto group3 = std::make_shared<GroupBox>("SignaturePad");
    auto pad = std::make_shared<SignaturePad>();
    group3->add_child(pad, 1);
    row1->add_child(group3, 1);
    
    main_layout->add_child(row1, 1);
    
    auto row2 = std::make_shared<HBox>();
    row2->set_spacing(20);
    
    auto group4 = std::make_shared<GroupBox>("GanttChart & Timeline");
    auto gantt = std::make_shared<GanttChart>();
    gantt->set_tasks({
        {"Design", 0, 2, "#3b82f6"},
        {"Develop", 2, 5, "#10b981"},
        {"Test", 7, 2, "#f59e0b"}
    });
    auto timeline = std::make_shared<Timeline>();
    timeline->set_steps({"Step 1", "Step 2", "Step 3"});
    timeline->set_current_step(1);
    group4->add_child(gantt, 1);
    group4->add_spacing(10);
    group4->add_child(timeline);
    row2->add_child(group4, 1);
    
    auto group5 = std::make_shared<GroupBox>("HeatmapCalendar");
    auto heatmap = std::make_shared<HeatmapCalendar>();
    std::map<std::string, int> heat_data = {
        {"2026-10-01", 1}, {"2026-10-02", 3}, {"2026-10-03", 4}
    };
    heatmap->set_data(heat_data);
    group5->add_child(heatmap, 1);
    row2->add_child(group5, 1);
    
    main_layout->add_child(row2, 1);
    
    auto row3 = std::make_shared<HBox>();
    row3->set_spacing(20);
    
    auto group6 = std::make_shared<GroupBox>("CodeEditor & PropertyGrid");
    auto code = std::make_shared<CodeEditor>("int main() {\n    return 0;\n}");
    
    auto prop = std::make_shared<PropertyGrid>();
    prop->set_properties({
        {"Visible", "true", PropertyItem::Type::Boolean},
        {"Color", "#FF0000", PropertyItem::Type::Color}
    });
    
    group6->add_child(code, 1);
    group6->add_child(prop, 1);
    row3->add_child(group6, 1);
    
    auto group7 = std::make_shared<GroupBox>("Carousel & GaugeCluster");
    auto carousel = std::make_shared<Carousel>();
    auto l1 = std::make_shared<Label>("Slide 1"); l1->set_alignment("center");
    auto l2 = std::make_shared<Label>("Slide 2"); l2->set_alignment("center");
    carousel->add_child(l1);
    carousel->add_child(l2);
    
    auto gauge_cluster = std::make_shared<GaugeCluster>();
    auto g1 = std::make_shared<RadialGauge>("CPU", 0, 100); g1->set_value(50);
    auto g2 = std::make_shared<RadialGauge>("RAM", 0, 100); g2->set_value(80);
    gauge_cluster->add_gauge(g1, "CPU");
    gauge_cluster->add_gauge(g2, "RAM");
    
    group7->add_child(carousel, 1);
    group7->add_child(gauge_cluster, 1);
    row3->add_child(group7, 1);
    
    main_layout->add_child(row3, 1);
    
    window.set_content(main_layout);
    
    // Timer to save screenshot after layout stabilizes
    Timer timer(500);
    timer.set_single_shot(true);
    timer.on_tick([&window]() {
        window.save_screenshot("screenshots/gallery_batch2.png");
        Application::quit();
    });
    timer.start();
    
    window.show();
    return app.run();
}
