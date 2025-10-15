#include "Greeks.h"
#include "blackScholes.h" 
#include <cmath>
double N(double x);      // CDF
double NDash(double x);  // PDF (your function name)

static double D1(const Parameters& p) {
    return (std::log(p.spotPrice / p.strikePrice) +
           (p.riskFreeInterestRate - p.dividendYield + 0.5 * p.volatilityOfUnderlyingAsset * p.volatilityOfUnderlyingAsset) * p.timeToMaturity)
           / (p.volatilityOfUnderlyingAsset * std::sqrt(p.timeToMaturity));
}
static double D2(const Parameters& p) {
    return D1(p) - p.volatilityOfUnderlyingAsset * std::sqrt(p.timeToMaturity);
}


// Greeks
namespace Greeks{
double deltaCall(const Parameters& p){
    return std::exp(-p.dividendYield * p.timeToMaturity) * N(D1(p));
}

double deltaPut(const Parameters& p){
    return std::exp(-p.dividendYield * p.timeToMaturity) * (N(D1(p)) - 1);
};

double gamma(const Parameters& p){
    return (NDash(D1(p)) * std::exp(-p.dividendYield * p.timeToMaturity)) /
         (p.spotPrice * p.volatilityOfUnderlyingAsset * std::sqrt(p.timeToMaturity));

}
double thetaCall(const Parameters& p){
    return -(p.spotPrice * NDash(D1(p)) * p.volatilityOfUnderlyingAsset *
           std::exp(-p.dividendYield * p.timeToMaturity)) /
             (2 * std::sqrt(p.timeToMaturity)) -
         p.dividendYield * p.spotPrice * N(D1(p)) *
             std::exp(-p.dividendYield * p.timeToMaturity) +
         p.riskFreeInterestRate * p.strikePrice *
             std::exp(-p.riskFreeInterestRate * p.timeToMaturity) * N(D2(p));

}
double thetaPut(const Parameters& p){
    return -(p.spotPrice * NDash(D1(p)) * p.volatilityOfUnderlyingAsset *
           std::exp(-p.dividendYield * p.timeToMaturity)) /
             (2 * std::sqrt(p.timeToMaturity)) +
         p.dividendYield * p.spotPrice * N(-D1(p)) *
             std::exp(-p.dividendYield * p.timeToMaturity) -
         p.riskFreeInterestRate * p.strikePrice * N(-D2(p)) *
             std::exp(-p.riskFreeInterestRate * p.timeToMaturity);

}
double vega(const Parameters& p){
  return (p.spotPrice * std::sqrt(p.timeToMaturity) * NDash(D1(p)) *
          std::exp(-p.dividendYield * p.timeToMaturity));
}
double rhoCall(const Parameters& p){
  return (p.strikePrice * p.timeToMaturity *
          std::exp(-p.riskFreeInterestRate * p.timeToMaturity) * N(D2(p)));
}
double rhoPut(const Parameters& p){
  return -(p.strikePrice * p.timeToMaturity *
           std::exp(-p.riskFreeInterestRate * p.timeToMaturity) * N(-D2(p)));
}
}