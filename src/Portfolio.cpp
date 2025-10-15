#include "Portfolio.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Order.h"  // Include here to resolve forward declaration
using namespace std;

// Default constructor - calls Position default constructor
Portfolio::Portfolio() {
  ownerId = "";
  cashBalance = 10000.0;  // Default starting cash
  initialValue = 10000.0;
  totalPortfolioValue = 10000.0;
  creationDate = time(0);
  lastUpdated = time(0);
}

// Constructor with basic portfolio info
Portfolio::Portfolio(string portfolioId, string ownerId,
                     double initialCash) {  // Call Position default constructor
  this->portfolioId = portfolioId;
  this->ownerId = ownerId;
  this->cashBalance = initialCash;
  this->initialValue = initialCash;
  this->totalPortfolioValue = initialCash;
  this->creationDate = time(0);
  this->lastUpdated = time(0);
}

// Constructor with portfolio and initial position
Portfolio::Portfolio(string portfolioId, string ownerId, string ticker,
                     string companyName, double currentPrice, int quantity,
                     double entryPrice, double initialCash) {
  this->portfolioId = portfolioId;
  this->ownerId = ownerId;
  this->cashBalance = initialCash;
  this->initialValue = initialCash + (quantity * entryPrice);
  this->totalPortfolioValue = this->initialValue;
  this->creationDate = time(0);
  this->lastUpdated = time(0);
}

// Getters
string Portfolio::getPortfolioId() const { return portfolioId; }

string Portfolio::getOwnerId() const { return ownerId; }

double Portfolio::getCashBalance() const { return cashBalance; }

double Portfolio::getInitialValue() const { return initialValue; }

vector<Order*> Portfolio::getOrders() const { return orders; }

time_t Portfolio::getCreationDate() const { return creationDate; }

time_t Portfolio::getLastUpdated() const { return lastUpdated; }

// Setters
void Portfolio::setPortfolioId(string portfolioId) {
  this->portfolioId = portfolioId;
  lastUpdated = time(0);
}

void Portfolio::setOwnerId(string ownerId) {
  this->ownerId = ownerId;
  lastUpdated = time(0);
}

void Portfolio::setCashBalance(double balance) {
  this->cashBalance = balance;
  lastUpdated = time(0);
}

// Override virtual methods from Position/Stocks

// Order management
void Portfolio::addOrder(Order* order) {
  if (order->getPrice() > cashBalance) {
    cout << "You do not have the funds to take this order." << endl;
    return;
  }
  if (order == nullptr) return;
  orders.push_back(order);
  lastUpdated = time(0);
  cashBalance -= order->getPrice();
}

void Portfolio::cancelOrder(string orderId) {
  for (Order* order : orders) {
    if (order->getOrderId() == orderId) {
      cashBalance += order->getPrice();
      order->cancelOrder();
      break;
    }
  }
  lastUpdated = time(0);
}

Order* Portfolio::getOrder(string orderId) const {
  for (Order* order : orders) {
    if (order->getOrderId() == orderId) {
      return order;
    }
  }
  return nullptr;
}

vector<Order*> Portfolio::getPendingOrders() const {
  vector<Order*> pendingOrders;
  for (Order* order : orders) {
    if (order->getStatus() == PENDING ||
        order->getStatus() == PARTIALLY_FILLED) {
      pendingOrders.push_back(order);
    }
  }
  return pendingOrders;
}

vector<Order*> Portfolio::getFilledOrders() const {
  vector<Order*> filledOrders;
  for (Order* order : orders) {
    if (order->getStatus() == FILLED) {
      filledOrders.push_back(order);
    }
  }
  return filledOrders;
}

void Portfolio::processOrders(string date) {
  for (Order* order : orders) {
    if (order->canExecute(date)) {
      // Simplified execution logic - in reality this would be more complex
      double executionPrice = order->getStock()->getCurrentPrice(date);
      int quantityToExecute = order->getRemainingQuantity();

      // Check if we have sufficient funds for buy orders
      if ((order->getOrderType() == BUY ||
           order->getOrderType() == BUY_TO_COVER)) {
        double totalCost = executionPrice * quantityToExecute;
        if (!hasSufficientFunds(totalCost)) continue;
      }

      order->executeOrder(executionPrice, quantityToExecute, date);

      // Update cash balance
      if (order->getOrderType() == BUY ||
          order->getOrderType() == BUY_TO_COVER) {
        deductCash(order->getFilledValue());
      } else {
        addCash(order->getFilledValue());
      }

      // Create or update position if order is filled
      if (order->isCompletelyFilled()) {
      }
    }
  }
}

// Cash management
bool Portfolio::hasSufficientFunds(double amount) const {
  return cashBalance >= amount;
}

void Portfolio::addCash(double amount) {
  if (amount > 0) {
    cashBalance += amount;
    lastUpdated = time(0);
  }
}

void Portfolio::deductCash(double amount) {
  if (amount > 0 && amount <= cashBalance) {
    cashBalance -= amount;
    lastUpdated = time(0);
  }
}

// Portfolio calculations
double Portfolio::calculateTotalValue() const {
  return cashBalance;
}

double Portfolio::calculateTotalPnL() const {
  return calculateRealisedPnL();
}

double Portfolio::calculateRealisedPnL() const {
  // This would typically come from closed positions or completed trades
  // For now, we'll calculate based on filled orders
  double realisedPnL = 0.0;
  // Implementation would depend on tracking closed positions
  return realisedPnL;
}

double Portfolio::calculateTotalReturn() const {
  if (initialValue == 0.0) return 0.0;
  return ((calculateTotalValue() - initialValue) / initialValue) * 100.0;
}

// Watchlist management

// Portfolio analysis
unordered_map<string, double> Portfolio::getAssetAllocation() const {
  unordered_map<string, double> allocation;
  double totalValue = calculateTotalValue();

  if (totalValue > 0) {
    // Cash allocation
    allocation["CASH"] = (cashBalance / totalValue) * 100.0;

    // Stock allocations
  }

  return allocation;
}

double Portfolio::calculatePortfolioBeta() const {
  // Simplified implementation - would need market data for accurate calculation
  return 1.0;  // Placeholder
}

double Portfolio::calculateSharpeRatio() const {
  // Simplified implementation - would need risk-free rate and volatility data
  return 0.0;  // Placeholder
}

// Utility functions
void Portfolio::printPortfolioSummary() const {
  cout << "Portfolio Summary" << endl;
  cout << "================" << endl;
  cout << "Portfolio ID: " << portfolioId << endl;
  cout << "Owner ID: " << ownerId << endl;
  cout << fixed << setprecision(2);
  cout << "Cash Balance: $" << cashBalance << endl;
  cout << "Total Value: $" << calculateTotalValue() << endl;
  cout << "Total Return: " << calculateTotalReturn() << "%" << endl;
  cout << "Number of Orders: " << orders.size() << endl;

  // Print creation date
  char buffer[26];
  struct tm* tm_info = localtime(&creationDate);
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  cout << "Created: " << buffer << endl;
}

void Portfolio::printOrders() const {
  cout << "\nRecent Orders:" << endl;
  cout << "-------------" << endl;

  if (orders.empty()) {
    cout << "No orders" << endl;
    return;
  }

  // Print last 10 orders
  int count = 0;
  for (auto it = orders.rbegin(); it != orders.rend() && count < 10;
       ++it, ++count) {
    Order* order = *it;
    if (order->getStock() != nullptr) {
      cout << order->getOrderTypeString() << " " << order->getQuantity() << " "
           << order->getStock()->getTicker() << " @ $" << fixed
           << setprecision(2) << order->getPrice() << " ["
           << order->getOrderStatusString() << "]" << endl;
    }
  }
}

// Destructor
Portfolio::~Portfolio() {
  // Clean up positions

  // Clean up orders
  for (Order* order : orders) {
    delete order;
  }
  orders.clear();

  // Note: We don't delete stocks from watchlist as they might be owned by other
  // objects
}
