#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

// A text input with a browse button for selecting files or folders.
class PathPicker : public Control {
public:
    enum class Mode { File, Folder };

    explicit PathPicker(Mode mode = Mode::File);
    ~PathPicker() override;

    void set_path(const std::string& path);
    std::string get_path() const;
    void set_filter(const std::string& filter); // e.g. "Images (*.png *.jpg)"

    EventConnection on_change(std::function<void(const std::string& path)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
