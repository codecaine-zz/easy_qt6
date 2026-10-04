#include "simplegui/combo_box.h"
#include "detail/common.h"

#include <QComboBox>
#include <QLineEdit>

namespace simplegui {

struct ComboBox::Impl {
    QPointer<QComboBox> combo;
    explicit Impl(const std::vector<std::string>& items) : combo(new QComboBox()) {
        combo->setEditable(true);
        combo->setInsertPolicy(QComboBox::NoInsert);  // typing doesn't silently grow the list
        for (const auto& item : items) combo->addItem(detail::qs(item));
    }
    ~Impl() { detail::delete_if_orphan(combo); }
};

ComboBox::ComboBox(const std::vector<std::string>& items) : pimpl(std::make_shared<Impl>(items)) {}

ComboBox::~ComboBox() = default;

void ComboBox::add_item(const std::string& item) {
    if (pimpl->combo) pimpl->combo->addItem(detail::qs(item));
}

void ComboBox::set_items(const std::vector<std::string>& items) {
    if (!pimpl->combo) return;
    pimpl->combo->clear();
    for (const auto& item : items) pimpl->combo->addItem(detail::qs(item));
}

std::vector<std::string> ComboBox::items() const {
    std::vector<std::string> result;
    if (!pimpl->combo) return result;
    for (int i = 0; i < pimpl->combo->count(); ++i) result.push_back(detail::ss(pimpl->combo->itemText(i)));
    return result;
}

void ComboBox::clear() {
    if (pimpl->combo) pimpl->combo->clear();
}

std::string ComboBox::get_text() const {
    return pimpl->combo ? detail::ss(pimpl->combo->currentText()) : std::string();
}

void ComboBox::set_text(const std::string& text) {
    if (pimpl->combo) pimpl->combo->setCurrentText(detail::qs(text));
}

void ComboBox::set_placeholder(const std::string& placeholder) {
    if (pimpl->combo && pimpl->combo->lineEdit()) pimpl->combo->lineEdit()->setPlaceholderText(detail::qs(placeholder));
}

EventConnection ComboBox::on_change(std::function<void(const std::string&)> handler) {
    if (!pimpl->combo || !handler) return {};
    return detail::wrap(QObject::connect(pimpl->combo.data(), &QComboBox::currentTextChanged,
                                         [handler = std::move(handler)](const QString& text) {
                                             handler(detail::ss(text));
                                         }));
}

QWidget* ComboBox::get_qwidget() const {
    return pimpl->combo.data();
}

}  // namespace simplegui
