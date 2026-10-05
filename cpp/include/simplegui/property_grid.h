#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace simplegui {

struct PropertyItem {
    std::string name;
    std::string value;
    enum class Type { Text, Boolean, Number, Color } type = Type::Text;
};

// A control for editing properties of an object (Delphi: Object Inspector).
class PropertyGrid : public Control {
public:
    PropertyGrid();
    ~PropertyGrid() override;

    void set_properties(const std::vector<PropertyItem>& props);
    std::vector<PropertyItem> get_properties() const;

    // Runs when any property is changed by the user.
    EventConnection on_property_changed(std::function<void(const std::string& name, const std::string& value)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
