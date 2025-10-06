#ifndef BLACK_SCHOLES_H
#define BLACK_SCHOLES_H

#include <cmath>
using namespace std;

class BlackScholes {
 private:
  // the required variables for the black scholes equation
  double spotPrice;
  double strikePrice;
  double timeToMaturity;
  double riskFreeInterestRate;
  double volatilityOfUnderlyingAsset;
  double dividendYield;

  //Greeks
  double delta;
  double gamma;
  double vega;
  double theta;
  double rho;


 public:
  // No deafult constructor cos inputs should be checked before being passed,
  // ensuring validity
  BlackScholes(double spotPrice, double strikePrice, double timeToMaturity,
               double riskFreeInterestRate, double volatilityOfUnderlyingAsset,
               double dividendYield);
  //Black Schole equation functions
  double D1();
  double D2();
  double callPrice();
  double putPrice();

  //Greeks functions
  double deltaCall();
  double deltaPut();
  double gamma();
  double thetaCall();
  double thetaPut();
  double vega();
  double rhoCall();
  double rhoPut();

};

#endif