#include "User.h"
#include "Portfolio.h" 
#include "Position.h"
#include "Stocks.h"
#include <iostream>
#include <iomanip>
using namespace std;

int main() {
    cout << "=== 3-Layer Inheritance Hierarchy Demo ===" << endl;
    cout << "Stocks (Layer 1) -> Position (Layer 2) -> Portfolio (Layer 3)" << endl;
    cout << "=============================================================" << endl;
    
    // Demonstrate Layer 1: Base class (Stocks)
    cout << "\n1. Layer 1 - Base Class (Stocks):" << endl;
    Stocks* appleStock = new Stocks("AAPL", "Apple Inc.", 150.0, 148.0, 149.0, 152.0, 145.0, 1000000);
    cout << "Created: ";
    appleStock->displayInfo();
    cout << "Class Type: " << appleStock->getClassType() << endl;
    cout << "Value: $" << fixed << setprecision(2) << appleStock->calculateValue() << endl;
    cout << "Risk: " << appleStock->calculateRisk() << endl;
    
    // Demonstrate Layer 2: Middle class (Position inherits from Stocks)
    cout << "\n2. Layer 2 - Middle Class (Position inherits from Stocks):" << endl;
    Position* googlePosition = new Position("GOOGL", "Alphabet Inc.", 2800.0, LONG, 50, 2750.0);
    cout << "Created: ";
    googlePosition->displayInfo();
    cout << "Class Type: " << googlePosition->getClassType() << endl;
    cout << "Value: $" << fixed << setprecision(2) << googlePosition->calculateValue() << endl;
    cout << "Risk: " << googlePosition->calculateRisk() << endl;
    cout << "Position Details - Quantity: " << googlePosition->getQuantity() 
         << ", Entry Price: $" << googlePosition->getEntryPrice() << endl;
    
    // Demonstrate Layer 3: Final class (Portfolio inherits from Position)
    cout << "\n3. Layer 3 - Final Class (Portfolio inherits from Position):" << endl;
    Portfolio* mainPortfolio = new Portfolio("PORTFOLIO_001", "USER_001", "MSFT", "Microsoft Corp.", 
                                           320.0, LONG, 100, 315.0, 50000.0);
    cout << "Created: ";
    mainPortfolio->displayInfo();
    cout << "Class Type: " << mainPortfolio->getClassType() << endl;
    cout << "Value: $" << fixed << setprecision(2) << mainPortfolio->calculateValue() << endl;
    cout << "Risk: " << mainPortfolio->calculateRisk() << endl;
    cout << "Portfolio Details - Cash: $" << mainPortfolio->getCashBalance() 
         << ", Owner: " << mainPortfolio->getOwnerId() << endl;
    
    // Demonstrate polymorphism - treat all as base class
    cout << "\n4. Polymorphism Demonstration:" << endl;
    cout << "Treating all objects as base class (Stocks*):" << endl;
    
    vector<Stocks*> instruments;
    instruments.push_back(appleStock);
    instruments.push_back(googlePosition);
    instruments.push_back(mainPortfolio);
    
    for (size_t i = 0; i < instruments.size(); i++) {
        cout << "Instrument " << (i + 1) << ": ";
        instruments[i]->displayInfo();  // Virtual method calls appropriate override
        cout << "  - Virtual calculateValue(): $" << instruments[i]->calculateValue() << endl;
        cout << "  - Virtual getClassType(): " << instruments[i]->getClassType() << endl;
    }
    
    // Demonstrate virtual method overriding by updating prices
    cout << "\n5. Virtual Method Override Demonstration:" << endl;
    cout << "Updating current price to $160 for all instruments..." << endl;
    
    for (size_t i = 0; i < instruments.size(); i++) {
        cout << "Before: " << instruments[i]->getClassType() << " value = $" 
             << instruments[i]->calculateValue() << endl;
        
        instruments[i]->updateCurrentPrice(160.0);  // Virtual method
        
        cout << "After: " << instruments[i]->getClassType() << " value = $" 
             << instruments[i]->calculateValue() << endl;
        cout << endl;
    }
    
    // Show inheritance chain access
    cout << "6. Inheritance Chain Access:" << endl;
    cout << "Portfolio object accessing methods from all layers:" << endl;
    cout << "  - From Stocks (Layer 1): Ticker = " << mainPortfolio->getTicker() << endl;
    cout << "  - From Position (Layer 2): Quantity = " << mainPortfolio->getQuantity() << endl;
    cout << "  - From Portfolio (Layer 3): Owner = " << mainPortfolio->getOwnerId() << endl;
    
    // Create additional positions and add them to portfolio
    cout << "\n7. Portfolio Management:" << endl;
    Position* teslaPosition = new Position("TSLA", "Tesla Inc.", 250.0, LONG, 20, 240.0);
    mainPortfolio->addPosition(teslaPosition);
    
    cout << "Added Tesla position to portfolio." << endl;
    cout << "Portfolio now contains " << mainPortfolio->getPositions().size() << " positions." << endl;
    cout << "Updated portfolio value: $" << mainPortfolio->calculateValue() << endl;
    
    // Create a User to show how it connects to the hierarchy
    cout << "\n8. User Class Integration:" << endl;
    User* trader = new User("USER_001", "john_trader", "john@example.com", "John", "Doe", MODERATE, 60.0);
    
    // Create a separate portfolio for the user (to avoid double-deletion)
    Portfolio* userPortfolio = new Portfolio("USER_PORTFOLIO", "USER_001", 100000.0);
    trader->addPortfolio(userPortfolio);
    
    cout << "User " << trader->getFullName() << " owns portfolio: " 
         << trader->getPrimaryPortfolio()->getPortfolioId() << endl;
    cout << "Portfolio value through user: $" 
         << trader->calculateTotalPortfolioValue() << endl;
    
    // Summary
    cout << "\n9. Inheritance Hierarchy Summary:" << endl;
    cout << "==================================" << endl;
    cout << "✓ Stocks (Base Layer 1): Financial instrument with price data" << endl;
    cout << "✓ Position (Middle Layer 2): Inherits stock data + adds position management" << endl;  
    cout << "✓ Portfolio (Final Layer 3): Inherits position data + adds portfolio management" << endl;
    cout << "✓ User (Manager): Contains multiple portfolios and manages user data" << endl;
    cout << "\nAll virtual methods (calculateValue, calculateRisk, displayInfo, getClassType)" << endl;
    cout << "are properly overridden at each layer to provide specialized behavior!" << endl;
    cout << "\nThis demonstrates a true 3-layer inheritance hierarchy where:" << endl;
    cout << "- Each child class inherits ALL functionality from its parent" << endl;
    cout << "- Each child class adds its own specialized functionality" << endl;
    cout << "- Polymorphism allows treating all objects as the base class" << endl;
    cout << "- Virtual method overriding provides specialized behavior at each layer" << endl;
    
    // Clean up 
    delete appleStock;
    delete googlePosition;
    delete mainPortfolio; // This will also clean up teslaPosition
    delete trader;        // This will clean up userPortfolio
    
    return 0;
}
