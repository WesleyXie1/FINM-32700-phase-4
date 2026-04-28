# FINM-32700-phase-4
Phase 4: Build &amp; Benchmark a High Frequency Trading System in C++

This project implements a simplified high frequency trading system prototype in C++. The main goal is to build the core infrastructure of a low latency trading system and benchmark tick to trade latency.

The system simulates market data, manages orders, maintains a limit order book, matches buy and sell orders, records executed trades, and reports latency statistics.

## Project Overview

The project is organized into several core modules:

- `MarketDataFeed`: simulates incoming market data ticks with bid and ask prices.
- `OrderBook`: stores buy and sell orders and keeps track of the best bid and best ask.
- `OrderManager`: tracks order states such as new, partially filled, filled, and canceled.
- `MatchingEngine`: matches orders when the best buy price crosses the best sell price.
- `TradeLogger`: records executed trades and writes trade logs to a CSV file.
- `Timer`: measures tick-to-trade latency in nanoseconds.
- `MemoryPool`: provides a simple custom memory pool to reduce allocation overhead.

The focus of this project is not on alpha generation or trading strategy. Instead, it focuses on the system side of trading: order processing, matching logic, memory management, and latency measurement.

## System Architecture

```text
MarketDataFeed
      |
      v
OrderBook <---- OrderManager
      |
      v
MatchingEngine
      |
      v
TradeLogger
      |
      v
Latency Benchmark



phase 4/
├── include/
│   ├── MarketData.hpp
│   ├── MatchingEngine.hpp
│   ├── MemoryPool.hpp
│   ├── Order.hpp
│   ├── OrderBook.hpp
│   ├── OrderManager.hpp
│   ├── Timer.hpp
│   ├── Trade.hpp
│   └── TradeLogger.hpp
├── src/
│   ├── main.cpp
│   ├── MarketData.cpp
│   ├── MatchingEngine.cpp
│   ├── OrderBook.cpp
│   ├── OrderManager.cpp
│   └── TradeLogger.cpp
├── test/
│   └── test_latency.cpp
├── CMakeLists.txt
├── .gitignore
└── README.md


Market Data Simulation

The system generates mock market data ticks for a single symbol. Each tick contains a bid price, ask price, and timestamp.

The MarketData structure uses alignas(64) to improve cache-line alignment, which is a common consideration in low-latency systems.

Limit Order Book

The order book stores buy and sell orders separately.

Buy orders are sorted from highest price to lowest price, while sell orders are sorted from lowest price to highest price. This allows the matching engine to quickly access the best bid and best ask.

Matching Engine

The matching logic follows a simple price-crossing rule:

If best bid >= best ask, the orders can match.

The matched quantity is the smaller quantity between the buy order and the sell order. If an order is only partially filled, its remaining quantity stays in the order book.



Order Management

The order manager tracks the state of each order:

New
Partial
Filled
Canceled

This makes it easier to monitor the lifecycle of each order as it moves through the system.

Memory Pool and Smart Pointers

The project uses a custom memory pool for order allocation. This reduces the overhead from repeated dynamic memory allocation.

Orders are managed with std::unique_ptr and a custom pool deleter. This keeps the code safer while still allowing control over memory behavior.



















