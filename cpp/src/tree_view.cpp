#include "simplegui/tree_view.h"
#include <QTreeWidget>

namespace simplegui {

struct TreeView::Impl {
    QTreeWidget* widget;
};

TreeView::TreeView() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QTreeWidget();
}

TreeView::~TreeView() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

QWidget* TreeView::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
