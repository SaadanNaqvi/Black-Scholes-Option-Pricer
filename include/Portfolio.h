#ifndef PORTFOLIO_H

#define PORTFOLIO_H

// Forward declarations to avoid circular dependencies
class Order;
class User;

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// Final child class (Layer 3) - Inherits from Position, adds portfolio
// management
class Portfolio {
 protected:  // Protected for potential future inheritance
  string portfolioId;
  string ownerId;         // User ID who owns this portfolio
                          // Open positions (ticker -> Position)
  vector<Order*> orders;  // All orders (both active and historical)
  double cashBalance;
  unordered_map<string, long long> stocks;
  double initialValue;         // Starting portfolio value
  double totalPortfolioValue;  // Cached total value calculation
  time_t creationDate;
  time_t lastUpdated;
  double totalPnl = 0;

 public:
  // Constructors - inherit from Position chain
  Portfolio();
  Portfolio(string portfolioId, string ownerId, double initialCash);
  Portfolio(string portfolioId, string ownerId, string ticker,
            string companyName, double currentPrice, int quantity,
            double entryPrice, double initialCash);

  // Virtual destructor
  virtual ~Portfolio();

  // get stocks.
  unordered_map<string, long long> getStocks();

  // Portfolio-specific getters
  string getPortfolioId() const;
  string getOwnerId() const;
  double getCashBalance() const;
  double getInitialValue() const;
  vector<Order*> getOrders() const;
  time_t getCreationDate() const;
  time_t getLastUpdated() const;

  // Portfolio-specific setters
  void setPortfolioId(string portfolioId);
  void setOwnerId(string ownerId);
  void setCashBalance(double balance);

  // Order management
  // TODO
  // Adds order to the portfolio.
  void appendStock(string ticker, long long quantity);
  void addOrder(Order* order);
  // Cancels order from the portfolio (refunds).
  void cancelOrder(string orderId);
  // returns the order details from an ID.
  Order* getOrder(string orderId) const;
  vector<Order*> getPendingOrders() const;
  vector<Order*> getFilledOrders() const;
  void processOrders(string date);

  // Cash management
  // Not necessary.
  bool hasSufficientFunds(double amount) const;
  void addCash(double amount);
  void deductCash(double amount);
  // Not needed?
  //  Portfolio calculations
  double calculateTotalValue() const;
  double calculateTotalPnL() const;
  double calculateRealisedPnL() const;
  double calculateTotalReturn() const;
  void updatePnl(double change);
  double getTotalPnl();

  // Watchlist management

  // Portfolio analysis
  unordered_map<string, double> getAssetAllocation() const;
  double calculatePortfolioBeta() const;
  double calculateSharpeRatio() const;

  // Utility functions
  void printPortfolioSummary() const;
  void printOrders() const;
};

#endif
