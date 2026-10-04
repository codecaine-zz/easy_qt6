#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <memory>
#include <string>
#include <functional>

namespace simplegui {

class ProductCard : public Control {
public:
    ProductCard(const std::string& title,
                const std::string& description,
                const std::string& price,
                const std::string& badge = "",
                double rating = 4.8,
                const std::string& button_text = "Add to Cart");
    ~ProductCard() override;

    void set_price(const std::string& price);
    void set_badge(const std::string& badge);
    void set_rating(double rating);
    void set_in_stock(bool in_stock);

    EventConnection on_buy(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
