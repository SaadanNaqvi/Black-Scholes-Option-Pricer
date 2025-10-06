#include <bits/stdc++.h>

#include "../include/CSVDatag.h"

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

<<<<<<< HEAD
map<string, vector<double>> CSVData::get_allData() { return this->allData; }
double CSVData::openPrice(string date) { return allData[date][0]; }
double CSVData::highPrice(string date) { return allData[date][1]; }
double CSVData::lowPrice(string date) { return allData[date][2]; }
double CSVData::closePrice(string date) { return allData[date][3]; }
double CSVData::adjPrice(string date) { return allData[date][4]; }
double CSVData::currentPrice(string date) { return allData[date][5]; }
long long CSVData::volume(string date) { return allData[date][6]; }
vector<string> CSVData::get_header() { return header; }
vector<string> CSVData::get_dates() {
=======
map<string, vector<double>> CSV_DATA::get_allData() { return this->allData; }
double CSV_DATA::openPrice(string date) { return allData[date][0]; }
double CSV_DATA::highPrice(string date) { return allData[date][1]; }
double CSV_DATA::lowPrice(string date) { return allData[date][2]; }
double CSV_DATA::closePrice(string date) { return allData[date][3]; }
double CSV_DATA::adjPrice(string date) { return allData[date][4]; }
double CSV_DATA::currentPrice(string date) { return allData[date][5]; }
long long CSV_DATA::volume(string date) { return allData[date][6]; }
vector<string> CSV_DATA::get_header() { return header; }
vector<string> CSV_DATA::get_dates() {
>>>>>>> de4b6f508c1cb35d98590efb10207743ee65fec1
  vector<string> dates;
  for (auto [x, y] : allData) {
    dates.push_back(x);
  }
  return dates;
}

<<<<<<< HEAD
string CSVData::get_ticker() { return ticker; }
=======
string CSV_DATA::get_ticker() { return ticker; }
>>>>>>> de4b6f508c1cb35d98590efb10207743ee65fec1
