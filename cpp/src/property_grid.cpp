#include "simplegui/property_grid.h"
#include "detail/common.h"
#include <QScrollArea>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QColorDialog>
#include <QWidget>

namespace simplegui {

struct PropertyGrid::Impl {
    QPointer<QScrollArea> scroll = new QScrollArea();
    QPointer<QWidget> container = new QWidget();
    QPointer<QFormLayout> layout = new QFormLayout(container);
    std::vector<PropertyItem> items;
    std::function<void(const std::string&, const std::string&)> on_change;

    Impl() {
        scroll->setWidget(container);
        scroll->setWidgetResizable(true);
    }
    ~Impl() {
        detail::delete_if_orphan(scroll);
    }
};

PropertyGrid::PropertyGrid() : pimpl(std::make_shared<Impl>()) {}
PropertyGrid::~PropertyGrid() = default;

void PropertyGrid::set_properties(const std::vector<PropertyItem>& props) {
    if (!pimpl->container) return;
    
    // Clear layout
    QLayoutItem* item;
    while ((item = pimpl->layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    pimpl->items = props;
    
    for (size_t i = 0; i < props.size(); ++i) {
        const auto& prop = props[i];
        QString qname = detail::qs(prop.name);
        
        if (prop.type == PropertyItem::Type::Text) {
            auto edit = new QLineEdit(detail::qs(prop.value));
            QObject::connect(edit, &QLineEdit::textChanged, pimpl->container, [this, i](const QString& text) {
                pimpl->items[i].value = text.toStdString();
                if (pimpl->on_change) pimpl->on_change(pimpl->items[i].name, pimpl->items[i].value);
            });
            pimpl->layout->addRow(qname, edit);
        } else if (prop.type == PropertyItem::Type::Boolean) {
            auto check = new QCheckBox();
            check->setChecked(prop.value == "true" || prop.value == "1");
            QObject::connect(check, &QCheckBox::toggled, pimpl->container, [this, i](bool checked) {
                pimpl->items[i].value = checked ? "true" : "false";
                if (pimpl->on_change) pimpl->on_change(pimpl->items[i].name, pimpl->items[i].value);
            });
            pimpl->layout->addRow(qname, check);
        } else if (prop.type == PropertyItem::Type::Number) {
            auto spin = new QSpinBox();
            spin->setRange(-2147483647, 2147483647);
            try { spin->setValue(std::stoi(prop.value)); } catch(...) {}
            QObject::connect(spin, QOverload<int>::of(&QSpinBox::valueChanged), pimpl->container, [this, i](int val) {
                pimpl->items[i].value = std::to_string(val);
                if (pimpl->on_change) pimpl->on_change(pimpl->items[i].name, pimpl->items[i].value);
            });
            pimpl->layout->addRow(qname, spin);
        } else if (prop.type == PropertyItem::Type::Color) {
            auto btn = new QPushButton("...");
            btn->setStyleSheet(QString("background-color: %1").arg(detail::qs(prop.value)));
            QObject::connect(btn, &QPushButton::clicked, pimpl->container, [this, i, btn]() {
                QColor col = QColorDialog::getColor(QColor(detail::qs(pimpl->items[i].value)), pimpl->container);
                if (col.isValid()) {
                    std::string hex = col.name().toStdString();
                    pimpl->items[i].value = hex;
                    btn->setStyleSheet(QString("background-color: %1").arg(col.name()));
                    if (pimpl->on_change) pimpl->on_change(pimpl->items[i].name, pimpl->items[i].value);
                }
            });
            pimpl->layout->addRow(qname, btn);
        }
    }
}

std::vector<PropertyItem> PropertyGrid::get_properties() const {
    return pimpl->items;
}

EventConnection PropertyGrid::on_property_changed(std::function<void(const std::string&, const std::string&)> handler) {
    pimpl->on_change = std::move(handler);
    return EventConnection();
}

QWidget* PropertyGrid::get_qwidget() const {
    return pimpl->scroll.data();
}

}
