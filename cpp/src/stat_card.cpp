#include "simplegui/stat_card.h"
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QPointer>
#include <QString>

namespace simplegui {

struct StatCard::Impl {
    QPointer<QFrame> frame;
    QLabel* lbl_title;
    QLabel* lbl_value;
    QLabel* lbl_subtext;

    Impl(const std::string& title, const std::string& value, const std::string& subtext, const std::string& accent_color) {
        frame = new QFrame();
        frame->setStyleSheet(
            "QFrame {"
            "  background-color: #1a1a1e;"
            "  border: 1px solid #27272a;"
            "  border-radius: 8px;"
            "  padding: 10px;"
            "}"
        );
        frame->setMinimumWidth(130);

        QVBoxLayout* layout = new QVBoxLayout(frame);
        layout->setContentsMargins(10, 10, 10, 10);
        layout->setSpacing(4);

        lbl_title = new QLabel(QString::fromStdString(title));
        lbl_title->setStyleSheet("color: #a1a1aa; font-size: 11px; font-weight: 600; text-transform: uppercase; border: none; background: transparent;");

        lbl_value = new QLabel(QString::fromStdString(value));
        QString val_style = QString("color: %1; font-size: 22px; font-weight: bold; font-family: monospace, sans-serif; border: none; background: transparent;").arg(QString::fromStdString(accent_color));
        lbl_value->setStyleSheet(val_style);

        lbl_subtext = new QLabel(QString::fromStdString(subtext));
        lbl_subtext->setStyleSheet("color: #71717a; font-size: 11px; border: none; background: transparent;");
        if (subtext.empty()) {
            lbl_subtext->setVisible(false);
        }

        layout->addWidget(lbl_title);
        layout->addWidget(lbl_value);
        layout->addWidget(lbl_subtext);
    }
    ~Impl() { if (frame && !frame->parent()) delete frame; }
};

StatCard::StatCard(const std::string& title, const std::string& value, const std::string& subtext, const std::string& accent_color)
    : pimpl(std::make_shared<Impl>(title, value, subtext, accent_color)) {}

StatCard::~StatCard() = default;

void StatCard::set_title(const std::string& title) {
    if (pimpl->lbl_title) {
        pimpl->lbl_title->setText(QString::fromStdString(title));
    }
}

void StatCard::set_value(const std::string& value) {
    if (pimpl->lbl_value) {
        pimpl->lbl_value->setText(QString::fromStdString(value));
    }
}

void StatCard::set_subtext(const std::string& subtext) {
    if (pimpl->lbl_subtext) {
        pimpl->lbl_subtext->setText(QString::fromStdString(subtext));
        pimpl->lbl_subtext->setVisible(!subtext.empty());
    }
}

void StatCard::set_accent_color(const std::string& hex_color) {
    if (pimpl->lbl_value) {
        QString val_style = QString("color: %1; font-size: 22px; font-weight: bold; font-family: monospace, sans-serif; border: none; background: transparent;").arg(QString::fromStdString(hex_color));
        pimpl->lbl_value->setStyleSheet(val_style);
    }
}

QWidget* StatCard::get_qwidget() const {
    return pimpl->frame.data();
}

}
