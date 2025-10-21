#include "../include/CSVData.h"

#include <bits/stdc++.h>

// GEt's the header.
vector<string> functionReader(const stringstream& s) {
  // Vector for the information.
  vector<string> row;
  // Set up the data string.
  string data;
  stringstream ss(s.str());
  while (getline(ss, data, ',')) {
    row.push_back(data);
  }
  return row;
}

CSVData::CSVData() {}

// input ticker () : type
CSVData::CSVData(string ticker) {
  fstream fin;
  this->ticker = ticker;

  map<string, double> a;
  fin.open("assets/stocksData/" + ticker + ".csv", ios::in);
  cout << ticker + ".csv" << endl;
  if (!fin.is_open()) {
    // ADD THE INVALID TICKER MESSAGE LATER.

    cout << "Invalid Pathway" << endl;
    return;
  }
  string stockDetails, check, price, date, category;
  vector<string> header;
  map<string, vector<double>> allData;
  getline(fin, category);
  stringstream s(category);

  header = functionReader(s);
  while (getline(fin, stockDetails)) {
    stringstream ss(stockDetails);
    string date;
    getline(ss, date, ',');

    while (getline(ss, price, ',')) {
      allData[date].push_back(stod(price));
    }
  }
  this->header = header;
  this->allData = allData;
  colSize = header.size();
}

map<string, vector<double>> CSVData::getAllData() { return this->allData; }
double CSVData::openPrice(string date) { return allData[date][0]; }
double CSVData::highPrice(string date) { return allData[date][1]; }
double CSVData::lowPrice(string date) { return allData[date][2]; }
double CSVData::closePrice(string date) { return allData[date][3]; }
double CSVData::adjPrice(string date) { return allData[date][4]; }
double CSVData::currentPrice(string date) { return allData[date][5]; }
long long CSVData::volume(string date) { return allData[date][6]; }
vector<string> CSVData::getHeader() { return header; }
vector<string> CSVData::getDates() {
  vector<string> dates;
  for (auto [x, y] : allData) {
    dates.push_back(x);
  }
  return dates;
}

string CSVData::getTicker() { return ticker; }
