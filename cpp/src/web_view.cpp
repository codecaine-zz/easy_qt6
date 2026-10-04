#include "simplegui/web_view.h"
#include <QWebEngineView>
#include <QPointer>
#include <QUrl>
#include <QString>

namespace simplegui {

struct WebView::Impl {
    QPointer<QWebEngineView> qweb;
    Impl(const std::string& url) {
        qweb = new QWebEngineView();
        if (!url.empty()) {
            qweb->setUrl(QUrl(QString::fromStdString(url)));
        }
    }
    ~Impl() { if (qweb && !qweb->parent()) delete qweb; }
};

WebView::WebView(const std::string& url)
    : pimpl(std::make_shared<Impl>(url)) {}

WebView::~WebView() = default;

void WebView::set_url(const std::string& url) {
    if (pimpl->qweb) pimpl->qweb->setUrl(QUrl(QString::fromStdString(url)));
}

void WebView::set_html(const std::string& html) {
    if (pimpl->qweb) pimpl->qweb->setHtml(QString::fromStdString(html));
}

QWidget* WebView::get_qwidget() const {
    return pimpl->qweb.data();
}

void MapView::set_coordinates(double lat, double lng) {
    QString html = QString(R"(
        <!DOCTYPE html>
        <html>
        <head>
            <title>Map</title>
            <meta charset="utf-8" />
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <link rel="stylesheet" href="https://unpkg.com/leaflet@1.7.1/dist/leaflet.css" />
            <script src="https://unpkg.com/leaflet@1.7.1/dist/leaflet.js"></script>
        </head>
        <body style="margin: 0; padding: 0;">
            <div id="map" style="width: 100vw; height: 100vh;"></div>
            <script>
                var map = L.map('map').setView([%1, %2], 13);
                L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                    maxZoom: 19
                }).addTo(map);
                L.marker([%1, %2]).addTo(map);
            </script>
        </body>
        </html>
    )").arg(lat).arg(lng);
    set_html(html.toStdString());
}

}
