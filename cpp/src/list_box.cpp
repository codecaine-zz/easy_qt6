#include "simplegui/list_box.h"
#include "detail/common.h"

#include <QListWidget>

namespace simplegui {

struct ListBox::Impl {
    QPointer<QListWidget> list = new QListWidget();
    detail::Event<int, const std::string&> selected = detail::make_event<int, const std::string&>(list);
    detail::Event<int, const std::string&> activated = detail::make_event<int, const std::string&>(list);
    ~Impl() { detail::delete_if_orphan(list); }

    bool valid(int index) const { return list && index >= 0 && index < list->count(); }
};

ListBox::ListBox(const std::vector<std::string>& items) : pimpl(std::make_shared<Impl>()) {
    pimpl->list->setSelectionMode(QAbstractItemView::SingleSelection);
    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->list.data(), &QListWidget::currentRowChanged, [weak](int row) {
        auto d = weak.lock();
        if (!d || !d->list) return;
        const QListWidgetItem* it = d->list->item(row);
        detail::fire(d->selected, row, it ? detail::ss(it->text()) : std::string());
    });
    QObject::connect(pimpl->list.data(), &QListWidget::itemActivated, [weak](QListWidgetItem* it) {
        auto d = weak.lock();
        if (!d || !d->list || !it) return;
        detail::fire(d->activated, d->list->row(it), detail::ss(it->text()));
    });
    set_items(items);
}

ListBox::~ListBox() = default;

void ListBox::add_item(const std::string& text) {
    if (pimpl->list) pimpl->list->addItem(detail::qs(text));
}

void ListBox::insert_item(int index, const std::string& text) {
    if (!pimpl->list) return;
    pimpl->list->insertItem(std::clamp(index, 0, pimpl->list->count()), detail::qs(text));
}

void ListBox::set_item(int index, const std::string& text) {
    if (pimpl->valid(index)) pimpl->list->item(index)->setText(detail::qs(text));
}

void ListBox::remove_item(int index) {
    if (pimpl->valid(index)) delete pimpl->list->takeItem(index);
}

void ListBox::set_items(const std::vector<std::string>& items) {
    if (!pimpl->list) return;
    pimpl->list->clear();
    for (const auto& s : items) pimpl->list->addItem(detail::qs(s));
}

void ListBox::clear() {
    if (pimpl->list) pimpl->list->clear();
}

std::vector<std::string> ListBox::items() const {
    std::vector<std::string> out;
    if (!pimpl->list) return out;
    for (int i = 0; i < pimpl->list->count(); ++i) out.push_back(detail::ss(pimpl->list->item(i)->text()));
    return out;
}

std::string ListBox::item(int index) const {
    return pimpl->valid(index) ? detail::ss(pimpl->list->item(index)->text()) : std::string();
}

int ListBox::count() const { return pimpl->list ? pimpl->list->count() : 0; }

int ListBox::selected_index() const {
    if (!pimpl->list) return -1;
    const QListWidgetItem* cur = pimpl->list->currentItem();
    return (cur && cur->isSelected()) ? pimpl->list->row(cur) : -1;
}

std::string ListBox::selected_text() const { return item(selected_index()); }

void ListBox::set_selected_index(int index) {
    if (!pimpl->list) return;
    if (pimpl->valid(index)) {
        pimpl->list->setCurrentRow(index);
    } else {
        pimpl->list->setCurrentRow(-1);
        pimpl->list->clearSelection();
    }
}

void ListBox::set_sorted(bool sorted) {
    if (!pimpl->list) return;
    pimpl->list->setSortingEnabled(sorted);
    if (sorted) pimpl->list->sortItems(Qt::AscendingOrder);
}

EventConnection ListBox::on_select(std::function<void(int, const std::string&)> handler) {
    return detail::add_handler(pimpl->selected, std::move(handler));
}

EventConnection ListBox::on_double_click(std::function<void(int, const std::string&)> handler) {
    return detail::add_handler(pimpl->activated, std::move(handler));
}

QWidget* ListBox::get_qwidget() const { return pimpl->list.data(); }

}  // namespace simplegui
