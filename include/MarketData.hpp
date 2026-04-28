#pragma once

#include <chrono>
#include <string>
#include <utility>

struct alignas(64) MarketData {
    std::string symbol;
    double bid_price;
    double ask_price;
    std::chrono::high_resolution_clock::time_point timestamp;

    MarketData(std::string sym, double bid, double ask)
        : symbol(std::move(sym)),
          bid_price(bid),
          ask_price(ask),
          timestamp(std::chrono::high_resolution_clock::now()) {}
};

class MarketDataFeed {
public:
    explicit MarketDataFeed(std::string symbol = "AAPL");

    MarketData nextTick();

private:
    std::string symbol_;
    int tick_count_;
};