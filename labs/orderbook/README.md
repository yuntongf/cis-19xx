# Limit Order Book

As a developer at Shein Street, you are tasked with building a limit order book — a core component of an exchange’s trading system.

A financial market consists of **buyers** and **sellers**. Buyers submit orders specifying the maximum price and quantity they are willing to purchase (these are called **bids**). Sellers, on the other hand, submit orders specifying the minimum price and quantity they are willing to sell (these are called **asks** or offers).

The difference between the best bid (the highest price a buyer is willing to pay) and the best ask (the lowest price a seller is willing to accept) is known as the **bid-ask spread**. When a new order arrives such that the best bid price is greater than or equal to the best ask price, it means a buyer is willing to pay at least as much as a seller is asking - in other words, their orders **cross**, and a **trade** is executed.

The exchange continuously receives bids and asks from participants and maintains an order book, which represents the current state of the market. The **order book** lists all active buy and sell orders, tracks the best bid and ask prices, and records executed trades. The exchange runs a **matching engine** on this data to pair compatible buy and sell orders according to predefined matching rules. Because trades can occur hundreds of thousands of times per second, this system must update with extreme speed and precision to ensure a fair, orderly, and liquid market.

You may think that most orders come from individual investors, in reality, the majority are placed by brokers and especially **market makers**. Market makers provide liquidity by continuously posting bids and asks, profiting from small price movements and the spread between buy and sell prices. For instance, if a market maker expects the price of a stock to rise from $10 to $11, they might post a bid to buy at $10. If that order is filled and the price later increases to $11, they can sell at a profit of $1. This is, of course, a simplified example — but it captures the basic principle of how market makers operate.

For market makers (and exchanges) to function effectively, they must maintain an accurate and real-time view of the market — exactly what an order book provides.

In this lab, your task is to implement an order book that tracks the state of a market and matches buy and sell orders according to the exchange’s rules. Below is a list of functional requirements that your implementation should satisfy:

## Functional Requirements

### Order Creation

**FR-1.1**
The system shall support the creation of limit orders with the following properties:

- Order ID (unique per order)
- Side (Buy or Sell)
- Price (the limit price)
- Amount (the quantity to buy or sell)

**FR-1.2**
The system shall automatically assign a unique, incrementing ID to every new order.

**FR-1.3**
The system shall maintain two separate books:
- Bid Book (Buy orders) — sorted by descending price (highest first).
- Ask Book (Sell orders) — sorted by ascending price (lowest first).

**FR-1.4**
Multiple orders at the same price level shall be stored in the order of arrival (FIFO).

**FR-1.5**
New buy or sell orders shall be inserted into the appropriate price level queue in the book.

### Order Matching (Trade Execution)

**FR-2.1**
The system shall continuously compare the best (top) bid and ask prices.

**FR-2.2**
A trade shall be executed if and only if:

Best Bid Price >= Best Ask Price

i.e. buyers are willing to pay at least as much as sellers want.

**FR-2.3**
The execution price shall be determined by the buy order’s price.

**FR-2.4**
When a trade is executed:

The traded amount is the minimum of the remaining amounts in the matched orders.

Both orders’ remaining amounts are reduced by that traded quantity.

A trade record is created containing:
- Buy order ID
- Sell order ID
- Traded amount
- Execution price

**FR-2.5**
Orders may be partially filled. If the order still has remaining quantity, it stays in the book for future matching.

**FR-2.6**
Fully filled orders shall be removed from the book and deleted from the active order map.

**FR-2.7**
Matching shall continue recursively until no overlapping prices remain:

i.e. until best_bid < best_ask.

### Order Cancellation

**FR-3.1**
The system shall support cancellation of any active order using its unique ID.

**FR-3.2**
When canceling an order:

The order is removed from its corresponding list at the price level.

The price level is deleted if it becomes empty.

The order’s entry is erased from the global lookup table (orders_).

**FR-3.3**
Canceling a non-existent order (e.g., already filled or canceled) shall have no effect.

### Trade Recording

**FR-4.1**
Each completed trade shall be stored in an internal trade log (trades_).

**FR-4.2**
Each trade record shall contain:

- IDs of the two matched orders

- Quantity traded

- Price of execution

**FR-4.3**
The system shall preserve the full history of trades in chronological order for later retrieval or reporting.

### Market State & Querying

**FR-5.1**
The system shall maintain the current best bid and best ask prices

**FR-5.2**
The system shall be able to compute the bid-ask spread:

Spread = Best Ask Price - Best Bid Price

**FR-5.3**
The system shall store the full depth of book, i.e. all active price levels and their order queues.

**FR-5.4**
The system shall be able to return all active orders and their quantities for diagnostic or analytical use.

### Run time requirements

**FR-6.1**
The matching process (MatchOrders) shall operate in O(log N) for insertion/removal 

**FR-6.2**
O(1) for top-of-book comparison.

### System Behavior

**FR-7.1**
The system shall operate in a deterministic manner — identical sequences of orders must yield identical outcomes.

**FR-7.2**
Matching shall be triggered automatically when new orders are added.

**FR-7.3**
The order book shall remain consistent at all times:
- No duplicate IDs.
- No price level with an empty list.

