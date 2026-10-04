#include "simplegui/split_view.h"
#include "detail/common.h"

#include <QList>
#include <QSplitter>

namespace simplegui {

struct SplitView::Impl {
    QPointer<QSplitter> splitter = new QSplitter();
    std::vector<std::shared_ptr<Control>> children;
    ~Impl() { detail::delete_if_orphan(splitter); }
};

SplitView::SplitView(bool horizontal) : pimpl(std::make_shared<Impl>()) {
    pimpl->splitter->setOrientation(horizontal ? Qt::Horizontal : Qt::Vertical);
}

SplitView::~SplitView() = default;

void SplitView::add_child(std::shared_ptr<Control> control) {
    if (!pimpl->splitter || !control || !control->get_qwidget()) return;
    pimpl->children.push_back(control);
    pimpl->splitter->addWidget(control->get_qwidget());
}

void SplitView::set_sizes(int first, int second) {
    set_sizes(std::vector<int>{first, second});
}

void SplitView::set_sizes(const std::vector<int>& sizes) {
    if (!pimpl->splitter) return;
    QList<int> list;
    for (int s : sizes) list.append(std::max(0, s));
    pimpl->splitter->setSizes(list);
}

QWidget* SplitView::get_qwidget() const {
    return pimpl->splitter.data();
}

}  // namespace simplegui
