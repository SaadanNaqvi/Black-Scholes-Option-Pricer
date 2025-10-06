#ifndef ORDER_H
#define ORDER_H

#include "Stocks.h"
#include "Position.h"
#include <string>
#include <ctime>
using namespace std;

enum OrderType {
    BUY,
    SELL,
    BUY_TO_COVER,
    SELL_SHORT
};

enum OrderStatus {
    PENDING,
    FILLED,
    PARTIALLY_FILLED,
    CANCELLED,
    REJECTED
};

enum OrderPriceType {
    MARKET,
    LIMIT,
    STOP,
    STOP_LIMIT
};

class Order {
    private:
        string orderId;          // Unique identifier for this order
        Stocks* stock;           // The stock being traded
        OrderType orderType;     // Buy, Sell, etc.
        OrderStatus status;      // Current status of the order
        OrderPriceType priceType; // Market, Limit, etc.
        int quantity;            // Number of shares to trade
        double price;            // Limit price (if applicable)
        double stopPrice;        // Stop price (if applicable)
        int filledQuantity;      // How many shares have been filled
        double avgFillPrice;     // Average price at which shares were filled
        time_t orderTime;        // When order was created
        time_t lastUpdated;      // Last time order was updated
        string userId;           // User who placed the order
        
    public:
        // Constructors
        Order();
        Order(string orderId, Stocks* stock, OrderType orderType, OrderPriceType priceType, int quantity, double price, string userId);
        Order(string orderId, Stocks* stock, OrderType orderType, OrderPriceType priceType, int quantity, double price, double stopPrice, string userId);
        
        // Getters
        string getOrderId() const;
        Stocks* getStock() const;
        OrderType getOrderType() const;
        OrderStatus getStatus() const;
        OrderPriceType getPriceType() const;
        int getQuantity() const;
        double getPrice() const;
        double getStopPrice() const;
        int getFilledQuantity() const;
        double getAvgFillPrice() const;
        time_t getOrderTime() const;
        time_t getLastUpdated() const;
        string getUserId() const;
        
        // Setters
        void setStock(Stocks* stock);
        void setOrderType(OrderType orderType);
        void setStatus(OrderStatus status);
        void setQuantity(int quantity);
        void setPrice(double price);
        void setStopPrice(double stopPrice);
        
        // Order management
        bool canExecute() const;
        void executeOrder(double executionPrice, int executedQuantity);
        void partialFill(double fillPrice, int fillQuantity);
        void cancelOrder();
        void rejectOrder();
        
        // Utility functions
        int getRemainingQuantity() const;
        double getTotalValue() const;
        double getFilledValue() const;
        bool isCompletelyFilled() const;
        string getOrderTypeString() const;
        string getOrderStatusString() const;
        string getPriceTypeString() const;
        Position* createPositionFromOrder() const;
        void printOrderDetails() const;
        
        // Destructor
        ~Order();
};

#endif
