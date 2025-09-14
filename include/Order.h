#ifndef ORDER_H
#define ORDER_H

using namespace std;
#include "Portfolio.h"
#include "Stocks.h"

#include <queue>
#include <ctime>

class Order: public Portfolio{
    protected:
        priority_queue<pair<time_t, double>, vector<pair<time_t, double>>, greater<pair<time_t,double>>> orderBook;
    public:
};

#endif
