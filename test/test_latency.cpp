#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

#include "../include/MatchingEngine.hpp"
#include "../include/OrderBook.hpp"
#include "../include/OrderManager.hpp"
#include "../include/Timer.hpp"
#include "../include/TradeLogger.hpp"

int main() {
    using PriceType = double;
    using OrderIdType = int;

    OrderBook<PriceType, OrderIdType> book;
    TradeLogger<PriceType, OrderIdType> logger;
    OrderManager<OrderIdType> manager;
    MatchingEngine<PriceType, OrderIdType> engine(book, logger, manager);

    manager.onNewOrder(1);
    manager.onNewOrder(2);

    book.addOrder(1, "AAPL", 150.25, 100, true);
    book.addOrder(2, "AAPL", 150.20, 50, false);

    Timer timer;
    timer.start();

    auto trades = engine.match();

    long long latency = timer.stopNanoseconds();

    assert(trades.size() == 1);
    assert(trades[0].quantity == 50);
    assert(trades[0].price == 150.20);
    assert(logger.size() == 1);
    assert(latency >= 0);

    std::cout << "Latency test passed.\n";
    std::cout << "Measured latency: " << latency << " ns\n";

    return 0;
}