#ifndef BSUNITTEST_H
#define BSUNITTEST_H
#include <cmath>
#include <iostream>

#include "Greeks.h"
#include "blackScholes.h"


class UnitTestBS {
private:
int passes_=0;
int fails_=0;
void testEU1();
void testEuExtreme();
void testGreeks();

public:
int runBSTest();
};


#endif