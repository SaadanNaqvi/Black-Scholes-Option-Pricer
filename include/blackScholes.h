#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include <cmath>

// ===== Global helper functions =====
double N(double x);
double NDash(double x);

// ===== Struct for market parameters =====
struct MarketParams {
    double spotPrice;
    double strikePrice;
    double timeToMaturity;
    double riskFreeInterestRate;
    double dividendYield;
    double volatilityOfUnderlyingAsset;
};

// ===== Base OptionType =====
class OptionType {
protected:
    MarketParams marketParams;

    // internal helpers
    double discountedR() const; // e^(-rT)
    double discountedQ() const; // e^(-qT)

public:
    explicit OptionType(const MarketParams& mp) : marketParams(mp) {}
    virtual ~OptionType() = default;

    // make D1/D2 accessible for pricing & Greeks
    double D1() const;
    double D2() const;
    virtual double price() = 0;
};

#endif