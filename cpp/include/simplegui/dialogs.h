#pragma once
#include <string>
#include <vector>

namespace simplegui {

// Ready-made pop-up windows (Delphi: ShowMessage / MessageDlg / InputBox,
// VB: MsgBox / InputBox). Each function waits until the user answers.
// Call them only after the Application has been created.

// --- Messages -------------------------------------------------------------
void show_message(const std::string& text, const std::string& title = "Message");
void show_info(const std::string& text, const std::string& title = "Information");
void show_warning(const std::string& text, const std::string& title = "Warning");
void show_error(const std::string& text, const std::string& title = "Error");

// --- Questions ------------------------------------------------------------
// true = Yes, false = No (or the dialog was closed).
bool ask_yes_no(const std::string& question, const std::string& title = "Question");
// true = OK, false = Cancel.
bool ask_ok_cancel(const std::string& question, const std::string& title = "Confirm");

// --- Asking for values ----------------------------------------------------
// Returns what the user typed, or `default_text` if they pressed Cancel.
std::string input_box(const std::string& prompt, const std::string& title = "Input",
                      const std::string& default_text = "");
// Same, but `ok` tells you whether the user pressed OK.
std::string input_text(const std::string& prompt, bool& ok, const std::string& title = "Input",
                       const std::string& default_text = "");
// Asks for a whole number between min and max. Returns `value` on Cancel.
int input_number(const std::string& prompt, int value = 0, int min = -2147483647, int max = 2147483647,
                 const std::string& title = "Input");
// Lets the user choose one entry from a list. Returns "" on Cancel.
std::string input_choice(const std::string& prompt, const std::vector<std::string>& choices,
                         const std::string& title = "Choose");

// --- Files, folders & colors ---------------------------------------------
// filter examples: "Text files (*.txt)", "Images (*.png *.jpg);;All files (*)".
// Every function returns "" (or an empty list) when the user cancels.
std::string open_file_dialog(const std::string& title = "Open File", const std::string& filter = "All files (*)",
                             const std::string& start_folder = "");
std::vector<std::string> open_files_dialog(const std::string& title = "Open Files",
                                           const std::string& filter = "All files (*)",
                                           const std::string& start_folder = "");
std::string save_file_dialog(const std::string& title = "Save File", const std::string& filter = "All files (*)",
                             const std::string& start_path = "");
std::string select_folder_dialog(const std::string& title = "Select Folder", const std::string& start_folder = "");
// Returns "#rrggbb", or "" on Cancel.
std::string pick_color(const std::string& initial_color = "#ffffff", const std::string& title = "Select Color");

}  // namespace simplegui
