#include "simplegui/rich_text_editor.h"
#include "detail/common.h"
#include <QTextEdit>

namespace simplegui {

struct RichTextEditor::Impl {
    QTextEdit* widget;
};

RichTextEditor::RichTextEditor() : pimpl(std::make_shared<Impl>()) {
    pimpl->widget = new QTextEdit();
}

RichTextEditor::~RichTextEditor() {
    if (pimpl->widget && !pimpl->widget->parent()) {
        delete pimpl->widget;
    }
}

void RichTextEditor::set_html(const std::string& html) {
    pimpl->widget->setHtml(detail::qs(html));
}

QWidget* RichTextEditor::get_qwidget() const {
    return pimpl->widget;
}

}  // namespace simplegui
