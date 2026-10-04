#include "simplegui/form_layout.h"
#include "detail/common.h"
#include <QFormLayout>
#include <QPointer>
#include <vector>

namespace simplegui {

struct FormLayout::Impl {
    QPointer<QWidget> container = new QWidget();
    QFormLayout* layout = new QFormLayout(container);
    std::vector<std::shared_ptr<Control>> children; // Keep references to prevent deletion
    
    ~Impl() { 
        if (container && !container->parent()) {
            delete container;
        }
    }
};

FormLayout::FormLayout() : pimpl(std::make_shared<Impl>()) {}

FormLayout::~FormLayout() = default;

void FormLayout::add_row(const std::string& label, std::shared_ptr<Control> field) {
    if (field && field->get_qwidget()) {
        pimpl->layout->addRow(detail::qs(label), field->get_qwidget());
        pimpl->children.push_back(field);
    }
}

void FormLayout::add_row(std::shared_ptr<Control> field) {
    if (field && field->get_qwidget()) {
        pimpl->layout->addRow(field->get_qwidget());
        pimpl->children.push_back(field);
    }
}

QWidget* FormLayout::get_qwidget() const {
    return pimpl->container.data();
}

}  // namespace simplegui
