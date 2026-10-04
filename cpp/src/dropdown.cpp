#include "simplegui/dropdown.h"
#include "detail/common.h"

#include <QComboBox>

namespace simplegui {

struct Dropdown::Impl {
    QPointer<QComboBox> combo;
    explicit Impl(const std::vector<std::string>& items) : combo(new QComboBox()) {
        for (const auto& item : items) combo->addItem(detail::qs(item));
    }
    ~Impl() { detail::delete_if_orphan(combo); }
};

Dropdown::Dropdown(const std::vector<std::string>& items) : pimpl(std::make_shared<Impl>(items)) {}

Dropdown::~Dropdown() = default;

void Dropdown::add_item(const std::string& item) {
    if (pimpl->combo) pimpl->combo->addItem(detail::qs(item));
}

void Dropdown::set_items(const std::vector<std::string>& items) {
    if (!pimpl->combo) return;
    pimpl->combo->clear();
    for (const auto& item : items) pimpl->combo->addItem(detail::qs(item));
}

std::vector<std::string> Dropdown::items() const {
    std::vector<std::string> result;
    if (!pimpl->combo) return result;
    for (int i = 0; i < pimpl->combo->count(); ++i) result.push_back(detail::ss(pimpl->combo->itemText(i)));
    return result;
}

void Dropdown::clear() {
    if (pimpl->combo) pimpl->combo->clear();
}

int Dropdown::count() const {
    return pimpl->combo ? pimpl->combo->count() : 0;
}

std::string Dropdown::get_selected() const {
    return pimpl->combo ? detail::ss(pimpl->combo->currentText()) : std::string();
}

void Dropdown::set_selected(const std::string& item) {
    if (!pimpl->combo) return;
    const int index = pimpl->combo->findText(detail::qs(item));
    if (index >= 0) pimpl->combo->setCurrentIndex(index);
}

int Dropdown::selected_index() const {
    return pimpl->combo ? pimpl->combo->currentIndex() : -1;
}

void Dropdown::set_selected_index(int index) {
    if (pimpl->combo && index >= -1 && index < pimpl->combo->count()) pimpl->combo->setCurrentIndex(index);
}

EventConnection Dropdown::on_change(std::function<void(const std::string&)> handler) {
    if (!pimpl->combo || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->combo.data(), &QComboBox::currentTextChanged,
                                         [handler = std::move(handler)](const QString& text) {
                                             handler(detail::ss(text));
                                         }));
}

QWidget* Dropdown::get_qwidget() const {
    return pimpl->combo.data();
}

}  // namespace simplegui
