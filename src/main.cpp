#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

#include "../include/MarketData.hpp"
#include "../include/MatchingEngine.hpp"
#include "../include/OrderBook.hpp"
#include "../include/OrderManager.hpp"
#include "../include/Timer.hpp"
#include "../include/TradeLogger.hpp"

using PriceType = double;
using OrderIdType = int;

using Book = OrderBook<PriceType, OrderIdType>;
using Logger = TradeLogger<PriceType, OrderIdType>;
using Manager = OrderManager<OrderIdType>;
using Engine = MatchingEngine<PriceType, OrderIdType>;

void analyzeLatencies(std::vector<long long> latencies) {
    if (latencies.empty()) {
        std::cout << "No latency data collected.\n";
        return;
    }

    std::sort(latencies.begin(), latencies.end());

    long long min_latency = latencies.front();
    long long max_latency = latencies.back();

    double mean = std::accumulate(latencies.begin(), latencies.end(), 0.0)
                  / static_cast<double>(latencies.size());

    double variance = 0.0;

    for (long long latency : latencies) {
        double diff = static_cast<double>(latency) - mean;
        variance += diff * diff;
    }

    variance /= static_cast<double>(latencies.size());

    double stddev = std::sqrt(variance);

    std::size_t p99_index = static_cast<std::size_t>(
        0.99 * static_cast<double>(latencies.size() - 1)
    );

    long long p99 = latencies[p99_index];

    std::cout << "\nTick-to-Trade Latency Statistics (nanoseconds)\n";
    std::cout << "------------------------------------------------\n";
    std::cout << "Samples: " << latencies.size() << "\n";
    std::cout << "Min:     " << min_latency << "\n";
    std::cout << "Max:     " << max_latency << "\n";
    std::cout << "Mean:    " << mean << "\n";
    std::cout << "StdDev:  " << stddev << "\n";
    std::cout << "P99:     " << p99 << "\n";
}

int main() {
    constexpr int num_ticks = 100000;

    MarketDataFeed feed("AAPL");

    Book order_book;
    Logger trade_logger(num_ticks);
    Manager order_manager;
    Engine matching_engine(order_book, trade_logger, order_manager);

    std::vector<long long> latencies;
    latencies.reserve(num_ticks);

    OrderIdType next_order_id = 1;

    for (int i = 0; i < num_ticks; ++i) {
        Timer timer;
        timer.start();

        MarketData tick = feed.nextTick();

        bool is_buy = (i % 2 == 0);

        PriceType order_price;

        if (is_buy) {
            order_price = tick.ask_price + 0.01;
        } else {
            order_price = tick.bid_price - 0.01;
        }

        int quantity = 100 + (i % 5) * 10;

        OrderIdType order_id = next_order_id++;

        order_manager.onNewOrder(order_id);

        order_book.addOrder(
            order_id,
            tick.symbol,
            order_price,
            quantity,
            is_buy
        );

        matching_engine.match();

        long long latency = timer.stopNanoseconds();
        latencies.push_back(latency);
    }

    analyzeLatencies(latencies);

    std::cout << "\nFinal System Summary\n";
    std::cout << "--------------------\n";
    std::cout << "Trades executed: " << trade_logger.size() << "\n";
    std::cout << "Remaining buy orders: " << order_book.buySize() << "\n";
    std::cout << "Remaining sell orders: " << order_book.sellSize() << "\n";

    if (order_book.bestBid().has_value()) {
        std::cout << "Best bid: " << order_book.bestBid().value() << "\n";
    } else {
        std::cout << "Best bid: N/A\n";
    }

    if (order_book.bestAsk().has_value()) {
        std::cout << "Best ask: " << order_book.bestAsk().value() << "\n";
    } else {
        std::cout << "Best ask: N/A\n";
    }

    trade_logger.flushToCsv("trades.csv");

    std::cout << "\nTrade log written to trades.csv\n";

    return 0;
}