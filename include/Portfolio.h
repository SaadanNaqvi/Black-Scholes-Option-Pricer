#ifndef PORTFOLIO_H
#define PORTFOLIO_H

#include "User.h"
#include "Position.h"
#include "Order.h"

#include <unordered_map>
#include <ctime>
#include <string>

using namespace std;

class Portfolio{
    private:
        int portfolioID;  

        unordered_map<string, Position*> positions; // Open Positions in stocks/options
        Order* orders; // Ordering a stock
    protected:
        double balance;
        time_t lastUpdated;
    public:
        Portfolio();

        // getters
        double getBalance();

        double calcPortfolioValue(); // Cash Balance + Positions
        double calcPNL(); // Profit and Loss
};

#endif