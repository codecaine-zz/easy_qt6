#pragma once
// Internal helpers shared by the SimpleGUI implementation files.
// This header is private: it is never installed and never included by public headers.

#include "simplegui/event_connection.h"

#include <QColor>
#include <QLabel>
#include <QMetaObject>
#include <QObject>
#include <QPointer>
#include <QString>

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <QWidget>

namespace simplegui::detail {

// ---------------------------------------------------------------------------
// String helpers
// ---------------------------------------------------------------------------
inline QString qs(const std::string& s) { return QString::fromStdString(s); }
inline std::string ss(const QString& s) { return s.toStdString(); }

// Parses a color string ("#RRGGBB", "#AARRGGBB", or an SVG color name such as "red").
// Returns `fallback` when the string is not a valid color. Because the result is
// re-serialised through QColor, it is always safe to embed in a style sheet.
inline QColor parse_color(const std::string& text, const QColor& fallback) {
    QColor c(QString::fromStdString(text).trimmed());
    return c.isValid() ? c : fallback;
}

// Creates a QLabel that never interprets its text as HTML. Use for any text that
// may come from users, files, or the network.
inline QLabel* plain_label(const std::string& text, QWidget* parent = nullptr) {
    auto* label = new QLabel(parent);
    label->setTextFormat(Qt::PlainText);
    label->setText(qs(text));
    return label;
}

// Deletes a widget that was never handed to a parent. Widgets that live inside a
// layout or window are owned (and later deleted) by Qt itself.
template <typename W>
void delete_if_orphan(QPointer<W>& widget) {
    if (widget && !widget->parent()) {
        delete widget.data();
    }
}

// Wraps a Qt connection in a SimpleGUI EventConnection token.
inline EventConnection wrap(QMetaObject::Connection conn) {
    return EventConnection([conn]() { QObject::disconnect(conn); });
}

// ---------------------------------------------------------------------------
// Multi-subscriber event lists
// ---------------------------------------------------------------------------
template <typename... Args>
class HandlerList : public std::enable_shared_from_this<HandlerList<Args...>> {
public:
    using Fn = std::function<void(Args...)>;

    EventConnection add(Fn fn) {
        if (!fn) return {};
        const std::size_t id = ++next_id_;
        entries_.push_back({id, std::move(fn)});
        std::weak_ptr<HandlerList> weak = this->shared_from_this();
        return EventConnection([weak, id]() {
            if (auto self = weak.lock()) self->remove(id);
        });
    }

    template <typename... Vals>
    void fire(Vals&&... vals) const {
        // Copy first so a handler may safely disconnect itself (or others) while running.
        const auto snapshot = entries_;
        for (const auto& entry : snapshot) entry.fn(vals...);
    }

private:
    void remove(std::size_t id) {
        entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
                                      [id](const Entry& e) { return e.id == id; }),
                       entries_.end());
    }

    struct Entry {
        std::size_t id;
        Fn fn;
    };
    std::vector<Entry> entries_;
    std::size_t next_id_ = 0;
};

template <typename... Args>
using Event = std::weak_ptr<HandlerList<Args...>>;

// Keeps a shared object alive for exactly as long as a QObject exists. Handler
// lists are tied to the control's root widget so user lambdas that capture the
// control itself never create a reference cycle.
class Keeper : public QObject {
public:
    Keeper(QObject* owner, std::shared_ptr<void> value) : QObject(owner), value_(std::move(value)) {}

private:
    std::shared_ptr<void> value_;
};

template <typename... Args>
Event<Args...> make_event(QObject* owner) {
    auto list = std::make_shared<HandlerList<Args...>>();
    if (owner) new Keeper(owner, list);
    return list;
}

template <typename... Args>
EventConnection add_handler(const Event<Args...>& event, std::function<void(Args...)> fn) {
    if (auto list = event.lock()) return list->add(std::move(fn));
    return {};
}

template <typename... Args, typename... Vals>
void fire(const Event<Args...>& event, Vals&&... vals) {
    if (auto list = event.lock()) list->fire(std::forward<Vals>(vals)...);
}

// ---------------------------------------------------------------------------
// Style helpers (implemented in control.cpp)
// ---------------------------------------------------------------------------
// Replaces the built-in look of a widget while keeping any colors/fonts the user
// applied through Control::set_text_color(), set_font_size(), and friends.
void set_base_style(QWidget* widget, const QString& css);

// Base class for controls that paint themselves (gauges, charts, LEDs...).
// The "sg_transparent" property lets the application theme keep their
// background see-through, so they blend into panels such as GlassPanel.
class PaintedWidget : public QWidget {
public:
    explicit PaintedWidget(QWidget* parent = nullptr) : QWidget(parent) {
        setProperty("sg_transparent", true);
    }
};

}  // namespace simplegui::detail
