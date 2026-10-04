#include "simplegui/breadcrumbs.h"
#include "detail/common.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

namespace simplegui {
namespace {

class ClickableLabel : public QLabel {
public:
    ClickableLabel(const QString& text, std::function<void()> click) : click_(std::move(click)) {
        setTextFormat(Qt::PlainText);  // crumbs are shown literally, never as HTML
        setText(text);
        setStyleSheet(QStringLiteral("color: #3498db; text-decoration: underline;"));
        setCursor(Qt::PointingHandCursor);
    }

protected:
    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton && rect().contains(event->position().toPoint()) && click_) click_();
        QLabel::mouseReleaseEvent(event);
    }

private:
    std::function<void()> click_;
};

}  // namespace

struct Breadcrumbs::Impl : std::enable_shared_from_this<Breadcrumbs::Impl> {
    QPointer<QWidget> widget = new QWidget();
    QPointer<QHBoxLayout> layout = new QHBoxLayout(widget);
    std::vector<std::string> items;
    detail::Event<int> clicked = detail::make_event<int>(widget);

    Impl() {
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(5);
    }
    ~Impl() { detail::delete_if_orphan(widget); }

    void rebuild() {
        if (!layout) return;
        while (QLayoutItem* item = layout->takeAt(0)) {
            if (QWidget* w = item->widget()) w->deleteLater();
            delete item;
        }
        std::weak_ptr<Impl> weak = weak_from_this();
        for (size_t i = 0; i < items.size(); ++i) {
            const int index = static_cast<int>(i);
            layout->addWidget(new ClickableLabel(detail::qs(items[i]), [weak, index]() {
                if (auto d = weak.lock()) detail::fire(d->clicked, index);
            }));
            if (i + 1 < items.size()) {
                auto* sep = new QLabel(QStringLiteral(">"));
                sep->setStyleSheet(QStringLiteral("color: #7f8c8d;"));
                layout->addWidget(sep);
            }
        }
        layout->addStretch();
    }
};

Breadcrumbs::Breadcrumbs(const std::vector<std::string>& crumbs) : pimpl(std::make_shared<Impl>()) {
    set_crumbs(crumbs);
}

Breadcrumbs::~Breadcrumbs() = default;

void Breadcrumbs::set_crumbs(const std::vector<std::string>& crumbs) {
    pimpl->items = crumbs;
    pimpl->rebuild();
}

void Breadcrumbs::push(const std::string& crumb) {
    pimpl->items.push_back(crumb);
    pimpl->rebuild();
}

void Breadcrumbs::pop() {
    if (pimpl->items.empty()) return;
    pimpl->items.pop_back();
    pimpl->rebuild();
}

std::vector<std::string> Breadcrumbs::crumbs() const { return pimpl->items; }

EventConnection Breadcrumbs::on_click(std::function<void(int)> handler) {
    return detail::add_handler(pimpl->clicked, std::move(handler));
}

QWidget* Breadcrumbs::get_qwidget() const { return pimpl->widget.data(); }

}  // namespace simplegui
