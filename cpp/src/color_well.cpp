#include "simplegui/color_well.h"
#include "detail/common.h"

#include <QColorDialog>
#include <QPushButton>

namespace simplegui {

struct ColorWell::Impl {
    QPointer<QPushButton> button;
    QColor current_color = Qt::white;
    detail::Event<const std::string&> changed;

    explicit Impl(const std::string& hex_color) : button(new QPushButton()) {
        button->setCursor(Qt::PointingHandCursor);
        button->setMinimumSize(36, 24);
        changed = detail::make_event<const std::string&>(button);
        apply(detail::parse_color(hex_color, Qt::white));
    }
    ~Impl() { detail::delete_if_orphan(button); }

    void apply(const QColor& color) {
        current_color = color;
        // Colors are re-serialised by QColor, so user text never reaches the style sheet.
        detail::set_base_style(button, QStringLiteral("QPushButton { background-color: %1; border: 1px solid #888; border-radius: 4px; }")
                                           .arg(color.name()));
    }
};

ColorWell::ColorWell(const std::string& hex_color) : pimpl(std::make_shared<Impl>(hex_color)) {
    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->button.data(), &QPushButton::clicked, [weak]() {
        auto d = weak.lock();
        if (!d || !d->button) return;
        const QColor picked = QColorDialog::getColor(d->current_color, d->button->window(), QStringLiteral("Select Color"));
        // The dialog runs its own event loop; re-check that we still exist afterwards.
        d = weak.lock();
        if (!d || !picked.isValid()) return;
        d->apply(picked);
        detail::fire(d->changed, detail::ss(picked.name()));
    });
}

ColorWell::~ColorWell() = default;

std::string ColorWell::get_color() const {
    return detail::ss(pimpl->current_color.name());
}

void ColorWell::set_color(const std::string& hex_color) {
    const QColor c = detail::parse_color(hex_color, QColor());
    if (c.isValid()) pimpl->apply(c);
}

EventConnection ColorWell::on_change(std::function<void(const std::string&)> handler) {
    return detail::add_handler(pimpl->changed, std::move(handler));
}

QWidget* ColorWell::get_qwidget() const {
    return pimpl->button.data();
}

}  // namespace simplegui
