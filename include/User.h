#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

class User{
    protected:
        string name;
        int userID:

        Portfolio portfolio; // Users Portfolio

        double riskTolerance;
    public:
        User();
        User(string name, Portfolio portfolio);

        virtual double getBalance();

        ~User();
};

#endif