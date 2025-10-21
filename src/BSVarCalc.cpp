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
double BSVarCalc::historicalVol(CSVData& px, const std::string& startDate, const std::string& endDate) {
    std::vector<std::string> allDates = px.getDates();
    std::vector<std::string> filteredDates;

    for (const auto& d : allDates) {
        if (d >= startDate && d <= endDate)
            filteredDates.push_back(d);
    }

    if (filteredDates.size() < 2)
        throw std::runtime_error("Not enough data to calculate volatility.");

    std::vector<double> logReturn;
    for (size_t i = 1; i < filteredDates.size(); ++i) {
        double p0 = px.closePrice(filteredDates[i - 1]);
        double p1 = px.closePrice(filteredDates[i]);
        logReturn.push_back(std::log(p1 / p0));
    }

    double mean = std::accumulate(logReturn.begin(), logReturn.end(), 0.0) / logReturn.size();
    double variance = 0.0;
    for (double r : logReturn) variance += (r - mean) * (r - mean);

    double stdDevDaily = std::sqrt(variance / (logReturn.size() - 1));
    return stdDevDaily * std::sqrt(252); // annualized volatility
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
    if (allDates.empty()) throw std::runtime_error("No dates in CSV.");

    // Ensure ascending order once (or guarantee this in CSVData)
    if (!std::is_sorted(allDates.begin(), allDates.end())) {
        std::sort(allDates.begin(), allDates.end());
    }

    // First element strictly greater than uptoDate
    auto itEnd = std::upper_bound(allDates.begin(), allDates.end(), uptoDate);
    if (itEnd == allDates.begin()) {
        throw std::runtime_error("No data on or before uptoDate.");
    }

    // Clamp window length to what exists
    const auto avail = static_cast<int>(std::distance(allDates.begin(), itEnd));
    const int take = std::max(0, std::min(countNeeded, avail));
    auto itBegin = itEnd - take;

    return std::vector<std::string>(itBegin, itEnd);
}
