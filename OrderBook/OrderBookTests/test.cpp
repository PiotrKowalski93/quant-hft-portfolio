//#include "pch.h"
#include "gtest/gtest.h"
#include "../OrderBook/orderbook.h"

TEST(AddOrder, ShouldAddProperNumberOfPriceLevelsForBids) {

	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);

	EXPECT_EQ(2, order_book.bids.size());

	EXPECT_EQ(3, order_book.bids[100].queue.size());
	EXPECT_EQ(1, order_book.bids[105].queue.size());

	EXPECT_EQ(15, order_book.bids[100].total_Qty);
	EXPECT_EQ(15, order_book.bids[105].total_Qty);
}

TEST(AddOrder, ShouldAddProperNumberOfPriceLevelsForAsks) {

	OrderBook order_book;

	order_book.add_order(1, Side::Sell, 100, 5, 1);
	order_book.add_order(2, Side::Sell, 105, 15, 2);
	order_book.add_order(3, Side::Sell, 104, 3, 4);

	EXPECT_EQ(3, order_book.asks.size());

	EXPECT_EQ(1, order_book.asks[100].queue.size());
	EXPECT_EQ(1, order_book.asks[104].queue.size());
	EXPECT_EQ(1, order_book.asks[105].queue.size());

	EXPECT_EQ(5, order_book.asks[100].total_Qty);
	EXPECT_EQ(3, order_book.asks[104].total_Qty);
	EXPECT_EQ(15, order_book.asks[105].total_Qty);
}

TEST(ReduceOrder, ReduceBid) {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);

	EXPECT_EQ(15, order_book.bids[105].total_Qty);

	order_book.reduce_order(2, 2);

	auto order = order_book.GetBidById(2);

	EXPECT_EQ(105, order->price);
	EXPECT_EQ(13, order->qty);
	EXPECT_EQ(13, order_book.bids[105].total_Qty);
}

TEST(ReduceOrder, ReduceAsk) {
	OrderBook order_book;

	order_book.add_order(5, Side::Sell, 99, 5, 1);
	order_book.add_order(6, Side::Sell, 98, 15, 2);
	order_book.add_order(7, Side::Sell, 98, 5, 3);

	EXPECT_EQ(5, order_book.asks[99].total_Qty);
	EXPECT_EQ(20, order_book.asks[98].total_Qty);

	order_book.reduce_order(6, 5);

	auto order = order_book.GetAskById(6);

	EXPECT_EQ(98, order->price);
	EXPECT_EQ(10, order->qty);
	EXPECT_EQ(15, order_book.asks[98].total_Qty);
}

TEST(CancelOrder, CancelBidAndAsk) {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);
	order_book.add_order(5, Side::Sell, 99, 5, 1);
	order_book.add_order(6, Side::Sell, 98, 15, 2);
	order_book.add_order(7, Side::Sell, 98, 5, 3);
		
	EXPECT_EQ(2, order_book.asks.size());
	EXPECT_EQ(2, order_book.bids.size());

	order_book.cancel_order(5);
	order_book.cancel_order(3);

	EXPECT_EQ(1, order_book.asks.size());
	EXPECT_EQ(2, order_book.bids.size());

	EXPECT_EQ(2, order_book.bids[100].queue.size());

	auto bid = order_book.GetBidById(3);
	auto ask = order_book.GetAskById(5);

	EXPECT_TRUE(bid == nullptr);
	EXPECT_TRUE(ask == nullptr);
}

TEST(RepriceOrder, RepriceBid) {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 10, 1);
	order_book.add_order(2, Side::Buy, 101, 5, 2);

	EXPECT_EQ(2, order_book.bids.size());

	order_book.reprice_order(1, 102);

	EXPECT_EQ(2, order_book.bids.size());
	EXPECT_EQ(1, order_book.bids[102].queue.size());
}

TEST(RepriceOrder, RepriceBidWithExistingPriceLevel) {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 102, 10, 1);
	order_book.add_order(2, Side::Buy, 102, 10, 1);
	order_book.add_order(3, Side::Buy, 100, 10, 1);
	order_book.add_order(4, Side::Buy, 101, 5, 2);

	EXPECT_EQ(3, order_book.bids.size());
	EXPECT_EQ(2, order_book.bids[102].queue.size());

	order_book.reprice_order(3, 102);

	EXPECT_EQ(2, order_book.bids.size());
	EXPECT_EQ(3, order_book.bids[102].queue.size());
	EXPECT_EQ(30, order_book.bids[102].total_Qty);
}

TEST(RepriceOrder, RepriceAsk) {
	OrderBook order_book;

	order_book.add_order(1, Side::Sell, 100, 10, 1);
	order_book.add_order(2, Side::Sell, 101, 5, 2);

	EXPECT_EQ(2, order_book.asks.size());

	order_book.reprice_order(1, 102);

	EXPECT_EQ(2, order_book.asks.size());
	EXPECT_EQ(1, order_book.asks[102].queue.size());
}

TEST(RepriceOrder, RepriceAskWithExistingPriceLevel) {
	OrderBook order_book;

	order_book.add_order(1, Side::Sell, 102, 10, 1);
	order_book.add_order(2, Side::Sell, 102, 10, 1);
	order_book.add_order(3, Side::Sell, 100, 10, 1);
	order_book.add_order(4, Side::Sell, 101, 5, 2);

	EXPECT_EQ(3, order_book.asks.size());
	EXPECT_EQ(2, order_book.asks[102].queue.size());

	order_book.reprice_order(3, 102);

	EXPECT_EQ(2, order_book.asks.size());
	EXPECT_EQ(3, order_book.asks[102].queue.size());
	EXPECT_EQ(30, order_book.asks[102].total_Qty);
}

TEST(BestOrder, GetBestBid) {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);
	order_book.add_order(5, Side::Sell, 99, 5, 1);
	order_book.add_order(6, Side::Sell, 98, 15, 2);
	order_book.add_order(7, Side::Sell, 98, 5, 3);

	auto price_level = order_book.best_bid();

	EXPECT_EQ(105, price_level->price);
	EXPECT_EQ(1, price_level->queue.size());
	EXPECT_EQ(15, price_level->total_Qty);
}

TEST(BestOrder, GetBestAsk) {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);
	order_book.add_order(5, Side::Sell, 99, 5, 1);
	order_book.add_order(6, Side::Sell, 98, 15, 2);
	order_book.add_order(7, Side::Sell, 98, 5, 3);

	auto price_level = order_book.best_ask();

	EXPECT_EQ(98, price_level->price);
	EXPECT_EQ(2, price_level->queue.size());
	EXPECT_EQ(20, price_level->total_Qty);
}

TEST(TotalQty, ShouldCountQtyForBids) {

	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);

	EXPECT_EQ(2, order_book.bids.size());
	EXPECT_EQ(30, order_book.get_bids_total_qty());
}

TEST(TotalQty, ShouldCountQtyForAsks) {

	OrderBook order_book;

	order_book.add_order(1, Side::Sell, 100, 5, 1);
	order_book.add_order(2, Side::Sell, 105, 15, 2);
	order_book.add_order(3, Side::Sell, 104, 3, 4);

	EXPECT_EQ(3, order_book.asks.size());
	EXPECT_EQ(23, order_book.get_asks_total_qty());
}