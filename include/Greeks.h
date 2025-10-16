#ifndef GREEKS_H
#define GREEKS_H  
struct Parameters;

namespace Greeks {

double deltaCall(const Parameters& p);
double deltaPut(const Parameters& p);
double gamma(const Parameters& p);
double thetaCall(const Parameters& p);
double thetaPut(const Parameters& p);
double vega(const Parameters& p);
double rhoCall(const Parameters& p);
double rhoPut(const Parameters& p);

}

#endif
