#include "simplegui/markdown_viewer.h"
#include "detail/common.h"
#include <QTextBrowser>

namespace simplegui {

struct MarkdownViewer::Impl {
    QPointer<QTextBrowser> browser = new QTextBrowser();
    std::string markdown;

    Impl() {
        browser->setOpenExternalLinks(true);
        // Make it look a bit modern
        browser->setStyleSheet("QTextBrowser { border: none; background-color: transparent; }");
    }
    ~Impl() {
        detail::delete_if_orphan(browser);
    }
};

MarkdownViewer::MarkdownViewer(const std::string& markdown_text) : pimpl(std::make_shared<Impl>()) {
    if (!markdown_text.empty()) {
        set_markdown(markdown_text);
    }
}

MarkdownViewer::~MarkdownViewer() = default;

void MarkdownViewer::set_markdown(const std::string& markdown_text) {
    if (!pimpl->browser) return;
    pimpl->markdown = markdown_text;
    pimpl->browser->setMarkdown(detail::qs(markdown_text));
}

std::string MarkdownViewer::get_markdown() const {
    return pimpl->markdown;
}

QWidget* MarkdownViewer::get_qwidget() const {
    return pimpl->browser.data();
}

}
