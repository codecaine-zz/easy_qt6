#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>

namespace simplegui {

class WebView : public Control {
public:
    explicit WebView(const std::string& url = "");
    ~WebView() override;

    void set_url(const std::string& url);
    void set_html(const std::string& html);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

// Aliases for the other views requested by the API
class HtmlView : public WebView {
public:
    explicit HtmlView(const std::string& html = "") : WebView() {
        if (!html.empty()) set_html(html);
    }
};

class PdfView : public WebView {
public:
    explicit PdfView(const std::string& pdf_path = "") : WebView(pdf_path) {}
};

class MapView : public WebView {
public:
    explicit MapView(double lat = 0.0, double lng = 0.0) : WebView() {
        set_coordinates(lat, lng);
    }
    void set_coordinates(double lat, double lng);
};

}
