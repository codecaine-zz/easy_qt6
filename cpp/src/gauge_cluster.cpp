#include "simplegui/gauge_cluster.h"
#include "detail/common.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

namespace simplegui {

struct GaugeCluster::Impl {
    QPointer<QWidget> widget = new QWidget();
    QPointer<QHBoxLayout> layout = new QHBoxLayout(widget);

    Impl() {
        layout->setSpacing(20);
        layout->setAlignment(Qt::AlignCenter);
    }
    ~Impl() {
        detail::delete_if_orphan(widget);
    }
};

GaugeCluster::GaugeCluster() : pimpl(std::make_shared<Impl>()) {}
GaugeCluster::~GaugeCluster() = default;

void GaugeCluster::add_gauge(std::shared_ptr<Control> gauge, const std::string& label) {
    if (!pimpl->widget || !gauge) return;
    
    auto container = new QWidget();
    auto vlayout = new QVBoxLayout(container);
    vlayout->setAlignment(Qt::AlignCenter);
    
    vlayout->addWidget(gauge->get_qwidget());
    
    if (!label.empty()) {
        auto lbl = new QLabel(detail::qs(label));
        lbl->setAlignment(Qt::AlignCenter);
        vlayout->addWidget(lbl);
    }
    
    pimpl->layout->addWidget(container);
}

QWidget* GaugeCluster::get_qwidget() const {
    return pimpl->widget.data();
}

}
