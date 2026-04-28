#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>

#include "MemoryPool.hpp"
#include "Order.hpp"

template <typename PriceType, typename OrderIdType>
class OrderBook {
public:
    using OrderType = Order<PriceType, OrderIdType>;
    using OrderPtr = std::unique_ptr<OrderType, PoolDeleter<OrderType>>;

    using BuyOrders = std::multimap<PriceType, OrderPtr, std::greater<PriceType>>;
    using SellOrders = std::multimap<PriceType, OrderPtr, std::less<PriceType>>;

    using BuyIterator = typename BuyOrders::iterator;
    using SellIterator = typename SellOrders::iterator;

public:
    OrderBook() = default;

    OrderBook(const OrderBook&) = delete;
    OrderBook& operator=(const OrderBook&) = delete;

    void addOrder(OrderIdType id,
                  const std::string& symbol,
                  PriceType price,
                  int quantity,
                  bool is_buy) {
        if (quantity <= 0) {
            return;
        }

        OrderType* raw_order = pool_.create(id, symbol, price, quantity, is_buy);
        OrderPtr order(raw_order, PoolDeleter<OrderType>(&pool_));

        if (is_buy) {
            buy_orders_.emplace(price, std::move(order));
        } else {
            sell_orders_.emplace(price, std::move(order));
        }
    }

    bool cancelOrder(OrderIdType id) {
        for (auto it = buy_orders_.begin(); it != buy_orders_.end(); ++it) {
            if (it->second->id == id) {
                buy_orders_.erase(it);
                return true;
            }
        }

        for (auto it = sell_orders_.begin(); it != sell_orders_.end(); ++it) {
            if (it->second->id == id) {
                sell_orders_.erase(it);
                return true;
            }
        }

        return false;
    }

    bool canMatch() const {
        if (buy_orders_.empty() || sell_orders_.empty()) {
            return false;
        }

        const auto& best_buy = buy_orders_.begin()->second;
        const auto& best_sell = sell_orders_.begin()->second;

        return best_buy->price >= best_sell->price;
    }

    BuyIterator buyBegin() {
        return buy_orders_.begin();
    }

    SellIterator sellBegin() {
        return sell_orders_.begin();
    }

    BuyIterator eraseBuy(BuyIterator it) {
        return buy_orders_.erase(it);
    }

    SellIterator eraseSell(SellIterator it) {
        return sell_orders_.erase(it);
    }

    bool empty() const {
        return buy_orders_.empty() && sell_orders_.empty();
    }

    std::size_t buySize() const {
        return buy_orders_.size();
    }

    std::size_t sellSize() const {
        return sell_orders_.size();
    }

    std::optional<PriceType> bestBid() const {
        if (buy_orders_.empty()) {
            return std::nullopt;
        }

        return buy_orders_.begin()->second->price;
    }

    std::optional<PriceType> bestAsk() const {
        if (sell_orders_.empty()) {
            return std::nullopt;
        }

        return sell_orders_.begin()->second->price;
    }

private:
    MemoryPool<OrderType> pool_;
    BuyOrders buy_orders_;
    SellOrders sell_orders_;
};