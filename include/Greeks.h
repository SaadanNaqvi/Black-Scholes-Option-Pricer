#ifndef GREEKS_H
#define GREEKS_H  
struct MarketParams;

//so can declare using just marketparams instead of seperate
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
