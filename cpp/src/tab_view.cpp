#include "simplegui/tab_view.h"
#include "detail/common.h"

#include <QTabWidget>

#include <vector>

namespace simplegui {

struct TabView::Impl {
    QPointer<QTabWidget> tabs = new QTabWidget();
    std::vector<std::shared_ptr<Control>> pages;
    ~Impl() { detail::delete_if_orphan(tabs); }
};

TabView::TabView() : pimpl(std::make_shared<Impl>()) {}

TabView::~TabView() = default;

void TabView::add_tab(const std::string& title, std::shared_ptr<Control> content) {
    if (!pimpl->tabs || !content || !content->get_qwidget()) return;
    pimpl->pages.push_back(content);
    pimpl->tabs->addTab(content->get_qwidget(), detail::qs(title));
}

int TabView::tab_count() const {
    return pimpl->tabs ? pimpl->tabs->count() : 0;
}

int TabView::current_index() const {
    return pimpl->tabs ? pimpl->tabs->currentIndex() : -1;
}

void TabView::set_current_index(int index) {
    if (pimpl->tabs && index >= 0 && index < pimpl->tabs->count()) pimpl->tabs->setCurrentIndex(index);
}

void TabView::set_tab_title(int index, const std::string& title) {
    if (pimpl->tabs && index >= 0 && index < pimpl->tabs->count()) pimpl->tabs->setTabText(index, detail::qs(title));
}

EventConnection TabView::on_change(std::function<void(int)> handler) {
    if (!pimpl->tabs || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->tabs.data(), &QTabWidget::currentChanged,
                                         [handler = std::move(handler)](int index) { handler(index); }));
}

QWidget* TabView::get_qwidget() const {
    return pimpl->tabs.data();
}

}  // namespace simplegui
