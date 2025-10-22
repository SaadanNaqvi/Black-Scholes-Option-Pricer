#ifndef PUT_H
#define PUT_H

#include "options.h"
#include "BlackScholes.h"
// ===== Call/Put base classes =====
class Put : public OptionType {
public:
    explicit Put(const MarketParams& mp) : OptionType(mp) {}
};

// ===== European options =====

class EuropeanPut : public Put {
public:
    explicit EuropeanPut(const MarketParams& mp) : Put(mp) {}
    double price() override;
};

// ===== American options (optional placeholder) =====
class AmericanPut : public Put {
public:
    explicit AmericanPut(const MarketParams& mp) : Put(mp) {}
    double price() override;
};

#endif