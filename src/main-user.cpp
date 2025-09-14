#include "User.h"
#include <iostream>


int main(){
    Portfolio* P1;
    User Saadan("Saadan", 1, P1, 0.1);

    cout << Saadan.getName() << endl;
    cout << Saadan.getUserID() << endl;
    
    return 0;
}