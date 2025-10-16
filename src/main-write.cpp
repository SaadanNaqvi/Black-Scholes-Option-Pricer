#include <iostream>

#include "Stocks.h"
#include "User.h"

int main() {
  User a = User();
  a.addStock("TSLA", 10032);
  a.changePnl(43.22);
  a.CSVWrite();
  return 0;
}