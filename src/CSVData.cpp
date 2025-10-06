#include <bits/stdc++.h>

#include "../include/CSVData.h"

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

CSVData::CSVData(string ticker) {
  // Set up filestream.
  fstream fin;
  this->ticker = ticker;

  // Open the CSV.
  map<string, double> a;
  fin.open(ticker + ".csv", ios::in);
  cout << ticker + ".csv" << endl;
  if (!fin.is_open()) {
    // ADD THE INVALID TICKER MESSAGE LATER.

    cout << "Invalid Pathway" << endl;
    return;
  }
  // Set up the strings.
  string stockDetails, check, price, date, category;
  // Set up the header string
  vector<string> header;
  // Set up the data vector
  map<string, vector<double>> allData;
  // Get's the first line.
  getline(fin, category);
  stringstream s(category);

  header = functionReader(s);
  // Loop for all data sets.
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
