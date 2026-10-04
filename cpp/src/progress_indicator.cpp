#include "simplegui/progress_indicator.h"
#include <QProgressBar>
#include <QPointer>

namespace simplegui {

struct ProgressIndicator::Impl {
    QPointer<QProgressBar> qprogress;
    Impl(int min_val, int max_val, int initial_val) {
        qprogress = new QProgressBar();
        qprogress->setRange(min_val, max_val);
        qprogress->setValue(initial_val);
    }
    ~Impl() { if (qprogress && !qprogress->parent()) delete qprogress; }
};

ProgressIndicator::ProgressIndicator(int min_val, int max_val, int initial_val)
    : pimpl(std::make_shared<Impl>(min_val, max_val, initial_val)) {}

ProgressIndicator::~ProgressIndicator() = default;

int ProgressIndicator::get_value() const {
    if (pimpl->qprogress) {
        return pimpl->qprogress->value();
    }
    return 0;
}

void ProgressIndicator::set_value(int value) {
    if (pimpl->qprogress) {
        pimpl->qprogress->setValue(value);
    }
}

void ProgressIndicator::set_indeterminate(bool indeterminate) {
    if (pimpl->qprogress) {
        if (indeterminate) {
            pimpl->qprogress->setRange(0, 0); // Qt convention for indeterminate
        } else {
            pimpl->qprogress->setRange(0, 100); // Default restore
        }
    }
}

QWidget* ProgressIndicator::get_qwidget() const {
    return pimpl->qprogress.data();
}

}
