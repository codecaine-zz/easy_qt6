#pragma once
#include "simplegui/control.h"
#include <memory>
#include <string>
#include <vector>

namespace simplegui {

struct CandleData {
    std::string label; // e.g. "09:30", "Mon", "10-01"
    double open;
    double high;
    double low;
    double close;
};

class CandlestickChart : public Control {
public:
    explicit CandlestickChart(const std::string& title = "");
    ~CandlestickChart() override;

    void add_candle(const std::string& label, double open, double high, double low, double close);
    void set_candles(const std::vector<CandleData>& candles);
    void clear_candles();

    void set_title(const std::string& title);
    void show_grid(bool show);

    QWidget* get_qwidget() const override;

private:
    struct Impl;
    std::shared_ptr<Impl> pimpl;
};

}
