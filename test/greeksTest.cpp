//compile with g++ src/BlackScholesEquation.cpp src/Greeks.cpp "test/greeksTest.cpp" -Iinclude -o gtest
#include <iostream>
#include "Greeks.h"
#include "blackScholes.h"
#include "Put.h"
#include "Call.h"
int main() {
  // same parameters you used for price test
  MartketParams var{100, 90, 0.5, 0.04, 0.05, 0.015};
  MarketParams var2{50, 2, 5, 0.1, 5, 15};
  std::cout << "Delta (Call): " << Greeks::deltaCall(var) << std::endl;
  std::cout << "Delta (Put): " << Greeks::deltaPut(var) << std::endl;
  std::cout << "Gamma: " << Greeks::gamma(var) << std::endl;
  std::cout << "Theta (Call): " << Greeks::thetaCall(var) << std::endl;
  std::cout << "Theta (Put): " << Greeks::thetaPut(var) << std::endl;
  std::cout << "Vega: " << Greeks::vega(var) << std::endl;
  std::cout << "Rho (Call): " << Greeks::rhoCall(var) << std::endl;
  std::cout << "Rho (Put): " << Greeks::rhoPut(var) << std::endl;

//var2 stuff
  std::cout << "Delta (Call): " << Greeks::deltaCall(var2) << std::endl;
  std::cout << "Delta (Put): " << Greeks::deltaPut(var2) << std::endl;
  std::cout << "Gamma: " << Greeks::gamma(var2) << std::endl;
  std::cout << "Theta (Call): " << Greeks::thetaCall(var2) << std::endl;
  std::cout << "Theta (Put): " << Greeks::thetaPut(var2) << std::endl;
  std::cout << "Vega: " << Greeks::vega(var2) << std::endl;
  std::cout << "Rho (Call): " << Greeks::rhoCall(var2) << std::endl;
  std::cout << "Rho (Put): " << Greeks::rhoPut(var2) << std::endl;

  return 0;
}