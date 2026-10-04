#include "simplegui/application.h"
#include <QApplication>
#include <QFont>
#include <QString>

namespace simplegui {

static const char* MODERN_DARK_THEME = R"(
QWidget {
    background-color: #16181d;
    color: #f5f5f7;
    font-family: -apple-system, BlinkMacSystemFont, "SF Pro Text", "SF Pro Display", "Helvetica Neue", sans-serif;
    font-size: 13px;
}

QMainWindow, QDialog {
    background-color: #0d0f12;
}

/* Push Buttons (macOS HIG styling) */
QPushButton {
    background-color: #24272f;
    color: #f5f5f7;
    border: 1px solid #383c48;
    border-radius: 8px;
    padding: 7px 16px;
    font-weight: 500;
}
QPushButton:hover {
    background-color: #2d313b;
    border-color: #4a4f5f;
}
QPushButton:pressed {
    background-color: #1a1c22;
    border-color: #24272f;
}
QPushButton:disabled {
    background-color: #1c1e24;
    color: #636775;
    border-color: #282a32;
}

/* Input Fields (macOS HIG styling) */
QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox {
    background-color: #101216;
    color: #f5f5f7;
    border: 1px solid #2d313b;
    border-radius: 8px;
    padding: 7px 12px;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus {
    border: 1.5px solid #0a84ff;
    background-color: #121419;
}

/* Combo Box & Dropdown */
QComboBox {
    background-color: #24272f;
    color: #f5f5f7;
    border: 1px solid #383c48;
    border-radius: 8px;
    padding: 6px 12px;
    min-height: 22px;
}
QComboBox:hover {
    background-color: #2d313b;
    border-color: #4a4f5f;
}
QComboBox::drop-down {
    border: none;
    width: 22px;
}
QComboBox QAbstractItemView {
    background-color: #1e2027;
    color: #f5f5f7;
    border: 1px solid #383c48;
    selection-background-color: #0a84ff;
    selection-color: #ffffff;
    border-radius: 6px;
    padding: 4px;
}

/* Checkbox & Radio Button */
QCheckBox, QRadioButton {
    color: #f5f5f7;
    spacing: 9px;
}
QCheckBox::indicator, QRadioButton::indicator {
    width: 17px;
    height: 17px;
    border: 1.5px solid #484d5c;
    border-radius: 5px;
    background-color: #1c1e24;
}
QRadioButton::indicator {
    border-radius: 9px;
}
QCheckBox::indicator:hover, QRadioButton::indicator:hover {
    border-color: #686f84;
}
QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: #0a84ff;
    border-color: #0a84ff;
}

/* Sliders (macOS HIG smooth pill) */
QSlider::groove:horizontal {
    height: 5px;
    background: #2d313b;
    border-radius: 2.5px;
}
QSlider::sub-page:horizontal {
    background: #0a84ff;
    border-radius: 2.5px;
}
QSlider::handle:horizontal {
    background: #ffffff;
    border: 1px solid rgba(0, 0, 0, 0.2);
    width: 18px;
    height: 18px;
    margin-top: -6.5px;
    margin-bottom: -6.5px;
    border-radius: 9px;
}
QSlider::handle:horizontal:hover {
    background: #fdfdfd;
    border-color: #0a84ff;
}

/* GroupBox (macOS Inset Group) */
QGroupBox {
    font-weight: 600;
    border: 1px solid #282a32;
    border-radius: 12px;
    margin-top: 22px;
    padding-top: 16px;
    background-color: #181a20;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 14px;
    padding: 0 8px;
    color: #8b92a5;
    font-size: 11px;
    text-transform: uppercase;
    letter-spacing: 0.5px;
}

/* TabWidget (macOS Segmented Bar feel) */
QTabWidget::pane {
    border: 1px solid #282a32;
    background-color: #16181d;
    border-radius: 8px;
}
QTabBar::tab {
    background-color: #202228;
    color: #8b92a5;
    padding: 7px 18px;
    border-top-left-radius: 8px;
    border-top-right-radius: 8px;
    margin-right: 2px;
    font-weight: 500;
}
QTabBar::tab:selected {
    background-color: #16181d;
    color: #ffffff;
    font-weight: 600;
    border-bottom: 2px solid #0a84ff;
}
QTabBar::tab:hover:!selected {
    background-color: #282b33;
    color: #f5f5f7;
}

/* Table / Grid */
QTableWidget, QTableView {
    background-color: #09090b;
    alternate-background-color: #18181b;
    gridline-color: #27272a;
    border: 1px solid #27272a;
    border-radius: 6px;
}
QHeaderView::section {
    background-color: #1e1e24;
    color: #94a3b8;
    padding: 6px 10px;
    font-weight: 600;
    border: none;
    border-right: 1px solid #27272a;
    border-bottom: 1px solid #27272a;
}

/* ScrollBars */
QScrollBar:vertical {
    border: none;
    background: #18181b;
    width: 8px;
    margin: 0px;
}
QScrollBar::handle:vertical {
    background: #3f3f46;
    min-height: 20px;
    border-radius: 4px;
}
QScrollBar::handle:vertical:hover {
    background: #52525b;
}
QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
    height: 0px;
}
)";

static const char* MODERN_LIGHT_THEME = R"(
QWidget {
    background-color: #ffffff;
    color: #0f172a;
    font-size: 13px;
}

QMainWindow, QDialog {
    background-color: #f8fafc;
}

/* Push Buttons */
QPushButton {
    background-color: #f1f5f9;
    color: #0f172a;
    border: 1px solid #cbd5e1;
    border-radius: 6px;
    padding: 6px 14px;
    font-weight: 500;
}
QPushButton:hover {
    background-color: #e2e8f0;
    border-color: #94a3b8;
}
QPushButton:pressed {
    background-color: #cbd5e1;
}

/* Input Fields */
QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox {
    background-color: #ffffff;
    color: #0f172a;
    border: 1px solid #cbd5e1;
    border-radius: 6px;
    padding: 6px 10px;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus {
    border: 1.5px solid #2563eb;
}

/* Combo Box & Dropdown */
QComboBox {
    background-color: #ffffff;
    color: #0f172a;
    border: 1px solid #cbd5e1;
    border-radius: 6px;
    padding: 5px 10px;
    min-height: 20px;
}
QComboBox:hover {
    border-color: #94a3b8;
}
QComboBox::drop-down {
    border: none;
    width: 20px;
}
QComboBox QAbstractItemView {
    background-color: #ffffff;
    color: #0f172a;
    border: 1px solid #cbd5e1;
    selection-background-color: #2563eb;
    selection-color: #ffffff;
    border-radius: 4px;
}

/* Checkbox & Radio Button */
QCheckBox, QRadioButton {
    color: #0f172a;
    spacing: 8px;
}
QCheckBox::indicator, QRadioButton::indicator {
    width: 16px;
    height: 16px;
    border: 1px solid #cbd5e1;
    border-radius: 4px;
    background-color: #ffffff;
}
QRadioButton::indicator {
    border-radius: 8px;
}
QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: #2563eb;
    border-color: #2563eb;
}

/* Sliders */
QSlider::groove:horizontal {
    height: 6px;
    background: #e2e8f0;
    border-radius: 3px;
}
QSlider::sub-page:horizontal {
    background: #2563eb;
    border-radius: 3px;
}
QSlider::handle:horizontal {
    background: #ffffff;
    border: 1px solid #94a3b8;
    width: 16px;
    margin-top: -5px;
    margin-bottom: -5px;
    border-radius: 8px;
}

/* GroupBox */
QGroupBox {
    font-weight: 600;
    border: 1px solid #e2e8f0;
    border-radius: 8px;
    margin-top: 20px;
    padding-top: 14px;
    background-color: #ffffff;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 12px;
    padding: 0 6px;
    color: #475569;
}

/* TabWidget */
QTabWidget::pane {
    border: 1px solid #e2e8f0;
    background-color: #ffffff;
    border-radius: 6px;
}
QTabBar::tab {
    background-color: #f1f5f9;
    color: #64748b;
    padding: 8px 16px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    margin-right: 2px;
}
QTabBar::tab:selected {
    background-color: #ffffff;
    color: #0f172a;
    font-weight: 600;
    border-bottom: 2px solid #2563eb;
}
QTabBar::tab:hover:!selected {
    background-color: #e2e8f0;
}

/* Table / Grid */
QTableWidget, QTableView {
    background-color: #ffffff;
    alternate-background-color: #f8fafc;
    gridline-color: #f1f5f9;
    border: 1px solid #e2e8f0;
    border-radius: 6px;
}
QHeaderView::section {
    background-color: #f1f5f9;
    color: #475569;
    padding: 6px 10px;
    font-weight: 600;
    border: none;
    border-right: 1px solid #e2e8f0;
    border-bottom: 1px solid #e2e8f0;
}
)";

static const char* ADWAITA_DARK_THEME = R"(
QWidget {
    background-color: #242424;
    color: #ffffff;
    font-family: "Cantarell", "Inter", "Ubuntu", "DejaVu Sans", sans-serif;
    font-size: 13px;
}

QMainWindow, QDialog {
    background-color: #1e1e1e;
}

QPushButton {
    background-color: #383838;
    color: #ffffff;
    border: none;
    border-radius: 8px;
    padding: 8px 16px;
    font-weight: 600;
}
QPushButton:hover {
    background-color: #454545;
}
QPushButton:pressed {
    background-color: #2c2c2c;
}

QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox {
    background-color: #303030;
    color: #ffffff;
    border: 1px solid #424242;
    border-radius: 8px;
    padding: 8px 12px;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus {
    border: 2px solid #3584e4;
    background-color: #343434;
}

QComboBox {
    background-color: #383838;
    color: #ffffff;
    border: none;
    border-radius: 8px;
    padding: 7px 14px;
}
QComboBox:hover {
    background-color: #454545;
}
QComboBox::drop-down {
    border: none;
    width: 24px;
}
QComboBox QAbstractItemView {
    background-color: #303030;
    color: #ffffff;
    border: 1px solid #484848;
    selection-background-color: #3584e4;
    selection-color: #ffffff;
    border-radius: 6px;
    padding: 4px;
}

QCheckBox, QRadioButton {
    color: #ffffff;
    spacing: 10px;
}
QCheckBox::indicator, QRadioButton::indicator {
    width: 18px;
    height: 18px;
    border-radius: 6px;
    background-color: #383838;
    border: 1.5px solid #4e4e4e;
}
QRadioButton::indicator {
    border-radius: 10px;
}
QCheckBox::indicator:hover, QRadioButton::indicator:hover {
    border-color: #707070;
}
QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: #3584e4;
    border-color: #3584e4;
}

QSlider::groove:horizontal {
    height: 6px;
    background: #383838;
    border-radius: 3px;
}
QSlider::sub-page:horizontal {
    background: #3584e4;
    border-radius: 3px;
}
QSlider::handle:horizontal {
    background: #ffffff;
    width: 20px;
    height: 20px;
    margin-top: -7px;
    margin-bottom: -7px;
    border-radius: 10px;
}

QGroupBox {
    font-weight: 700;
    border: 1px solid #363636;
    border-radius: 12px;
    margin-top: 22px;
    padding-top: 16px;
    background-color: #2b2b2b;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 14px;
    padding: 0 8px;
    color: #9a9996;
    font-size: 11px;
    text-transform: uppercase;
    letter-spacing: 0.5px;
}

QTabWidget::pane {
    border: 1px solid #363636;
    background-color: #242424;
    border-radius: 8px;
}
QTabBar::tab {
    background-color: #2e2e2e;
    color: #9a9996;
    padding: 8px 20px;
    border-top-left-radius: 8px;
    border-top-right-radius: 8px;
    margin-right: 3px;
    font-weight: 600;
}
QTabBar::tab:selected {
    background-color: #242424;
    color: #ffffff;
    border-bottom: 2px solid #3584e4;
}
QTabBar::tab:hover:!selected {
    background-color: #383838;
    color: #ffffff;
}

QTableWidget, QTableView {
    background-color: #1e1e1e;
    alternate-background-color: #262626;
    gridline-color: transparent;
    border: 1px solid #363636;
    border-radius: 8px;
    color: #ffffff;
}
QHeaderView::section {
    background-color: #2d2d2d;
    color: #9a9996;
    padding: 7px 12px;
    font-weight: 700;
    border: none;
    border-bottom: 1px solid #363636;
    text-transform: uppercase;
}

QScrollBar:vertical {
    border: none;
    background: transparent;
    width: 8px;
    margin: 0px;
}
QScrollBar::handle:vertical {
    background: #4a4a4a;
    min-height: 24px;
    border-radius: 4px;
}
QScrollBar::handle:vertical:hover {
    background: #606060;
}
)";

static const char* BREEZE_DARK_THEME = R"(
QWidget {
    background-color: #232629;
    color: #eff0f1;
    font-family: "Noto Sans", "Cantarell", sans-serif;
    font-size: 13px;
}
QMainWindow, QDialog {
    background-color: #1b1e20;
}
QPushButton {
    background-color: #31363b;
    color: #eff0f1;
    border: 1px solid #474e54;
    border-radius: 4px;
    padding: 7px 16px;
    font-weight: 500;
}
QPushButton:hover {
    background-color: #3d4349;
    border-color: #3daee9;
}
QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox {
    background-color: #1b1e20;
    color: #eff0f1;
    border: 1px solid #474e54;
    border-radius: 4px;
    padding: 6px 10px;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus {
    border: 1.5px solid #3daee9;
}
QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: #3daee9;
    border-color: #3daee9;
}
QSlider::sub-page:horizontal {
    background: #3daee9;
}
)";

static const char* YARU_DARK_THEME = R"(
QWidget {
    background-color: #1e1e1e;
    color: #ffffff;
    font-family: "Ubuntu", "Cantarell", sans-serif;
    font-size: 13px;
}
QMainWindow, QDialog {
    background-color: #181818;
}
QPushButton {
    background-color: #2d2d2d;
    color: #ffffff;
    border: 1px solid #3c3c3c;
    border-radius: 6px;
    padding: 7px 16px;
    font-weight: 600;
}
QPushButton:hover {
    background-color: #383838;
    border-color: #e95420;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus {
    border: 1.5px solid #e95420;
}
QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: #e95420;
    border-color: #e95420;
}
)";

static const char* WINDOWS_FLUENT_THEME = R"(
QWidget {
    background-color: #202020;
    color: #ffffff;
    font-family: "Segoe UI Variable Text", "Segoe UI", sans-serif;
    font-size: 13px;
}

QMainWindow, QDialog {
    background-color: #1a1a1a;
}

QPushButton {
    background-color: #2d2d2d;
    color: #ffffff;
    border: 1px solid #3c3c3c;
    border-radius: 4px;
    padding: 6px 16px;
    font-weight: 500;
}
QPushButton:hover {
    background-color: #383838;
    border-color: #555555;
}
QPushButton:pressed {
    background-color: #262626;
    border-color: #333333;
}

QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox {
    background-color: #1f1f1f;
    color: #ffffff;
    border: 1px solid #383838;
    border-bottom: 2px solid #858585;
    border-radius: 4px;
    padding: 6px 10px;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus {
    border-bottom: 2px solid #60cdff;
    background-color: #1f1f1f;
}

QComboBox {
    background-color: #2d2d2d;
    color: #ffffff;
    border: 1px solid #3c3c3c;
    border-radius: 4px;
    padding: 5px 12px;
}
QComboBox:hover {
    background-color: #383838;
}

QCheckBox, QRadioButton {
    color: #ffffff;
    spacing: 8px;
}
QCheckBox::indicator, QRadioButton::indicator {
    width: 18px;
    height: 18px;
    border-radius: 4px;
    background-color: #2d2d2d;
    border: 1px solid #484848;
}
QRadioButton::indicator {
    border-radius: 9px;
}
QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: #0078d4;
    border-color: #0078d4;
}

QSlider::groove:horizontal {
    height: 4px;
    background: #383838;
    border-radius: 2px;
}
QSlider::sub-page:horizontal {
    background: #60cdff;
    border-radius: 2px;
}
QSlider::handle:horizontal {
    background: #ffffff;
    border: 1px solid rgba(0, 0, 0, 0.3);
    width: 18px;
    height: 18px;
    margin-top: -7px;
    margin-bottom: -7px;
    border-radius: 9px;
}

QGroupBox {
    font-weight: 600;
    border: 1px solid #333333;
    border-radius: 8px;
    margin-top: 20px;
    padding-top: 14px;
    background-color: #262626;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 12px;
    padding: 0 6px;
    color: #a0a0a0;
    font-size: 11px;
}

QTabWidget::pane {
    border: 1px solid #333333;
    background-color: #202020;
    border-radius: 6px;
}
QTabBar::tab {
    background-color: #2a2a2a;
    color: #a0a0a0;
    padding: 7px 16px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    margin-right: 2px;
}
QTabBar::tab:selected {
    background-color: #202020;
    color: #ffffff;
    border-bottom: 2px solid #60cdff;
}

QTableWidget, QTableView {
    background-color: #1b1b1b;
    alternate-background-color: #242424;
    gridline-color: transparent;
    border: 1px solid #333333;
    border-radius: 6px;
    color: #ffffff;
}
QHeaderView::section {
    background-color: #262626;
    color: #a0a0a0;
    padding: 6px 10px;
    font-weight: 600;
    border: none;
    border-bottom: 1px solid #333333;
}
)";

// Futuristic "sci-fi console" look: deep navy background with cyan neon accents.
static const char* NEON_THEME = R"(
QWidget {
    background-color: #05070f;
    color: #c8f7ff;
    font-family: "JetBrains Mono", "Cascadia Code", "SF Mono", Menlo, Consolas, monospace;
    font-size: 13px;
}
QMainWindow, QDialog { background-color: #03040a; }
QPushButton {
    background-color: rgba(0, 229, 255, 0.08);
    color: #00e5ff;
    border: 1px solid #00e5ff;
    border-radius: 4px;
    padding: 6px 16px;
    font-weight: 600;
}
QPushButton:hover { background-color: rgba(0, 229, 255, 0.22); color: #ffffff; }
QPushButton:pressed { background-color: rgba(0, 229, 255, 0.35); }
QPushButton:disabled { color: #2b4a55; border-color: #1a2a33; }
QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox, QDateEdit {
    background-color: #0a0f1e;
    color: #e0fbff;
    border: 1px solid #1b3a4b;
    border-radius: 4px;
    padding: 5px 8px;
    selection-background-color: #ff2bd6;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus, QComboBox:focus { border: 1px solid #00e5ff; }
QComboBox QAbstractItemView { background-color: #0a0f1e; color: #e0fbff; selection-background-color: #00e5ff; selection-color: #05070f; }
QCheckBox, QRadioButton { spacing: 8px; }
QCheckBox::indicator, QRadioButton::indicator { width: 14px; height: 14px; border: 1px solid #00e5ff; background: #0a0f1e; }
QRadioButton::indicator { border-radius: 7px; }
QCheckBox::indicator:checked, QRadioButton::indicator:checked { background: #00e5ff; }
QSlider::groove:horizontal { height: 4px; background: #1b3a4b; border-radius: 2px; }
QSlider::sub-page:horizontal { background: #00e5ff; border-radius: 2px; }
QSlider::handle:horizontal { background: #ff2bd6; width: 14px; margin: -6px 0; border-radius: 7px; }
QProgressBar { background: #0a0f1e; border: 1px solid #1b3a4b; border-radius: 4px; text-align: center; color: #e0fbff; }
QProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #00e5ff, stop:1 #ff2bd6); border-radius: 3px; }
QGroupBox { border: 1px solid #1b3a4b; border-radius: 6px; margin-top: 14px; padding-top: 8px; }
QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 4px; color: #00e5ff; }
QTabWidget::pane { border: 1px solid #1b3a4b; border-radius: 4px; }
QTabBar::tab { background: #0a0f1e; color: #6c8a99; padding: 6px 14px; border: 1px solid #1b3a4b; border-bottom: none; }
QTabBar::tab:selected { color: #00e5ff; border-color: #00e5ff; }
QMenuBar, QMenu { background-color: #0a0f1e; color: #c8f7ff; }
QMenu::item:selected, QMenuBar::item:selected { background-color: #00e5ff; color: #05070f; }
QStatusBar { background: #03040a; color: #6c8a99; border-top: 1px solid #1b3a4b; }
QScrollBar:vertical { background: #05070f; width: 10px; }
QScrollBar::handle:vertical { background: #1b3a4b; border-radius: 5px; min-height: 24px; }
QScrollBar::handle:vertical:hover { background: #00e5ff; }
QToolTip { background-color: #0a0f1e; color: #00e5ff; border: 1px solid #00e5ff; }
QHeaderView::section { background-color: #0a0f1e; color: #00e5ff; border: none; border-bottom: 1px solid #1b3a4b; padding: 6px 10px; }
)";

namespace {

struct ThemeEntry {
    const char* name;
    const char* const* css;  // pointer to the stylesheet constant (nullptr = Qt default look)
};

// Every accepted theme name. Several names share one look so people can use the
// word they already know ("gnome", "kde", "win11", ...).
const ThemeEntry kThemes[] = {
    {"dark", &MODERN_DARK_THEME},         {"modern_dark", &MODERN_DARK_THEME},
    {"apple", &MODERN_DARK_THEME},        {"apple_dark", &MODERN_DARK_THEME},
    {"linux", &ADWAITA_DARK_THEME},       {"adwaita", &ADWAITA_DARK_THEME},
    {"linux_adwaita", &ADWAITA_DARK_THEME}, {"gnome", &ADWAITA_DARK_THEME},
    {"breeze", &BREEZE_DARK_THEME},       {"kde", &BREEZE_DARK_THEME},
    {"linux_breeze", &BREEZE_DARK_THEME},
    {"yaru", &YARU_DARK_THEME},           {"ubuntu", &YARU_DARK_THEME},
    {"linux_yaru", &YARU_DARK_THEME},
    {"windows", &WINDOWS_FLUENT_THEME},   {"fluent", &WINDOWS_FLUENT_THEME},
    {"windows_dark", &WINDOWS_FLUENT_THEME}, {"win11", &WINDOWS_FLUENT_THEME},
    {"light", &MODERN_LIGHT_THEME},       {"modern_light", &MODERN_LIGHT_THEME},
    {"neon", &NEON_THEME},                {"cyber", &NEON_THEME},
    {"futuristic", &NEON_THEME},
    {"default", nullptr},
};

}  // namespace

struct Application::Impl {
    QApplication qapp;
    std::string theme = "default";
    Impl(int& argc, char** argv) : qapp(argc, argv) {}
};

Application::Application(int& argc, char** argv)
    : pimpl(std::make_unique<Impl>(argc, argv)) {}

Application::~Application() = default;

int Application::run() {
    return pimpl->qapp.exec();
}

bool Application::set_theme(const std::string& theme_name) {
    for (const auto& t : kThemes) {
        if (theme_name == t.name) {
            set_stylesheet(t.css ? *t.css : "");
            pimpl->theme = theme_name;
            return true;
        }
    }
    return false;  // unknown name: the current look is kept
}

std::string Application::theme() const { return pimpl->theme; }

std::vector<std::string> Application::available_themes() {
    std::vector<std::string> names;
    for (const auto& t : kThemes) names.emplace_back(t.name);
    return names;
}

void Application::set_stylesheet(const std::string& qss) {
    pimpl->qapp.setStyleSheet(QString::fromStdString(qss));
}

void Application::set_app_name(const std::string& name) {
    QApplication::setApplicationName(QString::fromStdString(name));
    QApplication::setApplicationDisplayName(QString::fromStdString(name));
}

void Application::set_font(const std::string& family, int point_size) {
    QFont f = QApplication::font();
    if (!family.empty()) f.setFamily(QString::fromStdString(family));
    if (point_size > 0) f.setPointSize(point_size);
    QApplication::setFont(f);
}

void Application::quit(int exit_code) {
    if (QCoreApplication::instance()) QCoreApplication::exit(exit_code);
}

void Application::process_events() {
    if (QCoreApplication::instance()) QCoreApplication::processEvents();
}

}  // namespace simplegui
