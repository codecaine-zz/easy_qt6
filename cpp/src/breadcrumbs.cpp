#include "simplegui/breadcrumbs.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPointer>

namespace simplegui {

class ClickableLabel : public QLabel {
public:
    int index;
    std::function<void(int)> on_click;
    
    ClickableLabel(const QString& text, int idx) : QLabel(text), index(idx) {
        setStyleSheet("color: #3498db; text-decoration: underline;");
        setCursor(Qt::PointingHandCursor);
    }
    
protected:
    void mousePressEvent(QMouseEvent*) override {
        if (on_click) on_click(index);
    }
};

struct Breadcrumbs::Impl {
    QPointer<QWidget> widget;
    std::vector<ClickableLabel*> labels;
    
    Impl(const std::vector<std::string>& crumbs) {
        widget = new QWidget();
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);
        
        for (size_t i = 0; i < crumbs.size(); ++i) {
            auto label = new ClickableLabel(QString::fromStdString(crumbs[i]), i);
            labels.push_back(label);
            layout->addWidget(label);
            
            if (i < crumbs.size() - 1) {
                auto sep = new QLabel(">");
                sep->setStyleSheet("color: #7f8c8d;");
                layout->addWidget(sep);
            }
        }
        layout->addStretch();
    }
    ~Impl() { if (widget && !widget->parent()) delete widget; }
};

Breadcrumbs::Breadcrumbs(const std::vector<std::string>& crumbs)
    : pimpl(std::make_shared<Impl>(crumbs)) {}

Breadcrumbs::~Breadcrumbs() = default;

EventConnection Breadcrumbs::on_click(std::function<void(int)> handler) {
    if (pimpl->widget) {
        for (auto lbl : pimpl->labels) {
            lbl->on_click = handler;
        }
    }
    return EventConnection();
}

QWidget* Breadcrumbs::get_qwidget() const {
    return pimpl->widget.data();
}

}
