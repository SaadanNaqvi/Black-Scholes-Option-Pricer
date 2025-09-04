#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "User.h"
#include "Position.h"
#include "Order.h"
#include <ctime>
using namespace std;

class Portfolio{
    protected:
        int portfolioID;  
        
        double balance; // Cash Balance

        Position* positions; // Open Positions in stocks/options
        Order* orders; // Ordering a stock

        double portfolioValue; // Cash Balance + Positions
        double PNL; // Profit and Loss

        time_t lastUpdated;
    public:
        Portfolio();


        double getBalance();

};

#endif