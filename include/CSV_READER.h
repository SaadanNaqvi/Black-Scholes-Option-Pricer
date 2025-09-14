#ifndef CSV_DATA_H
#define CSV_DATA_H
#include <string>
#include <vector>
class CSV_DATA {
 private:
  vector<vector<double, double>> allData;

 protected:
 public:
  CSV_DATA();
  CSV_DATA(string name);
  std::string get_Name();
  std::time_t get_Date();
  double get_currentPrice();
  double get_openPrice();
  double get_closePrice();
  double get_highPrice();
  double get_lowPrice();
  long long get_Volume();
  vector < vector<double, double> get_allData();
};

#endif