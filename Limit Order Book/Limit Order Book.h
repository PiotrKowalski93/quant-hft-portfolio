// Limit Order Book.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <deque>
#include <list>
#include <map>
#include <chrono>
#include <unordered_map>
#include <variant>

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
	list<Order> queue;			// FIFO queue of active orders
};

// --- Book definition ---
using BidLevels = map<Price, PriceLevel, greater<Price>>;
using AskLevels = map<Price, PriceLevel, less<Price>>;

// -- Set of locators
struct BidLocator { 
	BidLevels::iterator level_it; 
	list<Order>::iterator order_it;
};

struct AskLocator { 
	AskLevels::iterator level_it; 
	list<Order>::iterator order_it;
};

struct OrderLocator { 
	Side side; 
	variant<BidLocator, AskLocator> locator; 
};

// -- Class for Order Book
class OrderBook {

	BidLevels bids;
	AskLevels asks;

	unordered_map<OrderId, OrderLocator> orderId_index;

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
			Order new_order { orderId, side, price, qty, ts };

			switch (side) {
				case Side::Buy:
					if (bids.contains(price))
					{
						bids[price].total_Qty += qty;
						bids[price].queue.push_back(new_order);

						auto bid_it = bids.find(price);
						auto itQueue = prev(bids[price].queue.end());

						BidLocator newBidLoc { bid_it, itQueue };
						OrderLocator newOrderLoc { side, newBidLoc };
						orderId_index[orderId] = newOrderLoc;
					}
					else {
						PriceLevel price_level{	price, qty, {} };
						price_level.queue.push_back(new_order);
						bids[price] = price_level;

						auto it = bids.find(price);
						auto itQueue = prev(bids[price].queue.end());

						BidLocator newBidLoc { it, itQueue };
						OrderLocator newOrderLoc { side, newBidLoc };
						orderId_index[orderId] = newOrderLoc;
					}
				break;
				case Side::Sell:
					if (asks.contains(price))
					{
						asks[price].total_Qty += qty;
						asks[price].queue.push_back(new_order);

						auto ask_it = asks.find(price);
						auto itQueue = prev(asks[price].queue.end());

						AskLocator newAskLoc{ ask_it, itQueue };
						OrderLocator newOrderLoc{ side, newAskLoc };
						orderId_index[orderId] = newOrderLoc;
					}
					else {
						PriceLevel price_level{	price, qty, {} };
						price_level.queue.push_back(new_order);
						asks[price] = price_level;

						auto it = asks.end();
						auto itQueue = prev(price_level.queue.end());

						AskLocator newAskLoc{ it, itQueue };
						OrderLocator newOrderLoc{ side, newAskLoc };
						orderId_index[orderId] = newOrderLoc;
					}
				break;
			}

		};

	void cancel_order(OrderId orderId) {

		// Order does not exist
		if (orderId_index.count(orderId) == 0)
		{
			cout << "Order does not exist" << endl;
			return;
		};

		auto& loc = orderId_index[orderId].locator;

		std::visit([&](auto&& locator) {
			using T = std::decay_t<decltype(locator)>;

			auto& level = locator.level_it->second;
			auto& order = *(locator.order_it);

			level.total_Qty -= order.qty;
			level.queue.erase(locator.order_it);

			if (level.queue.empty()) {
				if (std::is_same_v<T, BidLocator>) {
					bids.erase(locator.level_it);
				}
				else if (std::is_same_v<T, AskLocator>) {
					asks.erase(locator.level_it);
				}
			}
		}, loc);

		orderId_index.erase(orderId);
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

	void print_orderDetails(OrderId orderId) {
		auto& loc = orderId_index[orderId].locator;
		// Wierd magic down below
		std::visit([&](auto&& locator) {
			Order order = *locator.order_it;
			cout << format("{0} {1} {2} {3}", order.id, order.price, order.qty, order.ts);
		}, loc);
	}
};

