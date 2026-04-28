#pragma once

#include <chrono>
#include <string>

template <typename PriceType, typename OrderIdType>
struct Trade {
    OrderIdType buy_order_id;
    OrderIdType sell_order_id;
    std::string symbol;
    PriceType price;
    int quantity;
    std::chrono::high_resolution_clock::time_point timestamp;

    Trade(OrderIdType buy_id,
          OrderIdType sell_id,
          std::string sym,
          PriceType trade_price,
          int qty)
        : buy_order_id(buy_id),
          sell_order_id(sell_id),
          symbol(std::move(sym)),
          price(trade_price),
          quantity(qty),
          timestamp(std::chrono::high_resolution_clock::now()) {}
};