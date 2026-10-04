#pragma once
#include <memory>

class QWidget;

namespace simplegui {
class Control {
public:
    virtual ~Control() = default;
    virtual QWidget* get_qwidget() const = 0;
};
}
