#include <algorithm>
#include <numeric>

#include "CSVToBSVals.h"
// spot price dont need any calculations as used like this in BS equations
double BSVarCalc::spotPrice(CSVData& px, const std::string& date) {
  return px.closePrice(date);
}

// year/365
double BSVarCalc::timeToMaturity(const std::string& valuationDate,
                                 const std::string& expiryDate) {
  int days = daysBetween(valuationDate, expiryDate);
  return days / 365.25;
}

// finding the running total of the volatility until current date
double BSVarCalc::historicalVol(CSVData& px, const std::string& uptoDate) {
  // getting dates from csv
  std::vector<std::string> dates = datesUpTo(px, uptoDate, 252);
  // list of all log values for volatility
  std::vector<double> logReturn;

  for (int i = 1; i < (int)dates.size(); ++i) {
    double priceDay0 = px.closePrice(dates[i - 1]);
    double priceDay1 = px.closePrice(dates[i]);
    // calculating the log and storing
    logReturn.push_back(std::log(priceDay1 / priceDay0));
  }
  // avg of all logs
  double total = 0.0;
  for (int i = 0; i < (int)logReturn.size(); i++) {
    total = total + logReturn[i];
  }
  double mean = total / logReturn.size();

  // variation from mean
  double squaredDifference = 0.0;

  for (int i = 0; i < (int)logReturn.size(); i++) {
    double difference = logReturn[i] - mean;
    squaredDifference = squaredDifference + difference * difference;
  }

  // standard deviation, note: variance=sum(x-mean)^2/(n-1) as a sample not
  // population
  double standardDeviationDaily =
      std::sqrt(squaredDifference) / (logReturn.size() - 1);
  // Yearly volatility as this is what BS equations need
  // note 252 is how many actual trading days per year, check csv and u will see
  // missing dates cos not trading dates
  double yearlyVolatility = standardDeviationDaily * std::sqrt(252);

  return yearlyVolatility;
}

// dividend calculations
double BSVarCalc::dividendYieldFixed(double annualDivPerShare, double price) {
  return std::log(1 + (annualDivPerShare / price));
}

// turning YYYY-MM-DD into 3 seprate ints
void BSVarCalc::YMDtoInt(const std::string& ymd, int& y, int& m, int& d) {
  if (ymd.size() < 10) {
    throw std::runtime_error("Date must be YYYY-MM-DD");
  }
  // note for example 0,4 just means starts at position 0 and is for char long
  // stoi is string to int
  y = std::stoi(ymd.substr(0, 4));
  m = std::stoi(ymd.substr(5, 2));
  d = std::stoi(ymd.substr(8, 2));
}

// turn the date into an integer by assigning val to y m d
// idea from
// https://www.sunshine2k.de/articles/coding/datediffindays/calcdiffofdatesindates.html
int BSVarCalc::ymdToSerialDay(int y, int m, int d) {
  int monthDays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  int daysFromYears = y * 365;
  int daysFromMonths = 0;
  for (int x = 1; x < m; x++) {
    daysFromMonths = daysFromMonths + monthDays[x - 1];
  }
  int total = daysFromYears + daysFromMonths + d;
  return total;
}

// days between expiry and start
int BSVarCalc::daysBetween(const std::string& ymd1, const std::string& ymd2) {
  int y1, m1, d1, y2, m2, d2;
  YMDtoInt(ymd1, y1, m1, d1);
  YMDtoInt(ymd2, y2, m2, d2);
  return abs(ymdToSerialDay(y1, m1, d1) - ymdToSerialDay(y2, m2, d2));
}

// used to calc volatility by getting trading dates up to the date required
std::vector<std::string> BSVarCalc::datesUpTo(CSVData& px,
                                              const std::string& uptoDate,
                                              int countNeeded) {
  std::vector<std::string> allDates = px.getDates();
  std::vector<std::string> filteredDates;
  filteredDates.reserve(allDates.size());

  for (size_t i = 0; i < allDates.size(); i++) {
    if (allDates[i] <= uptoDate) {
      filteredDates.push_back(allDates[i]);
    }
  }
  return std::vector<std::string>(filteredDates.end() - countNeeded,
                                  filteredDates.end());
}
