// Limit Order Book.cpp : Defines the entry point for the application.
//

#include "Limit Order Book.h"

using namespace std;

void Test_BestBid_BestAsk() {
	OrderBook ob;

	ob.add_order(1, Side::Buy, 100, 10, 1);
	ob.add_order(2, Side::Buy, 101, 20, 2);
	ob.add_order(3, Side::Sell, 105, 5, 3);
	ob.add_order(4, Side::Sell, 106, 15, 4);

	auto bid = ob.best_bid();
	auto ask = ob.best_ask();

	cout << "Best Bid: " << bid->price << " qty=" << bid->total_Qty << endl;
	cout << "Best Ask: " << ask->price << " qty=" << ask->total_Qty << endl;
}

void Test_AddAsk_AddBid() {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);

	order_book.add_order(5, Side::Sell, 99, 5, 1);
	order_book.add_order(6, Side::Sell, 98, 15, 2);
	order_book.add_order(7, Side::Sell, 98, 5, 3);

	cout << "Bids: " << endl;
	order_book.print_bids();
	cout << endl;

	cout << "Asks: " << endl;
	order_book.print_asks();
	cout << endl;
}

void Test_Get_OrderDetail() {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);

	order_book.add_order(5, Side::Sell, 99, 5, 1);
	order_book.add_order(6, Side::Sell, 98, 15, 2);
	order_book.add_order(7, Side::Sell, 98, 5, 3);

	cout << "Bids: " << endl;
	order_book.print_bids();
	cout << endl;

	cout << "Asks: " << endl;
	order_book.print_asks();
	cout << endl;

	// Order Details Check
	cout << "OrderId: 4" << endl;
	order_book.print_orderDetails(4);
	cout << endl;

	cout << "OrderId: 7" << endl;
	order_book.print_orderDetails(7);
	cout << endl;
}

void Test_CancelOrder() {
	OrderBook order_book;

	order_book.add_order(1, Side::Buy, 100, 5, 1);
	order_book.add_order(2, Side::Buy, 105, 15, 2);
	order_book.add_order(3, Side::Buy, 100, 5, 3);
	order_book.add_order(4, Side::Buy, 100, 5, 4);
	order_book.add_order(5, Side::Sell, 99, 5, 1);
	order_book.add_order(6, Side::Sell, 98, 15, 2);
	order_book.add_order(7, Side::Sell, 98, 5, 3);

	// Cancel Order Check
	cout << "Cancel Order: 4" << endl;
	order_book.cancel_order(4);
	order_book.print_bids();
	cout << endl;
	// ---------
}

void Test_TopLevels() {
	OrderBook ob;

	ob.add_order(1, Side::Buy, 100, 10, 1);
	ob.add_order(2, Side::Buy, 101, 20, 2);
	ob.add_order(2, Side::Buy, 102, 20, 3);
	ob.add_order(3, Side::Sell, 105, 5, 4);
	ob.add_order(4, Side::Sell, 106, 15, 5);
	ob.add_order(4, Side::Sell, 107, 15, 6);

	cout << "\n=== Top 2 levels ===" << endl;
	ob.top_asks_levels(2);
	ob.top_bids_levels(2);
}

int main()
{
	Test_AddAsk_AddBid();
	Test_Get_OrderDetail();
	Test_CancelOrder();
	Test_BestBid_BestAsk();
	Test_TopLevels();

	return 0;
}




