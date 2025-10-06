#ifndef STOCKS_H
#define STOCKS_H

#include <string>
#include <iostream>
#include <vector>
#include <ctime>
using namespace std;

struct stockData{
    double currentPrice;
    double openPrice;
    double closePrice;
    double highPrice;
    double lowPrice;
    long long volume;
    time_t timestamp;
};

// Base class (Layer 1) - Financial Instrument
class Stocks {
    protected:  // Changed to protected so child classes can access
        string ticker;
        string companyName;
        vector<stockData> priceHistory;
        stockData currentData;
        string instrumentId;     // Base identifier for any financial instrument
        double baseValue;        // Base value for inheritance hierarchy
        time_t creationTime;
        
    public:
        // Constructors
        Stocks();
        Stocks(string ticker, string companyName = "");
        Stocks(string ticker, string companyName, double currentPrice, double openPrice, double closePrice, double highPrice, double lowPrice, long long volume);
        
        // Virtual destructor for proper inheritance
        virtual ~Stocks();
        
        // Getters
        string getTicker() const;
        string getCompanyName() const;
        double getCurrentPrice() const;
        double getOpenPrice() const;
        double getClosePrice() const;
        double getHighPrice() const;
        double getLowPrice() const;
        long long getVolume() const;
        vector<stockData> getPriceHistory() const;
        stockData getCurrentData() const;
        string getInstrumentId() const;
        double getBaseValue() const;
        time_t getCreationTime() const;
        
        // Setters
        void setTicker(string ticker);
        void setCompanyName(string companyName);
        void setInstrumentId(string id);
        void setBaseValue(double value);
        virtual void updateCurrentPrice(double price);  // Virtual for override
        void updateStockData(double currentPrice, double openPrice, double closePrice, double highPrice, double lowPrice, long long volume);
        
        // Virtual methods for inheritance hierarchy
        virtual double calculateValue() const;           // Base calculation method
        virtual double calculateRisk() const;            // Risk calculation method
        virtual void displayInfo() const;                // Display information method
        virtual string getClassType() const;             // Get class type string
        
        // Utility functions
        void addHistoricalData(stockData data);
        double calculateDailyReturn() const;
        double calculateVolatility() const;
        void printStockInfo() const;
        
        // Static helper for creating IDs
        static string generateId(string prefix);
};

#endif
