#include "simplegui/code_editor.h"
#include "detail/common.h"
#include <QPlainTextEdit>
#include <QFontDatabase>

namespace simplegui {

struct CodeEditor::Impl {
    QPointer<QPlainTextEdit> edit = new QPlainTextEdit();
    std::function<void()> on_change;

    Impl() {
        const QFont fixedFont = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        edit->setFont(fixedFont);
        edit->setLineWrapMode(QPlainTextEdit::NoWrap);
        edit->setTabStopDistance(4 * QFontMetrics(fixedFont).horizontalAdvance(' '));
    }
    ~Impl() {
        detail::delete_if_orphan(edit);
    }
};

CodeEditor::CodeEditor(const std::string& text) : pimpl(std::make_shared<Impl>()) {
    if (!text.empty()) set_text(text);

    QObject::connect(pimpl->edit, &QPlainTextEdit::textChanged, pimpl->edit, [this]() {
        if (pimpl->on_change) pimpl->on_change();
    });
}

CodeEditor::~CodeEditor() = default;

void CodeEditor::set_text(const std::string& text) {
    if (pimpl->edit) pimpl->edit->setPlainText(detail::qs(text));
}

std::string CodeEditor::get_text() const {
    return pimpl->edit ? pimpl->edit->toPlainText().toStdString() : "";
}

EventConnection CodeEditor::on_change(std::function<void()> handler) {
    pimpl->on_change = std::move(handler);
    return EventConnection();
}

QWidget* CodeEditor::get_qwidget() const {
    return pimpl->edit.data();
}

}
