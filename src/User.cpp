#include "User.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

// Default constructor
User::User() {
    userId = "";
    userName = "";
    firstName = "";
    lastName = "";
    riskLevel = MODERATE;
    riskTolerance = 50.0;
    registrationDate = time(0);
    lastLoginDate = 0;
    isActive = true;
    totalInvestmentCapital = 0.0;
}

// Basic constructor
User::User(string userId, string userName, string firstName, string lastName) {
    this->userId = userId;
    this->userName = userName;
    this->firstName = firstName;
    this->lastName = lastName;
    this->riskLevel = MODERATE;
    this->riskTolerance = 50.0;
    this->registrationDate = time(0);
    this->lastLoginDate = 0;
    this->isActive = true;
    this->totalInvestmentCapital = 0.0;
}

// Full constructor
User::User(string userId, string userName, string firstName, string lastName, 
           RiskLevel riskLevel, double riskTolerance) {
    this->userId = userId;
    this->userName = userName;
    this->firstName = firstName;
    this->lastName = lastName;
    this->riskLevel = riskLevel;
    this->riskTolerance = riskTolerance;
    this->registrationDate = time(0);
    this->lastLoginDate = 0;
    this->isActive = true;
    this->totalInvestmentCapital = 0.0;
}

// Getters
string User::getUserId() const {
    return userId;
}

string User::getUserName() const {
    return userName;
}

string User::getFirstName() const {
    return firstName;
}

string User::getLastName() const {
    return lastName;
}

string User::getFullName() const {
    return firstName + " " + lastName;
}

vector<Portfolio*> User::getPortfolios() const {
    return portfolios;
}

RiskLevel User::getRiskLevel() const {
    return riskLevel;
}

double User::getRiskTolerance() const {
    return riskTolerance;
}

time_t User::getRegistrationDate() const {
    return registrationDate;
}

time_t User::getLastLoginDate() const {
    return lastLoginDate;
}

bool User::getIsActive() const {
    return isActive;
}

double User::getTotalInvestmentCapital() const {
    return totalInvestmentCapital;
}

// Setters
void User::setUserId(string userId) {
    this->userId = userId;
}

void User::setUserName(string userName) {
    this->userName = userName;
}

void User::setFirstName(string firstName) {
    this->firstName = firstName;
}

void User::setLastName(string lastName) {
    this->lastName = lastName;
}

void User::setRiskLevel(RiskLevel riskLevel) {
    this->riskLevel = riskLevel;
}

void User::setRiskTolerance(double riskTolerance) {
    if (riskTolerance >= 0.0 && riskTolerance <= 100.0) {
        this->riskTolerance = riskTolerance;
    }
}

void User::setIsActive(bool isActive) {
    this->isActive = isActive;
}

// Portfolio management
void User::addPortfolio(Portfolio* portfolio) {
    if (portfolio == nullptr) return;
    
    // Set the owner ID to this user's ID
    portfolio->setOwnerId(userId);
    portfolios.push_back(portfolio);
    
    // Update total investment capital
    totalInvestmentCapital += portfolio->getInitialValue();
}

void User::removePortfolio(string portfolioId) {
    auto it = find_if(portfolios.begin(), portfolios.end(),
                     [&portfolioId](const Portfolio* p) {
                         return p->getPortfolioId() == portfolioId;
                     });
    
    if (it != portfolios.end()) {
        totalInvestmentCapital -= (*it)->getInitialValue();
        delete *it;
        portfolios.erase(it);
    }
}

Portfolio* User::getPortfolio(string portfolioId) const {
    for (Portfolio* portfolio : portfolios) {
        if (portfolio->getPortfolioId() == portfolioId) {
            return portfolio;
        }
    }
    return nullptr;
}

Portfolio* User::getPrimaryPortfolio() const {
    return portfolios.empty() ? nullptr : portfolios[0];
}

bool User::hasPortfolio(string portfolioId) const {
    return getPortfolio(portfolioId) != nullptr;
}

// User account management
void User::updateLastLogin() {
    lastLoginDate = time(0);
}

void User::activateAccount() {
    isActive = true;
}

void User::deactivateAccount() {
    isActive = false;
}

// Financial calculations
double User::calculateTotalPortfolioValue() const {
    double totalValue = 0.0;
    for (const Portfolio* portfolio : portfolios) {
        totalValue += portfolio->calculateTotalValue();
    }
    return totalValue;
}

double User::calculateTotalCashBalance() const {
    double totalCash = 0.0;
    for (const Portfolio* portfolio : portfolios) {
        totalCash += portfolio->getCashBalance();
    }
    return totalCash;
}

double User::calculateTotalUnrealizedPnL() const {
    double totalUnrealizedPnL = 0.0;
    for (const Portfolio* portfolio : portfolios) {
        totalUnrealizedPnL += portfolio->calculateUnrealizedPnL();
    }
    return totalUnrealizedPnL;
}

double User::calculateTotalRealizedPnL() const {
    double totalRealizedPnL = 0.0;
    for (const Portfolio* portfolio : portfolios) {
        totalRealizedPnL += portfolio->calculateRealizedPnL();
    }
    return totalRealizedPnL;
}

double User::calculateOverallReturn() const {
    if (totalInvestmentCapital == 0.0) return 0.0;
    
    double currentValue = calculateTotalPortfolioValue();
    return ((currentValue - totalInvestmentCapital) / totalInvestmentCapital) * 100.0;
}

// Risk management
string User::getRiskLevelString() const {
    switch (riskLevel) {
        case CONSERVATIVE: return "Conservative";
        case MODERATE: return "Moderate";
        case AGGRESSIVE: return "Aggressive";
        case VERY_AGGRESSIVE: return "Very Aggressive";
        default: return "Unknown";
    }
}

bool User::isRiskToleranceExceeded(double portfolioRisk) const {
    return portfolioRisk > riskTolerance;
}

double User::getRecommendedMaxPositionSize(double portfolioValue) const {
    // Recommend maximum position size based on risk level
    double maxPercentage;
    switch (riskLevel) {
        case CONSERVATIVE: maxPercentage = 5.0; break;
        case MODERATE: maxPercentage = 10.0; break;
        case AGGRESSIVE: maxPercentage = 20.0; break;
        case VERY_AGGRESSIVE: maxPercentage = 30.0; break;
        default: maxPercentage = 10.0;
    }
    
    return portfolioValue * (maxPercentage / 100.0);
}

// Utility functions
void User::printUserProfile() const {
    cout << "User Profile" << endl;
    cout << "============" << endl;
    cout << "User ID: " << userId << endl;
    cout << "Username: " << userName << endl;
    cout << "Full Name: " << getFullName() << endl;
    cout << "Risk Level: " << getRiskLevelString() << endl;
    cout << "Risk Tolerance: " << fixed << setprecision(1) << riskTolerance << "%" << endl;
    cout << "Account Status: " << (isActive ? "Active" : "Inactive") << endl;
    cout << "Number of Portfolios: " << portfolios.size() << endl;
    
    cout << fixed << setprecision(2);
    cout << "Total Investment Capital: $" << totalInvestmentCapital << endl;
    cout << "Total Portfolio Value: $" << calculateTotalPortfolioValue() << endl;
    cout << "Total Cash Balance: $" << calculateTotalCashBalance() << endl;
    cout << "Overall Return: " << calculateOverallReturn() << "%" << endl;
    cout << "Total Unrealized P&L: $" << calculateTotalUnrealizedPnL() << endl;
    
    // Print registration date
    char buffer[26];
    struct tm* tm_info = localtime(&registrationDate);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    cout << "Registration Date: " << buffer << endl;
    
    if (lastLoginDate > 0) {
        tm_info = localtime(&lastLoginDate);
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        cout << "Last Login: " << buffer << endl;
    } else {
        cout << "Last Login: Never" << endl;
    }
}

void User::printPortfolioSummary() const {
    cout << "\nPortfolios Summary for " << getFullName() << ":" << endl;
    cout << "==========================================" << endl;
    
    if (portfolios.empty()) {
        cout << "No portfolios found." << endl;
        return;
    }
    
    for (size_t i = 0; i < portfolios.size(); ++i) {
        Portfolio* portfolio = portfolios[i];
        cout << "\nPortfolio " << (i + 1) << " (" << portfolio->getPortfolioId() << "):" << endl;
        cout << "Cash Balance: $" << fixed << setprecision(2) << portfolio->getCashBalance() << endl;
        cout << "Total Value: $" << portfolio->calculateTotalValue() << endl;
        cout << "Total Return: " << portfolio->calculateTotalReturn() << "%" << endl;
        cout << "Positions: " << portfolio->getPositions().size() << endl;
        cout << "Orders: " << portfolio->getOrders().size() << endl;
    }
}

vector<Position*> User::getAllPositions() const {
    vector<Position*> allPositions;
    for (const Portfolio* portfolio : portfolios) {
        auto positions = portfolio->getPositions();
        for (const auto& pair : positions) {
            allPositions.push_back(pair.second);
        }
    }
    return allPositions;
}

vector<Order*> User::getAllOrders() const {
    vector<Order*> allOrders;
    for (const Portfolio* portfolio : portfolios) {
        auto orders = portfolio->getOrders();
        allOrders.insert(allOrders.end(), orders.begin(), orders.end());
    }
    return allOrders;
}

// Destructor
User::~User() {
    // Clean up portfolios
    for (Portfolio* portfolio : portfolios) {
        delete portfolio;
    }
    portfolios.clear();
}

