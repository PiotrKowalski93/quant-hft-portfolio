#pragma once

#include "../orderbook.h"

struct Trade {
	OrderId order_id_buy;
	OrderId order_id_sell;
	Qty qty;
	Price price;
	Ts timestamp;
};

