#include "simplegui/simplegui.h"
#include <QFile>
#include <QDir>
#include <filesystem>
#include <thread>
#include <atomic>
#include <regex>
#include <mutex>
#include <vector>
#include <fstream>
#include <iostream>

using namespace simplegui;
namespace fs = std::filesystem;

// --- rip2 (Graveyard) Logic ---
std::string get_graveyard_dir() {
    std::string home = fs::absolute(fs::path(QDir::homePath().toStdString())).string();
    return home + "/.file_ninja_graveyard";
}

bool rip2_trash(const std::string& path, bool use_system_trash) {
    if (use_system_trash) {
        return QFile(QString::fromStdString(path)).moveToTrash();
    }
    
    fs::path p(path);
    if (!fs::exists(p)) return false;
    
    std::string graveyard = get_graveyard_dir();
    fs::create_directories(graveyard + "/files");
    fs::create_directories(graveyard + "/info");
    
    std::string filename = p.filename().string() + "_" + std::to_string(std::time(nullptr));
    std::string trash_path = graveyard + "/files/" + filename;
    
    try {
        fs::rename(p, trash_path);
        std::ofstream info(graveyard + "/info/" + filename + ".txt");
        info << p.string();
        return true;
    } catch(...) { return false; }
}

bool rip2_permanent(const std::string& path) {
    try { return fs::remove_all(path) > 0; } catch(...) { return false; }
}

std::vector<std::pair<std::string, std::string>> rip2_seance() {
    std::vector<std::pair<std::string, std::string>> items;
    std::string graveyard = get_graveyard_dir();
    if (!fs::exists(graveyard + "/info")) return items;
    
    for (auto& entry : fs::directory_iterator(graveyard + "/info")) {
        std::ifstream info(entry.path());
        std::string orig_path;
        std::getline(info, orig_path);
        items.push_back({entry.path().filename().string(), orig_path});
    }
    return items;
}

bool rip2_undo(const std::string& filename) {
    std::string graveyard = get_graveyard_dir();
    std::string info_path = graveyard + "/info/" + filename;
    std::string trash_path = graveyard + "/files/" + filename.substr(0, filename.size() - 4);
    
    std::ifstream info(info_path);
    std::string orig_path;
    std::getline(info, orig_path);
    if (orig_path.empty()) return false;
    
    try {
        fs::rename(trash_path, orig_path);
        fs::remove(info_path);
        return true;
    } catch(...) { return false; }
}

// --- fd (Search) Logic ---
struct SearchResult {
    std::string path;
    std::string name;
    uintmax_t size;
    bool is_dir;
};

struct SearchState {
    std::atomic<bool> searching{false};
    std::atomic<bool> cancel{false};
    std::mutex mtx;
    std::vector<SearchResult> results;
};

std::string glob_to_regex(const std::string& glob) {
    std::string rx = "^";
    for (char c : glob) {
        if (c == '*') rx += ".*";
        else if (c == '?') rx += ".";
        else if (std::string(".+*?^$()[]{}|\\").find(c) != std::string::npos) rx += "\\" + std::string(1, c);
        else rx += c;
    }
    return rx + "$";
}

enum class SizeOp { None, Greater, Less, Equal };
struct SizeFilter {
    SizeOp op = SizeOp::None;
    uintmax_t size = 0;
};
SizeFilter parse_size_filter(const std::string& str) {
    SizeFilter sf;
    if (str.empty()) return sf;
    std::string s = str;
    if (s[0] == '>' || s[0] == '+') { sf.op = SizeOp::Greater; s = s.substr(1); }
    else if (s[0] == '<' || s[0] == '-') { sf.op = SizeOp::Less; s = s.substr(1); }
    else { sf.op = SizeOp::Equal; }
    
    uintmax_t mult = 1;
    if (!s.empty()) {
        char last = std::tolower(s.back());
        if (last == 'b') { mult = 1; s.pop_back(); }
        else if (last == 'k') { mult = 1024; s.pop_back(); }
        else if (last == 'm') { mult = 1024 * 1024; s.pop_back(); }
        else if (last == 'g') { mult = 1024 * 1024 * 1024; s.pop_back(); }
        else if (last == 't') { mult = 1024ULL * 1024ULL * 1024ULL * 1024ULL; s.pop_back(); }
    }
    try { sf.size = std::stoull(s) * mult; } catch(...) { sf.op = SizeOp::None; }
    return sf;
}

int main(int argc, char* argv[]) {
    Application app(argc, argv);
    app.set_theme("apple");
    Window window("File Ninja", 1100, 800);

    auto tabs = std::make_shared<TabView>();
    
    // ==========================================
    // TAB 1: fd (Search) - APPLE STYLE SPLIT VIEW
    // ==========================================
    auto fd_split = std::make_shared<SplitView>();
    
    // -- SIDEBAR (Filters & Options) --
    auto sidebar_scroll = std::make_shared<ScrollView>();
    auto sidebar = std::make_shared<VBox>();
    sidebar->set_margins(15);
    sidebar->set_spacing(15);
    
    auto lbl_filters = std::make_shared<Label>("Filters & Locations");
    lbl_filters->set_font_size(16);
    lbl_filters->set_bold(true);
    sidebar->add_child(lbl_filters);
    
    auto loc_group = std::make_shared<GroupBox>("Search In");
    auto path_picker = std::make_shared<PathPicker>(PathPicker::Mode::Folder);
    path_picker->set_path(fs::current_path().string());
    loc_group->add_child(path_picker);
    sidebar->add_child(loc_group);
    
    auto crit_group = std::make_shared<GroupBox>("File Criteria");
    
    auto mk_row = [](const std::string& text, std::shared_ptr<Control> ctrl) {
        auto row = std::make_shared<HBox>();
        auto lbl = std::make_shared<Label>(text);
        lbl->set_width(70);
        row->add_child(lbl);
        row->add_child(ctrl, 1);
        return row;
    };
    
    auto combo_type = std::make_shared<ComboBox>(std::vector<std::string>{"Any", "File", "Directory", "Symlink", "Executable", "Empty"});
    auto combo_match = std::make_shared<ComboBox>(std::vector<std::string>{"Smart Case", "Case Sensitive", "Ignore Case", "Glob", "Fixed"});
    auto txt_ext = std::make_shared<TextInput>();
    auto txt_size = std::make_shared<TextInput>();
    auto num_depth = std::make_shared<NumberInput>();
    num_depth->set_value(0); // 0 = unlimited
    
    crit_group->add_child(mk_row("Type:", combo_type));
    crit_group->add_child(mk_row("Match:", combo_match));
    crit_group->add_child(mk_row("Ext:", txt_ext));
    crit_group->add_child(mk_row("Size:", txt_size));
    crit_group->add_child(mk_row("Depth:", num_depth));
    sidebar->add_child(crit_group);
    
    auto opt_group = std::make_shared<GroupBox>("Search Options");
    auto chk_hidden = std::make_shared<Checkbox>("Include Hidden Files");
    auto chk_symlinks = std::make_shared<Checkbox>("Follow Symlinks");
    auto chk_fullpath = std::make_shared<Checkbox>("Match Full Path");
    auto chk_absolute = std::make_shared<Checkbox>("Return Absolute Path");
    
    opt_group->add_child(chk_hidden);
    opt_group->add_child(chk_symlinks);
    opt_group->add_child(chk_fullpath);
    opt_group->add_child(chk_absolute);
    sidebar->add_child(opt_group);
    
    sidebar->add_stretch(); // push everything to top
    sidebar_scroll->set_content(sidebar);
    
    // -- MAIN AREA (Search box + Results) --
    auto main_area = std::make_shared<VBox>();
    main_area->set_margins(15);
    main_area->set_spacing(10);
    
    auto search_bar = std::make_shared<HBox>();
    search_bar->set_spacing(10);
    auto search_field = std::make_shared<SearchField>("Search pattern...");
    search_field->set_font_size(15);
    auto btn_search = std::make_shared<Button>("Search");
    btn_search->set_width(120);
    search_bar->add_child(search_field, 1);
    search_bar->add_child(btn_search);
    
    auto fd_grid = std::make_shared<Grid>(0, 3, std::vector<std::string>{"Name", "Path", "Size (bytes)"});
    
    auto fd_toolbar = std::make_shared<HBox>();
    fd_toolbar->set_spacing(10);
    auto fd_status = std::make_shared<Label>("Ready.");
    fd_status->set_text_color("gray");
    auto btn_fd_trash = std::make_shared<Button>("Move to Graveyard");
    auto btn_fd_sys = std::make_shared<Button>("System Trash");
    auto btn_fd_move = std::make_shared<Button>("Move to Folder...");
    
    fd_toolbar->add_child(fd_status, 1);
    fd_toolbar->add_child(btn_fd_move);
    fd_toolbar->add_child(btn_fd_sys);
    fd_toolbar->add_child(btn_fd_trash);
    
    main_area->add_child(search_bar);
    main_area->add_child(fd_grid, 1);
    main_area->add_child(fd_toolbar);
    
    fd_split->add_child(sidebar_scroll);
    fd_split->add_child(main_area); 
    fd_split->set_sizes(250, 850);
    tabs->add_tab("🔎 Find (fd)", fd_split);

    // ==========================================
    // TAB 2: rip2 (Graveyard)
    // ==========================================
    auto rip_tab = std::make_shared<VBox>();
    rip_tab->set_margins(15);
    rip_tab->set_spacing(15);
    
    auto rip_header = std::make_shared<Label>("Graveyard (rip2)");
    rip_header->set_font_size(18);
    rip_header->set_bold(true);
    
    auto rip_grid = std::make_shared<Grid>(0, 2, std::vector<std::string>{"Original Path", "Trash ID"});
    
    auto rip_toolbar = std::make_shared<HBox>();
    rip_toolbar->set_spacing(10);
    auto btn_seance = std::make_shared<Button>("Refresh");
    auto btn_undo = std::make_shared<Button>("Undo (Restore)");
    auto btn_perm = std::make_shared<Button>("Permanently Delete");
    btn_perm->set_text_color("red");
    
    rip_toolbar->add_child(btn_seance);
    rip_toolbar->add_stretch();
    rip_toolbar->add_child(btn_undo);
    rip_toolbar->add_child(btn_perm);
    
    rip_tab->add_child(rip_header);
    rip_tab->add_child(rip_toolbar);
    rip_tab->add_child(rip_grid, 1);
    tabs->add_tab("🗑 Graveyard", rip_tab);
    
    window.set_content(tabs);

    // ==========================================
    // FD SEARCH LOGIC
    // ==========================================
    auto state = std::make_shared<SearchState>();
    int selected_fd = -1;
    fd_grid->on_select([&](int r) { selected_fd = r; });
    
    auto do_search = [&, state, path_picker, chk_hidden, chk_symlinks, chk_fullpath, chk_absolute, num_depth, combo_type, combo_match, txt_size, txt_ext, search_field, fd_grid, fd_status, btn_search]() {
        if (state->searching) { state->cancel = true; return; }
        
        std::string root = path_picker->get_path();
        if (root.empty()) return;
        
        bool hidden = chk_hidden->is_checked();
        bool symlinks = chk_symlinks->is_checked();
        bool fullpath = chk_fullpath->is_checked();
        bool absolute = chk_absolute->is_checked();
        int depth = num_depth->get_value();
        std::string type = combo_type->get_text();
        std::string match = combo_match->get_text();
        std::string size_str = txt_size->get_text();
        std::string ext = txt_ext->get_text();
        std::string pattern_str = search_field->get_text();
        
        SizeFilter sf = parse_size_filter(size_str);
        
        std::regex rx;
        bool use_rx = true;
        
        if (match.find("Fixed") != std::string::npos) {
            use_rx = false;
        } else if (match.find("Glob") != std::string::npos) {
            rx = std::regex(glob_to_regex(pattern_str), std::regex_constants::icase);
        } else {
            auto flags = std::regex_constants::ECMAScript;
            if (match.find("Ignore") != std::string::npos || (match.find("Smart") != std::string::npos && pattern_str == std::string(pattern_str.begin(), pattern_str.end()))) {
                flags |= std::regex_constants::icase;
            }
            try { rx = std::regex(pattern_str, flags); } catch(...) { rx = std::regex(".*"); }
        }

        fd_grid->clear_rows();
        selected_fd = -1;
        fd_status->set_text("Searching...");
        btn_search->set_text("Cancel");
        state->searching = true;
        state->cancel = false;
        
        { std::lock_guard<std::mutex> lock(state->mtx); state->results.clear(); }
        
        std::thread([=]() {
            try {
                auto options = fs::directory_options::skip_permission_denied;
                if (symlinks) options |= fs::directory_options::follow_directory_symlink;
                
                for (auto it = fs::recursive_directory_iterator(root, options); it != fs::recursive_directory_iterator();) {
                    if (state->cancel) break;
                    try {
                        auto& entry = *it;
                        if (!hidden && entry.path().filename().string()[0] == '.') {
                            if (entry.is_directory()) it.disable_recursion_pending();
                            ++it; continue;
                        }
                        if (depth > 0 && it.depth() >= depth) {
                            if (entry.is_directory()) it.disable_recursion_pending();
                        }
                        
                        bool skip = false;
                        if (type.find("File") != std::string::npos && !entry.is_regular_file()) skip = true;
                        if (type.find("Dir") != std::string::npos && !entry.is_directory()) skip = true;
                        if (type.find("Symlink") != std::string::npos && !entry.is_symlink()) skip = true;
                        if (type.find("Empty") != std::string::npos && (!entry.is_regular_file() || entry.file_size() > 0)) skip = true;
                        if (!ext.empty() && entry.path().extension().string() != (ext[0] == '.' ? ext : "." + ext)) skip = true;
                        
                        if (!skip && sf.op != SizeOp::None && entry.is_regular_file()) {
                            try {
                                uintmax_t s = entry.file_size();
                                if (sf.op == SizeOp::Greater && s <= sf.size) skip = true;
                                if (sf.op == SizeOp::Less && s >= sf.size) skip = true;
                                if (sf.op == SizeOp::Equal && s != sf.size) skip = true;
                            } catch(...) { skip = true; }
                        }
                        
                        if (!skip) {
                            std::string target = fullpath ? entry.path().string() : entry.path().filename().string();
                            bool matches = false;
                            
                            if (!use_rx) {
                                matches = target.find(pattern_str) != std::string::npos;
                            } else {
                                matches = pattern_str.empty() || std::regex_search(target, rx);
                            }
                            
                            if (matches) {
                                SearchResult res;
                                res.name = entry.path().filename().string();
                                res.path = absolute ? fs::absolute(entry.path()).string() : entry.path().string();
                                res.is_dir = entry.is_directory();
                                try { res.size = entry.is_regular_file() ? entry.file_size() : 0; } catch(...) { res.size = 0; }
                                std::lock_guard<std::mutex> lock(state->mtx);
                                state->results.push_back(res);
                            }
                        }
                    } catch (...) {}
                    try { ++it; } catch (...) { it.disable_recursion_pending(); try { ++it; } catch(...) { break; } }
                }
            } catch(...) {}
            state->searching = false;
        }).detach();
    };
    
    btn_search->on_click(do_search);
    search_field->on_enter([do_search](const std::string&) { do_search(); });
    
    Timer ui_timer(100);
    ui_timer.on_tick([&, state, fd_grid, btn_search, fd_status]() {
        std::vector<SearchResult> batch;
        {
            std::lock_guard<std::mutex> lock(state->mtx);
            if (!state->results.empty()) {
                batch = std::move(state->results);
                state->results.clear();
            }
        }
        for (const auto& res : batch) {
            fd_grid->add_row({res.name, res.path, res.is_dir ? "<DIR>" : std::to_string(res.size)});
        }
        if (!state->searching && btn_search->get_text() == "Cancel") {
            btn_search->set_text("Search");
            fd_status->set_text("Found " + std::to_string(fd_grid->row_count()) + " items.");
        }
    });
    ui_timer.start();

    // ==========================================
    // ACTIONS LOGIC
    // ==========================================
    btn_fd_trash->on_click([&]() {
        if (selected_fd >= 0) {
            std::string path = fd_grid->get_cell(selected_fd, 1);
            if (rip2_trash(path, false)) {
                fd_grid->remove_row(selected_fd);
                selected_fd = -1;
                show_message("Moved to Graveyard.");
            }
        }
    });
    
    btn_fd_sys->on_click([&]() {
        if (selected_fd >= 0) {
            std::string path = fd_grid->get_cell(selected_fd, 1);
            if (QFile(QString::fromStdString(path)).moveToTrash()) {
                fd_grid->remove_row(selected_fd);
                selected_fd = -1;
                show_message("Moved to System Trash.");
            }
        }
    });
    
    btn_fd_move->on_click([&]() {
        if (selected_fd >= 0) {
            std::string path = fd_grid->get_cell(selected_fd, 1);
            std::string dest_folder = select_folder_dialog("Select Destination Folder");
            if (!dest_folder.empty()) {
                fs::path p(path);
                fs::path dest = fs::path(dest_folder) / p.filename();
                try {
                    fs::rename(p, dest);
                    fd_grid->remove_row(selected_fd);
                    selected_fd = -1;
                    show_message("Moved successfully.");
                } catch(const std::exception& e) {
                    show_message(std::string("Failed to move: ") + e.what());
                }
            }
        }
    });

    int selected_rip = -1;
    rip_grid->on_select([&](int r) { selected_rip = r; });
    
    auto refresh_seance = [&]() {
        rip_grid->clear_rows();
        selected_rip = -1;
        for (auto item : rip2_seance()) {
            rip_grid->add_row({item.second, item.first});
        }
    };
    
    btn_seance->on_click(refresh_seance);
    
    btn_undo->on_click([&]() {
        if (selected_rip >= 0) {
            if (rip2_undo(rip_grid->get_cell(selected_rip, 1))) {
                refresh_seance();
                show_message("Restored!");
            }
        }
    });
    
    btn_perm->on_click([&]() {
        if (selected_rip >= 0) {
            std::string graveyard = get_graveyard_dir();
            fs::remove(graveyard + "/files/" + rip_grid->get_cell(selected_rip, 1).substr(0, rip_grid->get_cell(selected_rip, 1).size() - 4));
            fs::remove(graveyard + "/info/" + rip_grid->get_cell(selected_rip, 1));
            refresh_seance();
            show_message("Permanently Deleted.");
        }
    });

    window.show();
    return app.run();
}
