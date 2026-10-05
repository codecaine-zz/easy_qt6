#include "simplegui/carousel.h"
#include "detail/common.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QPushButton>

namespace simplegui {

struct Carousel::Impl {
    QPointer<QWidget> widget = new QWidget();
    QPointer<QStackedWidget> stack = new QStackedWidget();
    QPointer<QHBoxLayout> nav_layout = new QHBoxLayout();
    std::vector<std::shared_ptr<Control>> children;
    
    QPointer<QPushButton> prev_btn = new QPushButton("<");
    QPointer<QPushButton> next_btn = new QPushButton(">");

    Impl() {
        auto main_layout = new QVBoxLayout(widget);
        main_layout->setContentsMargins(0,0,0,0);
        
        main_layout->addWidget(stack);
        
        nav_layout->addStretch();
        nav_layout->addWidget(prev_btn);
        nav_layout->addWidget(next_btn);
        nav_layout->addStretch();
        
        main_layout->addLayout(nav_layout);
    }
    ~Impl() {
        detail::delete_if_orphan(widget);
    }
};

Carousel::Carousel() : pimpl(std::make_shared<Impl>()) {
    QObject::connect(pimpl->prev_btn, &QPushButton::clicked, pimpl->widget, [this]() {
        previous();
    });
    QObject::connect(pimpl->next_btn, &QPushButton::clicked, pimpl->widget, [this]() {
        next();
    });
}

Carousel::~Carousel() = default;

void Carousel::add_child(std::shared_ptr<Control> control) {
    if (!pimpl->stack || !control) return;
    pimpl->children.push_back(control);
    pimpl->stack->addWidget(control->get_qwidget());
}

void Carousel::set_current_index(int index) {
    if (!pimpl->stack) return;
    if (index >= 0 && index < pimpl->stack->count()) {
        pimpl->stack->setCurrentIndex(index);
    }
}

int Carousel::current_index() const {
    return pimpl->stack ? pimpl->stack->currentIndex() : 0;
}

int Carousel::count() const {
    return pimpl->stack ? pimpl->stack->count() : 0;
}

void Carousel::next() {
    if (!pimpl->stack) return;
    if (pimpl->stack->count() == 0) return;
    int i = pimpl->stack->currentIndex();
    if (i < pimpl->stack->count() - 1) pimpl->stack->setCurrentIndex(i + 1);
    else pimpl->stack->setCurrentIndex(0); // loop
}

void Carousel::previous() {
    if (!pimpl->stack) return;
    if (pimpl->stack->count() == 0) return;
    int i = pimpl->stack->currentIndex();
    if (i > 0) pimpl->stack->setCurrentIndex(i - 1);
    else pimpl->stack->setCurrentIndex(pimpl->stack->count() - 1);
}

QWidget* Carousel::get_qwidget() const {
    return pimpl->widget.data();
}

}
