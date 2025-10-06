#ifndef CSV_DATA_H
#define CSV_DATA_H

#include <fstream>
#include <bits/stdc++.h>
using namespace std;
class CSV_DATA{
  private:
    map<string,vector<double>> allData;
    string ticker;
    vector<string> header;
    int colSize=0;
  protected:
  
  public:
    CSV_DATA();
    CSV_DATA(string ticker); //Done.
    string getName(); // Get Name
    time_t getDate();
    double getCurrentPrice();
    double getOpenPrice();
    double getClosePrice();
    double getHighPrice();
    double getLowPrice();
    long long getVolume();
    map<string, vector<double>> getAllData();
};

#endif