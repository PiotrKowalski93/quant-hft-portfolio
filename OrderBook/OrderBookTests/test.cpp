#include "pch.h"
#include "../orderbook.h"

TEST(AddOrder, ShouldAddProperNumberOfPriceLevels) {

	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);

	EXPECT_EQ(2, order_book.bids.size());

	EXPECT_EQ(3, order_book.bids[100].queue.size());
	EXPECT_EQ(1, order_book.bids[105].queue.size());
}