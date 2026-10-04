#include "simplegui/status_pill.h"
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPointer>
#include <QString>

namespace simplegui {

struct StatusPill::Impl {
    QPointer<QFrame> frame;
    QLabel* dot;
    QLabel* text_label;

    Impl(const std::string& text, const std::string& dot_color) {
        frame = new QFrame();
        frame->setStyleSheet(
            "QFrame {"
            "  background-color: #1a1a1e;"
            "  border: 1px solid #27272a;"
            "  border-radius: 12px;"
            "  padding: 3px 10px;"
            "}"
        );
        frame->setFixedHeight(26);

        QHBoxLayout* layout = new QHBoxLayout(frame);
        layout->setContentsMargins(6, 2, 8, 2);
        layout->setSpacing(6);

        dot = new QLabel();
        dot->setFixedSize(8, 8);
        update_dot(dot_color);

        text_label = new QLabel(QString::fromStdString(text));
        text_label->setStyleSheet("color: #e2e8f0; font-size: 11px; font-weight: 600; letter-spacing: 0.5px; border: none; background: transparent;");

        layout->addWidget(dot);
        layout->addWidget(text_label);
    }
    ~Impl() { if (frame && !frame->parent()) delete frame; }

    void update_dot(const std::string& color) {
        if (dot) {
            dot->setStyleSheet(QString(
                "background-color: %1;"
                "border-radius: 4px;"
                "border: none;"
            ).arg(QString::fromStdString(color)));
        }
    }
};

StatusPill::StatusPill(const std::string& text, const std::string& dot_color)
    : pimpl(std::make_shared<Impl>(text, dot_color)) {}

StatusPill::~StatusPill() = default;

void StatusPill::set_status(const std::string& text, const std::string& dot_color) {
    set_text(text);
    set_color(dot_color);
}

void StatusPill::set_text(const std::string& text) {
    if (pimpl->text_label) {
        pimpl->text_label->setText(QString::fromStdString(text));
    }
}

void StatusPill::set_color(const std::string& dot_color) {
    pimpl->update_dot(dot_color);
}

QWidget* StatusPill::get_qwidget() const {
    return pimpl->frame.data();
}

}
