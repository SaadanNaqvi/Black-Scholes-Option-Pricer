#include "Stocks.h"
using namespace std;
int main() {
  Stocks a = Stocks("AAPL", "Tesla");
  for (auto [x, y] : a.priceHistory.getAllData()) {
    cout << x << " ";
    for (auto n : y) {
      cout << n << " ";
    }
    cout << endl;
  }
  return 0;
}