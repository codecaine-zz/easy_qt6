#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct FormField {
    std::string name;
    std::string label;
    std::string default_value;
    enum class Type { Text, Password, Number, Checkbox } type = Type::Text;
};

// Generates a form automatically from a list of fields.
class DataForm : public Control {
public:
    DataForm();
    ~DataForm() override;

    void set_fields(const std::vector<FormField>& fields);
    
    // Retrieves all user inputs as a key-value list where key is field name.
    std::vector<std::pair<std::string, std::string>> get_values() const;
    
    // Gets a single value by field name
    std::string get_value(const std::string& field_name) const;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
