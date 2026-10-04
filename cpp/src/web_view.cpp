#include "simplegui/web_view.h"
#include "detail/common.h"

#include <QFileInfo>
#include <QUrl>
#include <QWebEngineSettings>
#include <QWebEngineView>

#include <algorithm>
#include <cmath>

namespace simplegui {

struct WebView::Impl {
    QPointer<QWebEngineView> view = new QWebEngineView();
    detail::Event<bool> loaded = detail::make_event<bool>(view);
    ~Impl() { detail::delete_if_orphan(view); }
};

WebView::WebView(const std::string& url) : pimpl(std::make_shared<Impl>()) {
    std::weak_ptr<Impl> weak = pimpl;
    QObject::connect(pimpl->view.data(), &QWebEngineView::loadFinished, [weak](bool ok) {
        if (auto d = weak.lock()) detail::fire(d->loaded, ok);
    });
    if (!url.empty()) set_url(url);
}

WebView::~WebView() = default;

void WebView::set_url(const std::string& url) {
    if (pimpl->view) pimpl->view->setUrl(QUrl::fromUserInput(detail::qs(url)));
}

std::string WebView::url() const {
    return pimpl->view ? detail::ss(pimpl->view->url().toString()) : std::string();
}

void WebView::set_html(const std::string& html, const std::string& base_url) {
    if (pimpl->view) pimpl->view->setHtml(detail::qs(html), base_url.empty() ? QUrl() : QUrl::fromUserInput(detail::qs(base_url)));
}

void WebView::back() {
    if (pimpl->view) pimpl->view->back();
}
void WebView::forward() {
    if (pimpl->view) pimpl->view->forward();
}
void WebView::reload() {
    if (pimpl->view) pimpl->view->reload();
}

EventConnection WebView::on_load_finished(std::function<void(bool)> handler) {
    return detail::add_handler(pimpl->loaded, std::move(handler));
}

QWidget* WebView::get_qwidget() const { return pimpl->view.data(); }

// ---------------------------------------------------------------------------
// PdfView
// ---------------------------------------------------------------------------
PdfView::PdfView(const std::string& pdf_path) : WebView() {
    if (auto* view = static_cast<QWebEngineView*>(get_qwidget())) {
        view->settings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
        view->settings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);
    }
    if (!pdf_path.empty()) set_file(pdf_path);
}

bool PdfView::set_file(const std::string& pdf_path) {
    const QFileInfo info(detail::qs(pdf_path));
    if (!info.isFile()) return false;
    set_url(detail::ss(QUrl::fromLocalFile(info.absoluteFilePath()).toString()));
    return true;
}

// ---------------------------------------------------------------------------
// MapView
// ---------------------------------------------------------------------------
MapView::MapView(double lat, double lng, int zoom) : WebView() {
    lat_ = lat;
    lng_ = lng;
    zoom_ = zoom;
    render();
}

void MapView::set_coordinates(double lat, double lng) {
    lat_ = lat;
    lng_ = lng;
    render();
}

void MapView::set_zoom(int zoom) {
    zoom_ = zoom;
    render();
}

void MapView::render() {
    // Only clamped numbers are inserted into the page, never user text.
    const double lat = std::isfinite(lat_) ? std::clamp(lat_, -90.0, 90.0) : 0.0;
    const double lng = std::isfinite(lng_) ? std::clamp(lng_, -180.0, 180.0) : 0.0;
    const int zoom = std::clamp(zoom_, 1, 19);

    // Leaflet is pinned to an exact version with Subresource Integrity hashes, so a
    // tampered CDN file is refused by the browser engine.
    const QString html = QStringLiteral(R"(<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Map</title>
  <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"
        integrity="sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY=" crossorigin="">
  <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"
          integrity="sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo=" crossorigin=""></script>
</head>
<body style="margin:0;padding:0;">
  <div id="map" style="width:100vw;height:100vh;"></div>
  <script>
    var map = L.map('map').setView([%1, %2], %3);
    L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
      maxZoom: 19,
      attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
    }).addTo(map);
    L.marker([%1, %2]).addTo(map);
  </script>
</body>
</html>)")
                             .arg(lat, 0, 'f', 6)
                             .arg(lng, 0, 'f', 6)
                             .arg(zoom);
    set_html(detail::ss(html));
}

}  // namespace simplegui
