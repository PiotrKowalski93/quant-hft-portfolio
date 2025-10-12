//#include "pch.h"
#include "gtest/gtest.h"
#include "../MatchingEngine/engine.h"

// 1. No buy orders
TEST(ProcessSellMarketOrder, EmptyOrderBook) {
    MatchingEngine engine;
    vector<Trade> result;

    result = engine.process_sell_marked_order(10, 1);

    EXPECT_EQ(0, result.size());
}

// 2. Only one ASK present (no buy orders)
TEST(ProcessSellMarketOrder, OnlyAsks_NoTrade) {
    MatchingEngine engine;
    engine.add_order(1, Side::Sell, 101, 5, 1); // ask

    auto result = engine.process_sell_marked_order(10, 2);

    EXPECT_EQ(result.size(), 0);
    EXPECT_EQ(engine.get_asks_total_qty(), 5);
}

// 3. One Buy, full match
TEST(ProcessSellMarketOrder, SingleBuy_FullMatch) {
    MatchingEngine engine;
    engine.add_order(1, Side::Buy, 100, 10, 1);

    auto result = engine.process_sell_marked_order(10, 2);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].qty, 10);
    EXPECT_EQ(result[0].price, 100);
    EXPECT_EQ(engine.get_bids_total_qty(), 0);
}

// 4. BUY smaller than market sell — partially filled
TEST(ProcessSellMarketOrder, PartialFill_MultipleBuys) {
    MatchingEngine engine;
    engine.add_order(1, Side::Buy, 100, 5, 1);
    engine.add_order(2, Side::Buy, 99, 5, 1);

    auto result = engine.process_sell_marked_order(12, 2);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].qty, 5);
    EXPECT_EQ(result[0].price, 100);
    EXPECT_EQ(result[1].qty, 5);
    EXPECT_EQ(result[1].price, 99);
    EXPECT_EQ(engine.get_bids_total_qty(), 0); // 2 orders fully done
}

// 5. BUY bigger than market sell — partially done BUY
TEST(ProcessSellMarketOrder, PartialExecution_RemainingBuy) {
    MatchingEngine engine;
    engine.add_order(1, Side::Buy, 100, 15, 1);

    auto result = engine.process_sell_marked_order(10, 2);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].qty, 10);
    EXPECT_EQ(result[0].price, 100);
    EXPECT_EQ(engine.get_bids_total_qty(), 5); // 5 units left
}

// 6. BUY with different prices — price level priority test
TEST(ProcessSellMarketOrder, BestPricePriority) {
    MatchingEngine engine;
    engine.add_order(1, Side::Buy, 98, 10, 1);
    engine.add_order(2, Side::Buy, 101, 5, 2);
    engine.add_order(3, Side::Buy, 99, 10, 3);

    auto result = engine.process_sell_marked_order(10, 4);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].price, 101);
    EXPECT_EQ(result[0].qty, 5);
    EXPECT_EQ(result[1].price, 99);
    EXPECT_EQ(result[1].qty, 5);
    EXPECT_EQ(engine.get_bids_total_qty(), 15);
}

// ------------------------------------------------------------------------------------

TEST(ProcessBuyMarketOrder, EmptyOrderBook) {
    MatchingEngine engine;
    auto result = engine.process_buy_marked_order(10, 1);

    EXPECT_EQ(result.size(), 0);
}

TEST(ProcessBuyMarketOrder, OnlyBids_NoTrade) {
    MatchingEngine engine;
    engine.add_order(1, Side::Buy, 100, 10, 1);

    auto result = engine.process_buy_marked_order(5, 2);
    EXPECT_EQ(result.size(), 0);
}

TEST(ProcessBuyMarketOrder, SingleAsk_FullMatch) {
    MatchingEngine engine;
    engine.add_order(1, Side::Sell, 101, 10, 1);

    auto result = engine.process_buy_marked_order(10, 2);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].qty, 10);
    EXPECT_EQ(result[0].price, 101);
    EXPECT_EQ(engine.get_asks_total_qty(), 0);
}

TEST(ProcessBuyMarketOrder, PartialFill_MultipleAsks) {
    MatchingEngine engine;
    engine.add_order(1, Side::Sell, 101, 5, 1);
    engine.add_order(2, Side::Sell, 102, 5, 2);

    auto result = engine.process_buy_marked_order(12, 3);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].price, 101);
    EXPECT_EQ(result[1].price, 102);
    EXPECT_EQ(result[0].qty, 5);
    EXPECT_EQ(result[1].qty, 5);
}

TEST(ProcessBuyMarketOrder, PartialExecution_RemainingAsk) {
    MatchingEngine engine;
    engine.add_order(1, Side::Sell, 101, 15, 1);

    auto result = engine.process_buy_marked_order(10, 2);

    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].qty, 10);
    EXPECT_EQ(engine.get_asks_total_qty(), 5);
}

TEST(ProcessBuyMarketOrder, BestPricePriority) {
    MatchingEngine engine;
    engine.add_order(1, Side::Sell, 105, 10, 1);
    engine.add_order(2, Side::Sell, 103, 5, 2);
    engine.add_order(3, Side::Sell, 104, 10, 3);

    auto result = engine.process_buy_marked_order(10, 4);

    ASSERT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].price, 103);
    EXPECT_EQ(result[0].qty, 5);
    EXPECT_EQ(result[1].price, 104);
    EXPECT_EQ(result[1].qty, 5);
    EXPECT_EQ(engine.get_asks_total_qty(), 15);
}
