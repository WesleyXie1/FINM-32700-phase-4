#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Trade.hpp"

template <typename PriceType, typename OrderIdType>
class TradeLogger {
public:
    using TradeType = Trade<PriceType, OrderIdType>;

    explicit TradeLogger(std::size_t reserve_size = 100000) {
        trades_.reserve(reserve_size);
    }

    void logTrade(const TradeType& trade) {
        trades_.push_back(trade);
    }

    const std::vector<TradeType>& trades() const {
        return trades_;
    }

    std::size_t size() const {
        return trades_.size();
    }

    void flushToCsv(const std::string& filename) const {
        std::ofstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Failed to open trade log file.\n";
            return;
        }

        file << "symbol,buy_order_id,sell_order_id,price,quantity\n";

        for (const auto& trade : trades_) {
            file << trade.symbol << ","
                 << trade.buy_order_id << ","
                 << trade.sell_order_id << ","
                 << trade.price << ","
                 << trade.quantity << "\n";
        }
    }

private:
    std::vector<TradeType> trades_;
};