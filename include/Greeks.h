#ifndef GREEKS_H
#define GREEKS_H  
struct MarketParams;

namespace Greeks {

double deltaCall(const MarketParams& p);
double deltaPut(const MarketParams& p);
double gamma(const MarketParams& p);
double thetaCall(const MarketParams& p);
double thetaPut(const MarketParams& p);
double vega(const MarketParams& p);
double rhoCall(const MarketParams& p);
double rhoPut(const MarketParams& p);

}

#endif
