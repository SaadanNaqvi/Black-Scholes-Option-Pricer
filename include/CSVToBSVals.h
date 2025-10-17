#ifndef BS_VAR_CALC_H
#define BS_VAR_CALC_H

#include <cmath>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "CSVData.h"

// Choose which field from CSVData to use for S_t
enum class SpotField { Open, High, Low, Close, AdjClose, Current };

struct BSInputs {
  double spotPrice;
  double strikePrice;
  double timeToMaturity;        //  years
  double riskFreeInterestRate;  // in decimal for percentage
  double volatility;            // in decimal for percentage
  double dividendYield;         // in decimal for percentage
};

class BSVarCalc {

 private:
  // days between expiry and start
  static int daysBetween(const std::string& ymd1, const std::string& ymd2);

  // turns date into seperate integers to find the difference between dates
  static void YMDtoInt(const std::string& ymd, int& y, int& m, int& d);

  // convert date to num to easier subtract
  static int ymdToSerialDay(int y, int m, int d);

 public:
    // get all dates before trade date for volatility
  static std::vector<std::string> datesUpTo(CSVData& px,
                                            const std::string& uptoDate,
                                            int countNeeded);
  // spot from csv
  static double spotPrice(CSVData& px, const std::string& date);
  // year/365
  static double timeToMaturity(const std::string& valuationDate,
                               const std::string& expiryDate);
  // volatility
  static double historicalVol(CSVData& px, const std::string& startDate, const std::string& endDate);
  // dividends
  static double dividendYieldFixed(double annualDivPerShare, double price);
};

#endif