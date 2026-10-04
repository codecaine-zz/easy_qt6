#include "simplegui/dialogs.h"
#include "detail/common.h"

#include <QApplication>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QStringList>

#include <algorithm>

namespace simplegui {
namespace {

// Dialogs are centred over the active window when there is one.
QWidget* parent_window() { return QApplication::activeWindow(); }

bool app_ready() { return QApplication::instance() != nullptr; }

// Message boxes always show plain text, so "<b>" in user data is never treated as HTML.
void message(QMessageBox::Icon icon, const std::string& text, const std::string& title) {
    if (!app_ready()) return;
    QMessageBox box(icon, detail::qs(title), QString(), QMessageBox::Ok, parent_window());
    box.setTextFormat(Qt::PlainText);
    box.setText(detail::qs(text));
    box.exec();
}

bool question(const std::string& text, const std::string& title, QMessageBox::StandardButtons buttons,
              QMessageBox::StandardButton yes) {
    if (!app_ready()) return false;
    QMessageBox box(QMessageBox::Question, detail::qs(title), QString(), buttons, parent_window());
    box.setTextFormat(Qt::PlainText);
    box.setText(detail::qs(text));
    return box.exec() == yes;
}

std::vector<std::string> to_vector(const QStringList& list) {
    std::vector<std::string> out;
    out.reserve(static_cast<size_t>(list.size()));
    for (const QString& s : list) out.push_back(detail::ss(s));
    return out;
}

}  // namespace

void show_message(const std::string& text, const std::string& title) { message(QMessageBox::NoIcon, text, title); }
void show_info(const std::string& text, const std::string& title) { message(QMessageBox::Information, text, title); }
void show_warning(const std::string& text, const std::string& title) { message(QMessageBox::Warning, text, title); }
void show_error(const std::string& text, const std::string& title) { message(QMessageBox::Critical, text, title); }

bool ask_yes_no(const std::string& q, const std::string& title) {
    return question(q, title, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

bool ask_ok_cancel(const std::string& q, const std::string& title) {
    return question(q, title, QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
}

std::string input_text(const std::string& prompt, bool& ok, const std::string& title, const std::string& default_text) {
    ok = false;
    if (!app_ready()) return default_text;
    const QString result = QInputDialog::getText(parent_window(), detail::qs(title), detail::qs(prompt),
                                                 QLineEdit::Normal, detail::qs(default_text), &ok);
    return ok ? detail::ss(result) : default_text;
}

std::string input_box(const std::string& prompt, const std::string& title, const std::string& default_text) {
    bool ok = false;
    return input_text(prompt, ok, title, default_text);
}

int input_number(const std::string& prompt, int value, int min, int max, const std::string& title) {
    if (!app_ready()) return value;
    if (min > max) std::swap(min, max);
    bool ok = false;
    const int result = QInputDialog::getInt(parent_window(), detail::qs(title), detail::qs(prompt),
                                            std::clamp(value, min, max), min, max, 1, &ok);
    return ok ? result : value;
}

std::string input_choice(const std::string& prompt, const std::vector<std::string>& choices, const std::string& title) {
    if (!app_ready() || choices.empty()) return {};
    QStringList items;
    for (const auto& c : choices) items << detail::qs(c);
    bool ok = false;
    const QString result = QInputDialog::getItem(parent_window(), detail::qs(title), detail::qs(prompt), items, 0, false, &ok);
    return ok ? detail::ss(result) : std::string();
}

std::string open_file_dialog(const std::string& title, const std::string& filter, const std::string& start_folder) {
    if (!app_ready()) return {};
    return detail::ss(QFileDialog::getOpenFileName(parent_window(), detail::qs(title), detail::qs(start_folder), detail::qs(filter)));
}

std::vector<std::string> open_files_dialog(const std::string& title, const std::string& filter, const std::string& start_folder) {
    if (!app_ready()) return {};
    return to_vector(QFileDialog::getOpenFileNames(parent_window(), detail::qs(title), detail::qs(start_folder), detail::qs(filter)));
}

std::string save_file_dialog(const std::string& title, const std::string& filter, const std::string& start_path) {
    if (!app_ready()) return {};
    return detail::ss(QFileDialog::getSaveFileName(parent_window(), detail::qs(title), detail::qs(start_path), detail::qs(filter)));
}

std::string select_folder_dialog(const std::string& title, const std::string& start_folder) {
    if (!app_ready()) return {};
    return detail::ss(QFileDialog::getExistingDirectory(parent_window(), detail::qs(title), detail::qs(start_folder)));
}

std::string pick_color(const std::string& initial_color, const std::string& title) {
    if (!app_ready()) return {};
    const QColor c = QColorDialog::getColor(detail::parse_color(initial_color, Qt::white), parent_window(), detail::qs(title));
    return c.isValid() ? detail::ss(c.name()) : std::string();
}

}  // namespace simplegui
