#include "simplegui/progress_indicator.h"
#include "detail/common.h"

#include <QProgressBar>

namespace simplegui {

struct ProgressIndicator::Impl {
    QPointer<QProgressBar> bar;
    int min_val = 0;
    int max_val = 100;
    bool indeterminate = false;

    Impl(int lo, int hi, int initial_val) : bar(new QProgressBar()), min_val(std::min(lo, hi)), max_val(std::max(lo, hi)) {
        bar->setRange(min_val, max_val);
        bar->setValue(initial_val);
    }
    ~Impl() { detail::delete_if_orphan(bar); }
};

ProgressIndicator::ProgressIndicator(int min_val, int max_val, int initial_val)
    : pimpl(std::make_shared<Impl>(min_val, max_val, initial_val)) {}

ProgressIndicator::~ProgressIndicator() = default;

int ProgressIndicator::get_value() const {
    return pimpl->bar ? pimpl->bar->value() : 0;
}

void ProgressIndicator::set_value(int value) {
    if (pimpl->bar) pimpl->bar->setValue(value);
}

void ProgressIndicator::set_range(int min_val, int max_val) {
    pimpl->min_val = std::min(min_val, max_val);
    pimpl->max_val = std::max(min_val, max_val);
    if (pimpl->bar && !pimpl->indeterminate) pimpl->bar->setRange(pimpl->min_val, pimpl->max_val);
}

void ProgressIndicator::set_indeterminate(bool indeterminate) {
    pimpl->indeterminate = indeterminate;
    if (!pimpl->bar) return;
    if (indeterminate) {
        pimpl->bar->setRange(0, 0);  // Qt convention for a "busy" bar
    } else {
        pimpl->bar->setRange(pimpl->min_val, pimpl->max_val);  // restore the real range
    }
}

void ProgressIndicator::set_show_text(bool show) {
    if (pimpl->bar) pimpl->bar->setTextVisible(show);
}

QWidget* ProgressIndicator::get_qwidget() const {
    return pimpl->bar.data();
}

}  // namespace simplegui
