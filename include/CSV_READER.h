#ifndef CSV_DATA_H
#define CSV_DATA_H
#include <bits/stdc++.h>

#include <string>
using namespace std;
class CSV_DATA {
 private:
  map<string, vector<double>> allData;

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
  map<string, vector<double>> get_allData();
};

#endif