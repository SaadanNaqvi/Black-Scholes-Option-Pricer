#include "Portfolio.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "Order.h"  // Include here to resolve forward declaration
using namespace std;

// Default constructor - calls Position default constructor
Portfolio::Portfolio() : Position() {
  portfolioId = generateId("PORT");
  ownerId = "";
  cashBalance = 10000.0;  // Default starting cash
  initialValue = 10000.0;
  totalPortfolioValue = 10000.0;
  creationDate = time(0);
  lastUpdated = time(0);
}

// Constructor with basic portfolio info
Portfolio::Portfolio(string portfolioId, string ownerId, double initialCash)
    : Position() {  // Call Position default constructor
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
                     string companyName, double currentPrice, PositionType type,
                     int quantity, double entryPrice, double initialCash)
    : Position(ticker, companyName, currentPrice, type, quantity, entryPrice) {
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

unordered_map<string, Position*> Portfolio::getPositions() const {
  return positions;
}

vector<Order*> Portfolio::getOrders() const { return orders; }

unordered_map<string, Stocks*> Portfolio::getWatchlist() const {
  return watchlist;
}

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
double Portfolio::calculateValue() const {
  // Portfolio value = cash + all position values
  return cashBalance + calculatePositionsValue();
}

double Portfolio::calculateRisk() const {
  // Portfolio risk = weighted average of position risks
  double totalValue = calculateValue();
  if (totalValue == 0.0) return 0.0;

  double weightedRisk = 0.0;
  for (const auto& pair : positions) {
    double positionValue = pair.second->getPositionValue();
    double positionRisk = pair.second->calculateRisk();
    double weight = positionValue / totalValue;
    weightedRisk += weight * positionRisk;
  }

  return weightedRisk;
}

void Portfolio::displayInfo() const {
  cout << "[PORTFOLIO] " << getClassType() << ": " << portfolioId
       << " (Owner: " << ownerId << ", Value: $" << fixed << setprecision(2)
       << calculateValue() << ")" << endl;
}

string Portfolio::getClassType() const { return "Portfolio"; }

void Portfolio::updateCurrentPrice(double price) {
  // For portfolio, update the inherited position (if any) and recalculate
  // totals
  Position::updateCurrentPrice(price);  // Call parent method

  // Update all positions with new market data if applicable
  for (auto& pair : positions) {
    if (pair.second->getTicker() == getTicker()) {
      pair.second->updateCurrentPrice(price);
    }
  }

  totalPortfolioValue = calculateValue();
  lastUpdated = time(0);
}

// Position management
void Portfolio::addPosition(Position* position) {
  if (position == nullptr) return;

  string ticker = position->getTicker();  // Use inherited getTicker()
  positions[ticker] = position;
  lastUpdated = time(0);
}

void Portfolio::removePosition(string ticker) {
  auto it = positions.find(ticker);
  if (it != positions.end()) {
    delete it->second;
    positions.erase(it);
    lastUpdated = time(0);
  }
}

Position* Portfolio::getPosition(string ticker) const {
  auto it = positions.find(ticker);
  return (it != positions.end()) ? it->second : nullptr;
}

bool Portfolio::hasPosition(string ticker) const {
  return positions.find(ticker) != positions.end();
}

void Portfolio::updatePositions() {
  for (auto& pair : positions) {
    pair.second->updatePositionValue();
  }
  lastUpdated = time(0);
}

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
    if (order->canExecute()) {
      // Simplified execution logic - in reality this would be more complex
      double executionPrice = order->getStock()->getCurrentPrice(date);
      int quantityToExecute = order->getRemainingQuantity();

      // Check if we have sufficient funds for buy orders
      if ((order->getOrderType() == BUY ||
           order->getOrderType() == BUY_TO_COVER)) {
        double totalCost = executionPrice * quantityToExecute;
        if (!hasSufficientFunds(totalCost)) continue;
      }

      order->executeOrder(executionPrice, quantityToExecute);

      // Update cash balance
      if (order->getOrderType() == BUY ||
          order->getOrderType() == BUY_TO_COVER) {
        deductCash(order->getFilledValue());
      } else {
        addCash(order->getFilledValue());
      }

      // Create or update position if order is filled
      if (order->isCompletelyFilled()) {
        Position* newPosition = order->createPositionFromOrder();
        if (newPosition != nullptr) {
          string ticker = newPosition->getTicker();  // Use inherited method

          // Check if we already have a position in this stock
          if (hasPosition(ticker)) {
            Position* existingPos = getPosition(ticker);
            // Combine positions (simplified logic)
            existingPos->addToPosition(abs(newPosition->getQuantity()),
                                       newPosition->getEntryPrice());
            delete newPosition;
          } else {
            addPosition(newPosition);
          }
        }
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
  return cashBalance + calculatePositionsValue();
}

double Portfolio::calculatePositionsValue() const {
  double totalValue = 0.0;
  for (const auto& pair : positions) {
    totalValue += pair.second->getPositionValue();
  }
  return totalValue;
}

double Portfolio::calculateTotalPnL() const {
  return calculateUnrealisedPnL() + calculateRealisedPnL();
}

double Portfolio::calculateUnrealisedPnL() const {
  double totalPnL = 0.0;
  for (const auto& pair : positions) {
    if (pair.second->getIsOpen()) {
      totalPnL += pair.second->calculateUnrealisedPnL();
    }
  }
  return totalPnL;
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
void Portfolio::addToWatchlist(Stocks* stock) {
  if (stock == nullptr) return;
  watchlist[stock->getTicker()] = stock;
  lastUpdated = time(0);
}

void Portfolio::removeFromWatchlist(string ticker) {
  watchlist.erase(ticker);
  lastUpdated = time(0);
}

bool Portfolio::isInWatchlist(string ticker) const {
  return watchlist.find(ticker) != watchlist.end();
}

// Portfolio analysis
unordered_map<string, double> Portfolio::getAssetAllocation() const {
  unordered_map<string, double> allocation;
  double totalValue = calculateTotalValue();

  if (totalValue > 0) {
    // Cash allocation
    allocation["CASH"] = (cashBalance / totalValue) * 100.0;

    // Stock allocations
    for (const auto& pair : positions) {
      string ticker = pair.first;
      double positionValue = pair.second->getPositionValue();
      allocation[ticker] = (positionValue / totalValue) * 100.0;
    }
  }

  return allocation;
}

double Portfolio::getPositionWeight(string ticker) const {
  if (!hasPosition(ticker)) return 0.0;

  double totalValue = calculateTotalValue();
  if (totalValue == 0.0) return 0.0;

  double positionValue = getPosition(ticker)->getPositionValue();
  return (positionValue / totalValue) * 100.0;
}

vector<Position*> Portfolio::getTopPositions(int count) const {
  vector<Position*> allPositions;
  for (const auto& pair : positions) {
    allPositions.push_back(pair.second);
  }

  // Sort by position value (descending)
  sort(allPositions.begin(), allPositions.end(),
       [](const Position* a, const Position* b) {
         return a->getPositionValue() > b->getPositionValue();
       });

  // Return top N positions
  int actualCount = min(count, (int)allPositions.size());
  return vector<Position*>(allPositions.begin(),
                           allPositions.begin() + actualCount);
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
  cout << "Positions Value: $" << calculatePositionsValue() << endl;
  cout << "Total Value: $" << calculateTotalValue() << endl;
  cout << "Total Return: " << calculateTotalReturn() << "%" << endl;
  cout << "Unrealised P&L: $" << calculateUnrealisedPnL() << endl;
  cout << "Day Change: $" << calculateDayChange() << endl;
  cout << "Number of Positions: " << positions.size() << endl;
  cout << "Number of Orders: " << orders.size() << endl;
  cout << "Watchlist Size: " << watchlist.size() << endl;

  // Print creation date
  char buffer[26];
  struct tm* tm_info = localtime(&creationDate);
  strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
  cout << "Created: " << buffer << endl;
}

void Portfolio::printPositions(string date) {
  cout << "\nPositions:" << endl;
  cout << "----------" << endl;

  if (positions.empty()) {
    cout << "No positions" << endl;
    return;
  }

  for (const auto& pair : positions) {
    Position* pos = pair.second;
    cout << pos->getTicker() << ": "  // Use inherited method
         << pos->getQuantity() << " shares @ $" << fixed << setprecision(2)
         << pos->getCurrentPrice(date)  // Use inherited method
         << " (P&L: $" << pos->calculateUnrealisedPnL() << ")" << endl;
  }
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

void Portfolio::printWatchlist(string date) {
  cout << "\nWatchlist:" << endl;
  cout << "---------" << endl;

  if (watchlist.empty()) {
    cout << "No stocks in watchlist" << endl;
    return;
  }

  for (const auto& pair : watchlist) {
    Stocks* stock = pair.second;
    cout << stock->getTicker() << " (" << stock->getCompanyName() << "): $"
         << fixed << setprecision(2) << stock->getCurrentPrice(date) << endl;
  }
}

// Destructor
Portfolio::~Portfolio() {
  // Clean up positions
  for (auto& pair : positions) {
    delete pair.second;
  }
  positions.clear();

  // Clean up orders
  for (Order* order : orders) {
    delete order;
  }
  orders.clear();

  // Note: We don't delete stocks from watchlist as they might be owned by other
  // objects
  watchlist.clear();
}
