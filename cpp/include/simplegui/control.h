#pragma once
#include <memory>
#include <string>

class QWidget;

namespace simplegui {

class Control {
public:
    virtual ~Control() = default;
    virtual QWidget* get_qwidget() const = 0;

    virtual void set_style(const std::string& style);
    virtual void set_enabled(bool enabled);
    virtual bool is_enabled() const;
    virtual void set_visible(bool visible);
    virtual bool is_visible() const;
};

}
