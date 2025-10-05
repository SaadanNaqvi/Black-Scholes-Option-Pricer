#include "Position.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

// Default constructor - calls Stocks default constructor
Position::Position() : Stocks() {
    positionId = generateId("POS");
    type = LONG;
    quantity = 0;
    entryPrice = 0.0;
    currentValue = 0.0;
    openDate = time(0);
    lastUpdated = time(0);
    isOpen = false;
    unrealizedPnL = 0.0;
}

// Constructor with stock data and position info
Position::Position(const string& ticker, const string& companyName, double currentPrice, 
                  PositionType type, int quantity, double entryPrice) 
    : Stocks(ticker, companyName, currentPrice, currentPrice, currentPrice, currentPrice, currentPrice, 1000) {
    this->positionId = generateId("POS");
    this->type = type;
    this->quantity = quantity;
    this->entryPrice = entryPrice;
    this->openDate = time(0);
    this->lastUpdated = time(0);
    this->isOpen = true;
    this->unrealizedPnL = 0.0;
    updatePositionValue();
}

// Constructor copying from existing stock
Position::Position(const Stocks& stock, PositionType type, int quantity, double entryPrice) 
    : Stocks(stock) {
    this->positionId = generateId("POS");
    this->type = type;
    this->quantity = quantity;
    this->entryPrice = entryPrice;
    this->openDate = time(0);
    this->lastUpdated = time(0);
    this->isOpen = true;
    this->unrealizedPnL = 0.0;
    updatePositionValue();
}

// Position-specific getters
string Position::getPositionId() const {
    return positionId;
}

PositionType Position::getType() const {
    return type;
}

int Position::getQuantity() const {
    return quantity;
}

double Position::getEntryPrice() const {
    return entryPrice;
}

time_t Position::getOpenDate() const {
    return openDate;
}

time_t Position::getLastUpdated() const {
    return lastUpdated;
}

bool Position::getIsOpen() const {
    return isOpen;
}

double Position::getUnrealizedPnL() const {
    return unrealizedPnL;
}

// Position-specific setters
void Position::setPositionId(const string& positionId) {
    this->positionId = positionId;
}

void Position::setQuantity(int quantity) {
    this->quantity = quantity;
    updatePositionValue();
}

void Position::setEntryPrice(double entryPrice) {
    this->entryPrice = entryPrice;
}

// Override virtual methods from Stocks
double Position::calculateValue() const {
    if (!isOpen) return 0.0;
    return abs(quantity) * getCurrentPrice();  // Use inherited getCurrentPrice()
}

double Position::calculateRisk() const {
    // Position risk = stock volatility * position size factor
    double stockRisk = Stocks::calculateRisk();  // Call parent's risk calculation
    double positionFactor = abs(quantity) / 100.0;  // Simple position size factor
    return stockRisk * positionFactor;
}

void Position::displayInfo() const {
    cout << "[POSITION] " << getClassType() << ": " << getTicker() 
         << " (" << getPositionTypeString() << " " << quantity << " shares)" 
         << " Value: $" << fixed << setprecision(2) << calculateValue() << endl;
}

string Position::getClassType() const {
    return "Position";
}

void Position::updateCurrentPrice(double price) {
    Stocks::updateCurrentPrice(price);  // Call parent method first
    updatePositionValue();               // Then update position-specific values
}

// Position-specific methods
void Position::updatePositionValue() {
    if (isOpen) {
        currentValue = abs(quantity) * getCurrentPrice();
        unrealizedPnL = calculateUnrealizedPnL();
        lastUpdated = time(0);
    }
}

double Position::calculateUnrealizedPnL() const {
    if (!isOpen) return 0.0;
    
    double currentPrice = getCurrentPrice();  // Use inherited method
    double pnl = 0.0;
    
    if (type == LONG) {
        pnl = quantity * (currentPrice - entryPrice);
    } else { // SHORT
        pnl = quantity * (entryPrice - currentPrice);
    }
    
    return pnl;
}

double Position::calculateRealizedPnL(double closePrice) const {
    double pnl = 0.0;
    
    if (type == LONG) {
        pnl = quantity * (closePrice - entryPrice);
    } else { // SHORT
        pnl = quantity * (entryPrice - closePrice);
    }
    
    return pnl;
}

void Position::closePosition(double closePrice) {
    if (!isOpen) return;
    
    currentValue = abs(quantity) * closePrice;
    isOpen = false;
    lastUpdated = time(0);
}

void Position::addToPosition(int additionalQuantity, double newEntryPrice) {
    if (!isOpen || additionalQuantity <= 0) return;
    
    // Calculate weighted average entry price
    double totalValue = (quantity * entryPrice) + (additionalQuantity * newEntryPrice);
    int totalQuantity = quantity + additionalQuantity;
    
    entryPrice = totalValue / totalQuantity;
    quantity = totalQuantity;
    lastUpdated = time(0);
    updatePositionValue();
}

void Position::reducePosition(int quantityToReduce, double exitPrice) {
    if (!isOpen || quantityToReduce <= 0 || quantityToReduce > abs(quantity)) return;
    
    if (quantityToReduce == abs(quantity)) {
        closePosition(exitPrice);
    } else {
        quantity = (quantity > 0) ? quantity - quantityToReduce : quantity + quantityToReduce;
        lastUpdated = time(0);
        updatePositionValue();
    }
}

// Utility functions
double Position::getPositionValue() const {
    return currentValue;
}

double Position::getPositionWeight(double totalPortfolioValue) const {
    if (totalPortfolioValue == 0.0) return 0.0;
    return (currentValue / totalPortfolioValue) * 100.0;
}

string Position::getPositionTypeString() const {
    return (type == LONG) ? "LONG" : "SHORT";
}

void Position::printPositionDetails() const {
    cout << "Position Details:" << endl;
    cout << "Position ID: " << positionId << endl;
    cout << "Stock: " << getTicker() << " (" << getCompanyName() << ")" << endl;  // Use inherited methods
    cout << "Current Stock Price: $" << fixed << setprecision(2) << getCurrentPrice() << endl;
    cout << "Position Type: " << getPositionTypeString() << endl;
    cout << "Quantity: " << quantity << " shares" << endl;
    cout << "Entry Price: $" << fixed << setprecision(2) << entryPrice << endl;
    cout << "Current Value: $" << fixed << setprecision(2) << currentValue << endl;
    cout << "Unrealized P&L: $" << fixed << setprecision(2) << calculateUnrealizedPnL() << endl;
    cout << "Status: " << (isOpen ? "OPEN" : "CLOSED") << endl;
    
    // Convert timestamps to readable format
    char buffer[26];
    struct tm* tm_info;
    
    tm_info = localtime(&openDate);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    cout << "Open Date: " << buffer << endl;
    
    tm_info = localtime(&lastUpdated);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    cout << "Last Updated: " << buffer << endl;
}

// Virtual destructor
Position::~Position() {
    // Base class destructor will be called automatically
}
