#include "simplegui/tab_view.h"
#include <QTabWidget>
#include <QString>
#include <QPointer>
#include <vector>

namespace simplegui {

struct TabView::Impl {
    QPointer<QTabWidget> qtab;
    std::vector<std::shared_ptr<Control>> tabs;

    Impl() {
        qtab = new QTabWidget();
    }
    ~Impl() { if (qtab && !qtab->parent()) delete qtab; }
};

TabView::TabView() : pimpl(std::make_shared<Impl>()) {}

TabView::~TabView() = default;

void TabView::add_tab(const std::string& title, std::shared_ptr<Control> content) {
    pimpl->tabs.push_back(content);
    if (pimpl->qtab) {
        pimpl->qtab->addTab(content->get_qwidget(), QString::fromStdString(title));
    }
}

QWidget* TabView::get_qwidget() const {
    return pimpl->qtab.data();
}

}
