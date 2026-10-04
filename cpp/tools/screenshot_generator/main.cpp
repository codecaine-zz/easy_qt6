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

// Subtitle label for contextual descriptions
std::shared_ptr<simplegui::Label> make_desc_label(const std::string& text) {
    auto lbl = std::make_shared<simplegui::Label>(text);
    lbl->set_style("color: #64748b; font-size: 11px; font-family: -apple-system, BlinkMacSystemFont, 'SF Pro Text', sans-serif;");
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

    std::cout << "Generating Apple HIG production screenshots..." << std::endl;

    // =========================================================================
    // 1. BASIC CONTROLS (Staged in Apple HIG UI Contexts)
    // =========================================================================

    // Button: Apple Modal Action Bar
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("CONFIRM ACTION"));
        auto msg = std::make_shared<simplegui::Label>("Do you want to save the changes made to 'Document.swift'?");
        msg->set_style("color: #f1f5f9; font-size: 13px; font-weight: 500;");
        box->add_child(msg);
        box->add_child(make_desc_label("Your changes will be lost if you don't save them."));

        auto btn_row = std::make_shared<simplegui::HBox>();
        btn_row->set_spacing(10);
        auto cancel_btn = std::make_shared<simplegui::Button>("Don't Save");
        cancel_btn->set_style("background-color: #272a34; color: #e2e8f0; font-weight: 500; padding: 7px 16px; border-radius: 6px; border: 1px solid #383e4d;");
        auto save_btn = std::make_shared<simplegui::Button>("Save Changes");
        save_btn->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 7px 20px; border-radius: 6px; border: 1px solid #0071e3;");
        btn_row->add_stretch();
        btn_row->add_child(cancel_btn);
        btn_row->add_child(save_btn);
        box->add_child(btn_row);

        capture_apple("button", "Save Document", box, 420, 195);
    }

    // Label: Apple Typography Scale
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("SYSTEM TYPOGRAPHY"));
        auto h1 = std::make_shared<simplegui::Label>("Display & Brightness");
        h1->set_style("color: #f8fafc; font-size: 17px; font-weight: 700;");
        auto body = std::make_shared<simplegui::Label>("Automatically adapt your display based on ambient lighting conditions.");
        body->set_style("color: #94a3b8; font-size: 12px; line-height: 1.4;");
        box->add_child(h1);
        box->add_child(body);
        box->add_child(make_desc_label("macOS Sequoia 15.1 · Native San Francisco Typography"));
        capture_apple("label", "Typography", box, 420, 175);
    }

    // TextInput: Developer Account
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("DEVELOPER ACCOUNT"));
        auto input = std::make_shared<simplegui::TextInput>("developer@apple.com");
        box->add_child(input);
        box->add_child(make_desc_label("Enter the Apple Account associated with your team."));
        capture_apple("text_input", "Account Preferences", box, 420, 170);
    }

    // PasswordInput: Security & Passkeys
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("ENCRYPTION KEY"));
        auto input = std::make_shared<simplegui::PasswordInput>("super_secret_master_token_2026");
        box->add_child(input);
        auto badge_row = std::make_shared<simplegui::HBox>();
        badge_row->set_spacing(8);
        auto pill = std::make_shared<simplegui::StatusPill>("256-BIT AES ENCRYPTED", "#10b981");
        badge_row->add_child(pill);
        badge_row->add_stretch();
        badge_row->add_child(make_desc_label("Stored securely in Keychain"));
        box->add_child(badge_row);
        capture_apple("password_input", "Security Preferences", box, 420, 180);
    }

    // SearchField: macOS Spotlight
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("SPOTLIGHT SEARCH"));
        auto input = std::make_shared<simplegui::SearchField>("Search documents, preferences, and code...");
        box->add_child(input);
        box->add_child(make_desc_label("Press ⌘ Space to open Spotlight search anywhere."));
        capture_apple("search_field", "Spotlight", box, 440, 170);
    }

    // Checkbox: Software Update Preferences
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("AUTOMATIC UPDATES"));
        box->add_child(std::make_shared<simplegui::Checkbox>("Check for updates automatically", true));
        box->add_child(std::make_shared<simplegui::Checkbox>("Download new updates when available", true));
        box->add_child(std::make_shared<simplegui::Checkbox>("Install macOS updates automatically", false));
        box->add_child(make_desc_label("Security responses and system files are updated automatically."));
        capture_apple("checkbox", "Software Update", box, 440, 220);
    }

    // Radio: Appearance Mode
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("APPEARANCE THEME"));
        box->add_child(std::make_shared<simplegui::Radio>("Light Mode"));
        box->add_child(std::make_shared<simplegui::Radio>("Dark Mode"));
        box->add_child(std::make_shared<simplegui::Radio>("Auto (Adjust according to Daylight)"));
        box->add_child(make_desc_label("Automatically adapts interface appearance based on schedule."));
        capture_apple("radio", "Appearance", box, 420, 220);
    }

    // Slider: Sound Output Volume
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("OUTPUT VOLUME"));
        auto slider_row = std::make_shared<simplegui::HBox>();
        slider_row->set_spacing(12);
        auto min_lbl = std::make_shared<simplegui::Label>("🔈");
        auto max_lbl = std::make_shared<simplegui::Label>("🔊");
        auto slider = std::make_shared<simplegui::Slider>(0, 100, 72);
        auto pill = std::make_shared<simplegui::StatusPill>("72%", "#0a84ff");
        slider_row->add_child(min_lbl);
        slider_row->add_child(slider);
        slider_row->add_child(max_lbl);
        slider_row->add_child(pill);
        box->add_child(slider_row);
        box->add_child(make_desc_label("MacBook Pro Speakers (16-inch, 2026)"));
        capture_apple("slider", "Sound Output", box, 440, 175);
    }

    // Dropdown: Display Profile
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("COLOR PROFILE"));
        auto dropdown = std::make_shared<simplegui::Dropdown>(std::vector<std::string>{
            "Display P3 (Wide Gamut)", "sRGB IEC61966-2.1", "Adobe RGB (1998)", "Apple XDR Display (P3-1600 nits)"
        });
        box->add_child(dropdown);
        box->add_child(make_desc_label("Factory calibrated color profile for Liquid Retina XDR."));
        capture_apple("dropdown", "Display Preferences", box, 420, 170);
    }

    // ComboBox: Time Zone
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("PRIMARY TIME ZONE"));
        auto combo = std::make_shared<simplegui::ComboBox>(std::vector<std::string>{
            "Cupertino, California (PST)", "New York, USA (EST)", "London, United Kingdom (GMT)", "Tokyo, Japan (JST)"
        });
        box->add_child(combo);
        box->add_child(make_desc_label("Closest city determines your primary time zone."));
        capture_apple("combo_box", "Date & Time", box, 420, 170);
    }

    // NumberInput: Buffer Size
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("AUDIO BUFFER SIZE (SAMPLES)"));
        auto num = std::make_shared<simplegui::NumberInput>(64, 4096, 256);
        box->add_child(num);
        box->add_child(make_desc_label("Lower buffer size reduces round-trip latency for live monitoring."));
        capture_apple("number_input", "Logic Pro Preferences", box, 400, 170);
    }

    // Knob: Studio Gain Control
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("STUDIO MASTER GAIN"));
        auto row = std::make_shared<simplegui::HBox>();
        row->set_spacing(16);
        auto knob = std::make_shared<simplegui::Knob>(0, 100, 68);
        auto info = std::make_shared<simplegui::VBox>();
        info->set_spacing(4);
        auto val_lbl = std::make_shared<simplegui::Label>("+3.2 dB");
        val_lbl->set_style("color: #f8fafc; font-size: 20px; font-weight: 700;");
        info->add_child(val_lbl);
        info->add_child(make_desc_label("Reference headroom: -14 LUFS"));
        auto pill = std::make_shared<simplegui::StatusPill>("ZERO CLIPPING", "#10b981");
        info->add_child(pill);
        row->add_child(knob);
        row->add_child(info);
        box->add_child(row);
        capture_apple("knob", "Mastering Gain", box, 360, 210);
    }

    // DatePicker: Event Calendar
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("SCHEDULE RELEASE DATE"));
        auto dp = std::make_shared<simplegui::DatePicker>();
        box->add_child(dp);
        box->add_child(make_desc_label("Scheduled deployment date for production release."));
        capture_apple("date_picker", "Release Calendar", box, 400, 175);
    }

    // ColorWell: Accent Color
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("ACCENT COLOR"));
        auto row = std::make_shared<simplegui::HBox>();
        row->set_spacing(12);
        auto cw = std::make_shared<simplegui::ColorWell>("#0a84ff");
        auto name_lbl = std::make_shared<simplegui::Label>("Apple System Blue (#0A84FF)");
        name_lbl->set_style("color: #f1f5f9; font-size: 13px; font-weight: 500;");
        row->add_child(cw);
        row->add_child(name_lbl);
        box->add_child(row);
        box->add_child(make_desc_label("Applies to buttons, selection highlights, and active controls."));
        capture_apple("color_well", "Accent Color", box, 400, 170);
    }

    // ProgressIndicator: macOS Sequoia Update
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("DOWNLOADING MACOS SEQUOIA"));
        auto title = std::make_shared<simplegui::Label>("macOS Sequoia 15.1 · 3.42 GB of 4.80 GB");
        title->set_style("color: #f8fafc; font-size: 13px; font-weight: 600;");
        box->add_child(title);
        auto progress = std::make_shared<simplegui::ProgressIndicator>();
        progress->set_value(71);
        box->add_child(progress);
        box->add_child(make_desc_label("About 2 minutes remaining · High-speed connection"));
        capture_apple("progress_indicator", "Software Update", box, 440, 175);
    }

    // CircularProgress: Activity Ring
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("ACTIVITY GOAL"));
        auto row = std::make_shared<simplegui::HBox>();
        row->set_spacing(16);
        auto circ = std::make_shared<simplegui::CircularProgress>();
        circ->set_value(78);
        auto info = std::make_shared<simplegui::VBox>();
        info->set_spacing(4);
        auto val = std::make_shared<simplegui::Label>("78% Done");
        val->set_style("color: #f8fafc; font-size: 18px; font-weight: 700;");
        info->add_child(val);
        info->add_child(make_desc_label("610 of 780 kcal Move Goal"));
        auto pill = std::make_shared<simplegui::StatusPill>("ON PACE TODAY", "#10b981");
        info->add_child(pill);
        row->add_child(circ);
        row->add_child(info);
        box->add_child(row);
        capture_apple("circular_progress", "Fitness & Activity", box, 360, 200);
    }

    // Rating: App Store Review
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("APP STORE RATING"));
        auto rtg = std::make_shared<simplegui::Rating>(5);
        rtg->set_rating(5);
        box->add_child(rtg);
        auto stats = std::make_shared<simplegui::Label>("4.9 ★★★★★ · 28,420 Ratings");
        stats->set_style("color: #f1f5f9; font-size: 13px; font-weight: 600;");
        box->add_child(stats);
        box->add_child(make_desc_label("#1 in Developer Tools · Editors' Choice Award"));
        capture_apple("rating", "App Store Connect", box, 380, 175);
    }

    // Breadcrumbs: Finder Path Bar
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("FINDER PATH"));
        auto crumbs = std::make_shared<simplegui::Breadcrumbs>(std::vector<std::string>{
            "Macintosh HD", "Developer", "EasyQt6", "Widgets", "Source"
        });
        box->add_child(crumbs);
        box->add_child(make_desc_label("5 items · 1.2 TB available on APFS Volume"));
        capture_apple("breadcrumbs", "Finder", box, 440, 160);
    }

    // Textarea: Xcode Editor / Notes
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("NOTES / SCRATCHPAD"));
        auto area = std::make_shared<simplegui::Textarea>(
            "// EasyQt6 Apple HIG Integration\n"
            "auto app = simplegui::Application(argc, argv);\n"
            "app.set_theme(\"modern_dark\");\n"
            "auto win = simplegui::Window(\"Hello World\", 400, 300);\n"
            "win.show();"
        );
        box->add_child(area);
        box->add_child(make_desc_label("UTF-8 · C++20 · Native High-Performance Runtime"));
        capture_apple("textarea", "Scratchpad.cpp", box, 440, 210);
    }

    // Link: Apple Developer Documentation
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("HUMAN INTERFACE GUIDELINES"));
        auto lnk = std::make_shared<simplegui::Link>("Explore Apple Human Interface Guidelines →", "https://developer.apple.com/design/");
        box->add_child(lnk);
        box->add_child(make_desc_label("Discover design principles, patterns, and best practices."));
        capture_apple("link", "Developer Documentation", box, 440, 160);
    }

    // ImageButton: Music Playback Control
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("MEDIA ACTION"));
        auto btn = std::make_shared<simplegui::ImageButton>("", "▶ Play Album (Lossless)");
        btn->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 8px 18px; border-radius: 6px;");
        box->add_child(btn);
        box->add_child(make_desc_label("Apple Music · Spatial Audio with Dolby Atmos"));
        capture_apple("image_button", "Apple Music", box, 380, 170);
    }

    // =========================================================================
    // 2. TMOG PRECISION TELEMETRY CONTROLS
    // =========================================================================

    // Sparkline
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("CPU LOAD (LAST 60 SECONDS)"));
        auto spark = std::make_shared<simplegui::Sparkline>("#06b6d4");
        spark->set_samples({15, 20, 24, 18, 30, 45, 60, 52, 40, 35, 48, 70, 85, 65, 42, 38, 50});
        box->add_child(spark);
        box->add_child(make_desc_label("Sampling at 1000 Hz · Hardware Performance Counters"));
        capture_apple("sparkline", "Activity Monitor Telemetry", box, 440, 185);
    }

    // VFD Meter
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("ANALOG VU METER (CHANNEL 1)"));
        auto vfd = std::make_shared<simplegui::VfdMeter>(20, false);
        vfd->set_value(78.0);
        box->add_child(vfd);
        box->add_child(make_desc_label("Peak Program Meter · Calibrated to +4 dBu"));
        capture_apple("vfd_meter", "Audio Level Meter", box, 420, 165);
    }

    // StatCard
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("SILICON TELEMETRY"));
        auto stat = std::make_shared<simplegui::StatCard>("M3 MAX NEURAL ENGINE", "38 TOPS", "16-Core Matrix Co-Processor", "#06b6d4");
        box->add_child(stat);
        box->add_child(make_desc_label("Real-time on-device machine learning acceleration."));
        capture_apple("stat_card", "Hardware Overview", box, 380, 210);
    }

    // CompositionBar
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("UNIFIED MEMORY ALLOCATION (64 GB TOTAL)"));
        auto comp = std::make_shared<simplegui::CompositionBar>();
        comp->add_segment("Apps (22GB)", 22.0, "#0a84ff");
        comp->add_segment("Wired (10GB)", 10.0, "#06b6d4");
        comp->add_segment("Compressed (8GB)", 8.0, "#f59e0b");
        comp->add_segment("Free (24GB)", 24.0, "#272a34");
        box->add_child(comp);
        box->add_child(make_desc_label("Zero-copy memory bandwidth: 400 GB/s"));
        capture_apple("composition_bar", "Memory Allocation", box, 440, 165);
    }

    // StatusPill
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("SYSTEM INTEGRITY PROTECTION"));
        auto row = std::make_shared<simplegui::HBox>();
        row->set_spacing(8);
        auto pill1 = std::make_shared<simplegui::StatusPill>("SIP ENABLED", "#10b981");
        auto pill2 = std::make_shared<simplegui::StatusPill>("SECURE BOOT ACTIVE", "#0a84ff");
        row->add_child(pill1);
        row->add_child(pill2);
        box->add_child(row);
        box->add_child(make_desc_label("Kernel integrity verified by Apple Secure Enclave."));
        capture_apple("status_pill", "Security Verification", box, 400, 165);
    }

    // =========================================================================
    // 3. ENHANCED PARITY CONTROLS (vlang_simplegui + Qt6)
    // =========================================================================

    // ActivityHeatmap
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("XCODE CLOUD BUILD ACTIVITY (2026)"));
        auto heatmap = std::make_shared<simplegui::ActivityHeatmap>(24, 7);
        for (int w = 0; w < 24; ++w) {
            for (int d = 0; d < 7; ++d) {
                heatmap->set_cell(w, d, (w * 3 + d * 5) % 5);
            }
        }
        box->add_child(heatmap);
        box->add_child(make_desc_label("1,420 commits and workflow runs in the last 6 months."));
        capture_apple("activity_heatmap", "Xcode Cloud Analytics", box, 540, 230);
    }

    // StatGrid
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("APP STORE CONNECT METRICS"));
        auto statgrid = std::make_shared<simplegui::StatGrid>();
        statgrid->add_stat("Active Subscribers", "24,892", "+14.2%", true);
        statgrid->add_stat("Monthly Recurring", "$184,500", "+9.4%", true);
        statgrid->add_stat("Crash-Free Sessions", "99.98%", "+0.02%", true);
        statgrid->add_stat("Avg Latency", "12 ms", "-6.1%", true);
        box->add_child(statgrid);
        box->add_child(make_desc_label("Real-time aggregated metrics across all platforms."));
        capture_apple("stat_grid", "App Store Connect", box, 480, 250);
    }

    // UserProfileCard
    {
        auto profile = std::make_shared<simplegui::UserProfileCard>(
            "Dr. Elena Rostova", "@elena_ai", "Distinguished Apple Fellow",
            "Designing next-generation neural GUI architectures and high-performance native toolkits.",
            true, "Connect"
        );
        capture_apple("user_profile_card", "Developer Directory", profile, 460, 290, 16);
    }

    // ProductCard
    {
        auto product = std::make_shared<simplegui::ProductCard>(
            "MacBook Pro 16\" M3 Max",
            "16-core CPU, 40-core GPU, 64GB Unified Memory, 1TB SSD Storage with Liquid Retina XDR display.",
            "$3,499.00", "APPLE SILICON", 4.9, "Order Now"
        );
        capture_apple("product_card", "Apple Store", product, 440, 340, 16);
    }

    // DonutChart
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("MACINTOSH HD STORAGE BREAKDOWN"));
        auto donut = std::make_shared<simplegui::DonutChart>("78%", "APFS USED");
        donut->add_segment("Applications", 42.0, "#0a84ff");
        donut->add_segment("Developer SDKs", 28.0, "#8b5cf6");
        donut->add_segment("Documents", 16.0, "#06b6d4");
        donut->add_segment("Available", 22.0, "#2c303c");
        box->add_child(donut);
        box->add_child(make_desc_label("1.4 TB of 2.0 TB Used · APFS Encrypted"));
        capture_apple("donut_chart", "Storage Management", box, 360, 310);
    }

    // KanbanBoard
    {
        auto kanban = std::make_shared<simplegui::KanbanBoard>();
        kanban->add_column("todo", "To Do");
        kanban->add_column("progress", "In Progress");
        kanban->add_column("done", "Completed");
        kanban->add_card("todo", "k1", "Metal 3 Shaders", "GRAPHICS", "Compute pipeline for ray-traced GUI.");
        kanban->add_card("progress", "k2", "Apple HIG Parity", "DESIGN", "High-polish macOS window frames.");
        kanban->add_card("done", "k3", "Native Qt6 PIMPL", "CORE", "Zero-boilerplate memory-safe C++.");
        capture_apple("kanban_board", "Xcode Cloud Sprint Board", kanban, 620, 360, 16);
    }

    // FeedbackMood
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("DEVELOPER EXPERIENCE FEEDBACK"));
        auto mood = std::make_shared<simplegui::FeedbackMood>(5);
        box->add_child(mood);
        box->add_child(make_desc_label("How was your experience building with EasyQt6 today?"));
        capture_apple("feedback_mood", "Developer Feedback", box, 400, 175);
    }

    // DateRangePicker
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("ANALYTICS REPORTING INTERVAL"));
        auto drp = std::make_shared<simplegui::DateRangePicker>("2026-10-01", "2026-10-31");
        box->add_child(drp);
        box->add_child(make_desc_label("Compare against prior 30-day period."));
        capture_apple("date_range_picker", "Reporting Interval", box, 420, 170);
    }

    // MaskedInput
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("APPLE CASH / CARD NUMBER"));
        auto masked = std::make_shared<simplegui::MaskedInput>("9999-9999-9999-9999", "4532-8192-3401-9284");
        box->add_child(masked);
        box->add_child(make_desc_label("Secure virtual card token protected by Apple Pay."));
        capture_apple("masked_input", "Apple Wallet Preferences", box, 400, 170);
    }

    // TokenField
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        box->add_child(make_section_label("COMPILER DIRECTIVES & FRAMEWORKS"));
        auto token_field = std::make_shared<simplegui::TokenField>();
        token_field->add_token("macOS-Sequoia");
        token_field->add_token("AppleSilicon");
        token_field->add_token("Qt6.8");
        token_field->add_token("C++20");
        box->add_child(token_field);
        box->add_child(make_desc_label("Build targets and compiler search search paths."));
        capture_apple("token_field", "Build Settings", box, 440, 170);
    }

    // NavRail
    {
        auto box = std::make_shared<simplegui::HBox>();
        box->set_spacing(16);
        auto nav_rail = std::make_shared<simplegui::NavRail>();
        nav_rail->add_item("dash", "⚡", "Dash");
        nav_rail->add_item("analytics", "📊", "Stats");
        nav_rail->add_item("devices", "📱", "Devices");
        nav_rail->add_item("settings", "⚙", "Config");
        nav_rail->set_selected("dash");
        box->add_child(nav_rail);

        auto panel = std::make_shared<simplegui::VBox>();
        panel->set_spacing(8);
        panel->add_child(make_section_label("ACTIVE WORKSPACE"));
        auto title = std::make_shared<simplegui::Label>("Development Hub");
        title->set_style("color: #f8fafc; font-size: 16px; font-weight: 700;");
        panel->add_child(title);
        panel->add_child(make_desc_label("macOS Navigation Rail with instant keyboard shortcuts."));
        panel->add_stretch();
        box->add_child(panel);

        capture_apple("nav_rail", "Navigation Rail", box, 380, 260, 16);
    }

    // MediaPlayer
    {
        auto player = std::make_shared<simplegui::MediaPlayer>("Midnight City (Spatial Audio Edit)", "M83 · Hurry Up, We're Dreaming", "04:03");
        player->set_position(142, 243);
        player->set_playing(true);
        capture_apple("media_player", "Apple Music", player, 420, 240, 16);
    }

    // =========================================================================
    // 4. GRAPHICS & ADVANCED CHARTS
    // =========================================================================

    // BarChart: Monthly Revenue
    {
        auto bar_chart = std::make_shared<simplegui::BarChart>("App Store Revenue ($k)");
        bar_chart->add_bar("May", 145.0, "#0a84ff");
        bar_chart->add_bar("Jun", 182.0, "#0a84ff");
        bar_chart->add_bar("Jul", 215.0, "#06b6d4");
        bar_chart->add_bar("Aug", 260.0, "#0a84ff");
        bar_chart->add_bar("Sep", 310.0, "#10b981");
        bar_chart->add_bar("Oct", 385.0, "#10b981");
        capture_apple("bar_chart", "App Store Trends", bar_chart, 440, 260, 16);
    }

    // LineChart: Network Bandwidth
    {
        auto line_chart = std::make_shared<simplegui::LineChart>("Network Bandwidth (Gbps)");
        line_chart->set_x_labels({"00:00", "04:00", "08:00", "12:00", "16:00", "20:00", "24:00"});
        line_chart->add_series("Inbound", {14, 22, 58, 115, 142, 108, 45}, "#0a84ff", true);
        line_chart->add_series("Outbound", {10, 15, 34, 78, 92, 65, 28}, "#10b981", true);
        capture_apple("line_chart", "Activity Monitor Network", line_chart, 480, 270, 16);
    }

    // PieChart: Global Region Breakdown
    {
        auto pie_chart = std::make_shared<simplegui::PieChart>("Global Device Distribution");
        pie_chart->add_slice("Americas", 44.0, "#0a84ff");
        pie_chart->add_slice("Europe", 31.0, "#8b5cf6");
        pie_chart->add_slice("Asia-Pacific", 19.0, "#06b6d4");
        pie_chart->add_slice("Other", 6.0, "#f59e0b");
        capture_apple("pie_chart", "Device Telemetry", pie_chart, 420, 260, 16);
    }

    // Canvas: 2D Vector Graphics
    {
        auto canvas = std::make_shared<simplegui::Canvas>(360, 210);
        canvas->clear("#0b0e14");
        canvas->fill_rect(24, 24, 52, 52, "#0a84ff");
        canvas->fill_circle(148, 50, 26, "#ec4899");
        canvas->draw_rect(222, 24, 52, 52, "#10b981", 2);
        canvas->draw_line(24, 125, 310, 125, "#38bdf8", 2);
        canvas->fill_circle(64, 125, 8, "#f59e0b");
        canvas->fill_circle(148, 125, 12, "#38bdf8");
        canvas->fill_circle(232, 125, 8, "#a855f7");
        canvas->draw_text(28, 172, "EasyQt6 Native 2D Vector Canvas", "#94a3b8", 12);
        capture_apple("canvas", "Vector Graphics Canvas", canvas, 400, 270, 16);
    }

    // RadarChart: System Capabilities
    {
        auto radar = std::make_shared<simplegui::RadarChart>("Silicon Benchmark Evaluation");
        radar->set_dimensions({"Throughput", "Latency", "Resilience", "Security", "Scalability", "UX"});
        radar->add_dataset("M3 Max", {95, 92, 98, 90, 94, 96}, "#0a84ff");
        radar->add_dataset("Legacy M1", {72, 68, 75, 80, 60, 70}, "#f59e0b");
        capture_apple("radar_chart", "Hardware Benchmarks", radar, 380, 310, 16);
    }

    // CandlestickChart: AAPL Price Action
    {
        auto candle = std::make_shared<simplegui::CandlestickChart>("Apple Inc (AAPL) · NasdaqGS");
        candle->add_candle("10:00", 228.40, 230.10, 227.80, 229.80);
        candle->add_candle("11:00", 229.80, 231.50, 229.20, 231.20);
        candle->add_candle("12:00", 231.20, 232.00, 230.10, 230.60);
        candle->add_candle("13:00", 230.60, 231.40, 229.90, 231.10);
        candle->add_candle("14:00", 231.10, 233.20, 230.80, 232.90);
        candle->add_candle("15:00", 232.90, 234.00, 232.40, 233.80);
        candle->add_candle("16:00", 233.80, 234.50, 233.10, 234.20);
        capture_apple("candlestick_chart", "Apple Stocks", candle, 460, 270, 16);
    }

    // =========================================================================
    // 5. LAYOUTS
    // =========================================================================

    // VBox
    {
        auto vbox = std::make_shared<simplegui::VBox>();
        vbox->set_spacing(10);
        vbox->add_child(make_section_label("VERTICAL BOX LAYOUT"));
        auto b1 = std::make_shared<simplegui::Button>("Primary Action (Top)");
        b1->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 7px 16px; border-radius: 6px;");
        auto b2 = std::make_shared<simplegui::Button>("Secondary Action (Middle)");
        b2->set_style("background-color: #272a34; color: #e2e8f0; font-weight: 500; padding: 7px 16px; border-radius: 6px;");
        auto b3 = std::make_shared<simplegui::Button>("Tertiary Action (Bottom)");
        b3->set_style("background-color: #272a34; color: #e2e8f0; font-weight: 500; padding: 7px 16px; border-radius: 6px;");
        vbox->add_child(b1);
        vbox->add_child(b2);
        vbox->add_child(b3);
        capture_apple("vbox", "VBox Layout", vbox, 360, 220);
    }

    // HBox
    {
        auto hbox_wrap = std::make_shared<simplegui::VBox>();
        hbox_wrap->set_spacing(10);
        hbox_wrap->add_child(make_section_label("HORIZONTAL BOX LAYOUT"));
        auto hbox = std::make_shared<simplegui::HBox>();
        hbox->set_spacing(10);
        auto b1 = std::make_shared<simplegui::Button>("Left Panel");
        auto b2 = std::make_shared<simplegui::Button>("Center Panel");
        b2->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 7px 16px; border-radius: 6px;");
        auto b3 = std::make_shared<simplegui::Button>("Right Panel");
        hbox->add_child(b1);
        hbox->add_child(b2);
        hbox->add_child(b3);
        hbox_wrap->add_child(hbox);
        capture_apple("hbox", "HBox Layout", hbox_wrap, 420, 165);
    }

    // Grid
    {
        auto grid_wrap = std::make_shared<simplegui::VBox>();
        grid_wrap->set_spacing(10);
        grid_wrap->add_child(make_section_label("DATA GRID LAYOUT"));
        auto grid = std::make_shared<simplegui::Grid>(3, 3, std::vector<std::string>{"ID", "Component", "Status"});
        grid->set_cell(0, 0, "101");
        grid->set_cell(0, 1, "Native Controls");
        grid->set_cell(0, 2, "Active");
        grid->set_cell(1, 0, "102");
        grid->set_cell(1, 1, "WebEngine View");
        grid->set_cell(1, 2, "Ready");
        grid->set_cell(2, 0, "103");
        grid->set_cell(2, 1, "Metal / Qt6");
        grid->set_cell(2, 2, "Compiled");
        grid_wrap->add_child(grid);
        capture_apple("grid", "Grid Layout", grid_wrap, 440, 220);
    }

    // GroupBox
    {
        auto gb_wrap = std::make_shared<simplegui::VBox>();
        gb_wrap->set_spacing(10);
        auto group_box = std::make_shared<simplegui::GroupBox>("Security Preferences");
        auto group_vbox = std::make_shared<simplegui::VBox>();
        group_vbox->set_margins(12);
        group_vbox->set_spacing(8);
        group_vbox->add_child(std::make_shared<simplegui::Checkbox>("Require Touch ID for authentication", true));
        group_vbox->add_child(std::make_shared<simplegui::Checkbox>("Allow Apple Watch to unlock Mac", true));
        group_box->add_child(group_vbox);
        gb_wrap->add_child(group_box);
        capture_apple("group_box", "GroupBox Layout", gb_wrap, 420, 200);
    }

    // TabView
    {
        auto tab_wrap = std::make_shared<simplegui::VBox>();
        tab_wrap->set_spacing(8);
        auto tab_view = std::make_shared<simplegui::TabView>();
        auto tab1 = std::make_shared<simplegui::VBox>();
        tab1->set_margins(14);
        tab1->set_spacing(10);
        tab1->add_child(make_section_label("GENERAL PREFERENCES"));
        tab1->add_child(std::make_shared<simplegui::Checkbox>("Show scroll bars automatically", true));
        auto save_b = std::make_shared<simplegui::Button>("Apply Settings");
        save_b->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 6px 14px; border-radius: 6px;");
        tab1->add_child(save_b);

        auto tab2 = std::make_shared<simplegui::VBox>();
        tab2->set_margins(14);
        tab2->add_child(make_section_label("ACCOUNT SETTINGS"));
        tab_view->add_tab("General", tab1);
        tab_view->add_tab("Account", tab2);
        tab_wrap->add_child(tab_view);
        capture_apple("tab_view", "TabView Layout", tab_wrap, 440, 230);
    }

    // SplitView
    {
        auto split_wrap = std::make_shared<simplegui::VBox>();
        split_wrap->set_spacing(8);
        auto split_view = std::make_shared<simplegui::SplitView>(true);
        auto left_p = std::make_shared<simplegui::VBox>();
        left_p->set_margins(10);
        left_p->add_child(make_section_label("SIDEBAR"));
        left_p->add_child(std::make_shared<simplegui::Label>("Inbox (12)"));
        left_p->add_child(std::make_shared<simplegui::Label>("Archive"));

        auto right_p = std::make_shared<simplegui::VBox>();
        right_p->set_margins(10);
        right_p->add_child(make_section_label("CONTENT DETAIL"));
        right_p->add_child(std::make_shared<simplegui::Label>("Welcome to EasyQt6 High-Performance Native UI"));

        split_view->add_child(left_p);
        split_view->add_child(right_p);
        split_wrap->add_child(split_view);
        capture_apple("split_view", "SplitView Layout", split_wrap, 460, 200);
    }

    // ScrollView
    {
        auto scroll_wrap = std::make_shared<simplegui::VBox>();
        scroll_wrap->set_spacing(8);
        auto scroll_view = std::make_shared<simplegui::ScrollView>();
        auto content = std::make_shared<simplegui::VBox>();
        content->set_margins(12);
        content->set_spacing(6);
        for (int i = 1; i <= 8; ++i) {
            auto row = std::make_shared<simplegui::Label>("macOS System Log Entry #" + std::to_string(i) + " — Verified OK");
            row->set_style("color: #cbd5e1; font-size: 12px;");
            content->add_child(row);
        }
        scroll_view->set_content(content);
        scroll_wrap->add_child(scroll_view);
        capture_apple("scroll_view", "ScrollView Layout", scroll_wrap, 420, 230);
    }

    // =========================================================================
    // 6. FULL SHOWCASE DEMO APPLICATIONS
    // =========================================================================

    // Hello World
    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(12);
        auto title = std::make_shared<simplegui::Label>("Welcome to EasyQt6");
        title->set_style("color: #f8fafc; font-size: 18px; font-weight: 700;");
        auto sub = std::make_shared<simplegui::Label>("Zero-boilerplate C++20 GUI library with native Qt6 performance.");
        sub->set_style("color: #94a3b8; font-size: 13px;");
        auto btn = std::make_shared<simplegui::Button>("Get Started");
        btn->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 8px 20px; border-radius: 6px;");

        box->add_child(title);
        box->add_child(sub);
        box->add_child(btn);
        capture_apple("example_hello_world", "Hello World", box, 400, 200);
    }

    // Login Form
    {
        auto layout = std::make_shared<simplegui::VBox>();
        layout->set_spacing(10);

        layout->add_child(make_section_label("APPLE ACCOUNT SIGN IN"));
        auto user_input = std::make_shared<simplegui::TextInput>("developer@easyqt6.org");
        layout->add_child(user_input);

        layout->add_child(make_section_label("PASSWORD / PASSKEY"));
        auto pass_input = std::make_shared<simplegui::PasswordInput>("super_secret_master_password");
        layout->add_child(pass_input);

        auto remember = std::make_shared<simplegui::Checkbox>("Keep me signed in on this Mac", true);
        layout->add_child(remember);

        auto login_btn = std::make_shared<simplegui::Button>("Sign In with Apple");
        login_btn->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; padding: 8px 18px; border-radius: 6px;");
        layout->add_child(login_btn);

        capture_apple("example_login_form", "Sign In", layout, 400, 280);
    }

    // Calculator
    {
        auto layout = std::make_shared<simplegui::VBox>();
        layout->set_spacing(10);
        auto display = std::make_shared<simplegui::TextInput>("1,337.42");
        display->set_style("font-size: 24px; font-weight: 600; color: #f8fafc; background-color: #1a1d26; border: 1px solid #282c37; padding: 10px; border-radius: 8px;");
        layout->add_child(display);

        const std::vector<std::vector<std::string>> buttons = {
            {"C", "+/-", "%", "÷"},
            {"7", "8", "9", "×"},
            {"4", "5", "6", "−"},
            {"1", "2", "3", "+"},
            {"0", "", ".", "="}
        };

        for (const auto& row : buttons) {
            auto row_box = std::make_shared<simplegui::HBox>();
            row_box->set_spacing(8);
            for (const auto& text : row) {
                if (text.empty()) continue;
                auto b = std::make_shared<simplegui::Button>(text);
                if (text == "÷" || text == "×" || text == "−" || text == "+" || text == "=") {
                    b->set_style("background-color: #f59e0b; color: #ffffff; font-weight: bold; border-radius: 6px; font-size: 15px;");
                } else if (text == "C" || text == "+/-" || text == "%") {
                    b->set_style("background-color: #3b4252; color: #e2e8f0; font-weight: 500; border-radius: 6px;");
                }
                row_box->add_child(b);
            }
            layout->add_child(row_box);
        }
        capture_apple("example_calculator", "Calculator", layout, 340, 380);
    }

    // Settings Dashboard
    {
        auto main_layout = std::make_shared<simplegui::VBox>();
        main_layout->set_spacing(12);

        auto tabs = std::make_shared<simplegui::TabView>();

        // Tab 1: Profile & Account
        auto tab_profile = std::make_shared<simplegui::VBox>();
        tab_profile->set_margins(12);
        tab_profile->set_spacing(10);

        auto grp_user = std::make_shared<simplegui::GroupBox>("Apple Account");
        auto user_vbox = std::make_shared<simplegui::VBox>();
        user_vbox->set_margins(10);
        user_vbox->set_spacing(8);

        auto row_name = std::make_shared<simplegui::HBox>();
        row_name->set_spacing(10);
        row_name->add_child(std::make_shared<simplegui::Label>("Full Name:"));
        row_name->add_child(std::make_shared<simplegui::TextInput>("Jerome Scott"));

        auto row_email = std::make_shared<simplegui::HBox>();
        row_email->set_spacing(10);
        row_email->add_child(std::make_shared<simplegui::Label>("Email:"));
        row_email->add_child(std::make_shared<simplegui::TextInput>("developer@easyqt6.org"));

        user_vbox->add_child(row_name);
        user_vbox->add_child(row_email);
        grp_user->add_child(user_vbox);

        auto grp_region = std::make_shared<simplegui::GroupBox>("Language & Region");
        auto region_vbox = std::make_shared<simplegui::VBox>();
        region_vbox->set_margins(10);
        region_vbox->set_spacing(8);

        auto row_lang = std::make_shared<simplegui::HBox>();
        row_lang->set_spacing(10);
        row_lang->add_child(std::make_shared<simplegui::Label>("Preferred:"));
        row_lang->add_child(std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"English (US)", "Spanish", "German"}));

        region_vbox->add_child(row_lang);
        grp_region->add_child(region_vbox);

        tab_profile->add_child(grp_user);
        tab_profile->add_child(grp_region);
        tab_profile->add_stretch();

        // Tab 2: Appearance & Sound
        auto tab_appearance = std::make_shared<simplegui::VBox>();
        tab_appearance->set_margins(12);
        tab_appearance->set_spacing(10);

        auto grp_theme = std::make_shared<simplegui::GroupBox>("Theme & Colors");
        auto theme_vbox = std::make_shared<simplegui::VBox>();
        theme_vbox->set_margins(10);
        theme_vbox->set_spacing(8);

        auto chk_dark = std::make_shared<simplegui::Checkbox>("Enable High-Contrast Dark Mode", true);
        auto row_color = std::make_shared<simplegui::HBox>();
        row_color->set_spacing(10);
        row_color->add_child(std::make_shared<simplegui::Label>("Accent Color:"));
        row_color->add_child(std::make_shared<simplegui::ColorWell>("#0a84ff"));
        row_color->add_stretch();

        theme_vbox->add_child(chk_dark);
        theme_vbox->add_child(row_color);
        grp_theme->add_child(theme_vbox);

        auto grp_audio = std::make_shared<simplegui::GroupBox>("Audio Output");
        auto audio_hbox = std::make_shared<simplegui::HBox>();
        audio_hbox->set_margins(10);
        audio_hbox->set_spacing(16);

        auto audio_left = std::make_shared<simplegui::VBox>();
        audio_left->add_child(std::make_shared<simplegui::Label>("Master Volume"));
        audio_left->add_child(std::make_shared<simplegui::Knob>(0, 100, 75));

        auto audio_right = std::make_shared<simplegui::VBox>();
        audio_right->add_child(std::make_shared<simplegui::Label>("Satisfaction"));
        auto rtg = std::make_shared<simplegui::Rating>(5);
        rtg->set_rating(5);
        audio_right->add_child(rtg);

        audio_hbox->add_child(audio_left);
        audio_hbox->add_child(audio_right);
        audio_hbox->add_stretch();
        grp_audio->add_child(audio_hbox);

        tab_appearance->add_child(grp_theme);
        tab_appearance->add_child(grp_audio);
        tab_appearance->add_stretch();

        tabs->add_tab("Account", tab_profile);
        tabs->add_tab("Appearance", tab_appearance);

        // Bottom Bar
        auto bottom_bar = std::make_shared<simplegui::HBox>();
        bottom_bar->set_spacing(10);
        auto btn_cancel = std::make_shared<simplegui::Button>("Cancel");
        auto btn_save = std::make_shared<simplegui::Button>("Save Changes");
        btn_save->set_style("background-color: #0a84ff; color: #ffffff; font-weight: bold; border-radius: 6px;");

        bottom_bar->add_stretch();
        bottom_bar->add_child(btn_cancel);
        bottom_bar->add_child(btn_save);

        main_layout->add_child(tabs);
        main_layout->add_child(bottom_bar);
        capture_apple("example_settings_dashboard", "System Settings", main_layout, 580, 480);
    }

    // Data Explorer
    {
        auto root = std::make_shared<simplegui::VBox>();
        root->set_spacing(10);

        auto crumbs = std::make_shared<simplegui::Breadcrumbs>(std::vector<std::string>{"Organization", "Analytics", "Live Transactions"});

        auto toolbar = std::make_shared<simplegui::HBox>();
        toolbar->set_spacing(8);
        auto search = std::make_shared<simplegui::SearchField>("Search customers...");
        auto status_filter = std::make_shared<simplegui::Dropdown>(std::vector<std::string>{"All Statuses", "Paid", "Pending", "Failed"});
        auto btn_add = std::make_shared<simplegui::Button>("+ New Record");
        btn_add->set_style("background-color: #0a84ff; color: #ffffff; font-weight: 600; border-radius: 6px;");
        auto btn_refresh = std::make_shared<simplegui::Button>("Refresh");

        toolbar->add_child(search);
        toolbar->add_child(status_filter);
        toolbar->add_child(btn_add);
        toolbar->add_child(btn_refresh);

        auto split = std::make_shared<simplegui::SplitView>(true);

        auto sidebar = std::make_shared<simplegui::VBox>();
        sidebar->set_margins(8);
        sidebar->set_spacing(10);

        auto grp_summary = std::make_shared<simplegui::GroupBox>("Overview");
        auto summary_vbox = std::make_shared<simplegui::VBox>();
        summary_vbox->set_spacing(6);
        summary_vbox->add_child(std::make_shared<simplegui::Label>("Revenue: $148,200"));
        auto circ = std::make_shared<simplegui::CircularProgress>();
        circ->set_value(84);
        summary_vbox->add_child(circ);
        grp_summary->add_child(summary_vbox);

        auto grp_filter = std::make_shared<simplegui::GroupBox>("Quick Filters");
        auto filter_vbox = std::make_shared<simplegui::VBox>();
        filter_vbox->set_spacing(6);
        filter_vbox->add_child(std::make_shared<simplegui::Checkbox>("VIP Only", false));
        filter_vbox->add_child(std::make_shared<simplegui::Checkbox>("High Value", true));
        grp_filter->add_child(filter_vbox);

        sidebar->add_child(grp_summary);
        sidebar->add_child(grp_filter);
        sidebar->add_stretch();

        auto grid = std::make_shared<simplegui::Grid>(6, 5, std::vector<std::string>{"ID", "Customer", "Tier", "Amount", "Status"});
        const std::vector<std::vector<std::string>> data = {
            {"#401", "Apple Inc", "Enterprise", "$124,500", "Paid"},
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
        split->set_sizes(210, 670);

        auto status_bar = std::make_shared<simplegui::HBox>();
        status_bar->set_spacing(12);
        auto lbl_status = std::make_shared<simplegui::Label>("6 of 1,420 records · Synced");
        auto sync_progress = std::make_shared<simplegui::ProgressIndicator>();
        sync_progress->set_value(100);
        auto btn_export = std::make_shared<simplegui::Button>("Export CSV");

        status_bar->add_child(lbl_status);
        status_bar->add_stretch();
        status_bar->add_child(sync_progress);
        status_bar->add_child(btn_export);

        root->add_child(crumbs);
        root->add_child(toolbar);
        root->add_child(split);
        root->add_child(status_bar);

        capture_apple("example_data_explorer", "Enterprise Data Explorer", root, 920, 520);
    }

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

    // =========================================================================
    // FUTURISTIC CONTROLS
    // =========================================================================

    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(make_section_label("SYSTEMS"));
        box->add_child(std::make_shared<simplegui::ToggleSwitch>(true, "Shields online"));
        box->add_child(std::make_shared<simplegui::ToggleSwitch>(false, "Cloaking device"));
        auto warp = std::make_shared<simplegui::ToggleSwitch>(true, "Warp drive");
        warp->set_on_color("#a855f7");
        box->add_child(warp);
        capture_apple("toggle_switch", "ToggleSwitch", box, 360, 200);
    }

    {
        auto row = std::make_shared<simplegui::HBox>();
        row->set_spacing(12);
        auto g1 = std::make_shared<simplegui::RadialGauge>("CPU", 0, 100);
        g1->set_animated(false);
        g1->set_units("%");
        g1->set_value(42);
        auto g2 = std::make_shared<simplegui::RadialGauge>("CORE TEMP", 0, 120);
        g2->set_animated(false);
        g2->set_units("\xC2\xB0" "C");
        g2->set_thresholds(70, 95);
        g2->set_value(81);
        auto g3 = std::make_shared<simplegui::RadialGauge>("REACTOR", 0, 100);
        g3->set_animated(false);
        g3->set_units("%");
        g3->set_value(97);
        row->add_child(g1, 1);
        row->add_child(g2, 1);
        row->add_child(g3, 1);
        capture_apple("radial_gauge", "RadialGauge", row, 560, 260);
    }

    {
        auto row = std::make_shared<simplegui::HBox>();
        row->set_spacing(12);
        row->add_child(std::make_shared<simplegui::NeonButton>("ENGAGE"));
        row->add_child(std::make_shared<simplegui::NeonButton>("ABORT", "#ff2d75"));
        row->add_child(std::make_shared<simplegui::NeonButton>("SCAN", "#a3ff12"));
        capture_apple("neon_button", "NeonButton", row, 420, 140);
    }

    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(8);
        auto l1 = std::make_shared<simplegui::LedIndicator>("#22c55e", true);
        l1->set_label("Power");
        auto l2 = std::make_shared<simplegui::LedIndicator>("#f59e0b", true);
        l2->set_label("Network activity");
        auto l3 = std::make_shared<simplegui::LedIndicator>("#ef4444", false);
        l3->set_label("Fault (off)");
        box->add_child(l1);
        box->add_child(l2);
        box->add_child(l3);
        capture_apple("led_indicator", "LedIndicator", box, 320, 180);
    }

    {
        auto radar = std::make_shared<simplegui::RadarScope>();
        radar->stop();
        radar->add_blip(40, 0.6);
        radar->add_blip(160, 0.35, "#ff2d75");
        radar->add_blip(290, 0.8, "#facc15");
        radar->set_min_size(240, 240);
        capture_apple("radar_scope", "RadarScope", radar, 340, 340);
    }

    {
        auto term = std::make_shared<simplegui::TerminalView>();
        term->print_line("SimpleGUI terminal v1.0");
        term->print_line("> status", "#94a3b8");
        term->print_line("All systems nominal.");
        term->print_line("WARNING: coolant at 18%", "#f59e0b");
        capture_apple("terminal_view", "TerminalView", term, 480, 260);
    }

    {
        auto box = std::make_shared<simplegui::VBox>();
        box->set_spacing(10);
        box->add_child(std::make_shared<simplegui::SegmentedControl>(
            std::vector<std::string>{"Day", "Week", "Month", "Year"}, 1));
        auto seg = std::make_shared<simplegui::SegmentedControl>(
            std::vector<std::string>{"Low", "Medium", "High"}, 2);
        seg->set_accent_color("#ff2d75");
        box->add_child(seg);
        capture_apple("segmented_control", "SegmentedControl", box, 420, 160);
    }

    {
        auto panel = std::make_shared<simplegui::GlassPanel>("NAVIGATION");
        panel->add_child(std::make_shared<simplegui::Label>("Heading: 271\xC2\xB0"));
        panel->add_child(std::make_shared<simplegui::Label>("Velocity: 0.82 c"));
        auto bar = std::make_shared<simplegui::ProgressIndicator>();
        bar->set_value(64);
        panel->add_child(bar);
        capture_apple("glass_panel", "GlassPanel", panel, 360, 220);
    }

    {
        auto list = std::make_shared<simplegui::ListBox>(
            std::vector<std::string>{"Apples", "Bananas", "Cherries", "Dates", "Elderberries"});
        list->set_selected_index(2);
        capture_apple("list_box", "ListBox", list, 320, 240);
    }

    std::cout << "All Apple HIG production screenshots generated successfully!" << std::endl;
    return 0;
}
