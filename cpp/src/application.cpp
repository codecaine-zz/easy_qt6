#include "simplegui/application.h"
#include <QApplication>
#include <QString>

namespace simplegui {

static const char* MODERN_DARK_THEME = R"(
QWidget {
    background-color: #18181b;
    color: #f4f4f5;
    font-size: 13px;
}

QMainWindow, QDialog {
    background-color: #121215;
}

/* Push Buttons */
QPushButton {
    background-color: #27272a;
    color: #f4f4f5;
    border: 1px solid #3f3f46;
    border-radius: 6px;
    padding: 6px 14px;
    font-weight: 500;
}
QPushButton:hover {
    background-color: #3f3f46;
    border-color: #52525b;
}
QPushButton:pressed {
    background-color: #18181b;
    border-color: #27272a;
}
QPushButton:disabled {
    background-color: #18181b;
    color: #71717a;
    border-color: #27272a;
}

/* Input Fields */
QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox {
    background-color: #09090b;
    color: #f4f4f5;
    border: 1px solid #3f3f46;
    border-radius: 6px;
    padding: 6px 10px;
}
QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus, QSpinBox:focus {
    border: 1.5px solid #3b82f6;
}

/* Combo Box & Dropdown */
QComboBox {
    background-color: #27272a;
    color: #f4f4f5;
    border: 1px solid #3f3f46;
    border-radius: 6px;
    padding: 5px 10px;
    min-height: 20px;
}
QComboBox:hover {
    border-color: #52525b;
}
QComboBox::drop-down {
    border: none;
    width: 20px;
}
QComboBox QAbstractItemView {
    background-color: #27272a;
    color: #f4f4f5;
    border: 1px solid #3f3f46;
    selection-background-color: #2563eb;
    selection-color: #ffffff;
    border-radius: 4px;
}

/* Checkbox & Radio Button */
QCheckBox, QRadioButton {
    color: #e2e8f0;
    spacing: 8px;
}
QCheckBox::indicator, QRadioButton::indicator {
    width: 16px;
    height: 16px;
    border: 1px solid #52525b;
    border-radius: 4px;
    background-color: #18181b;
}
QRadioButton::indicator {
    border-radius: 8px;
}
QCheckBox::indicator:checked, QRadioButton::indicator:checked {
    background-color: #2563eb;
    border-color: #3b82f6;
}

/* Sliders */
QSlider::groove:horizontal {
    height: 6px;
    background: #3f3f46;
    border-radius: 3px;
}
QSlider::sub-page:horizontal {
    background: #2563eb;
    border-radius: 3px;
}
QSlider::handle:horizontal {
    background: #f4f4f5;
    border: 1px solid #71717a;
    width: 16px;
    margin-top: -5px;
    margin-bottom: -5px;
    border-radius: 8px;
}
QSlider::handle:horizontal:hover {
    background: #ffffff;
    border-color: #3b82f6;
}

/* GroupBox */
QGroupBox {
    font-weight: 600;
    border: 1px solid #27272a;
    border-radius: 8px;
    margin-top: 20px;
    padding-top: 14px;
    background-color: #1a1a1e;
}
QGroupBox::title {
    subcontrol-origin: margin;
    subcontrol-position: top left;
    left: 12px;
    padding: 0 6px;
    color: #94a3b8;
}

/* TabWidget */
QTabWidget::pane {
    border: 1px solid #27272a;
    background-color: #18181b;
    border-radius: 6px;
}
QTabBar::tab {
    background-color: #27272a;
    color: #a1a1aa;
    padding: 8px 16px;
    border-top-left-radius: 6px;
    border-top-right-radius: 6px;
    margin-right: 2px;
}
QTabBar::tab:selected {
    background-color: #18181b;
    color: #ffffff;
    font-weight: 600;
    border-bottom: 2px solid #2563eb;
}
QTabBar::tab:hover:!selected {
    background-color: #3f3f46;
    color: #f4f4f5;
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

struct Application::Impl {
    QApplication qapp;
    Impl(int& argc, char** argv) : qapp(argc, argv) {}
};

Application::Application(int& argc, char** argv)
    : pimpl(std::make_unique<Impl>(argc, argv)) {}

Application::~Application() = default;

int Application::run() {
    return pimpl->qapp.exec();
}

void Application::set_theme(const std::string& theme_name) {
    if (theme_name == "dark" || theme_name == "modern_dark") {
        set_stylesheet(MODERN_DARK_THEME);
    } else if (theme_name == "light" || theme_name == "modern_light") {
        set_stylesheet(MODERN_LIGHT_THEME);
    } else if (theme_name == "default") {
        set_stylesheet("");
    }
}

void Application::set_stylesheet(const std::string& qss) {
    pimpl->qapp.setStyleSheet(QString::fromStdString(qss));
}

}
