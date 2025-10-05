#ifndef POSITION_H
#define POSITION_H

#include "Stocks.h"
#include <string>
#include <ctime>
using namespace std;

enum PositionType {
    LONG,
    SHORT
};

// Middle layer (Layer 2) - Inherits from Stocks, adds position management
class Position : public Stocks {
    protected:  // Changed to protected for further inheritance
        string positionId;      // Unique identifier for this position
        PositionType type;      // Long or short position
        int quantity;           // Number of shares (positive for long, negative for short)
        double entryPrice;      // Price at which position was opened
        double currentValue;    // Current market value of position
        time_t openDate;        // When position was opened
        time_t lastUpdated;     // Last time position was updated
        bool isOpen;           // Whether position is still open
        double unrealizedPnL;   // Cached P&L calculation
        
    public:
        // Constructors (inherit from Stocks but add position-specific data)
        Position();
        Position(const string& ticker, const string& companyName, double currentPrice, 
                PositionType type, int quantity, double entryPrice);
        Position(const Stocks& stock, PositionType type, int quantity, double entryPrice);
        
        // Virtual destructor for further inheritance
        virtual ~Position();
        
        // Position-specific getters
        string getPositionId() const;
        PositionType getType() const;
        int getQuantity() const;
        double getEntryPrice() const;
        time_t getOpenDate() const;
        time_t getLastUpdated() const;
        bool getIsOpen() const;
        double getUnrealizedPnL() const;
        
        // Position-specific setters
        void setPositionId(const string& positionId);
        void setQuantity(int quantity);
        void setEntryPrice(double entryPrice);
        
        // Override virtual methods from Stocks
        virtual double calculateValue() const override;      // Override: position value calculation
        virtual double calculateRisk() const override;       // Override: position risk calculation
        virtual void displayInfo() const override;           // Override: position display
        virtual string getClassType() const override;        // Override: return "Position"
        virtual void updateCurrentPrice(double price) override; // Override: update and recalculate
        
        // Position-specific methods
        void updatePositionValue();
        double calculateUnrealizedPnL() const;
        double calculateRealizedPnL(double closePrice) const;
        void closePosition(double closePrice);
        void addToPosition(int additionalQuantity, double newEntryPrice);
        void reducePosition(int quantityToReduce, double exitPrice);
        
        // Utility functions
        double getPositionValue() const;
        double getPositionWeight(double totalPortfolioValue) const;
        string getPositionTypeString() const;
        void printPositionDetails() const;
};

#endif
