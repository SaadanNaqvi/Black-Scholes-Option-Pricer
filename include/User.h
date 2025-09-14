#ifndef USER_H
#define USER_H

#include <string>
using namespace std;
#include "Portfolio.h"

class User{
    private:
        string name; // User name
        int userID; 

        Portfolio* userPortfolio; // Users Portfolio

        double riskTolerance; // Users choice for risk tolerance
    public:
        User(); // Default Constructor
        User(string name, int userID, Portfolio* userPortfolio, double riskTolerance);

        // Getters
        string getName();
        int getUserID();
        double getRiskTolerance();
        Portfolio* getUserPortfolio();

        // Setters
        void setName(string name);
        void setUserID(int userID);
        void setRiskTolerance(double riskTolerance);
        void setUserPortfolio(Portfolio* userPortfolio);

        ~User();
};

#endif