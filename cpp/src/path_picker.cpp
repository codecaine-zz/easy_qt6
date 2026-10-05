#include "simplegui/path_picker.h"
#include "detail/common.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>
#include <QFileDialog>

namespace simplegui {

struct PathPicker::Impl {
    QPointer<QWidget> widget = new QWidget();
    QPointer<QLineEdit> edit = new QLineEdit();
    QPointer<QToolButton> btn = new QToolButton();
    PathPicker::Mode mode;
    QString filter = "All files (*)";
    std::function<void(const std::string&)> on_change;
    
    Impl(PathPicker::Mode mode) : mode(mode) {
        auto layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0,0,0,0);
        layout->addWidget(edit);
        btn->setText("...");
        layout->addWidget(btn);
    }
    ~Impl() {
        detail::delete_if_orphan(widget);
    }
};

PathPicker::PathPicker(Mode mode) : pimpl(std::make_shared<Impl>(mode)) {
    QObject::connect(pimpl->btn, &QToolButton::clicked, pimpl->widget, [this]() {
        QString path;
        if (pimpl->mode == Mode::File) {
            path = QFileDialog::getOpenFileName(pimpl->widget, "Open File", pimpl->edit->text(), pimpl->filter);
        } else {
            path = QFileDialog::getExistingDirectory(pimpl->widget, "Select Folder", pimpl->edit->text());
        }
        if (!path.isEmpty()) {
            pimpl->edit->setText(path);
            if (pimpl->on_change) pimpl->on_change(path.toStdString());
        }
    });

    QObject::connect(pimpl->edit, &QLineEdit::textChanged, pimpl->widget, [this](const QString& text) {
        if (pimpl->on_change) pimpl->on_change(text.toStdString());
    });
}

PathPicker::~PathPicker() = default;

void PathPicker::set_path(const std::string& path) {
    if (pimpl->edit) pimpl->edit->setText(detail::qs(path));
}

std::string PathPicker::get_path() const {
    return pimpl->edit ? pimpl->edit->text().toStdString() : "";
}

void PathPicker::set_filter(const std::string& filter) {
    pimpl->filter = detail::qs(filter);
}

EventConnection PathPicker::on_change(std::function<void(const std::string&)> handler) {
    pimpl->on_change = std::move(handler);
    return EventConnection();
}

QWidget* PathPicker::get_qwidget() const {
    return pimpl->widget.data();
}

}
