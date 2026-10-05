#include "simplegui/data_form.h"
#include "detail/common.h"
#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSpinBox>
#include <QMap>

namespace simplegui {

struct DataForm::Impl {
    QPointer<QWidget> widget = new QWidget();
    QPointer<QFormLayout> layout = new QFormLayout(widget);
    
    // Store pointers to inputs by field name for retrieval
    QMap<QString, QWidget*> inputs;

    ~Impl() {
        detail::delete_if_orphan(widget);
    }
};

DataForm::DataForm() : pimpl(std::make_shared<Impl>()) {}
DataForm::~DataForm() = default;

void DataForm::set_fields(const std::vector<FormField>& fields) {
    if (!pimpl->widget) return;

    // Clear existing
    QLayoutItem* item;
    while ((item = pimpl->layout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    pimpl->inputs.clear();

    for (const auto& field : fields) {
        QString qname = detail::qs(field.name);
        QString qlabel = detail::qs(field.label);
        
        if (field.type == FormField::Type::Text || field.type == FormField::Type::Password) {
            auto edit = new QLineEdit(detail::qs(field.default_value));
            if (field.type == FormField::Type::Password) edit->setEchoMode(QLineEdit::Password);
            pimpl->layout->addRow(qlabel, edit);
            pimpl->inputs.insert(qname, edit);
        } else if (field.type == FormField::Type::Checkbox) {
            auto check = new QCheckBox();
            check->setChecked(field.default_value == "true" || field.default_value == "1");
            pimpl->layout->addRow(qlabel, check);
            pimpl->inputs.insert(qname, check);
        } else if (field.type == FormField::Type::Number) {
            auto spin = new QSpinBox();
            spin->setRange(-2147483647, 2147483647);
            try { spin->setValue(std::stoi(field.default_value)); } catch(...) {}
            pimpl->layout->addRow(qlabel, spin);
            pimpl->inputs.insert(qname, spin);
        }
    }
}

std::vector<std::pair<std::string, std::string>> DataForm::get_values() const {
    std::vector<std::pair<std::string, std::string>> results;
    for (auto it = pimpl->inputs.constBegin(); it != pimpl->inputs.constEnd(); ++it) {
        results.push_back({it.key().toStdString(), get_value(it.key().toStdString())});
    }
    return results;
}

std::string DataForm::get_value(const std::string& field_name) const {
    if (!pimpl->inputs.contains(detail::qs(field_name))) return "";
    QWidget* w = pimpl->inputs.value(detail::qs(field_name));
    
    if (auto edit = qobject_cast<QLineEdit*>(w)) return edit->text().toStdString();
    if (auto check = qobject_cast<QCheckBox*>(w)) return check->isChecked() ? "true" : "false";
    if (auto spin = qobject_cast<QSpinBox*>(w)) return std::to_string(spin->value());
    
    return "";
}

QWidget* DataForm::get_qwidget() const {
    return pimpl->widget.data();
}

}
