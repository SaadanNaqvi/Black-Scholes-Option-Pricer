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

// ===== Call/Put base classes =====
class Call : public OptionType {
public:
    explicit Call(const MarketParams& mp) : OptionType(mp) {}
};

class Put : public OptionType {
public:
    explicit Put(const MarketParams& mp) : OptionType(mp) {}
};

// ===== European options =====
class EuropeanCall : public Call {
public:
    explicit EuropeanCall(const MarketParams& mp) : Call(mp) {}
    double price() override;
};

class EuropeanPut : public Put {
public:
    explicit EuropeanPut(const MarketParams& mp) : Put(mp) {}
    double price() override;
};

// ===== American options (optional placeholder) =====
class AmericanCall : public Call {
public:
    explicit AmericanCall(const MarketParams& mp) : Call(mp) {}
    double price() override;
};

class AmericanPut : public Put {
public:
    explicit AmericanPut(const MarketParams& mp) : Put(mp) {}
    double price() override;
};

#endif // BLACK_SCHOLES_H
