#include "simplegui/scroll_view.h"
#include "detail/common.h"

#include <QScrollArea>
#include <QScrollBar>

namespace simplegui {

struct ScrollView::Impl {
    QPointer<QScrollArea> scroll = new QScrollArea();
    std::shared_ptr<Control> content;
    Impl() { scroll->setWidgetResizable(true); }
    ~Impl() { detail::delete_if_orphan(scroll); }
};

ScrollView::ScrollView() : pimpl(std::make_shared<Impl>()) {}

ScrollView::~ScrollView() = default;

void ScrollView::set_content(std::shared_ptr<Control> content) {
    if (!pimpl->scroll || !content || !content->get_qwidget()) return;
    // QScrollArea deletes its previous widget; hand it back to its control first so
    // the old content can still be reused elsewhere.
    if (QWidget* old = pimpl->scroll->takeWidget()) old->setParent(nullptr);
    pimpl->content = content;
    pimpl->scroll->setWidget(content->get_qwidget());
}

void ScrollView::scroll_to_top() {
    if (pimpl->scroll) pimpl->scroll->verticalScrollBar()->setValue(0);
}

void ScrollView::scroll_to_bottom() {
    if (pimpl->scroll) pimpl->scroll->verticalScrollBar()->setValue(pimpl->scroll->verticalScrollBar()->maximum());
}

QWidget* ScrollView::get_qwidget() const {
    return pimpl->scroll.data();
}

}  // namespace simplegui
