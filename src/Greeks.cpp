#include "Greeks.h"
#include "blackScholes.h" 
#include <cmath>
double N(double x);      // CDF
double NDash(double x);  // PDF
//finding D1 and D2
static double D1(const MarketParams& p) {
    return (std::log(p.spotPrice / p.strikePrice) +
           (p.riskFreeInterestRate - p.dividendYield + 0.5 * p.volatilityOfUnderlyingAsset * p.volatilityOfUnderlyingAsset) * p.timeToMaturity)
           / (p.volatilityOfUnderlyingAsset * std::sqrt(p.timeToMaturity));
}
static double D2(const MarketParams& p) {
    return D1(p) - p.volatilityOfUnderlyingAsset * std::sqrt(p.timeToMaturity);
}


// Greeks
namespace Greeks{
//How much an option's price changes for a 1 dollaer change in the underlying asset's price.
double deltaCall(const MarketParams& p){
    return std::exp(-p.dividendYield * p.timeToMaturity) * N(D1(p));
}
double deltaPut(const MarketParams& p){
    return std::exp(-p.dividendYield * p.timeToMaturity) * (N(D1(p)) - 1);
};
//How much the option's delta is expected to change for a 1 dollar change in the underlying asset's price.
double gamma(const MarketParams& p){
    return (NDash(D1(p)) * std::exp(-p.dividendYield * p.timeToMaturity)) /
         (p.spotPrice * p.volatilityOfUnderlyingAsset * std::sqrt(p.timeToMaturity));

}
//rate of erosion of option price as it appraches expiration date
double thetaCall(const MarketParams& p){
    return -(p.spotPrice * NDash(D1(p)) * p.volatilityOfUnderlyingAsset *
           std::exp(-p.dividendYield * p.timeToMaturity)) /
             (2 * std::sqrt(p.timeToMaturity)) -
         p.dividendYield * p.spotPrice * N(D1(p)) *
             std::exp(-p.dividendYield * p.timeToMaturity) +
         p.riskFreeInterestRate * p.strikePrice *
             std::exp(-p.riskFreeInterestRate * p.timeToMaturity) * N(D2(p));

}
double thetaPut(const MarketParams& p){
    return -(p.spotPrice * NDash(D1(p)) * p.volatilityOfUnderlyingAsset *
           std::exp(-p.dividendYield * p.timeToMaturity)) /
             (2 * std::sqrt(p.timeToMaturity)) +
         p.dividendYield * p.spotPrice * N(-D1(p)) *
             std::exp(-p.dividendYield * p.timeToMaturity) -
         p.riskFreeInterestRate * p.strikePrice * N(-D2(p)) *
             std::exp(-p.riskFreeInterestRate * p.timeToMaturity);

}
//how much an options price is expected to change with a 1% change in implied volatility
double vega(const MarketParams& p){
  return (p.spotPrice * std::sqrt(p.timeToMaturity) * NDash(D1(p)) *
          std::exp(-p.dividendYield * p.timeToMaturity));
}
//How much an options price is expected to chaneg for a 1% change in the risk free interest rate
double rhoCall(const MarketParams& p){
  return (p.strikePrice * p.timeToMaturity *
          std::exp(-p.riskFreeInterestRate * p.timeToMaturity) * N(D2(p)));
}
double rhoPut(const MarketParams& p){
  return -(p.strikePrice * p.timeToMaturity *
           std::exp(-p.riskFreeInterestRate * p.timeToMaturity) * N(-D2(p)));
}
}