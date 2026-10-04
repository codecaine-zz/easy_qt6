#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// Clickable underlined text that opens a web page in the user's browser.
// For safety only http://, https:// and mailto: addresses are opened.
class Link : public Control {
public:
    Link(const std::string& text, const std::string& url);
    ~Link() override;

    void set_text(const std::string& text);
    std::string get_text() const;
    void set_url(const std::string& url);
    std::string get_url() const;

    // Runs when the link is clicked (in addition to opening the browser).
    EventConnection on_click(std::function<void(const std::string& url)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
