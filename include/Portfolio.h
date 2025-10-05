#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "Position.h"
// Forward declarations to avoid circular dependencies
class Order;
class User;

#include <unordered_map>
#include <vector>
#include <string>
#include <queue>

using namespace std;

// Final child class (Layer 3) - Inherits from Position, adds portfolio management
class Portfolio : public Position {
    protected:  // Protected for potential future inheritance
        string portfolioId;
        string ownerId;  // User ID who owns this portfolio
        unordered_map<string, Position*> positions; // Open positions (ticker -> Position)
        vector<Order*> orders; // All orders (both active and historical)
        unordered_map<string, Stocks*> watchlist; // Stocks being watched (ticker -> Stock)
        double cashBalance;
        double initialValue; // Starting portfolio value
        double totalPortfolioValue; // Cached total value calculation
        time_t creationDate;
        time_t lastUpdated;
        
    public:
        // Constructors - inherit from Position chain
        Portfolio();
        Portfolio(string portfolioId, string ownerId, double initialCash);
        Portfolio(string portfolioId, string ownerId, string ticker, 
                 string companyName, double currentPrice, PositionType type, 
                 int quantity, double entryPrice, double initialCash);
        
        // Virtual destructor
        virtual ~Portfolio();
        
        // Portfolio-specific getters
        string getPortfolioId() const;
        string getOwnerId() const;
        double getCashBalance() const;
        double getInitialValue() const;
        unordered_map<string, Position*> getPositions() const;
        vector<Order*> getOrders() const;
        unordered_map<string, Stocks*> getWatchlist() const;
        time_t getCreationDate() const;
        time_t getLastUpdated() const;
        
        // Portfolio-specific setters
        void setPortfolioId(string portfolioId);
        void setOwnerId(string ownerId);
        void setCashBalance(double balance);
        
        // Override virtual methods from Position/Stocks
        virtual double calculateValue() const override;      // Override: total portfolio value
        virtual double calculateRisk() const override;       // Override: portfolio risk calculation  
        virtual void displayInfo() const override;           // Override: portfolio display
        virtual string getClassType() const override;        // Override: return "Portfolio"
        virtual void updateCurrentPrice(double price) override; // Override: update portfolio calculations
        
        // Position management
        void addPosition(Position* position);
        void removePosition(string ticker);
        Position* getPosition(string ticker) const;
        bool hasPosition(string ticker) const;
        void updatePositions();
        
        // Order management
        void addOrder(Order* order);
        void cancelOrder(string orderId);
        Order* getOrder(string orderId) const;
        vector<Order*> getPendingOrders() const;
        vector<Order*> getFilledOrders() const;
        void processOrders();
        
        // Cash management
        bool hasSufficientFunds(double amount) const;
        void addCash(double amount);
        void deductCash(double amount);
        
        // Portfolio calculations
        double calculateTotalValue() const;
        double calculatePositionsValue() const;
        double calculateTotalPnL() const;
        double calculateUnrealizedPnL() const;
        double calculateRealizedPnL() const;
        double calculateDayChange() const;
        double calculateTotalReturn() const;
        
        // Watchlist management
        void addToWatchlist(Stocks* stock);
        void removeFromWatchlist(string ticker);
        bool isInWatchlist(string ticker) const;
        
        // Portfolio analysis
        unordered_map<string, double> getAssetAllocation() const;
        double getPositionWeight(string ticker) const;
        vector<Position*> getTopPositions(int count = 5) const;
        double calculatePortfolioBeta() const;
        double calculateSharpeRatio() const;
        
        // Utility functions
        void printPortfolioSummary() const;
        void printPositions() const;
        void printOrders() const;
        void printWatchlist() const;
};

#endif
