#pragma once

#include <algorithm>
#include <vector>

#include "OrderBook.hpp"
#include "OrderManager.hpp"
#include "Trade.hpp"
#include "TradeLogger.hpp"

template <typename PriceType, typename OrderIdType>
class MatchingEngine {
public:
    using OrderBookType = OrderBook<PriceType, OrderIdType>;
    using TradeType = Trade<PriceType, OrderIdType>;
    using LoggerType = TradeLogger<PriceType, OrderIdType>;
    using ManagerType = OrderManager<OrderIdType>;

    MatchingEngine(OrderBookType& book,
                   LoggerType& logger,
                   ManagerType& order_manager)
        : book_(book),
          logger_(logger),
          order_manager_(order_manager) {}

    std::vector<TradeType> match() {
        std::vector<TradeType> matched_trades;

        while (book_.canMatch()) {
            auto buy_it = book_.buyBegin();
            auto sell_it = book_.sellBegin();

            auto& buy_order = *(buy_it->second);
            auto& sell_order = *(sell_it->second);

            int trade_quantity = std::min(buy_order.quantity, sell_order.quantity);
            PriceType trade_price = sell_order.price;

            TradeType trade(
                buy_order.id,
                sell_order.id,
                buy_order.symbol,
                trade_price,
                trade_quantity
            );

            buy_order.quantity -= trade_quantity;
            sell_order.quantity -= trade_quantity;

            updateOrderState(buy_order.id, buy_order.quantity);
            updateOrderState(sell_order.id, sell_order.quantity);

            logger_.logTrade(trade);
            matched_trades.push_back(trade);

            if (buy_order.quantity == 0) {
                book_.eraseBuy(buy_it);
            }

            if (sell_order.quantity == 0) {
                book_.eraseSell(sell_it);
            }
        }

        return matched_trades;
    }

private:
    void updateOrderState(OrderIdType id, int remaining_quantity) {
        if (remaining_quantity == 0) {
            order_manager_.onFilled(id);
        } else {
            order_manager_.onPartialFill(id);
        }
    }

private:
    OrderBookType& book_;
    LoggerType& logger_;
    ManagerType& order_manager_;
};