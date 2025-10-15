#include "Stocks.h"

#include <cmath>
#include <iomanip>
#include <sstream>

#include "CSVData.h"

using namespace std;

// Default constructor

// Constructor with ticker and company name
Stocks::Stocks(string ticker, string companyName) {
  this->ticker = ticker;
  this->companyName = companyName;
  priceHistory = CSVData(ticker);
  instrumentId = generateId("STOCK");
  baseValue = 0.0;
  creationTime = time(0);
}

// Getters
string Stocks::getTicker() { return ticker; }

string Stocks::getCompanyName() { return companyName; }
double Stocks::getCurrentPrice(string date) {
  return priceHistory.currentPrice(date);
}
/*

double Stocks::getOpenPrice() const { return currentData.openPrice; }

double Stocks::getClosePrice() const { return currentData.closePrice; }

double Stocks::getHighPrice() const { return currentData.highPrice; }

double Stocks::getLowPrice() const { return currentData.lowPrice; }

long long Stocks::getVolume() const { return currentData.volume; }

vector<stockData> Stocks::getPriceHistory() const { return priceHistory; }
*/
// stockData Stocks::getCurrentData() const { return currentData; }

string Stocks::getInstrumentId() const { return instrumentId; }

double Stocks::getBaseValue() const { return baseValue; }

time_t Stocks::getCreationTime() const { return creationTime; }

// Setters
void Stocks::setTicker(string ticker) { this->ticker = ticker; }

void Stocks::setCompanyName(string companyName) {
  this->companyName = companyName;
}

void Stocks::setInstrumentId(string id) { this->instrumentId = id; }

void Stocks::setBaseValue(double value) { this->baseValue = value; }

// void Stocks::updateCurrentPrice(double price) {
// currentData.currentPrice = price;
//}

/*void Stocks::updateStockData(double currentPrice, double openPrice,
                             double closePrice, double highPrice,
                             double lowPrice, long long volume) {
  // Save current data to history before updating
  if (this->currentData.currentPrice != 0.0) {
    addHistoricalData(this->currentData);
  }

  this->currentData = {currentPrice, openPrice, closePrice, highPrice,
                       lowPrice,     volume,    time(0)};
}

// Utility functions
void Stocks::addHistoricalData(stockData data) { priceHistory.push_back(data); }

double Stocks::calculateDailyReturn() const {
  if (currentData.openPrice == 0.0) return 0.0;
  return ((currentData.currentPrice - currentData.openPrice) /
          currentData.openPrice) *
         100.0;
}
*/
// TODO :
/*double Stocks::calculateVolatility() {
  if (priceHistory.getAllData().size() < 2) return 0.0;

  vector<double> returns;
  for (size_t i = 1; i < priceHistory.size(); ++i) {
    if (priceHistory[i - 1].currentPrice != 0.0) {
      double dailyReturn =
          (priceHistory[i].currentPrice - priceHistory[i - 1].currentPrice)
/ priceHistory[i - 1].currentPrice; returns.push_back(dailyReturn);
    }
  }

  if (returns.empty()) return 0.0;

  // Calculate mean
  double mean = 0.0;
  for (double ret : returns) {
    mean += ret;
  }
  mean /= returns.size();

  // Calculate variance
  double variance = 0.0;
  for (double ret : returns) {
    variance += pow(ret - mean, 2);
  }
  variance /= returns.size();

  return sqrt(variance);
}
  */

void Stocks::printStockInfo(string date) {
  cout << "Stock Information:" << endl;
  cout << "Ticker: " << ticker << endl;
  cout << "Company: " << companyName << endl;
  cout << fixed << setprecision(2);
  cout << "Current Price: $" << priceHistory.currentPrice(date) << endl;
  cout << "Open Price: $" << priceHistory.openPrice(date) << endl;
  cout << "Close Price: $" << priceHistory.closePrice(date) << endl;
  cout << "High Price: $" << priceHistory.highPrice(date) << endl;
  cout << "Low Price: $" << priceHistory.lowPrice(date) << endl;
  cout << "Volume: " << priceHistory.volume(date) << endl;
}

// Virtual methods for inheritance
void Stocks::displayInfo(string date) {
  cout << "[STOCK] " << ": " << ticker << " ($" << fixed << setprecision(2)
       << priceHistory.currentPrice(date) << ")" << endl;
}

// Static helper method
string Stocks::generateId(string prefix) {
  static int counter = 0;
  ostringstream oss;
  oss << prefix << "_" << counter++;
  return oss.str();
}

// Destructor
Stocks::~Stocks() {
  // Vector cleanup is automatic
}
