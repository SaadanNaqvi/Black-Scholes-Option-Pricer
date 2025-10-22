#include "Order.h"

#include <iomanip>
#include <iostream>
#include <sstream>
using namespace std;

// Default constructor
Order::Order() {
  orderId = "";
  stock = nullptr;
  orderType = BUY;
  status = PENDING;
  priceType = MARKET;
  quantity = 0;
  price = 0.0;
  stopPrice = 0.0;
  filledQuantity = 0;
  avgFillPrice = 0.0;
  orderTime = time(0);
  lastUpdated = time(0);
  userId = "";
}

// Constructor without stop price
Order::Order(string orderId, Stocks* stock, OrderType orderType,
             OrderPriceType priceType, int quantity, double price,
             string userId) {
  this->orderId = orderId;
  this->stock = stock;
  this->orderType = orderType;
  this->status = PENDING;
  this->priceType = priceType;
  this->quantity = quantity;
  this->price = price;
  this->stopPrice = 0.0;
  this->filledQuantity = 0;
  this->avgFillPrice = 0.0;
  this->orderTime = time(0);
  this->lastUpdated = time(0);
  this->userId = userId;
}

// Constructor with stop price
Order::Order(string orderId, Stocks* stock, OrderType orderType,
             OrderPriceType priceType, int quantity, double price,
             double stopPrice, string userId) {
  this->orderId = orderId;
  this->stock = stock;
  this->orderType = orderType;
  this->status = PENDING;
  this->priceType = priceType;
  this->quantity = quantity;
  this->price = price;
  this->stopPrice = stopPrice;
  this->filledQuantity = 0;
  this->avgFillPrice = 0.0;
  this->orderTime = time(0);
  this->lastUpdated = time(0);
  this->userId = userId;
}

// Getters
string Order::getOrderId() const { return orderId; }

Stocks* Order::getStock() const { return stock; }

OrderType Order::getOrderType() const { return orderType; }

OrderStatus Order::getStatus() const { return status; }

OrderPriceType Order::getPriceType() const { return priceType; }

int Order::getQuantity() const { return quantity; }

double Order::getPrice() const { return price; }

double Order::getStopPrice() const { return stopPrice; }

int Order::getFilledQuantity() const { return filledQuantity; }

double Order::getAvgFillPrice() const { return avgFillPrice; }

time_t Order::getOrderTime() const { return orderTime; }

time_t Order::getLastUpdated() const { return lastUpdated; }

string Order::getUserId() const { return userId; }

// Setters
void Order::setStock(Stocks* stock) {
  this->stock = stock;
  lastUpdated = time(0);
}

void Order::setOrderType(OrderType orderType) {
  this->orderType = orderType;
  lastUpdated = time(0);
}

void Order::setStatus(OrderStatus status) {
  this->status = status;
  lastUpdated = time(0);
}

void Order::setQuantity(int quantity) {
  this->quantity = quantity;
  lastUpdated = time(0);
}

void Order::setPrice(double price) {
  this->price = price;
  lastUpdated = time(0);
}

void Order::setStopPrice(double stopPrice) {
  this->stopPrice = stopPrice;
  lastUpdated = time(0);
}

// Order management
bool Order::canExecute(string date) {
  if (stock == nullptr || status != PENDING) return false;

  double currentPrice = stock->getCurrentPrice(date);
//IS THE USER ALLOWED TO SELL BASED ON THE RESOURCES THEY HAVE? ENOUGH STOCK OR MONEY.
  switch (priceType) {
    case MARKET:
      return true;
    case LIMIT:
      if (orderType == BUY || orderType == BUY_TO_COVER) {
        return currentPrice <= price;
      } else {
        return currentPrice >= price;
      }
    case STOP:
      if (orderType == BUY || orderType == BUY_TO_COVER) {
        return currentPrice >= stopPrice;
      } else {
        return currentPrice <= stopPrice;
      }
    case STOP_LIMIT:
      if (orderType == BUY || orderType == BUY_TO_COVER) {
        return (currentPrice >= stopPrice) && (currentPrice <= price);
      } else {
        return (currentPrice <= stopPrice) && (currentPrice >= price);
      }
    default:
      return false;
  }
}

void Order::executeOrder(double executionPrice, int executedQuantity,
                         string date) {
  if (!canExecute(date) || executedQuantity <= 0) return;

  int actualExecutedQuantity = min(executedQuantity, getRemainingQuantity());

  // Update average fill price
  double totalFilledValue = (avgFillPrice * filledQuantity) +
                            (executionPrice * actualExecutedQuantity);
  filledQuantity += actualExecutedQuantity;
  avgFillPrice = totalFilledValue / filledQuantity;

  // Update status
  if (filledQuantity >= quantity) {
    status = FILLED;
  } else {
    status = PARTIALLY_FILLED;
  }

  lastUpdated = time(0);
}

void Order::partialFill(double fillPrice, int fillQuantity, string date) {
  executeOrder(fillPrice, fillQuantity, date);
}

void Order::cancelOrder() {
  if (status == PENDING || status == PARTIALLY_FILLED) {
    status = CANCELLED;
    lastUpdated = time(0);
  }
}

void Order::rejectOrder() {
  status = REJECTED;
  lastUpdated = time(0);
}

// Utility functions
int Order::getRemainingQuantity() const { return quantity - filledQuantity; }

double Order::getTotalValue() const { return quantity * price; }

double Order::getFilledValue() const { return filledQuantity * avgFillPrice; }

bool Order::isCompletelyFilled() const { return status == FILLED; }

string Order::getOrderTypeString() const {
  switch (orderType) {
    case BUY:
      return "BUY";
    case SELL:
      return "SELL";
    case BUY_TO_COVER:
      return "BUY_TO_COVER";
    case SELL_SHORT:
      return "SELL_SHORT";
    default:
      return "UNKNOWN";
  }
}

string Order::getOrderStatusString() const {
  switch (status) {
    case PENDING:
      return "PENDING";
    case FILLED:
      return "FILLED";
    case PARTIALLY_FILLED:
      return "PARTIALLY_FILLED";
    case CANCELLED:
      return "CANCELLED";
    case REJECTED:
      return "REJECTED";
    default:
      return "UNKNOWN";
  }
}

string Order::getPriceTypeString() const {
  switch (priceType) {
    case MARKET:
      return "MARKET";
    case LIMIT:
      return "LIMIT";
    case STOP:
      return "STOP";
    case STOP_LIMIT:
      return "STOP_LIMIT";
    default:
      return "UNKNOWN";
  }
}

void Order::printOrderDetails(string date) {
  cout << "Order Details:" << endl;
  cout << "Order ID: " << orderId << endl;
  cout << "User ID: " << userId << endl;

  if (stock != nullptr) {
    cout << "Stock: " << stock->getTicker() << " (" << stock->getCompanyName()
         << ")" << endl;
    cout << "Current Stock Price: $" << fixed << setprecision(2)
         << stock->getCurrentPrice(date) << endl;
  }

  cout << "Order Type: " << getOrderTypeString() << endl;
  cout << "Price Type: " << getPriceTypeString() << endl;
  cout << "Status: " << getOrderStatusString() << endl;
  cout << "Quantity: " << quantity << " shares" << endl;
  cout << "Filled Quantity: " << filledQuantity << " shares" << endl;
  cout << "Remaining Quantity: " << getRemainingQuantity() << " shares" << endl;

  cout << fixed << setprecision(2);
  if (priceType != MARKET) {
    cout << "Limit Price: $" << price << endl;
  }
  if (priceType == STOP || priceType == STOP_LIMIT) {
    cout << "Stop Price: $" << stopPrice << endl;
  }

  if (filledQuantity > 0) {
    cout << "Average Fill Price: $" << avgFillPrice << endl;
    cout << "Filled Value: $" << getFilledValue() << endl;
  }

  cout << "Total Order Value: $" << getTotalValue() << endl;

  // Convert timestamps to readable format
  char buffer[26];
  struct tm* tm_info;

  tm_info = localtime(&orderTime);
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  cout << "Order Time: " << buffer << endl;

  tm_info = localtime(&lastUpdated);
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  cout << "Last Updated: " << buffer << endl;
}

// Destructor
Order::~Order() {
  // stock pointer is not owned by Order, so we don't delete it
}
