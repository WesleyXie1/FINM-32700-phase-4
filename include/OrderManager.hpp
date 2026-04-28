#pragma once

#include <string>
#include <unordered_map>

#include "Order.hpp"

template <typename OrderIdType>
class OrderManager {
public:
    void onNewOrder(OrderIdType id) {
        states_[id] = OrderState::New;
    }

    void onPartialFill(OrderIdType id) {
        states_[id] = OrderState::Partial;
    }

    void onFilled(OrderIdType id) {
        states_[id] = OrderState::Filled;
    }

    void onCanceled(OrderIdType id) {
        states_[id] = OrderState::Canceled;
    }

    OrderState getState(OrderIdType id) const {
        auto it = states_.find(id);

        if (it == states_.end()) {
            return OrderState::New;
        }

        return it->second;
    }

    std::string stateToString(OrderIdType id) const {
        switch (getState(id)) {
            case OrderState::New:
                return "New";
            case OrderState::Partial:
                return "Partial";
            case OrderState::Filled:
                return "Filled";
            case OrderState::Canceled:
                return "Canceled";
            default:
                return "Unknown";
        }
    }

private:
    std::unordered_map<OrderIdType, OrderState> states_;
};