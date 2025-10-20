#include "BSUnitTest.h"

#include <cmath>
#include <iostream>

#include "Greeks.h"
#include "blackScholes.h"

#define TestAnswer(testName, condition)                \
  do {                                                 \
    if (!(condition)) {                                \
      std::cout << "Failed " << testName << std::endl; \
      fails++;                                         \
    } else {                                           \
      std::cout << "Passed " << testName << std::endl; \
      passes++;                                        \
    }                                                  \
  } while (0)

// used to check if numbers are within tolarance as double /decimals might not
// be exact same as theoretical.
inline void TestInTol(const char* testName, double actual, double expected,
                      double tolarance, int& passes, int& fails) {
  if (abs(actual - expected) > tolarance) {
    std::cout << "Failed " << testName << " got a value of " << actual
              << " while the expected was " << expected
              << " within a tolarance of " << tolarance << std::endl;
    fails++;
  } else {
    std::cout << " Passed " << testName << std::endl;
    passes++;
  }
}

// running tests
int UnitTestBS::runBSTest() {
  passes_ = 0;
  fails_ = 0;

  testEU1();
  testEuExtreme();
  testGreeks();

  std::cout << "This is how many passed " << passes_ << std::endl;
  std::cout << "This is how many fails " << fails_ << std::endl;

  if (fails_ == 0) {
    return 0;
  } else {
    return 1;
  }
}

// test with s=100, k=100, t=1, vol=0.2 RFI=0 Div=0;
void UnitTestBS::testEU1() {
  int& passes = passes_;
  int& fails = fails_;
  MarketParams mp{100, 100, 1, 0, 0, 0.2};
  EuropeanCall callTest(mp);
  EuropeanPut putTest(mp);

  double expectedValue = 7.9655;
  TestInTol("European call", callTest.price(), expectedValue, 0.01, passes,
            fails);
  TestInTol("European Put", putTest.price(), expectedValue, 0.01, passes,
            fails);
}
// test 2 extreme values (unrealistic)
void UnitTestBS::testEuExtreme() {
  int& passes = passes_;
  int& fails = fails_;
  MarketParams mp{120000, 110, 10, 0.30, 0.30, 0.60};
  EuropeanCall callTest2(mp);
  EuropeanPut putTest2(mp);

  double expectedValueCall = 5968.98;
  double expectedValuePut = 0.01;
  TestInTol("European call", callTest2.price(), expectedValueCall, 0.01, passes,
            fails);
  TestInTol("European Put", putTest2.price(), expectedValuePut, 0.01, passes,
            fails);
}

// testing greeks, testing if gamma call and put are equal (as should be)
// tesitng if if vega call=vega put as should be
// checking if delta(call)-delta(put)=e^(-qt)
void UnitTestBS::testGreeks() {
  int& passes = passes_;
  int& fails = fails_;
  MarketParams mp{105, 100, 0.6, 0.02, 0.015, 0.22};
  double gammaCall = Greeks::gamma(mp);
  double gammaPut = Greeks::gamma(mp);
  double vegaPut = Greeks::vega(mp);
  double vegaCall = Greeks::vega(mp);

  double DeltaDiff = Greeks::deltaCall(mp) - Greeks::deltaPut(mp);
  double expDeltaDiff = std::exp(-mp.dividendYield * mp.timeToMaturity);

  TestInTol("Are Gammas same? ", gammaCall, gammaPut, 0.01, passes, fails);
  TestInTol("Are Vegas same? ", vegaCall, vegaPut, 0.01, passes, fails);
  TestInTol("Is Delta correct to formula? ", DeltaDiff, expDeltaDiff, 0.01,
            passes, fails);
}

int main() {
  UnitTestBS Test;
  return Test.runBSTest();
}
// complied with g++ -std=c++17 -Iinclude -Itests src/BlackScholesEquation.cpp
// src/Greeks.cpp test/BSUnitTest.cpp -o bs_tests