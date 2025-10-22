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
