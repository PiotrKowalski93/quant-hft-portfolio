## Order Book

An order book is the list of orders (manual or electronic) that a trading venue (in particular stock exchanges) uses to record the interest of buyers and sellers in a particular financial instrument. A matching engine uses the book to determine which orders can be fully or partially executed.

### Level 1 (Market Surface/Best Bid/Ask): 
This is the most basic level, showing only the highest price a buyer is willing to pay (bid) and the lowest price a seller is willing to accept (ask), along with the corresponding volumes. It's suitable for general price monitoring but offers limited insight into market liquidity. 

### Level 2 (Market Depth): 
This level expands on Level 1 by displaying aggregated buy and sell orders at several price points below and above the current best bid and ask. It provides a more comprehensive view of the order book's depth, revealing market liquidity and potential support/resistance areas. 

### Level 3 (Tick-Level/Full Order Book): 
This is the most detailed level, revealing every individual buy and sell order with its unique order ID, price, timestamp, and its exact position in the queue. It offers the highest degree of transparency, including information about order modifications, cancellations, and the full lifecycle of each order. 

FIFO (First-In, First-Out)
Priority: Orders are matched based on their arrival time. 
Fairness: It rewards traders who are quickest to place an order at a specific price, promoting a "first-come, first-serve" dynamic. 
Process: If multiple orders exist at the same price, the one created earliest gets filled first. 
Benefit: It encourages timely responses to market changes. 

Pro-Rata
Priority: Orders at the same price level are matched based on their quantity. 
Fairness: Larger orders receive a proportion of the available volume based on their size. 
Process: Available shares are divided among competing orders, with each getting a percentage of the share that corresponds to its size. 
Benefit: It gives an incentive for traders to place larger orders. 