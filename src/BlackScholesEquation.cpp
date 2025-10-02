#include <cmath>
#include <iostream>
#include <numbers>

#include "blackScholes.h"
// for equations visit
// https://financetrainingcourse.com/education/2010/07/derivative-pricing-black-scholes-equation-binomial-trees-calculation-reference/

// This section is responsible for the black scholes equations
//  Capture the combined effect of time, volatility, and drift on the option
//  payoff distribution.

// constructor
BlackScholes::BlackScholes(double spotPrice, double strikePrice,
                           double timeToMaturity, double riskFreeInterestRate,
                           double volatilityOfUnderlyingAsset,
                           double dividendYield) {
  this->spotPrice = spotPrice;
  this->strikePrice = strikePrice;
  this->timeToMaturity = timeToMaturity;
  this->riskFreeInterestRate = riskFreeInterestRate;
  this->volatilityOfUnderlyingAsset = volatilityOfUnderlyingAsset;
  this->dividendYield = dividendYield;
}

// standard normal cumulative distribution function
double N(double x) {
  // N(x)=1/sqrt(2*Pi) *integral(-inf->x)(e^((-t^2)/2) dt)
  // N(x)=(1/2)+(1/2)erf(x/sqrt(2))

  return 0.5 + 0.5 * std::erf(x / std::sqrt(2));
}

// D1 equation
double BlackScholes::D1() {
  return (std::log(spotPrice / strikePrice) +
          (riskFreeInterestRate - dividendYield +
           0.5 * std::pow(volatilityOfUnderlyingAsset, 2)) *
              timeToMaturity) /
         (volatilityOfUnderlyingAsset * std::sqrt(timeToMaturity));
}
// D2 equation
double BlackScholes::D2() {
  return D1() - volatilityOfUnderlyingAsset * std::sqrt(timeToMaturity);
}

// Call equation
double BlackScholes::callPrice() {
  // formula for Call=Se^(-qt)N(D1)-ke^-(rt)N(D2)
  return spotPrice * std::exp(-dividendYield * timeToMaturity) * N(D1()) -
         strikePrice * std::exp(-riskFreeInterestRate * timeToMaturity) *
             N(D2());
}

// put equation
double BlackScholes::putPrice() {
  // formula is put=Ke^(-rt)N(-D2)-Se^-(qt)N(-D1)
  return strikePrice * std::exp(-riskFreeInterestRate * timeToMaturity) *
             N(-D2()) -
         spotPrice * std::exp(-dividendYield * timeToMaturity) * N(-D1());
}

// Greeks
double NDash(double x) {
  return (1 / (sqrt(2 * M_PI))) * (std::exp(-(std::pow(x, 2) / 2)));
}

double BlackScholes::deltaCall() {
  return std::exp(-dividendYield * timeToMaturity) * N(D1());
}

double BlackScholes::deltaPut() {
  return std::exp(-dividendYield * timeToMaturity) * (N(D1()) - 1);
}

double BlackScholes::gamma() {
  return (NDash(D1()) * std::exp(-dividendYield * timeToMaturity)) /
         (spotPrice * volatilityOfUnderlyingAsset * std::sqrt(timeToMaturity));
}
// this is theta per year
double BlackScholes::thetaCall() {
  return -(spotPrice * NDash(D1()) * volatilityOfUnderlyingAsset *
           std::exp(-dividendYield * timeToMaturity)) /
             (2 * std::sqrt(timeToMaturity)) +
         dividendYield * spotPrice * N(D1()) *
             std::exp(-dividendYield * timeToMaturity) -
         riskFreeInterestRate * strikePrice *
             std::exp(-riskFreeInterestRate * timeToMaturity) * N(D2());
}
double BlackScholes::thetaPut() {
  return -(spotPrice * NDash(D1()) * volatilityOfUnderlyingAsset *
           std::exp(-dividendYield * timeToMaturity)) /
             (2 * std::sqrt(timeToMaturity)) -
         dividendYield * spotPrice * N(-D1()) *
             std::exp(-dividendYield * timeToMaturity) +
         riskFreeInterestRate * strikePrice * N(-D2()) *
             std::exp(-riskFreeInterestRate * timeToMaturity);
}

// this is verga per %
double BlackScholes::vega() {
  return (spotPrice * std::sqrt(timeToMaturity) * NDash(D1()) *
          std::exp(-dividendYield * timeToMaturity)) /
         100;
}

// this is rho per %
double BlackScholes::rhoCall() {
  return (strikePrice * timeToMaturity *
          std::exp(-riskFreeInterestRate * timeToMaturity) * N(D2())) /
         100;
}
double BlackScholes::rhoPut() {
  return -(strikePrice * timeToMaturity *
           std::exp(-riskFreeInterestRate * timeToMaturity) * N(-D2())) /
         100;
}