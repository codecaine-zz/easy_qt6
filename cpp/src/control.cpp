#include "simplegui/control.h"
#include <QWidget>
#include <QString>

namespace simplegui {

void Control::set_style(const std::string& style) {
    if (auto* w = get_qwidget()) {
        w->setStyleSheet(QString::fromStdString(style));
    }
}

void Control::set_enabled(bool enabled) {
    if (auto* w = get_qwidget()) {
        w->setEnabled(enabled);
    }
}

bool Control::is_enabled() const {
    if (auto* w = get_qwidget()) {
        return w->isEnabled();
    }
    return false;
}

void Control::set_visible(bool visible) {
    if (auto* w = get_qwidget()) {
        w->setVisible(visible);
    }
}

bool Control::is_visible() const {
    if (auto* w = get_qwidget()) {
        return w->isVisible();
    }
    return false;
}

}
