#include "simplegui/label.h"
#include <QLabel>
#include <QString>
#include <QPointer>

namespace simplegui {

struct Label::Impl {
    QPointer<QLabel> qlabel;
    Impl(const std::string& text) { qlabel = new QLabel(QString::fromStdString(text)); }
    ~Impl() { if (qlabel && !qlabel->parent()) delete qlabel; }
};

Label::Label(const std::string& text)
    : pimpl(std::make_shared<Impl>(text)) {}

Label::~Label() = default;

void Label::set_text(const std::string& text) {
    if (pimpl->qlabel) {
        pimpl->qlabel->setText(QString::fromStdString(text));
    }
}

QWidget* Label::get_qwidget() const {
    return pimpl->qlabel.data();
}

}
