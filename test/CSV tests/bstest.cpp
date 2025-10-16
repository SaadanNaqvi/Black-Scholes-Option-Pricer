//compile with g++ src/BlackScholesEquation.cpp "test/CSV tests/bstest.cpp" -Iinclude -o bstest
#include "blackScholes.h"
#include "Greeks.h"
#include <iostream>

int main(){
Parameters var{100, 90, 0.5, 0.04, 0.05, 0.015};
EuropeanCall test1(var);
EuropeanPut test2(var);
std::cout<<test1.price()<<std::endl;
std::cout<<test2.price()<<std::endl;
    return 0;
}