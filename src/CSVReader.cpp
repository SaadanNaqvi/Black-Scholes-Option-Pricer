#include <bits/stdc++.h>

#include "../include/CSV_READER.h"

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

CSV_DATA::CSV_DATA() {}

CSV_DATA::CSV_DATA(string ticker) {
  // Set up filestream.
  fstream fin;
  this->ticker = ticker;

  // Open the CSV.
  map<string, double> a;
  fin.open(ticker + ".csv", ios::in);
  cout << ticker + ".csv" << endl;
  if (!fin.is_open()) {
    // ADD THE INVALID TICKER MESSAGE LATER.

    cout << "Invalid message" << endl;
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
