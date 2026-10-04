#include "simplegui/link.h"
#include "detail/common.h"

#include <QDesktopServices>
#include <QLabel>
#include <QUrl>

namespace simplegui {

namespace {

bool is_safe_url(const QUrl& url) {
    const QString scheme = url.scheme().toLower();
    return url.isValid() && (scheme == QLatin1String("http") || scheme == QLatin1String("https") ||
                             scheme == QLatin1String("mailto"));
}

}  // namespace

struct Link::Impl {
    QPointer<QLabel> label = new QLabel();
    QString text;
    QUrl url;
    detail::Event<const std::string&> clicked;

    Impl() {
        label->setTextFormat(Qt::RichText);
        label->setCursor(Qt::PointingHandCursor);
        label->setOpenExternalLinks(false);  // we open links ourselves after validating them
        label->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard);
        clicked = detail::make_event<const std::string&>(label);
    }
    ~Impl() { detail::delete_if_orphan(label); }

    void refresh() {
        if (!label) return;
        // Both parts are escaped so text/URLs can never inject markup.
        label->setText(QStringLiteral("<a href=\"%1\">%2</a>")
                           .arg(url.toString(QUrl::FullyEncoded).toHtmlEscaped(), text.toHtmlEscaped()));
    }
};

Link::Link(const std::string& text, const std::string& url) : pimpl(std::make_shared<Impl>()) {
    pimpl->text = detail::qs(text);
    pimpl->url = QUrl(detail::qs(url));
    pimpl->refresh();

    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->label.data(), &QLabel::linkActivated, [weak](const QString&) {
        auto d = weak.lock();
        if (!d) return;
        if (is_safe_url(d->url)) QDesktopServices::openUrl(d->url);
        detail::fire(d->clicked, detail::ss(d->url.toString()));
    });
}

Link::~Link() = default;

void Link::set_text(const std::string& text) {
    pimpl->text = detail::qs(text);
    pimpl->refresh();
}

std::string Link::get_text() const {
    return detail::ss(pimpl->text);
}

void Link::set_url(const std::string& url) {
    pimpl->url = QUrl(detail::qs(url));
    pimpl->refresh();
}

std::string Link::get_url() const {
    return detail::ss(pimpl->url.toString());
}

EventConnection Link::on_click(std::function<void(const std::string&)> handler) {
    return detail::add_handler(pimpl->clicked, std::move(handler));
}

QWidget* Link::get_qwidget() const {
    return pimpl->label.data();
}

}  // namespace simplegui
