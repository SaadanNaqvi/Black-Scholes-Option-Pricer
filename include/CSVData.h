#ifndef CSVDATA_H
#define CSVDATA_H
#include <bits/stdc++.h>

#include <string>
using namespace std;
class CSVData {
 private:
  map<string, vector<double>> allData;
  vector<string> header;
  string ticker;
  int colSize;
  
 protected:
 public:
  CSVData();
  CSVData(string name);
  std::string getName();
  std::time_t getDate();
  double currentPrice(string date);
  double openPrice(string date);
  double closePrice(string date);
  double highPrice(string date);
  double lowPrice(string date);
  long long volume(string date);
  double adjPrice(string date);
  map<string, vector<double>> getAllData();
  vector<string> getHeader();
  string getTicker();
  vector<string> getDates();
};

#endif