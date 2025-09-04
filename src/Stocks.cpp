#include "Stocks.h"
using namespace std;

Stocks::Stocks(){
    ticker = "";
    currentPirce = 0.0;
    openPrice = 0.0;
    closePrice = 0.0;
    highPrice = 0.0;
    lowPrice = 0.0;
    volume = 0;
}

Stocks::Stocks(string ticker, double currentPrice, double openPrice, double closePrice, double highPrice, double lowPrice, long long volume){
    this->ticker = ticker;
    this->currentPirce = currentPrice;
    this->openPrice = openPrice;
    this->closePrice = closePrice;
    this->highPrice = highPrice;
    this->lowPrice = lowPrice;
    this->volume = volume;
}