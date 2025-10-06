#include "../../include/CSV_READER.h"

int main() {
  CSV_DATA a = CSV_DATA("YELP");
  cout << a.closePrice("2013-05-10") << endl;
  return 0;
}