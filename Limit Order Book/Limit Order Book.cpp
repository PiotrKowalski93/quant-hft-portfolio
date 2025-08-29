// Limit Order Book.cpp : Defines the entry point for the application.
//

#include "Limit Order Book.h"

using namespace std;

int main()
{
	return 0;
}


void unit_tests_add_order() {
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
};
