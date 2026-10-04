#include "simplegui/label.h"
#include "detail/common.h"

#include <QLabel>

namespace simplegui {

struct Label::Impl {
    QPointer<QLabel> qlabel;
    explicit Impl(const std::string& text) : qlabel(detail::plain_label(text)) {}
    ~Impl() { detail::delete_if_orphan(qlabel); }
};

Label::Label(const std::string& text) : pimpl(std::make_shared<Impl>(text)) {}

Label::~Label() = default;

void Label::set_text(const std::string& text) {
    if (pimpl->qlabel) pimpl->qlabel->setText(detail::qs(text));
}

std::string Label::get_text() const {
    return pimpl->qlabel ? detail::ss(pimpl->qlabel->text()) : std::string();
}

void Label::set_alignment(const std::string& alignment) {
    if (!pimpl->qlabel) return;
    if (alignment == "left") {
        pimpl->qlabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    } else if (alignment == "center") {
        pimpl->qlabel->setAlignment(Qt::AlignCenter);
    } else if (alignment == "right") {
        pimpl->qlabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }
}

void Label::set_word_wrap(bool wrap) {
    if (pimpl->qlabel) pimpl->qlabel->setWordWrap(wrap);
}

void Label::set_selectable(bool selectable) {
    if (pimpl->qlabel) {
        pimpl->qlabel->setTextInteractionFlags(selectable ? Qt::TextSelectableByMouse : Qt::NoTextInteraction);
    }
}

QWidget* Label::get_qwidget() const {
    return pimpl->qlabel.data();
}

}  // namespace simplegui
