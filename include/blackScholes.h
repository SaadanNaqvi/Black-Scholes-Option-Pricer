#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include <cmath>

struct Parameters {
    double spotPrice;
    double strikePrice;
    double timeToMaturity;
    double riskFreeInterestRate;
    double volatilityOfUnderlyingAsset;
    double dividendYield;
};

class OptionType {
protected:
    Parameters marketParams;
    double D1() const;
    double D2() const;
    double discountedR() const; // e^(-rt)
    double discountedQ() const; // e^(-qt)

public:
    explicit OptionType(const Parameters& x) : marketParams(x) {}
    virtual double price() = 0;
    virtual ~OptionType() = default;
};

class call : public OptionType {
public:
    explicit call(const Parameters &x) : OptionType(x) {}
    ~call() override = default;
};

class put : public OptionType {
public:
    explicit put(const Parameters &x) : OptionType(x) {}
    double D1();
    double D2();
    ~put() override = default;
};

class EuropeanCall : public call {
public:
    EuropeanCall(Parameters &x) : call(x) {}
    double price() override;
};

class EuropeanPut : public put {
public:
    EuropeanPut(Parameters &x) : put(x) {}
    double price() override;
};

class AmericanCall : public call {
public:
    AmericanCall(Parameters &x) : call(x) {}
    double price() override;
};

class AmericanPut : public put {
public:
    AmericanPut(Parameters &x) : put(x) {}
    double price() override;
};

#endif
