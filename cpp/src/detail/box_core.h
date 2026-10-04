#pragma once
// Shared implementation for every container that stacks children in a row or column
// (VBox, HBox, GroupBox, GlassPanel). Private to the library.

#include "simplegui/control.h"
#include "detail/common.h"

#include <QBoxLayout>
#include <QLayoutItem>
#include <QWidget>

#include <algorithm>
#include <memory>
#include <vector>

namespace simplegui::detail {

class BoxCore {
public:
    BoxCore(QWidget* host, QBoxLayout* layout) : host_(host), layout_(layout) {}

    void add_child(const std::shared_ptr<Control>& control, int stretch) {
        if (!control || !layout_) return;
        QWidget* w = control->get_qwidget();
        if (!w) return;
        children_.push_back(control);  // keep the C++ object alive as long as the layout
        layout_->addWidget(w, std::max(0, stretch));
    }

    void add_stretch(int stretch) {
        if (layout_) layout_->addStretch(std::max(0, stretch));
    }

    void add_spacing(int pixels) {
        if (layout_) layout_->addSpacing(std::max(0, pixels));
    }

    void set_spacing(int pixels) {
        if (layout_) layout_->setSpacing(std::max(0, pixels));
    }

    void set_margins(int left, int top, int right, int bottom) {
        if (layout_) layout_->setContentsMargins(std::max(0, left), std::max(0, top), std::max(0, right), std::max(0, bottom));
    }

    // Takes a child out of the layout. The control is not destroyed and can be added elsewhere.
    void remove_child(const std::shared_ptr<Control>& control) {
        if (!control) return;
        auto it = std::find(children_.begin(), children_.end(), control);
        if (it == children_.end()) return;
        if (QWidget* w = control->get_qwidget()) {
            if (layout_) layout_->removeWidget(w);
            w->hide();
            w->setParent(nullptr);  // the control now owns its widget again
        }
        children_.erase(it);
    }

    // Removes every child, stretch, and spacing.
    void clear() {
        if (layout_) {
            while (QLayoutItem* item = layout_->takeAt(0)) {
                if (QWidget* w = item->widget()) {
                    w->hide();
                    w->setParent(nullptr);
                }
                delete item;  // deletes the layout slot, never the widget
            }
        }
        children_.clear();
    }

    int count() const { return static_cast<int>(children_.size()); }

private:
    QPointer<QWidget> host_;
    QPointer<QBoxLayout> layout_;
    std::vector<std::shared_ptr<Control>> children_;
};

}  // namespace simplegui::detail
