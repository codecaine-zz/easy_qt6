#include "simplegui/scroll_view.h"
#include <QScrollArea>
#include <QPointer>

namespace simplegui {

struct ScrollView::Impl {
    QPointer<QScrollArea> qscroll;
    std::shared_ptr<Control> content;

    Impl() {
        qscroll = new QScrollArea();
        qscroll->setWidgetResizable(true);
    }
    ~Impl() { if (qscroll && !qscroll->parent()) delete qscroll; }
};

ScrollView::ScrollView() : pimpl(std::make_shared<Impl>()) {}

ScrollView::~ScrollView() = default;

void ScrollView::set_content(std::shared_ptr<Control> content) {
    pimpl->content = content;
    if (pimpl->qscroll) {
        pimpl->qscroll->setWidget(content->get_qwidget());
    }
}

QWidget* ScrollView::get_qwidget() const {
    return pimpl->qscroll.data();
}

}
