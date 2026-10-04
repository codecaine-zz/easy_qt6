#pragma once
#include "simplegui/control.h"
#include "simplegui/event_connection.h"
#include <functional>
#include <memory>
#include <string>

namespace simplegui {

// A shop-style card: badge, star rating, title, description, price, and a buy button.
// The price is plain text, so you choose the currency: "$19.99", "€5", "¥800".
class ProductCard : public Control {
public:
    ProductCard(const std::string& title,
                const std::string& description,
                const std::string& price,
                const std::string& badge = "",
                double rating = 4.8,
                const std::string& button_text = "Add to Cart");
    ~ProductCard() override;

    void set_title(const std::string& title);
    void set_description(const std::string& description);
    void set_price(const std::string& price);
    void set_badge(const std::string& badge);        // "" hides the badge
    void set_rating(double rating);                  // shown as "★ 4.8"
    void set_button_text(const std::string& text);
    void set_in_stock(bool in_stock);                // false = disabled "Out of Stock" button

    // Runs when the buy button is clicked.
    EventConnection on_buy(std::function<void()> handler);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}  // namespace simplegui
