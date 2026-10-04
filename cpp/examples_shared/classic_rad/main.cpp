// Classic RAD - a small "contact book" written the way Delphi, Lazarus or
// Visual Basic programmers are used to: named controls, menus, a status bar,
// message boxes, and familiar class names (Edit, Memo, CheckBox, ...).
#include "simplegui/simplegui.h"

#include <string>

using namespace simplegui;

namespace {

// Event handlers look controls up by name - like Form1.Edit1 in Delphi.
void add_contact() {
    auto name = find<Edit>("NameEdit");
    auto list = find<ListBox>("ContactList");
    if (!name || !list) return;

    if (name->get_text().empty()) {
        show_warning("Please type a name first.");
        name->set_focus();
        return;
    }
    list->add_item(name->get_text());
    name->clear();
    name->set_focus();
}

}  // namespace

int main(int argc, char* argv[]) {
    Application app(argc, argv);
    app.set_app_name("Contact Book");

    Window form("Contact Book", 640, 440);

    // ---- Controls (each gets a Name, like the Object Inspector) --------------
    auto name_label = std::make_shared<StaticText>("Name:");
    auto name_edit = std::make_shared<Edit>("Type a name and press Enter");
    name_edit->set_name("NameEdit");

    auto add_button = std::make_shared<CommandButton>("Add");
    auto remove_button = std::make_shared<CommandButton>("Remove");

    auto list = std::make_shared<ListBox>();
    list->set_name("ContactList");
    list->set_sorted(true);

    auto notes = std::make_shared<Memo>();
    notes->set_placeholder("Notes about the selected contact...");

    auto favorite = std::make_shared<CheckBox>("Favorite");

    // ---- Layout --------------------------------------------------------------
    auto input_row = std::make_shared<Row>();
    input_row->add_child(name_label);
    input_row->add_child(name_edit, 1);
    input_row->add_child(add_button);
    input_row->add_child(remove_button);

    auto details = std::make_shared<Frame>("Details");
    details->add_child(favorite);
    details->add_child(notes, 1);

    auto body = std::make_shared<Row>();
    body->add_child(list, 1);
    body->add_child(details, 2);

    auto root = std::make_shared<Column>();
    root->set_margins(12);
    root->add_child(input_row);
    root->add_child(body, 1);
    form.set_content(root);

    // ---- Events (OnClick, OnChange, ...) -------------------------------------
    add_button->on_click(add_contact);
    name_edit->on_enter([](const std::string&) { add_contact(); });

    remove_button->on_click([list, &form]() {
        const int index = list->selected_index();
        if (index < 0) {
            show_info("Select a contact to remove.");
            return;
        }
        if (ask_yes_no("Remove " + list->selected_text() + "?")) {
            list->remove_item(index);
            form.set_status_text("Contact removed.");
        }
    });

    list->on_select([&form](int, const std::string& text) {
        form.set_status_text("Selected: " + text);
    });

    list->on_double_click([](int, const std::string& text) {
        show_message("You double-clicked " + text);
    });

    // ---- Menus with keyboard shortcuts --------------------------------------
    form.add_menu_item("File", "Rename Window...", [&form]() {
        bool ok = false;
        const std::string title = input_text("New window title:", ok, "Rename", form.title());
        if (ok && !title.empty()) form.set_title(title);
    });
    form.add_menu_item("File", "Export...", [list]() {
        const std::string path = save_file_dialog("Export contacts", "Text files (*.txt)");
        if (!path.empty()) show_info(std::to_string(list->count()) + " contacts would be saved to:\n" + path);
    }, "Ctrl+E");
    form.add_menu_separator("File");
    form.add_menu_item("File", "Quit", [&form]() { form.close(); }, "Ctrl+Q");
    form.add_menu_item("Help", "About", []() {
        show_message("Contact Book\nBuilt with SimpleGUI for Qt 6.", "About");
    });

    // ---- OnCloseQuery: return false to keep the window open ------------------
    form.on_close([list]() {
        if (list->count() == 0) return true;
        return ask_yes_no("Quit and lose " + std::to_string(list->count()) + " contacts?");
    });

    form.set_status_text("Ready");
    form.center();
    form.show();
    return app.run();
}
