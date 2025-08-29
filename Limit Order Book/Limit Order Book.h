// Limit Order Book.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <deque>
#include <map>
#include <chrono>


using namespace std;

// --- Basic Types ---
using OrderId	= uint64_t;
using Price		= int64_t;		// integer price (e.g., ticks)
using Qty		= int64_t;		// integer quantity (e.g., shares/contracts)
using Ts		= uint64_t;		// nanoseconds or event seq

enum class Side : uint8_t { 
	Buy = 0, 
	Sell = 1 
};

struct Order {
	OrderId	id{};
	Side	side{ Side::Buy };
	Price	price{};
	Qty		qty{};		// remaining qty
	Ts		ts{};		// event timestamp or sequence
};

struct PriceLevel {
	Price	price{};
	Qty		total_Qty{};		// aggregated quantity at this price
	deque<Order> queue;			// FIFO queue of active orders
};

// --- Book definition ---
using BidLevels = map<Price, PriceLevel, greater<Price>>;
using AskLevels = map<Price, PriceLevel, less<Price>>;

// -- Class for Order Book
class OrderBook {

	BidLevels bids;
	AskLevels asks;

	public:
		void add_order(OrderId orderId, Side side, Price price, Qty qty, Ts ts) {
		
			if (price <= 0) {
				cout << "Price must be bigger than 0.";
				return;
			}

			if (qty <= 0) {
				cout << "Qty must be bigger than 0.";
				return;
			}

			// --- Create order
			// TODO: If orderId exist, return
			Order new_order { orderId, side, price, qty, ts};
		
			switch (side) {
				case Side::Buy:
					// --- Create price level or find existing
					if (bids.contains(price))
					{
						bids[price].total_Qty += qty;
						bids[price].queue.push_back(new_order);
					}
					else {
						PriceLevel price_level{	price, qty, {} };
						price_level.queue.push_back(new_order);
						bids[price] = price_level;
					}
				break;
				case Side::Sell:
					// --- Create price level or find existing
					if (asks.contains(price))
					{
						asks[price].total_Qty += qty;
						asks[price].queue.push_back(new_order);
					}
					else {
						PriceLevel price_level{	price, qty, {} };
						price_level.queue.push_back(new_order);
						asks[price] = price_level;
					}
				break;
			}

		};

	void cancel_order() {
		// TODO
	};

	void reduce_order() {
		// TODO
	};

	void reprice_order() {
		// TODO
	};

	void best_bid() {
		// TODO
	};

	void best_ask() {
		// TODO
	};

	void top_levels() {
		// TODO
	};

	void print_bids() {
		for (auto bid : bids)
		{
			cout << std::format("{0}:{1}:{2}", bid.first, bid.second.total_Qty, bid.second.queue.size()) << endl;
		}
	};

	void print_asks() {
		for (auto ask : asks)
		{
			cout << std::format("{0}:{1}:{2}", ask.first, ask.second.total_Qty, ask.second.queue.size()) << endl;
		}
	}
};

