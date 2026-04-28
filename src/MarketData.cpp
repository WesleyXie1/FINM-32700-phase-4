#include "../include/MarketData.hpp"

MarketDataFeed::MarketDataFeed(std::string symbol)
    : symbol_(std::move(symbol)), tick_count_(0) {}

MarketData MarketDataFeed::nextTick() {
    double mid = 150.0 + static_cast<double>(tick_count_ % 20) * 0.01;
    double spread = 0.02;

    ++tick_count_;

    return MarketData(symbol_, mid - spread / 2.0, mid + spread / 2.0);
}