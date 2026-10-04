#include "simplegui/group_box.h"
#include <QGroupBox>
#include <QVBoxLayout>
#include <QString>
#include <QPointer>
#include <vector>

namespace simplegui {

struct GroupBox::Impl {
    QPointer<QGroupBox> qgroup;
    QVBoxLayout* qlayout;
    std::vector<std::shared_ptr<Control>> children;

    Impl(const std::string& title) {
        qgroup = new QGroupBox(QString::fromStdString(title));
        qlayout = new QVBoxLayout(qgroup);
    }
    ~Impl() { if (qgroup && !qgroup->parent()) delete qgroup; }
};

GroupBox::GroupBox(const std::string& title)
    : pimpl(std::make_shared<Impl>(title)) {}

GroupBox::~GroupBox() = default;

void GroupBox::add_child(std::shared_ptr<Control> control) {
    pimpl->children.push_back(control);
    if (pimpl->qlayout) {
        pimpl->qlayout->addWidget(control->get_qwidget());
    }
}

QWidget* GroupBox::get_qwidget() const {
    return pimpl->qgroup.data();
}

}
