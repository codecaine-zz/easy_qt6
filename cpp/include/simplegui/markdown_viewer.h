#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

// Renders Markdown text as formatted HTML.
class MarkdownViewer : public Control {
public:
    MarkdownViewer(const std::string& markdown_text = "");
    ~MarkdownViewer() override;

    void set_markdown(const std::string& markdown_text);
    std::string get_markdown() const;

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
