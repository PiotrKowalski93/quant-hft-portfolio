#pragma once

#include "../orderbook.h"

struct Trade {
	OrderId order_id_buy;
	OrderId order_id_sell;
	Qty qty;
	Price price;
	Ts timestamp;
};

class MatchingEngine {
public: 
	vector<Trade> process_sell_marked_order(Qty qty, Ts timestamp) {
		vector<Trade> trades;
		
		// To prevent memory alloc resize
		trades.reserve(10);

		while (qty > 0 && !order_book.asks.empty())
		{
			auto best_ask_it = order_book.asks.begin();
			auto& level_it = best_ask_it->second;

			while (qty > 0 && !level_it.queue.empty())
			{
				auto& order = level_it.queue.front();
				Qty trade_qty = min(qty, order.qty);

				// Creates obj in vector mem
				trades.emplace_back(Trade {
					0,
					order.id,
					trade_qty,
					order.price,
					timestamp
				});

				order.qty -= trade_qty;
				qty -= trade_qty;

				// Remove order if fully consumed
				if (order.qty == 0) level_it.queue.pop_front();

				// Remove price level if empty
				if (level_it.queue.empty()) order_book.asks.erase(best_ask_it);
			}
		}

		return trades;
	}

	vector<Trade> process_buy_marked_order(Qty qty, Ts timestamp) {
		vector<Trade> trades;
		trades.reserve(10);

		while (qty > 0 && !order_book.bids.empty()) {
			auto best_bid_it = order_book.bids.begin();
			auto& level_it = best_bid_it->second;

			while (qty > 0 && !level_it.queue.empty()) {
				auto order = level_it.queue.front();
				Qty trade_qty = min(qty, order.qty);

				trades.emplace_back(Trade{
					order.id,
					0,
					trade_qty,
					order.price,
					timestamp
					});

				order.qty -= trade_qty;
				qty -= trade_qty;

				if (order.qty == 0) level_it.queue.pop_front();

				if (level_it.queue.empty()) order_book.bids.erase(best_bid_it);
			}
		}

		return trades;
	}

/// <summary>
/// Order Book API
/// </summary>

// TODO: Implement Order Book API


///  --------------------------

private:
	OrderBook order_book;
};