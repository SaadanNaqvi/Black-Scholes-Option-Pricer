#ifndef USER_H
#define USER_H

#include <ctime>
#include <fstream>
#include <string>
#include <vector>

#include "Order.h"
#include "Portfolio.h"
#include "Stocks.h"
using namespace std;

enum RiskLevel { CONSERVATIVE, MODERATE, AGGRESSIVE, VERY_AGGRESSIVE };

class User {
 private:
  string userId;
  string userName;
  string firstName;
  string password;
  string lastName;
  Portfolio portfolio;
  vector<Portfolio*> portfolios;  // User can have multiple portfolios
  RiskLevel riskLevel;
  double riskTolerance;  // Numerical risk tolerance (0-100)
  time_t registrationDate;
  time_t lastLoginDate;
  bool isActive;
  double totalInvestmentCapital;

 public:
  // Constructors
  User();
  User(string userId, string userName, string firstName, string lastName,
       string password);

  // Getters
  string getUserId() const;
  string getUserName() const;
  string getFirstName() const;
  string getLastName() const;
  string getFullName() const;
  string getPassword() const { return password; }
  vector<Portfolio*> getPortfolios() const;
  RiskLevel getRiskLevel() const;
  double getRiskTolerance() const;
  time_t getRegistrationDate() const;
  time_t getLastLoginDate() const;
  bool getIsActive() const;
  double getTotalInvestmentCapital() const;

  // Setters
  void setUserId(string userId);
  void setUserName(string userName);
  void setFirstName(string firstName);
  void setLastName(string lastName);
  void setRiskLevel(RiskLevel riskLevel);
  void setRiskTolerance(double riskTolerance);
  void setIsActive(bool isActive);

  // Portfolio management
  void addPortfolio(Portfolio* portfolio);
  void removePortfolio(string portfolioId);
  Portfolio* getPortfolio(string portfolioId) const;
  Portfolio* getPrimaryPortfolio() const;
  bool hasPortfolio(string portfolioId) const;
  void addStock(string ticker, long long quantity);
  void changePnl(double change);

  // User account management
  void updateLastLogin();
  void activateAccount();
  void deactivateAccount();

  // Financial calculations
  double calculateTotalPortfolioValue() const;
  double calculateTotalCashBalance() const;
  double calculateTotalRealisedPnL() const;
  double calculateOverallReturn() const;

  // Risk management
  string getRiskLevelString() const;
  bool isRiskToleranceExceeded(double portfolioRisk) const;
  double getRecommendedMaxPositionSize(double portfolioValue) const;

  // Utility functions
  void printUserProfile() const;
  void printPortfolioSummary() const;
  vector<Order*> getAllOrders() const;

  void CSVWrite();
  void CSVRead();
  // Destructor;
  bool canBuy(float cost);
  bool canSell(string ticker, float stock);

  ~User();
};

#endif
