#include "Stocks.h"
using namespace std;

Stocks::Stocks() {
  ticker = "";
  double currentPirce = 0.0;
  double openPrice = 0.0;
  double closePrice = 0.0;
  double highPrice = 0.0;
  double lowPrice = 0.0;
  double volume = 0;
}

Stocks::Stocks(string ticker, double currentPrice, double openPrice,
               double closePrice, double highPrice, double lowPrice,
               long long volume) {
  this->ticker = ticker;
  this->currentPirce = currentPrice;
  this->openPrice = openPrice;
  this->closePrice = closePrice;
  this->highPrice = highPrice;
  this->lowPrice = lowPrice;
  this->volume = volume;
}