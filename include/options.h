#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include "Stocks.h"

class Options{
    private:
        string optionType;
        Stocks stock;
    protected:
    
    public:
        Options();
        Options(string optionType, Stocks stock);
        
};

#endif