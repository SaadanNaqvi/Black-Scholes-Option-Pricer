#ifndef STOCKS_H
#define STOCKS_H

#include <string>
#include <iostream>
using namespace std;

class Stocks{
    private:
        string ticker;
        double currentPirce;
        double openPrice;
        double closePrice;
        double highPrice;
        double lowPrice;
        long long volume;
    public:
        Stocks();
        Stocks(string ticker, double currentPrice, double openPrice, double closePrice, double highPrice, double lowPrice, long long volume);


};

#endif