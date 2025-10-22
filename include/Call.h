#ifndef CALL_H
#define CALL_H

#include "options.h"
#include "BlackScholes.h"

class Call : public OptionType {
public:
    explicit Call(const MarketParams& mp) : OptionType(mp) {}
};
class EuropeanCall : public Call {
public:
    explicit EuropeanCall(const MarketParams& mp) : Call(mp) {}
    double price() override;
};
class AmericanCall : public Call {
public:
    explicit AmericanCall(const MarketParams& mp) : Call(mp) {}
    double price() override;
};


#endif
