#include "simplegui/split_view.h"
#include <QSplitter>
#include <QPointer>

namespace simplegui {

struct SplitView::Impl {
    QPointer<QSplitter> qsplitter;
    std::vector<std::shared_ptr<Control>> children;

    Impl(bool horizontal) {
        qsplitter = new QSplitter();
        qsplitter->setOrientation(horizontal ? Qt::Horizontal : Qt::Vertical);
    }
    ~Impl() { if (qsplitter && !qsplitter->parent()) delete qsplitter; }
};

SplitView::SplitView(bool horizontal)
    : pimpl(std::make_shared<Impl>(horizontal)) {}

SplitView::~SplitView() = default;

void SplitView::add_child(std::shared_ptr<Control> control) {
    pimpl->children.push_back(control);
    if (pimpl->qsplitter) {
        pimpl->qsplitter->addWidget(control->get_qwidget());
    }
}

QWidget* SplitView::get_qwidget() const {
    return pimpl->qsplitter.data();
}

}
