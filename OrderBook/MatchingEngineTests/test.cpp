//#include "pch.h"
#include "gtest/gtest.h"
#include "../MatchingEngine/engine.h"

TEST(ProcessSellMarketOrder, EmptyOrderBook) {
	MatchingEngine engine;
	vector<Trade> result;

	result = engine.process_sell_marked_order(10, 1);

	EXPECT_EQ(0, result.size());
}

TEST(ProcessBuyMarketOrder, EmptyOrderBook) {
	MatchingEngine engine;
	vector<Trade> result;

	result = engine.process_buy_marked_order(10, 1);

	EXPECT_EQ(0, result.size());
}