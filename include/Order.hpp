#pragma once

#include <string>
#include <type_traits>
#include <utility>

enum class OrderState {
    New,
    Partial,
    Filled,
    Canceled
};

template <typename PriceType, typename OrderIdType>
struct Order {
    static_assert(std::is_arithmetic<PriceType>::value,
                  "Price type must be numeric.");

    static_assert(std::is_integral<OrderIdType>::value,
                  "Order ID must be an integer.");

    OrderIdType id;
    std::string symbol;
    PriceType price;
    int quantity;
    bool is_buy;

    Order(OrderIdType order_id,
          std::string sym,
          PriceType order_price,
          int qty,
          bool buy)
        : id(order_id),
          symbol(std::move(sym)),
          price(order_price),
          quantity(qty),
          is_buy(buy) {}
};