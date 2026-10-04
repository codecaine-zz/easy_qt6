#include "simplegui/window.h"
#include "simplegui/control.h"
#include "detail/common.h"

#include <QAction>
#include <QCloseEvent>
#include <QGuiApplication>
#include <QIcon>
#include <QKeySequence>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPixmap>
#include <QScreen>
#include <QStatusBar>

#include <algorithm>
#include <map>

namespace simplegui {
namespace {

// Close handlers are stored in a shared list so EventConnection tokens stay safe
// even if they outlive the window.
struct CloseHandlers {
    std::vector<std::pair<std::size_t, std::function<bool()>>> list;
    std::size_t next_id = 0;

    bool allow_close() const {
        const auto snapshot = list;  // handlers may disconnect themselves
        for (const auto& entry : snapshot) {
            if (!entry.second()) return false;  // first "no" wins
        }
        return true;
    }
};

class MainWindow : public QMainWindow {
public:
    std::shared_ptr<CloseHandlers> close_handlers = std::make_shared<CloseHandlers>();

protected:
    void closeEvent(QCloseEvent* event) override {
        if (close_handlers->allow_close()) {
            event->accept();
        } else {
            event->ignore();
        }
    }
};

}  // namespace

struct Window::Impl {
    MainWindow qwindow;
    std::shared_ptr<Control> main_content;
    std::map<std::string, QPointer<QMenu>> menus;

    QMenu* menu(const std::string& name) {
        auto& m = menus[name];
        if (!m) m = qwindow.menuBar()->addMenu(detail::qs(name));
        return m;
    }
};

Window::Window(const std::string& title, int width, int height) : pimpl(std::make_unique<Impl>()) {
    set_title(title);
    set_size(width, height);
}

Window::~Window() {
    // Hand the content widget back to its Control (if someone else still owns it)
    // instead of letting the window delete it.
    if (pimpl->main_content && pimpl->main_content.use_count() > 1) {
        if (QWidget* w = pimpl->qwindow.takeCentralWidget()) w->setParent(nullptr);
    }
}

void Window::set_content(std::shared_ptr<Control> content) {
    // takeCentralWidget() keeps Qt from deleting the old content; it stays usable elsewhere.
    if (QWidget* old = pimpl->qwindow.takeCentralWidget()) old->setParent(nullptr);
    pimpl->main_content = std::move(content);
    if (pimpl->main_content) {
        if (QWidget* w = pimpl->main_content->get_qwidget()) pimpl->qwindow.setCentralWidget(w);
    }
}

std::shared_ptr<Control> Window::content() const { return pimpl->main_content; }

void Window::show() { pimpl->qwindow.show(); }
void Window::hide() { pimpl->qwindow.hide(); }
void Window::close() { pimpl->qwindow.close(); }
bool Window::is_visible() const { return pimpl->qwindow.isVisible(); }
void Window::maximize() { pimpl->qwindow.showMaximized(); }
void Window::minimize() { pimpl->qwindow.showMinimized(); }

void Window::set_fullscreen(bool fullscreen) {
    if (fullscreen) {
        pimpl->qwindow.showFullScreen();
    } else {
        pimpl->qwindow.showNormal();
    }
}

void Window::set_title(const std::string& title) { pimpl->qwindow.setWindowTitle(detail::qs(title)); }
std::string Window::title() const { return detail::ss(pimpl->qwindow.windowTitle()); }

void Window::set_size(int width, int height) { pimpl->qwindow.resize(std::max(1, width), std::max(1, height)); }
void Window::set_min_size(int width, int height) { pimpl->qwindow.setMinimumSize(std::max(0, width), std::max(0, height)); }
void Window::set_fixed_size(int width, int height) { pimpl->qwindow.setFixedSize(std::max(1, width), std::max(1, height)); }
int Window::width() const { return pimpl->qwindow.width(); }
int Window::height() const { return pimpl->qwindow.height(); }
void Window::set_position(int x, int y) { pimpl->qwindow.move(x, y); }

void Window::center() {
    QScreen* screen = pimpl->qwindow.screen() ? pimpl->qwindow.screen() : QGuiApplication::primaryScreen();
    if (!screen) return;
    QRect frame = pimpl->qwindow.frameGeometry();
    frame.moveCenter(screen->availableGeometry().center());
    pimpl->qwindow.move(frame.topLeft());
}

bool Window::set_icon(const std::string& image_path) {
    const QPixmap pix(detail::qs(image_path));
    if (pix.isNull()) return false;
    pimpl->qwindow.setWindowIcon(QIcon(pix));
    return true;
}

void Window::set_status_text(const std::string& text) { pimpl->qwindow.statusBar()->showMessage(detail::qs(text)); }

EventConnection Window::add_menu_item(const std::string& menu, const std::string& item,
                                      std::function<void()> handler, const std::string& shortcut) {
    QAction* action = pimpl->menu(menu)->addAction(detail::qs(item));
    if (!shortcut.empty()) action->setShortcut(QKeySequence(detail::qs(shortcut)));
    if (!handler) return {};
    return detail::wrap(QObject::connect(action, &QAction::triggered, [handler = std::move(handler)]() { handler(); }));
}

void Window::add_menu_separator(const std::string& menu) { pimpl->menu(menu)->addSeparator(); }

EventConnection Window::on_close(std::function<bool()> handler) {
    if (!handler) return {};
    auto handlers = pimpl->qwindow.close_handlers;
    const std::size_t id = ++handlers->next_id;
    handlers->list.emplace_back(id, std::move(handler));
    std::weak_ptr<CloseHandlers> weak = handlers;
    return EventConnection([weak, id]() {
        if (auto h = weak.lock()) {
            h->list.erase(std::remove_if(h->list.begin(), h->list.end(), [id](const auto& e) { return e.first == id; }),
                          h->list.end());
        }
    });
}

bool Window::save_screenshot(const std::string& filepath) {
    if (filepath.empty()) return false;
    pimpl->qwindow.show();
    pimpl->qwindow.repaint();
    return pimpl->qwindow.grab().save(detail::qs(filepath));
}

}  // namespace simplegui
