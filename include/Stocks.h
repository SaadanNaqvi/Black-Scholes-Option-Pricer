#ifndef STOCKS_H
#define STOCKS_H

#include <string>
#include <iostream>
#include <vector>
using namespace std;

struct stockData{
    double currentPirce;
    double openPrice;
    double closePrice;
    double highPrice;
    double lowPrice;
    long long volume;
};

class Stocks{
    private:
        string ticker;
        vector<stockData> pastPrices;
    public:
        Stocks();
        Stocks(string ticker, double currentPrice, double openPrice, double closePrice, double highPrice, double lowPrice, long long volume);
};

#endif