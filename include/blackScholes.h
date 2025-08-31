#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include <cmath>
using namespace std;

class BlackScholes{
    private: 
        double spotPrice;
        double strikePirce;
        double timeToMaturity;
        double riskFreeInterestRate;
        double volatilityOfUnderlyingAsset;
        double dividendYield;
    public:
        // Capture the combined effect of time, volatility, and drift on the option payoff distribution.
        double d1 = (log(spotPrice/strikePirce)+(riskFreeInterestRate-dividendYield+(0.5)*(pow(volatilityOfUnderlyingAsset,2))))/(volatilityOfUnderlyingAsset*(sqrt(timeToMaturity)));
        double d2 = d1-volatilityOfUnderlyingAsset*sqrt(timeToMaturity);
};

#endif