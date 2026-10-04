#include "simplegui/link.h"
#include <QLabel>
#include <QString>
#include <QPointer>

namespace simplegui {

struct Link::Impl {
    QPointer<QLabel> qlabel;

    Impl(const std::string& text, const std::string& url) {
        qlabel = new QLabel();
        QString html = QString("<a href=\"%1\">%2</a>").arg(QString::fromStdString(url), QString::fromStdString(text));
        qlabel->setText(html);
        qlabel->setOpenExternalLinks(true);
    }
    ~Impl() { if (qlabel && !qlabel->parent()) delete qlabel; }
};

Link::Link(const std::string& text, const std::string& url)
    : pimpl(std::make_shared<Impl>(text, url)) {}

Link::~Link() = default;

QWidget* Link::get_qwidget() const {
    return pimpl->qlabel.data();
}

}
