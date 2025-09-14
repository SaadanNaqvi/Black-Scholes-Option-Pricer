#include "User.h"
#include <iostream>

using namespace std;

User::User(){
    name = "";
    userID = 0;
    userPortfolio = nullptr;
    riskTolerance = 0.0;
}

User::User(string name, int userID, Portfolio* userPortfolio, double riskTolerance){
    this->name = name;
    this->userID = userID;
    this->userPortfolio = userPortfolio;
    this->riskTolerance = riskTolerance;
}


// Getters
string User::getName(){
    return this->name;
}

int User::getUserID(){
    return this->userID;
}

double User::getRiskTolerance(){
    return this->riskTolerance;
}

Portfolio* User::getUserPortfolio(){
    return this->userPortfolio;
}

// Setters
void User::setName(string name){
    this->name = name;
}

void User::setUserID(int userID){
    this->userID = userID;
}

void User::setRiskTolerance(double riskTolerance){
    this->riskTolerance = riskTolerance;
}

void User::setUserPortfolio(Portfolio* userPortfolio){
    this->userPortfolio = userPortfolio;
}

User::~User(){
    delete this; 
}

