#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A built-in web browser panel (Chromium engine via QtWebEngine).
//   auto web = std::make_shared<WebView>("https://example.com");
class WebView : public Control {
public:
    explicit WebView(const std::string& url = "");
    ~WebView() override;

    // Opens a web address. "example.com" is treated as "https://example.com".
    void set_url(const std::string& url);
    std::string url() const;
    // Shows your own HTML text. Relative links/images resolve against base_url if given.
    void set_html(const std::string& html, const std::string& base_url = "");

    void back();
    void forward();
    void reload();

    // Runs when a page finishes loading (ok = false if it failed).
    EventConnection on_load_finished(std::function<void(bool ok)> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

// A WebView that starts with HTML text instead of a web address.
class HtmlView : public WebView {
public:
    explicit HtmlView(const std::string& html = "") : WebView() {
        if (!html.empty()) set_html(html);
    }
};

// Shows a PDF file using the browser engine's built-in PDF viewer.
class PdfView : public WebView {
public:
    explicit PdfView(const std::string& pdf_path = "");
    bool set_file(const std::string& pdf_path);   // false if the file does not exist
};

// An interactive OpenStreetMap map with a marker (needs an internet connection).
// Latitude is -90..90 (south..north), longitude is -180..180 (west..east).
class MapView : public WebView {
public:
    explicit MapView(double lat = 0.0, double lng = 0.0, int zoom = 13);
    void set_coordinates(double lat, double lng);
    void set_zoom(int zoom);   // 1 (whole world) .. 19 (street level)

private:
    void render();
    double lat_ = 0.0;
    double lng_ = 0.0;
    int zoom_ = 13;
};

}  // namespace simplegui
