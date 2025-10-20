#include <cmath>
#include <iostream>
#include <numbers>

#include "blackScholes.h"
// for equations visit
// https://financetrainingcourse.com/education/2010/07/derivative-pricing-black-scholes-equation-binomial-trees-calculation-reference/

// standard normal cumulative distribution function
double N(double x) {
  // N(x)=1/sqrt(2*Pi) *integral(-inf->x)(e^((-t^2)/2) dt)
  // N(x)=(1/2)+(1/2)erf(x/sqrt(2))

  return 0.5+0.5 * std::erf(x / std::sqrt(2));
}
double NDash(double x) {
  // standard noral pdf
  return (1 / (sqrt(2 * M_PI))) * (std::exp(-(std::pow(x, 2) / 2)));
}

//
double OptionType::discountedR() const {
  return std::exp(-marketParams.riskFreeInterestRate *
                  marketParams.timeToMaturity);
}
double OptionType::discountedQ() const {
  return std::exp(-marketParams.dividendYield * marketParams.timeToMaturity);
}
//

// D1 D2 helpers (shared for eu and am functions as well as call and put)
double OptionType::D1() const {
  // for readability
  double S = marketParams.spotPrice;
  double K = marketParams.strikePrice;
  double T = marketParams.timeToMaturity;
  double r = marketParams.riskFreeInterestRate;
  double q = marketParams.dividendYield;
  double sigma = marketParams.volatilityOfUnderlyingAsset;

  // D1 formula
  double numerator = std::log(S / K) + (r - q + 0.5 * sigma * sigma) * T;
  double denominator = sigma * std::sqrt(T);

  return numerator / denominator;
}
// D2
double OptionType::D2() const {
  double sigma = marketParams.volatilityOfUnderlyingAsset;
  double T = marketParams.timeToMaturity;

  return D1() - sigma * std::sqrt(T);
}

// EUROPEAN PRICINGGGG

double EuropeanCall::price() {
  return N(OptionType::D1()) * marketParams.spotPrice*std::exp(-marketParams.dividendYield * marketParams.timeToMaturity) -
         N(OptionType::D2()) * marketParams.strikePrice *
             std::exp(-marketParams.riskFreeInterestRate *
                      marketParams.timeToMaturity);
}

double EuropeanPut::price() {
  // P = K e^{-rT} N(-d2) - S e^{-qT} N(-d1)
  return marketParams.strikePrice *
             std::exp(-marketParams.riskFreeInterestRate *
                      marketParams.timeToMaturity) *
             N(-OptionType::D2()) -
         marketParams.spotPrice *
             std::exp(-marketParams.dividendYield *
                      marketParams.timeToMaturity) *
             N(-OptionType::D1());
}

// American
double AmericanCall::price(){
return N(OptionType::D1()) * marketParams.spotPrice*std::exp(-marketParams.dividendYield * marketParams.timeToMaturity) -
         N(OptionType::D2()) * marketParams.strikePrice *
             std::exp(-marketParams.riskFreeInterestRate *
                      marketParams.timeToMaturity);
}

double AmericanPut::price(){
return marketParams.strikePrice *
             std::exp(-marketParams.riskFreeInterestRate *
                      marketParams.timeToMaturity) *
             N(-OptionType::D2()) -
         marketParams.spotPrice *
             std::exp(-marketParams.dividendYield *
                      marketParams.timeToMaturity) *
             N(-OptionType::D1());
}