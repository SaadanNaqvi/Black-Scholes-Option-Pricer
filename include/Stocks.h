#ifndef STOCKS_H
#define STOCKS_H

#include <ctime>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include "CSVData.h"
/*struct stockData {
  double currentPrice;
  double openPrice;
  double closePrice;
  double highPrice;
  double lowPrice;
  long long volume;
};
*/

// Base class (Layer 1) - Financial Instrument
class Stocks {
 protected:  // Changed to protected so child classes can access
  string ticker;
  string companyName;
  string instrumentId;  // Base identifier for any financial instrument
  double baseValue;     // Base value for inheritance hierarchy
  time_t creationTime;

 public:
  CSVData priceHistory;
  // Constructors
  Stocks();
  Stocks(string ticker, string companyName = "");
  Stocks(string ticker, string companyName, double currentPrice,
         double openPrice, double closePrice, double highPrice, double lowPrice,
         long long volume);

  // Virtual destructor for proper inheritance
  virtual ~Stocks();

  // Getters
  string getTicker();
  string getCompanyName();
  double getCurrentPrice(string date);
  /*
  double getOpenPrice() const;
  double getClosePrice() const;
  double getHighPrice() const;
  double getLowPrice() const;
  long long getVolume() const;
  */
  string getInstrumentId() const;
  double getBaseValue() const;
  time_t getCreationTime() const;

  // Setters
  void setTicker(string ticker);
  void setCompanyName(string companyName);
  void setInstrumentId(string id);
  void setBaseValue(double value);
  // virtual void updateCurrentPrice(double price);  // Virtual for override
  void updateStockData(double currentPrice, double openPrice, double closePrice,
                       double highPrice, double lowPrice, long long volume);

  // Virtual methods for inheritance hierarchy
  void displayInfo(string date);  // Display information method

  // Utility functions
  //  void addHistoricalData(stockData data);
  double calculateVolatility();
  void printStockInfo(string date);

  // Static helper for creating IDs
  static string generateId(string prefix);
};

#endif
