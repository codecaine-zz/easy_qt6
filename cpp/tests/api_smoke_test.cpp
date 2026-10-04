// API smoke tests. Run with: ctest --test-dir <build-dir> --output-on-failure
// Uses the offscreen Qt platform so no window ever appears.
#include "simplegui/simplegui.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

using namespace simplegui;

namespace {

int failures = 0;

void check(bool ok, const char* what, int line) {
    if (!ok) {
        ++failures;
        std::cerr << "FAILED (line " << line << "): " << what << "\n";
    }
}

#define CHECK(expr) check((expr), #expr, __LINE__)

void test_names_and_find() {
    auto button = std::make_shared<Button>("OK");
    button->set_name("ok_button");
    CHECK(find<Button>("ok_button") == button);
    CHECK(find<Label>("ok_button") == nullptr);   // wrong type
    CHECK(find_control("missing") == nullptr);
    CHECK(button->name() == "ok_button");
}

void test_events_and_disconnect() {
    auto toggle = std::make_shared<ToggleSwitch>();
    int calls = 0;
    EventConnection conn = toggle->on_toggle([&calls](bool) { ++calls; });
    toggle->toggle();
    CHECK(calls == 1);
    CHECK(toggle->is_on());
    CHECK(conn.connected());
    conn.disconnect();
    conn.disconnect();   // safe twice
    CHECK(!conn.connected());
    toggle->toggle();
    CHECK(calls == 1);

    toggle->set_on(true);   // setters never fire events
    CHECK(calls == 1);

    auto neon = std::make_shared<NeonButton>("Go");
    int clicks = 0;
    neon->on_click([&clicks]() { ++clicks; });
    neon->click();
    CHECK(clicks == 1);
}

void test_disconnect_after_destroy() {
    EventConnection conn;
    {
        auto button = std::make_shared<Button>("temp");
        conn = button->on_click([]() {});
    }
    conn.disconnect();   // must not crash
    CHECK(!conn.connected());
}

void test_layouts() {
    auto box = std::make_shared<VBox>();
    auto a = std::make_shared<Label>("a");
    auto b = std::make_shared<Label>("b");
    box->add_child(a);
    box->add_child(b);
    box->add_child(nullptr);   // ignored
    CHECK(box->child_count() == 2);
    box->remove_child(a);
    CHECK(box->child_count() == 1);
    box->clear();
    CHECK(box->child_count() == 0);

    auto panel = std::make_shared<GlassPanel>("Title");
    panel->add_child(a);
    CHECK(panel->child_count() == 1);
    CHECK(panel->get_title() == "TITLE");
}

void test_grid_bounds() {
    Grid grid(2, 2, {"A", "B"});
    grid.set_cell(5, 5, "ignored");
    grid.set_cell(-1, 0, "ignored");
    CHECK(grid.get_cell(5, 5).empty());
    grid.set_cell(1, 1, "x");
    CHECK(grid.get_cell(1, 1) == "x");
    const int row = grid.add_row({"r1", "r2", "extra"});
    CHECK(row == 2);
    CHECK(grid.row_count() == 3);
    CHECK(grid.get_cell(2, 0) == "r1");
    grid.remove_row(99);
    CHECK(grid.row_count() == 3);
    grid.clear_rows();
    CHECK(grid.row_count() == 0);
    CHECK(grid.column_count() == 2);
}

void test_kanban() {
    KanbanBoard board;
    CHECK(board.add_column("todo", "To Do"));
    CHECK(!board.add_column("todo", "Again"));
    CHECK(board.add_column("done", "Done"));
    CHECK(board.add_card("todo", "c1", "Write tests"));
    CHECK(!board.add_card("todo", "c1", "Duplicate"));
    CHECK(!board.add_card("nowhere", "c2", "Bad column"));
    CHECK(!board.move_card("c1", "unknown"));
    CHECK(board.card_column("c1") == "todo");
    CHECK(board.move_card("c1", "done"));
    CHECK(board.card_column("c1") == "done");
    CHECK(board.card_ids("done").size() == 1);
    CHECK(board.remove_card("c1"));
    CHECK(board.card_column("c1").empty());
}

void test_heatmap_bounds() {
    ActivityHeatmap map(4, 7);
    map.set_cell(100, 100, 3);   // ignored
    map.set_cell(-1, 0, 3);      // ignored
    CHECK(map.get_cell(100, 100) == 0);
    map.set_cell(1, 2, 3);
    CHECK(map.get_cell(1, 2) == 3);
    map.set_cell(1, 3, 99);      // clamped
    CHECK(map.get_cell(1, 3) <= 4);
    map.clear();
    CHECK(map.get_cell(1, 2) == 0);
}

void test_themes(Application& app) {
    CHECK(!app.set_theme("does-not-exist"));
    CHECK(app.set_theme("neon"));
    CHECK(app.theme() == "neon");
    CHECK(!Application::available_themes().empty());
}

void test_tokens() {
    TokenField field;
    field.add_token("red");
    field.add_token("red");
    field.add_token("");
    field.add_token("<b>blue</b>");
    CHECK(field.tokens().size() == 2);
    CHECK(field.has_token("<b>blue</b>"));
    field.remove_token("red");
    CHECK(!field.has_token("red"));
}

void test_media_player() {
    MediaPlayer player;
    player.set_track("Song", "Artist", "3:25");
    CHECK(player.duration() == 205);
    player.set_track("Long", "Artist", "1:02:03");
    CHECK(player.duration() == 3723);
    player.set_position(9999);
    CHECK(player.position() <= player.duration());
}

void test_list_box() {
    ListBox list({"b", "a"});
    CHECK(list.count() == 2);
    list.add_item("c");
    CHECK(list.item(2) == "c");
    CHECK(list.item(99).empty());
    list.set_selected_index(1);
    CHECK(list.selected_text() == "a");
    list.remove_item(99);   // ignored
    CHECK(list.count() == 3);
    list.set_sorted(true);
    CHECK(list.item(0) == "a");
    list.clear();
    CHECK(list.count() == 0);
    CHECK(list.selected_index() == -1);
}

void test_futuristic() {
    RadialGauge gauge("CPU", 0, 100);
    gauge.set_animated(false);
    gauge.set_value(150);
    CHECK(gauge.value() == 100.0);

    SegmentedControl seg({"A", "B", "C"}, 1);
    CHECK(seg.selected_text() == "B");
    seg.set_selected_index(10);   // ignored
    CHECK(seg.selected_index() == 1);

    RadarScope radar;
    const int id = radar.add_blip(10, 0.5);
    CHECK(radar.blip_count() == 1);
    radar.remove_blip(id);
    CHECK(radar.blip_count() == 0);
    radar.stop();
    CHECK(!radar.is_running());

    TerminalView term;
    term.print_line("hello <b>world</b>");
    CHECK(term.text().find("<b>world</b>") != std::string::npos);   // shown as plain text

    LedIndicator led;
    led.set_blinking(true, 100);
    CHECK(led.is_blinking());
    led.set_blinking(false);
    CHECK(!led.is_blinking());
}

void test_timer() {
    Timer timer(5);
    int ticks = 0;
    bool later = false;
    timer.set_single_shot(true);
    timer.on_tick([&ticks]() { ++ticks; });
    timer.start();
    CHECK(timer.is_running());
    run_later(5, [&later]() { later = true; });
    // Give the event loop up to ~2 seconds to deliver both callbacks.
    for (int i = 0; i < 200 && (ticks == 0 || !later); ++i) {
        Application::process_events();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    Application::process_events();
    CHECK(ticks == 1);
    CHECK(later);
    CHECK(!timer.is_running());
}

}  // namespace

int main(int argc, char* argv[]) {
    Application app(argc, argv);

    test_names_and_find();
    test_events_and_disconnect();
    test_disconnect_after_destroy();
    test_layouts();
    test_grid_bounds();
    test_kanban();
    test_heatmap_bounds();
    test_themes(app);
    test_tokens();
    test_media_player();
    test_list_box();
    test_futuristic();
    test_timer();

    if (failures == 0) std::cout << "All API smoke tests passed.\n";
    return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
