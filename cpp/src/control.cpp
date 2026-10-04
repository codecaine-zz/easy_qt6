#include "simplegui/control.h"
#include "detail/common.h"

#include <QFont>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QTextDocument>
#include <QVariant>
#include <QWidget>

#include <algorithm>
#include <unordered_map>

namespace simplegui {

namespace {

// ---------------------------------------------------------------------------
// Name registry (GUI thread only, like all Qt widgets)
// ---------------------------------------------------------------------------
struct RegistryEntry {
    std::weak_ptr<Control> weak;
    const Control* raw = nullptr;
};

std::unordered_map<std::string, RegistryEntry>& registry() {
    static std::unordered_map<std::string, RegistryEntry> map;
    return map;
}

void unregister(const std::string& name, const Control* who) {
    if (name.empty()) return;
    auto& map = registry();
    auto it = map.find(name);
    if (it != map.end() && it->second.raw == who) map.erase(it);
}

// ---------------------------------------------------------------------------
// Style composition
//
// A widget's final style sheet is built from two layers:
//   1. its base look: the built-in style (captured the first time we touch the
//      widget) or whatever the user passed to set_style();
//   2. the friendly properties (text color, background, font) written as a rule
//      that matches only this widget through a unique dynamic property.
// ---------------------------------------------------------------------------
constexpr const char* kBase = "sg_base";
constexpr const char* kBaseSet = "sg_base_set";
constexpr const char* kId = "sg_id";
constexpr const char* kColor = "sg_color";
constexpr const char* kBg = "sg_bg";
constexpr const char* kFontPx = "sg_font_px";
constexpr const char* kBold = "sg_bold";
constexpr const char* kFamily = "sg_family";

QString wrap_declarations(const QString& css, bool self_painted) {
    const QString trimmed = css.trimmed();
    if (trimmed.isEmpty()) return {};
    // Qt treats a selector-less style sheet as "* { ... }"; do the same explicitly so
    // it can be combined with our own rule below.
    if (trimmed.contains(QLatin1Char('{'))) return trimmed;
    QString out = QStringLiteral("* { %1 }").arg(trimmed);
    // "* { background... }" also reaches every child. Keep self-painted children
    // (gauges, charts, LEDs...) see-through so they don't get an opaque box.
    if (!self_painted) out += QStringLiteral("\n*[sg_transparent=\"true\"] { background: transparent; }");
    return out;
}

void capture_base(QWidget* w) {
    if (!w->property(kBaseSet).toBool()) {
        w->setProperty(kBase, w->styleSheet());
        w->setProperty(kBaseSet, true);
    }
}

int widget_id(QWidget* w) {
    static int next_id = 0;
    QVariant v = w->property(kId);
    if (!v.isValid()) {
        v = ++next_id;
        w->setProperty(kId, v);
    }
    return v.toInt();
}

void rebuild_style(QWidget* w) {
    QString css = wrap_declarations(w->property(kBase).toString(), w->property("sg_transparent").toBool());

    QStringList decls;
    const QString color = w->property(kColor).toString();
    const QString bg = w->property(kBg).toString();
    const int font_px = w->property(kFontPx).toInt();
    const QVariant bold = w->property(kBold);
    const QString family = w->property(kFamily).toString();

    if (!color.isEmpty()) decls << QStringLiteral("color: %1;").arg(color);
    if (!bg.isEmpty()) decls << QStringLiteral("background-color: %1;").arg(bg);
    if (font_px > 0) decls << QStringLiteral("font-size: %1px;").arg(font_px);
    if (bold.isValid()) decls << QStringLiteral("font-weight: %1;").arg(bold.toBool() ? "bold" : "normal");
    if (!family.isEmpty()) decls << QStringLiteral("font-family: \"%1\";").arg(family);

    if (!decls.isEmpty()) {
        css += QStringLiteral("\n*[%1=\"%2\"] { %3 }").arg(kId).arg(widget_id(w)).arg(decls.join(QLatin1Char(' ')));
    }
    w->setStyleSheet(css);
}

void set_style_property(QWidget* w, const char* key, const QVariant& value) {
    if (!w) return;
    capture_base(w);
    w->setProperty(key, value);
    rebuild_style(w);
}

}  // namespace

namespace detail {

void set_base_style(QWidget* widget, const QString& css) {
    if (!widget) return;
    widget->setProperty(kBase, css);
    widget->setProperty(kBaseSet, true);
    rebuild_style(widget);
}

}  // namespace detail

// ---------------------------------------------------------------------------
// Control
// ---------------------------------------------------------------------------
Control::~Control() {
    unregister(name_, this);
}

void Control::set_enabled(bool enabled) {
    if (auto* w = get_qwidget()) w->setEnabled(enabled);
}

bool Control::is_enabled() const {
    if (auto* w = get_qwidget()) return w->isEnabled();
    return false;
}

void Control::set_visible(bool visible) {
    if (auto* w = get_qwidget()) w->setVisible(visible);
}

bool Control::is_visible() const {
    // Reports the control's own setting, even before the window is shown.
    if (auto* w = get_qwidget()) return !w->isHidden();
    return false;
}

void Control::show() { set_visible(true); }
void Control::hide() { set_visible(false); }

void Control::set_width(int width) {
    if (auto* w = get_qwidget()) w->setFixedWidth(std::max(0, width));
}

void Control::set_height(int height) {
    if (auto* w = get_qwidget()) w->setFixedHeight(std::max(0, height));
}

void Control::set_size(int width, int height) {
    if (auto* w = get_qwidget()) w->setFixedSize(std::max(0, width), std::max(0, height));
}

void Control::set_min_size(int width, int height) {
    if (auto* w = get_qwidget()) w->setMinimumSize(std::max(0, width), std::max(0, height));
}

void Control::set_max_size(int width, int height) {
    if (auto* w = get_qwidget()) w->setMaximumSize(std::max(0, width), std::max(0, height));
}

int Control::width() const {
    if (auto* w = get_qwidget()) return w->width();
    return 0;
}

int Control::height() const {
    if (auto* w = get_qwidget()) return w->height();
    return 0;
}

void Control::set_tooltip(const std::string& text) {
    if (auto* w = get_qwidget()) {
        // Qt guesses whether a tooltip is HTML. Convert text that *looks* like HTML
        // so it is always displayed literally (never interpreted).
        const QString q = detail::qs(text);
        w->setProperty("sg_tooltip", q);
        w->setToolTip(Qt::mightBeRichText(q) ? Qt::convertFromPlainText(q) : q);
    }
}

std::string Control::tooltip() const {
    if (auto* w = get_qwidget()) return detail::ss(w->property("sg_tooltip").toString());
    return {};
}

void Control::set_text_color(const std::string& color) {
    const QColor c = detail::parse_color(color, QColor());
    if (c.isValid()) set_style_property(get_qwidget(), kColor, c.name(QColor::HexArgb));
}

void Control::set_background_color(const std::string& color) {
    const QColor c = detail::parse_color(color, QColor());
    if (c.isValid()) set_style_property(get_qwidget(), kBg, c.name(QColor::HexArgb));
}

void Control::set_font_size(int pixels) {
    if (pixels > 0) set_style_property(get_qwidget(), kFontPx, std::min(pixels, 512));
}

void Control::set_bold(bool bold) {
    set_style_property(get_qwidget(), kBold, bold);
}

void Control::set_font(const std::string& family) {
    // Strip characters that could break out of the style sheet string.
    QString clean = detail::qs(family);
    clean.remove(QRegularExpression(QStringLiteral("[\"\\\\;{}]")));
    set_style_property(get_qwidget(), kFamily, clean.trimmed());
}

void Control::set_style(const std::string& style) {
    if (auto* w = get_qwidget()) detail::set_base_style(w, detail::qs(style));
}

void Control::set_focus() {
    if (auto* w = get_qwidget()) w->setFocus(Qt::OtherFocusReason);
}

bool Control::has_focus() const {
    if (auto* w = get_qwidget()) return w->hasFocus();
    return false;
}

void Control::set_name(const std::string& name) {
    unregister(name_, this);
    name_ = name;
    if (auto* w = get_qwidget()) w->setObjectName(detail::qs(name));
    if (!name_.empty()) registry()[name_] = RegistryEntry{weak_from_this(), this};
}

std::string Control::name() const {
    return name_;
}

std::shared_ptr<Control> find_control(const std::string& name) {
    auto& map = registry();
    auto it = map.find(name);
    if (it == map.end()) return nullptr;
    auto control = it->second.weak.lock();
    if (!control) map.erase(it);
    return control;
}

}  // namespace simplegui
