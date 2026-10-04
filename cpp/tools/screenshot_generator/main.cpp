#include <QDir>
#include <iostream>
#include <memory>
#include <vector>

#include "simplegui/simplegui.h"

namespace {

// Creates an authentic Apple macOS window container with traffic lights and titlebar
std::shared_ptr<simplegui::Control> make_apple_window(
    const std::string& title,
    std::shared_ptr<simplegui::Control> content,
    int padding = 20) {

    auto root = std::make_shared<simplegui::VBox>();
    root->set_margins(0);
    root->set_spacing(0);
    root->set_style("background-color: #12141c;");

    // macOS Titlebar
    auto titlebar = std::make_shared<simplegui::HBox>();
    titlebar->set_margins(14, 10, 14, 10);
    titlebar->set_spacing(8);
    titlebar->set_name("apple_titlebar");
    titlebar->set_style("QWidget#apple_titlebar { background-color: #1a1d26; border-bottom: 1px solid #282c37; }");

    // Traffic Lights
    auto dot_red = std::make_shared<simplegui::Label>("");
    dot_red->set_style("background-color: #ff5f56; min-width: 12px; max-width: 12px; min-height: 12px; max-height: 12px; border-radius: 6px; border: 1px solid #e0443e;");

    auto dot_yellow = std::make_shared<simplegui::Label>("");
    dot_yellow->set_style("background-color: #ffbd2e; min-width: 12px; max-width: 12px; min-height: 12px; max-height: 12px; border-radius: 6px; border: 1px solid #dea123;");

    auto dot_green = std::make_shared<simplegui::Label>("");
    dot_green->set_style("background-color: #27c93f; min-width: 12px; max-width: 12px; min-height: 12px; max-height: 12px; border-radius: 6px; border: 1px solid #1aab29;");

    titlebar->add_child(dot_red);
    titlebar->add_child(dot_yellow);
    titlebar->add_child(dot_green);
    titlebar->add_stretch();

    auto title_lbl = std::make_shared<simplegui::Label>(title);
    title_lbl->set_style("color: #94a3b8; font-size: 11px; font-weight: 600; font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', sans-serif; letter-spacing: 0.3px; border: none; background: transparent;");
    titlebar->add_child(title_lbl);
    titlebar->add_stretch();

    auto right_spacer = std::make_shared<simplegui::HBox>();
    right_spacer->set_style("min-width: 52px; max-width: 52px; background: transparent; border: none;");
    titlebar->add_child(right_spacer);

    root->add_child(titlebar);

    // Staged Content Area
    auto body = std::make_shared<simplegui::VBox>();
    body->set_margins(padding);
    body->set_spacing(12);
    body->add_child(content);
    root->add_child(body);

    return root;
}

// Micro-label for Apple HIG section headers
std::shared_ptr<simplegui::Label> make_section_label(const std::string& text) {
    auto lbl = std::make_shared<simplegui::Label>(text);
    lbl->set_style("color: #818cf8; font-size: 10px; font-weight: 700; letter-spacing: 0.8px; text-transform: uppercase; font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', sans-serif;");
    return lbl;
}

} // namespace

int main(int argc, char* argv[]) {
    simplegui::Application app(argc, argv);
    app.set_theme("modern_dark");

    std::string out_dir = "screenshots";
    if (argc > 1) {
        out_dir = argv[1];
    }
    QDir().mkpath(QString::fromStdString(out_dir));

    auto capture_apple = [&](const std::string& name, const std::string& title, std::shared_ptr<simplegui::Control> ctrl, int w = 400, int h = 180, int padding = 20) {
        simplegui::Window win(title, w, h);
        auto framed = make_apple_window(title, ctrl, padding);
        win.set_content(framed);
        std::string filepath = out_dir + "/" + name + ".png";
        win.save_screenshot(filepath);
        std::cout << "Saved: " << filepath << std::endl;
    };

    std::cout << "Generating SimpleGUI screenshots..." << std::endl;

    // =========================================================================
    // CONTROL GALLERIES - several related controls per screenshot
    // =========================================================================

    // A small captioned "tile": the control's class name above the control.
    auto tile = [](const std::string& name, std::shared_ptr<simplegui::Control> ctrl, int stretch = 0) {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(6);
        box->add_child(make_section_label(name));
        box->add_child(ctrl, stretch);
        if (stretch == 0) box->add_stretch(1); // keep captions top-aligned
        return box;
    };
    // A row of tiles that share the width equally.
    auto row_of = [](std::initializer_list<std::shared_ptr<simplegui::Control>> items) {
        auto row = std::make_shared<simplegui::HBox>();
        row->set_spacing(20);
        for (const auto& item : items) row->add_child(item, 1);
        return row;
    };
    // A colored block used to visualise how layouts arrange children.
    auto block = [](const std::string& text, const std::string& color) {
        auto lbl = std::make_shared<simplegui::Label>(text);
        lbl->set_alignment("center");
        lbl->set_style("background-color: " + color + "; color: #ffffff; font-weight: 600; border-radius: 5px; padding: 6px;");
        return lbl;
    };
    const std::string icon = "resources/icon.png";

    // 1. Text & buttons ------------------------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(18);

        auto image_btn = std::make_shared<simplegui::ImageButton>(icon, "Open");
        image_btn->set_icon_size(20);
        image_btn->set_text("Open");
        root->add_child(row_of({
            tile("Label", std::make_shared<simplegui::Label>("Plain text label")),
            tile("Button", std::make_shared<simplegui::Button>("Save")),
            tile("NeonButton", std::make_shared<simplegui::NeonButton>("ENGAGE")),
            tile("ImageButton", image_btn),
            tile("Link", std::make_shared<simplegui::Link>("Qt documentation", "https://doc.qt.io")),
        }));

        auto text = std::make_shared<simplegui::TextInput>("Your name");
        text->set_text("Ada Lovelace");
        auto pass = std::make_shared<simplegui::PasswordInput>("Password");
        pass->set_text("secret123");
        auto search = std::make_shared<simplegui::SearchField>();
        search->set_text("invoices");
        root->add_child(row_of({
            tile("TextInput", text),
            tile("PasswordInput", pass),
            tile("SearchField", search),
            tile("MaskedInput", std::make_shared<simplegui::MaskedInput>("(999) 999-9999", "5551234567")),
        }));

        auto memo = std::make_shared<simplegui::Textarea>("Multi-line text.\nPress Enter for a new line.\nGreat for notes.");
        memo->set_height(90);
        auto tokens = std::make_shared<simplegui::TokenField>();
        tokens->set_tokens({"design", "qt6", "c++"});
        root->add_child(row_of({tile("Textarea", memo), tile("TokenField", tokens)}));

        capture_apple("gallery_text_buttons", "Text & Buttons", root, 980, 400);
    }

    // 2. Choices ------------------------------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(18);

        auto checks = std::make_shared<simplegui::VBox>();
        checks->add_child(std::make_shared<simplegui::Checkbox>("Auto-save", true));
        checks->add_child(std::make_shared<simplegui::Checkbox>("Spell check", false));
        auto radios = std::make_shared<simplegui::VBox>();
        radios->add_child(std::make_shared<simplegui::Radio>("Small", false));
        radios->add_child(std::make_shared<simplegui::Radio>("Large", true));
        auto switches = std::make_shared<simplegui::VBox>();
        switches->add_child(std::make_shared<simplegui::ToggleSwitch>(true, "Wi-Fi"));
        switches->add_child(std::make_shared<simplegui::ToggleSwitch>(false, "Bluetooth"));
        root->add_child(row_of({tile("Checkbox", checks), tile("Radio", radios), tile("ToggleSwitch", switches)}));

        auto drop = std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"English", "Deutsch", "Espa\xC3\xB1ol"});
        auto combo = std::make_shared<simplegui::ComboBox>(std::vector<std::string>{"Helvetica", "Inter", "Roboto"});
        combo->set_text("Inter");
        auto seg = std::make_shared<simplegui::SegmentedControl>(std::vector<std::string>{"Day", "Week", "Month"}, 1);
        root->add_child(row_of({tile("Dropdown", drop), tile("ComboBox", combo), tile("SegmentedControl", seg)}));

        auto list = std::make_shared<simplegui::ListBox>(
            std::vector<std::string>{"Apples", "Bananas", "Cherries", "Dates"});
        list->set_selected_index(1);
        list->set_height(110);
        root->add_child(tile("ListBox", list));

        capture_apple("gallery_choices", "Choices", root, 760, 440);
    }

    // 3. Numbers, dates & colors -------------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(18);

        auto knob = std::make_shared<simplegui::Knob>(0, 100, 65);
        knob->set_size(70, 70);
        auto number = std::make_shared<simplegui::NumberInput>(0, 500, 42);
        number->set_suffix(" px");
        root->add_child(row_of({
            tile("Slider", std::make_shared<simplegui::Slider>(0, 100, 70)),
            tile("Knob", knob),
            tile("NumberInput", number),
            tile("ColorWell", std::make_shared<simplegui::ColorWell>("#0a84ff")),
        }));

        auto rating = std::make_shared<simplegui::Rating>(5);
        rating->set_rating(4);
        root->add_child(row_of({
            tile("DatePicker", std::make_shared<simplegui::DatePicker>("2026-10-31")),
            tile("DateRangePicker", std::make_shared<simplegui::DateRangePicker>("2026-10-01", "2026-10-31")),
        }));
        root->add_child(row_of({tile("Rating", rating), tile("FeedbackMood", std::make_shared<simplegui::FeedbackMood>(4))}));

        capture_apple("gallery_numbers_dates", "Numbers, Dates & Colors", root, 820, 340);
    }

    // 4. Progress & status ---------------------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(18);

        auto bar = std::make_shared<simplegui::ProgressIndicator>(0, 100, 68);
        auto ring = std::make_shared<simplegui::CircularProgress>();
        ring->set_value(72);
        auto pills = std::make_shared<simplegui::HBox>();
        pills->add_child(std::make_shared<simplegui::StatusPill>("LIVE", "#10b981"));
        pills->add_child(std::make_shared<simplegui::StatusPill>("DEGRADED", "#f59e0b"));
        pills->add_stretch(1);
        root->add_child(row_of({tile("ProgressIndicator", bar), tile("CircularProgress", ring), tile("StatusPill", pills)}));

        auto leds = std::make_shared<simplegui::VBox>();
        auto led1 = std::make_shared<simplegui::LedIndicator>("#22c55e", true);
        led1->set_label("Online");
        auto led2 = std::make_shared<simplegui::LedIndicator>("#ef4444", false);
        led2->set_label("Fault");
        leds->add_child(led1);
        leds->add_child(led2);
        auto gauge = std::make_shared<simplegui::RadialGauge>("CPU", 0, 100);
        gauge->set_animated(false);
        gauge->set_units("%");
        gauge->set_value(64);
        gauge->set_min_size(150, 150);
        auto vfd = std::make_shared<simplegui::VfdMeter>(20, false);
        vfd->set_value(72);
        root->add_child(row_of({tile("LedIndicator", leds), tile("RadialGauge", gauge), tile("VfdMeter", vfd)}));

        capture_apple("gallery_progress_status", "Progress & Status", root, 820, 380);
    }

    // 5. Futuristic controls (neon theme) -----------------------------------
    {
        app.set_theme("neon");
        auto root = std::make_shared<simplegui::HBox>();
        root->set_spacing(16);

        auto left = std::make_shared<simplegui::GlassPanel>("GlassPanel");
        auto rpm = std::make_shared<simplegui::RadialGauge>("REACTOR", 0, 100);
        rpm->set_animated(false);
        rpm->set_units("%");
        rpm->set_thresholds(75, 90);
        rpm->set_value(82);
        rpm->set_min_size(170, 170);
        left->add_child(make_section_label("RadialGauge"));
        left->add_child(rpm, 1);
        left->add_child(make_section_label("ToggleSwitch"));
        left->add_child(std::make_shared<simplegui::ToggleSwitch>(true, "Shields"));
        left->add_child(make_section_label("LedIndicator"));
        auto led = std::make_shared<simplegui::LedIndicator>("#00e5ff", true);
        led->set_label("Uplink");
        left->add_child(led);

        auto middle = std::make_shared<simplegui::VBox>();
        middle->set_spacing(8);
        auto radar = std::make_shared<simplegui::RadarScope>();
        radar->stop();
        radar->add_blip(300, 0.6);
        radar->add_blip(160, 0.35, "#ff2d75");
        radar->set_min_size(240, 240);
        middle->add_child(make_section_label("RadarScope"));
        middle->add_child(radar, 1);
        middle->add_child(make_section_label("SegmentedControl"));
        middle->add_child(std::make_shared<simplegui::SegmentedControl>(
            std::vector<std::string>{"Cruise", "Scan", "Combat"}, 1));

        auto right = std::make_shared<simplegui::VBox>();
        right->set_spacing(8);
        auto term = std::make_shared<simplegui::TerminalView>();
        term->print_line("SHIP COMPUTER ONLINE");
        term->print_line("> scan", "#94a3b8");
        term->print_line("2 contacts detected.");
        term->print_line("WARNING: hull at 82%", "#f59e0b");
        right->add_child(make_section_label("TerminalView"));
        right->add_child(term, 1);
        right->add_child(make_section_label("NeonButton"));
        auto buttons = std::make_shared<simplegui::HBox>();
        buttons->add_child(std::make_shared<simplegui::NeonButton>("ENGAGE"), 1);
        buttons->add_child(std::make_shared<simplegui::NeonButton>("ABORT", "#ff2d75"), 1);
        right->add_child(buttons);

        root->add_child(left, 1);
        root->add_child(middle, 1);
        root->add_child(right, 1);
        capture_apple("gallery_futuristic", "Futuristic Controls (neon theme)", root, 1000, 440);
        app.set_theme("modern_dark");
    }

    // 6. Layouts -------------------------------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(18);

        auto vbox = std::make_shared<simplegui::VBox>();
        vbox->add_child(block("1", "#0a84ff"));
        vbox->add_child(block("2", "#5e5ce6"));
        vbox->add_child(block("3", "#bf5af2"));
        auto hbox = std::make_shared<simplegui::HBox>();
        hbox->add_child(block("1", "#0a84ff"), 1);
        hbox->add_child(block("2", "#5e5ce6"), 1);
        hbox->add_child(block("3", "#bf5af2"), 1);
        auto group = std::make_shared<simplegui::GroupBox>("Account");
        group->add_child(std::make_shared<simplegui::Checkbox>("Remember me", true));
        group->add_child(std::make_shared<simplegui::Button>("Sign out"));
        root->add_child(row_of({tile("VBox (Column)", vbox), tile("HBox (Row)", hbox), tile("GroupBox", group)}));

        auto tabs = std::make_shared<simplegui::TabView>();
        auto page = std::make_shared<simplegui::VBox>();
        page->add_child(std::make_shared<simplegui::Label>("General settings page"));
        page->add_stretch(1);
        tabs->add_tab("General", page);
        tabs->add_tab("Advanced", std::make_shared<simplegui::VBox>());
        tabs->set_height(110);
        auto split = std::make_shared<simplegui::SplitView>(true);
        split->add_child(block("Sidebar", "#30d158"));
        split->add_child(block("Content", "#0a84ff"));
        split->set_sizes(90, 180);
        split->set_height(110);
        auto scroll_content = std::make_shared<simplegui::VBox>();
        for (int i = 1; i <= 8; ++i) scroll_content->add_child(std::make_shared<simplegui::Label>("Row " + std::to_string(i)));
        auto scroll = std::make_shared<simplegui::ScrollView>();
        scroll->set_content(scroll_content);
        scroll->set_height(110);
        root->add_child(row_of({tile("TabView", tabs), tile("SplitView", split), tile("ScrollView", scroll)}));

        capture_apple("gallery_layouts", "Layouts", root, 900, 400);
    }

    // 7. Pictures, drawing & tables -----------------------------------------
    {
        auto root = std::make_shared<simplegui::HBox>();
        root->set_spacing(20);

        auto picture = std::make_shared<simplegui::Image>(icon);
        picture->set_scaled(true);
        picture->set_size(120, 120);

        auto canvas = std::make_shared<simplegui::Canvas>(260, 180);
        canvas->clear("#0f172a");
        canvas->fill_rounded_rect(14, 14, 110, 70, 10, "#0a84ff");
        canvas->fill_circle(190, 60, 38, "#ff9f0a");
        canvas->draw_line(14, 160, 246, 110, "#30d158", 3);
        canvas->draw_ellipse(120, 100, 120, 60, "#bf5af2", 2);
        canvas->draw_text(24, 56, "Canvas", "#ffffff", 16);

        auto grid = std::make_shared<simplegui::Grid>(0, 3, std::vector<std::string>{"Name", "Role", "City"});
        grid->add_row({"Ada", "Engineer", "London"});
        grid->add_row({"Linus", "Maintainer", "Portland"});
        grid->add_row({"Grace", "Admiral", "Arlington"});
        grid->set_selected_row(1);

        root->add_child(tile("Image", picture));
        root->add_child(tile("Canvas", canvas), 1);
        root->add_child(tile("Grid", grid, 1), 1);
        capture_apple("gallery_pictures_tables", "Pictures, Drawing & Tables", root, 940, 300);
    }

    // 8. Charts --------------------------------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(14);

        auto bars = std::make_shared<simplegui::BarChart>("BarChart");
        bars->add_bar("Mon", 12);
        bars->add_bar("Tue", 19);
        bars->add_bar("Wed", 15);
        bars->add_bar("Thu", 24);
        bars->add_bar("Fri", 21);
        auto lines = std::make_shared<simplegui::LineChart>("LineChart");
        lines->set_x_labels({"Q1", "Q2", "Q3", "Q4"});
        lines->add_series("Revenue", {40, 65, 58, 90}, "#0a84ff");
        lines->add_series("Costs", {30, 35, 42, 48}, "#30d158");
        auto pie = std::make_shared<simplegui::PieChart>("PieChart");
        pie->add_slice("Desktop", 55);
        pie->add_slice("Mobile", 35);
        pie->add_slice("Tablet", 10);
        auto top = std::make_shared<simplegui::HBox>();
        top->set_spacing(14);
        top->add_child(bars, 1);
        top->add_child(lines, 1);
        top->add_child(pie, 1);

        auto donut = std::make_shared<simplegui::DonutChart>("72%", "USED");
        donut->add_segment("Apps", 40, "#0a84ff");
        donut->add_segment("Photos", 32, "#bf5af2");
        donut->add_segment("Free", 28, "#2c303c");
        auto radar = std::make_shared<simplegui::RadarChart>("RadarChart");
        radar->set_dimensions({"Speed", "Power", "Range", "Comfort", "Safety"});
        radar->add_dataset("Model A", {80, 70, 90, 60, 85}, "#0a84ff");
        radar->add_dataset("Model B", {60, 90, 70, 80, 75}, "#ff9f0a");
        auto candles = std::make_shared<simplegui::CandlestickChart>("CandlestickChart");
        candles->add_candle("Mon", 100, 112, 96, 108);
        candles->add_candle("Tue", 108, 115, 101, 103);
        candles->add_candle("Wed", 103, 118, 102, 116);
        candles->add_candle("Thu", 116, 120, 107, 110);
        candles->add_candle("Fri", 110, 125, 109, 122);
        auto bottom = std::make_shared<simplegui::HBox>();
        bottom->set_spacing(14);
        bottom->add_child(tile("DonutChart", donut, 1), 1);
        bottom->add_child(radar, 1);
        bottom->add_child(candles, 1);

        root->add_child(top, 1);
        root->add_child(bottom, 1);
        capture_apple("gallery_charts", "Charts", root, 1040, 620);
    }

    // 9. Telemetry widgets ----------------------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(18);

        auto spark = std::make_shared<simplegui::Sparkline>("#06b6d4");
        spark->set_max_samples(13);
        spark->set_samples({12, 18, 15, 22, 30, 26, 34, 28, 40, 36, 44, 39, 48});
        spark->set_height(60);
        auto comp = std::make_shared<simplegui::CompositionBar>();
        comp->add_segment("Apps", 45, "#0a84ff");
        comp->add_segment("Wired", 20, "#ff9f0a");
        comp->add_segment("Compressed", 15, "#bf5af2");
        comp->add_segment("Free", 20, "#2c303c");
        root->add_child(row_of({tile("Sparkline", spark), tile("CompositionBar", comp)}));

        auto heat = std::make_shared<simplegui::ActivityHeatmap>(30, 7);
        for (int w = 0; w < 30; ++w)
            for (int d = 0; d < 7; ++d) heat->set_cell(w, d, (w * 7 + d * 3 + w * d) % 5);
        auto card = std::make_shared<simplegui::StatCard>("CPU", "42%", "8 cores", "#10b981");
        root->add_child(row_of({tile("ActivityHeatmap", heat), tile("StatCard", card)}));

        auto stats = std::make_shared<simplegui::StatGrid>();
        stats->add_stat("Users", "28,492", "+14.8%");
        stats->add_stat("Revenue", "$342k", "+9.2%");
        stats->add_stat("Latency", "14 ms", "+3.1%", false);
        root->add_child(tile("StatGrid", stats));

        capture_apple("gallery_telemetry", "Telemetry Widgets", root, 900, 470);
    }

    // 10. App & dashboard widgets ------------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(16);

        root->add_child(tile("Breadcrumbs", std::make_shared<simplegui::Breadcrumbs>(
            std::vector<std::string>{"Home", "Projects", "Report"})));

        auto nav = std::make_shared<simplegui::NavRail>();
        nav->add_item("home", "\xF0\x9F\x8F\xA0", "Home");
        nav->add_item("mail", "\xE2\x9C\x89", "Mail", 3);
        nav->add_item("settings", "\xE2\x9A\x99", "Settings");
        auto product = std::make_shared<simplegui::ProductCard>(
            "Studio Headphones", "Wireless, noise cancelling, 30h battery.", "$249", "NEW", 4.8);
        auto profile = std::make_shared<simplegui::UserProfileCard>(
            "Ada Lovelace", "@ada", "Engineer", "Writes the first programs.", true, "Connect");
        auto player = std::make_shared<simplegui::MediaPlayer>("Midnight City", "M83", "4:03");
        player->set_position(95);
        auto middle = std::make_shared<simplegui::HBox>();
        middle->set_spacing(16);
        middle->add_child(tile("NavRail", nav));
        middle->add_child(tile("ProductCard", product), 1);
        middle->add_child(tile("UserProfileCard", profile), 1);
        middle->add_child(tile("MediaPlayer", player), 1);
        root->add_child(middle);

        auto kanban = std::make_shared<simplegui::KanbanBoard>();
        kanban->add_column("todo", "To Do");
        kanban->add_column("doing", "In Progress");
        kanban->add_column("done", "Done");
        kanban->add_card("todo", "t1", "Write docs", "DOCS");
        kanban->add_card("doing", "t2", "Neon theme", "UI", "Futuristic look");
        kanban->add_card("done", "t3", "Smoke tests", "QA");
        root->add_child(tile("KanbanBoard", kanban, 1), 1);

        capture_apple("gallery_app_widgets", "App & Dashboard Widgets", root, 1180, 680);
    }

    // =========================================================================
    // SHOWCASE APPLICATIONS
    // =========================================================================

    // System Monitor
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(12);

        auto header = std::make_shared<simplegui::HBox>();
        header->set_spacing(10);
        auto title = std::make_shared<simplegui::Label>("ACTIVITY MONITOR / SILICON TELEMETRY");
        title->set_style("font-weight: bold; font-size: 13px; letter-spacing: 0.8px; color: #f4f4f5;");
        auto pill_live = std::make_shared<simplegui::StatusPill>("LIVE 1000 HZ", "#10b981");
        auto pill_sensors = std::make_shared<simplegui::StatusPill>("SENSORS ONLINE", "#06b6d4");
        header->add_child(title);
        header->add_child(pill_live);
        header->add_stretch();
        header->add_child(pill_sensors);

        auto stat_row = std::make_shared<simplegui::HBox>();
        stat_row->set_spacing(10);
        stat_row->add_child(std::make_shared<simplegui::StatCard>("M3 MAX CPU", "34.8%", "16 Cores · 4.05 GHz", "#06b6d4"));
        stat_row->add_child(std::make_shared<simplegui::StatCard>("UNIFIED MEMORY", "14.2 GB", "64 GB Total (22%)", "#10b981"));
        stat_row->add_child(std::make_shared<simplegui::StatCard>("SSD READ / WRITE", "1.42 GB/s", "APFS NVMe Gen4", "#f59e0b"));
        stat_row->add_child(std::make_shared<simplegui::StatCard>("PACKAGE POWER", "24.5 W", "Efficiency High", "#ec4899"));

        auto mid_row = std::make_shared<simplegui::HBox>();
        mid_row->set_spacing(12);

        auto grp_graph = std::make_shared<simplegui::GroupBox>("Utilization History (Rolling 60s)");
        auto graph_vbox = std::make_shared<simplegui::VBox>();
        graph_vbox->set_margins(12);
        graph_vbox->set_spacing(10);

        auto sparkline = std::make_shared<simplegui::Sparkline>("#06b6d4");
        sparkline->set_range(0.0, 100.0);
        sparkline->set_samples({18, 22, 25, 20, 19, 35, 48, 62, 58, 45, 30, 28, 32, 40, 55, 78, 85, 92, 70, 52, 41, 38, 35, 33, 30, 36, 42, 50, 48, 35});

        auto comp_label = std::make_shared<simplegui::Label>("Memory: Apps (14GB) | Wired (6GB) | Cache (8GB) | Free (36GB)");
        comp_label->set_style("color: #a1a1aa; font-size: 11px;");

        auto comp_bar = std::make_shared<simplegui::CompositionBar>();
        comp_bar->add_segment("Apps", 14.0, "#0a84ff");
        comp_bar->add_segment("Wired", 6.0, "#06b6d4");
        comp_bar->add_segment("Cached", 8.0, "#f59e0b");
        comp_bar->add_segment("Free", 36.0, "#272a34");

        graph_vbox->add_child(sparkline);
        graph_vbox->add_child(comp_label);
        graph_vbox->add_child(comp_bar);
        grp_graph->add_child(graph_vbox);

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

        auto grp_procs = std::make_shared<simplegui::GroupBox>("Top Resource Processes");
        auto procs_vbox = std::make_shared<simplegui::VBox>();
        procs_vbox->set_margins(10);

        auto grid = std::make_shared<simplegui::Grid>(5, 5, std::vector<std::string>{"PID", "Process Name", "CPU %", "Memory", "Energy"});
        const std::vector<std::vector<std::string>> proc_data = {
            {"1082", "clang++ (EasyQt6 Native Build)", "68.4%", "1.24 GB", "Very High"},
            {"429", "WindowServer (Metal 3 Pipeline)", "12.8%", "680 MB", "Medium"},
            {"8891", "QtWebEngineProcess", "8.2%", "512 MB", "Low"},
            {"1", "launchd (macOS init)", "0.1%", "28 MB", "Very Low"},
            {"7420", "easyqt6_telemetry_agent", "0.4%", "18 MB", "Very Low"}
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

        capture_apple("example_system_monitor", "Activity Monitor", root, 860, 740);
    }

    // Analytics Dashboard
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(12);

        auto header = std::make_shared<simplegui::HBox>();
        header->set_spacing(12);
        auto title = std::make_shared<simplegui::Label>("Enterprise Analytics Cloud");
        title->set_style("font-size: 18px; font-weight: bold; color: #f8fafc;");
        header->add_child(title);
        auto live_pill = std::make_shared<simplegui::StatusPill>("REALTIME STREAM", "#10b981");
        header->add_child(live_pill);
        header->add_stretch();
        auto date_range = std::make_shared<simplegui::DateRangePicker>("2026-10-01", "2026-10-31");
        header->add_child(date_range);
        auto export_btn = std::make_shared<simplegui::Button>("Export Report");
        export_btn->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 6px 14px; border-radius: 6px;");
        header->add_child(export_btn);
        root->add_child(header);

        auto stats = std::make_shared<simplegui::StatGrid>();
        stats->add_stat("Active Connections", "28,492", "+14.8%", true);
        stats->add_stat("Monthly Recurring", "$342,100", "+9.2%", true);
        stats->add_stat("Avg Latency (p99)", "14.2 ms", "-6.5%", true);
        stats->add_stat("Node Health", "99.98%", "Optimal", true);
        root->add_child(stats);

        auto mid_row = std::make_shared<simplegui::HBox>();
        mid_row->set_spacing(12);
        auto line_chart = std::make_shared<simplegui::LineChart>("Network Bandwidth (Gbps)");
        line_chart->set_x_labels({"00:00", "04:00", "08:00", "12:00", "16:00", "20:00", "24:00"});
        line_chart->add_series("Inbound", {24, 32, 68, 120, 145, 110, 48}, "#0a84ff", true);
        line_chart->add_series("Outbound", {18, 22, 45, 85, 96, 72, 35}, "#10b981", true);
        mid_row->add_child(line_chart);

        auto donut = std::make_shared<simplegui::DonutChart>("84%", "CAPACITY");
        donut->add_segment("Database", 40.0, "#0a84ff");
        donut->add_segment("Application", 26.0, "#8b5cf6");
        donut->add_segment("Caching", 18.0, "#06b6d4");
        donut->add_segment("Headroom", 16.0, "#2c303c");
        mid_row->add_child(donut);
        root->add_child(mid_row);

        auto bot_row = std::make_shared<simplegui::HBox>();
        bot_row->set_spacing(12);
        auto bar_chart = std::make_shared<simplegui::BarChart>("Monthly Revenue Growth ($k)");
        bar_chart->add_bar("May", 140.0, "#0a84ff");
        bar_chart->add_bar("Jun", 175.0, "#0a84ff");
        bar_chart->add_bar("Jul", 210.0, "#06b6d4");
        bar_chart->add_bar("Aug", 245.0, "#0a84ff");
        bar_chart->add_bar("Sep", 290.0, "#10b981");
        bar_chart->add_bar("Oct", 342.0, "#10b981");
        bot_row->add_child(bar_chart);

        auto radar = std::make_shared<simplegui::RadarChart>("Infrastructure Benchmark");
        radar->set_dimensions({"Throughput", "Latency", "Resilience", "Security", "Scale", "UX"});
        radar->add_dataset("Production", {92, 88, 96, 84, 90, 94}, "#0a84ff");
        radar->add_dataset("Benchmark", {75, 70, 80, 85, 65, 70}, "#f59e0b");
        bot_row->add_child(radar);
        root->add_child(bot_row);

        capture_apple("example_analytics_dashboard", "Enterprise Cloud Analytics", root, 960, 660);
    }

    // 11. New Controls (v0.2 additions) ---------------------------------------
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(18);

        auto time = std::make_shared<simplegui::TimePicker>();
        time->set_time(14, 30);
        
        auto cal = std::make_shared<simplegui::CalendarView>();
        cal->set_min_size(240, 180);
        
        root->add_child(row_of({tile("TimePicker", time), tile("CalendarView", cal)}));
        
        auto rtf = std::make_shared<simplegui::RichTextEditor>();
        rtf->set_html("<h1>Welcome</h1><p>This is <b>bold</b> and <i>italic</i> text.</p>");
        rtf->set_min_size(300, 100);
        
        auto form = std::make_shared<simplegui::FormLayout>();
        form->add_row("Email address:", std::make_shared<simplegui::TextInput>("user@example.com"));
        form->add_row("Password:", std::make_shared<simplegui::PasswordInput>());
        form->add_row(std::make_shared<simplegui::Button>("Login"));
        
        root->add_child(row_of({tile("RichTextEditor", rtf), tile("FormLayout", form)}));

        capture_apple("gallery_new_controls", "New Controls", root, 800, 480);
    }

    std::cout << "All screenshots generated successfully!" << std::endl;
    return 0;
}
