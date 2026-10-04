#include "simplegui/terminal_view.h"
#include "detail/common.h"

#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QStringList>
#include <QTextCursor>
#include <QVBoxLayout>

#include <algorithm>

namespace simplegui {
namespace {

constexpr int kMaxHistory = 200;

// A command line that remembers previous commands (Up/Down arrows).
class HistoryLineEdit : public QLineEdit {
public:
    QStringList history;
    int pos = 0;

    void remember(const QString& cmd) {
        if (!cmd.isEmpty() && (history.isEmpty() || history.last() != cmd)) history << cmd;
        while (history.size() > kMaxHistory) history.removeFirst();
        pos = static_cast<int>(history.size());
    }

protected:
    void keyPressEvent(QKeyEvent* e) override {
        if (e->key() == Qt::Key_Up && !history.isEmpty()) {
            pos = std::max(0, pos - 1);
            setText(history.at(pos));
            return;
        }
        if (e->key() == Qt::Key_Down && !history.isEmpty()) {
            pos = std::min(static_cast<int>(history.size()), pos + 1);
            setText(pos < history.size() ? history.at(pos) : QString());
            return;
        }
        QLineEdit::keyPressEvent(e);
    }
};

}  // namespace

struct TerminalView::Impl {
    QPointer<QWidget> container = new QWidget();
    QPointer<QPlainTextEdit> output;
    QPointer<QWidget> input_row;
    QPointer<QLabel> prompt;
    QPointer<HistoryLineEdit> input;
    QColor text_color = QColor(0x39, 0xff, 0x88);
    detail::Event<const std::string&> command = detail::make_event<const std::string&>(container);

    ~Impl() { detail::delete_if_orphan(container); }

    // Text is inserted as plain text with a color format - never parsed as HTML.
    void write(const QString& text, const std::string& color, bool newline) {
        if (!output) return;
        QScrollBar* bar = output->verticalScrollBar();
        const bool at_bottom = bar->value() >= bar->maximum() - 2;
        QTextCursor cursor(output->document());
        cursor.movePosition(QTextCursor::End);
        QTextCharFormat fmt;
        fmt.setForeground(detail::parse_color(color, text_color));
        cursor.insertText(text, fmt);
        if (newline) cursor.insertBlock();
        if (at_bottom) bar->setValue(bar->maximum());  // follow new output unless the user scrolled up
    }
};

TerminalView::TerminalView(bool show_input) : pimpl(std::make_shared<Impl>()) {
    QWidget* root = pimpl->container;
    root->setProperty("sg_role", QStringLiteral("terminal"));
    detail::set_base_style(root, QStringLiteral(
        "QWidget { background: transparent; }"
        "QWidget[sg_role=\"terminal\"] { background: #020805; border: 1px solid #14532d; border-radius: 6px; }"
        "QPlainTextEdit { background: transparent; border: none; color: #39ff88;"
        "  font-family: \"JetBrains Mono\", \"Cascadia Code\", \"SF Mono\", Menlo, Consolas, monospace; font-size: 13px; }"
        "QLineEdit { background: transparent; border: none; color: #e0ffe9;"
        "  font-family: \"JetBrains Mono\", \"Cascadia Code\", \"SF Mono\", Menlo, Consolas, monospace; font-size: 13px; }"
        "QLabel { background: transparent; border: none; color: #39ff88; font-weight: bold;"
        "  font-family: \"JetBrains Mono\", \"Cascadia Code\", \"SF Mono\", Menlo, Consolas, monospace; }"
        "QScrollBar:vertical { background: transparent; width: 8px; margin: 0; border: none; }"
        "QScrollBar::handle:vertical { background: #14532d; border-radius: 4px; min-height: 24px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"));
    root->setAttribute(Qt::WA_StyledBackground, true);  // plain QWidgets only paint style-sheet backgrounds with this
    root->setMinimumSize(260, 140);

    auto* layout = new QVBoxLayout(root);
    layout->setContentsMargins(10, 8, 10, 8);
    layout->setSpacing(4);

    pimpl->output = new QPlainTextEdit(root);
    pimpl->output->setReadOnly(true);
    pimpl->output->setUndoRedoEnabled(false);
    pimpl->output->setMaximumBlockCount(5000);
    pimpl->output->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    layout->addWidget(pimpl->output, 1);

    pimpl->input_row = new QWidget(root);
    auto* row = new QHBoxLayout(pimpl->input_row);
    row->setContentsMargins(0, 0, 0, 0);
    row->setSpacing(6);
    pimpl->prompt = new QLabel(QStringLiteral(">"), pimpl->input_row);
    pimpl->prompt->setTextFormat(Qt::PlainText);
    pimpl->input = new HistoryLineEdit();
    pimpl->input->setParent(pimpl->input_row);
    row->addWidget(pimpl->prompt);
    row->addWidget(pimpl->input, 1);
    layout->addWidget(pimpl->input_row);
    pimpl->input_row->setVisible(show_input);

    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->input.data(), &QLineEdit::returnPressed, [weak]() {
        auto d = weak.lock();
        if (!d || !d->input) return;
        const QString cmd = d->input->text();
        d->input->remember(cmd);
        d->input->clear();
        detail::fire(d->command, detail::ss(cmd));
    });
}

TerminalView::~TerminalView() = default;

void TerminalView::print_line(const std::string& text, const std::string& color) {
    pimpl->write(detail::qs(text), color, true);
}

void TerminalView::print(const std::string& text, const std::string& color) {
    pimpl->write(detail::qs(text), color, false);
}

void TerminalView::clear() {
    if (pimpl->output) pimpl->output->clear();
}

std::string TerminalView::text() const {
    return pimpl->output ? detail::ss(pimpl->output->toPlainText()) : std::string();
}

void TerminalView::set_max_lines(int lines) {
    if (pimpl->output) pimpl->output->setMaximumBlockCount(std::max(0, lines));
}

void TerminalView::set_prompt(const std::string& prompt) {
    if (pimpl->prompt) pimpl->prompt->setText(detail::qs(prompt));
}

void TerminalView::set_input_visible(bool visible) {
    if (pimpl->input_row) pimpl->input_row->setVisible(visible);
}

void TerminalView::set_output_color(const std::string& color) {
    pimpl->text_color = detail::parse_color(color, pimpl->text_color);
}

EventConnection TerminalView::on_command(std::function<void(const std::string&)> handler) {
    return detail::add_handler(pimpl->command, std::move(handler));
}

QWidget* TerminalView::get_qwidget() const { return pimpl->container.data(); }

}  // namespace simplegui
