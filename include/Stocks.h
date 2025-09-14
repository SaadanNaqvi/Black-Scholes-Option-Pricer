#ifndef STOCKS_H
#define STOCKS_H

#include <string>
#include <iostream>
using namespace std;

struct dailyStock{
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
        vector<dailyStock> pastPrices;
        
    public:
        Stocks();
        Stocks(string ticker, double currentPrice, double openPrice, double closePrice, double highPrice, double lowPrice, long long volume);


};

#endif