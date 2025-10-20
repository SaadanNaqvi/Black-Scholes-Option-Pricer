// compile with g++ src/BlackScholesEquation.cpp "test/CSV tests/bstest.cpp" -Iinclude -o bstest

// -Iinclude -o bstest
#include <iostream>

#include "Greeks.h"
#include "blackScholes.h"

int main() {
  MarketParams var{100, 90, 0.5, 0.04, 0, 0.3};

  EuropeanCall test1(var);
  EuropeanPut test2(var);
  std::cout << test1.price() << std::endl;
  std::cout << test2.price() << std::endl;

  AmericanCall test3(var);
  AmericanPut test4(var);
  std::cout << test3.price() << std::endl;
  std::cout << test4.price() << std::endl;

  MarketParams var2{10, 5, 0.3, 0.1, 0.2, 0.5};
  EuropeanCall test5(var2);
  EuropeanPut test6(var2);
  std::cout << test5.price() << std::endl;
  std::cout << test6.price() << std::endl;

  AmericanCall test7(var2);
  AmericanPut test8(var2);
  std::cout << test7.price() << std::endl;
  std::cout << test8.price() << std::endl;
  
  MarketParams var3{44444, 54111, 3, 0.3, 0.3, 0.4};
  EuropeanCall test10(var3);
  EuropeanPut test11(var3);
  std::cout << test10.price() << std::endl;
  std::cout << test11.price() << std::endl;

  AmericanCall test12(var3);
  AmericanPut test13(var3);
  std::cout << test12.price() << std::endl;
  std::cout << test13.price() << std::endl;



  return 0;
}