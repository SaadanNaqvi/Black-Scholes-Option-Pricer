#include "User.h"
#include "Portfolio.h"
#include "Order.h"
#include "Position.h"
#include "Stocks.h"
#include <iostream>
using namespace std;

int main() {
    cout << "=== Black-Scholes Option Pricer - 3-Layer Architecture Demo ===" << endl;
    
    // Layer 1 (Data Layer): Create some stocks
    cout << "\n1. Creating Stocks (Layer 1 - Data Layer):" << endl;
    Stocks* appleStock = new Stocks("AAPL", "Apple Inc.", 150.0, 148.0, 149.0, 152.0, 145.0, 1000000);
    Stocks* googleStock = new Stocks("GOOGL", "Alphabet Inc.", 2800.0, 2750.0, 2775.0, 2850.0, 2740.0, 500000);
    
    appleStock->printStockInfo();
    cout << endl;
    googleStock->printStockInfo();
    
    // Layer 2 (Business Logic Layer): Create positions and orders
    cout << "\n\n2. Creating Positions and Orders (Layer 2 - Business Logic Layer):" << endl;
    
    // Create a position for Apple stock
    Position* applePosition = new Position(appleStock, "POS001", LONG, 100, 148.0);
    applePosition->printPositionDetails();
    
    // Create an order for Google stock
    Order* googleOrder = new Order("ORD001", googleStock, BUY, LIMIT, 50, 2750.0, "USER001");
    cout << "\nOrder Details:" << endl;
    googleOrder->printOrderDetails();
    
    // Layer 3 (Interface Layer): Create portfolio and user
    cout << "\n\n3. Creating Portfolio and User (Layer 3 - Interface Layer):" << endl;
    
    // Create a portfolio
    Portfolio* mainPortfolio = new Portfolio("PORTFOLIO001", "USER001", 50000.0);
    
    // Add the Apple position to the portfolio
    mainPortfolio->addPosition(applePosition);
    
    // Add the Google order to the portfolio
    mainPortfolio->addOrder(googleOrder);
    
    // Add stocks to watchlist
    mainPortfolio->addToWatchlist(appleStock);
    mainPortfolio->addToWatchlist(googleStock);
    
    // Create a user
    User* trader = new User("USER001", "trader123", "trader@example.com", "John", "Doe", MODERATE, 60.0);
    
    // Add portfolio to user
    trader->addPortfolio(mainPortfolio);
    
    // Display user and portfolio information
    cout << "\nUser Profile:" << endl;
    trader->printUserProfile();
    
    cout << "\nPortfolio Details:" << endl;
    mainPortfolio->printPortfolioSummary();
    mainPortfolio->printPositions();
    mainPortfolio->printOrders();
    mainPortfolio->printWatchlist();
    
    // Demonstrate the connections between layers
    cout << "\n\n4. Demonstrating Layer Connections:" << endl;
    cout << "User -> Portfolio -> Position -> Stock:" << endl;
    cout << "User " << trader->getFullName() << " owns portfolio " 
         << trader->getPrimaryPortfolio()->getPortfolioId() << endl;
    
    auto positions = trader->getPrimaryPortfolio()->getPositions();
    for (const auto& pair : positions) {
        Position* pos = pair.second;
        cout << "  └─ Position in " << pos->getStock()->getTicker() 
             << " (" << pos->getStock()->getCompanyName() << "): " 
             << pos->getQuantity() << " shares" << endl;
    }
    
    cout << "\nUser -> Portfolio -> Order -> Stock:" << endl;
    auto orders = trader->getPrimaryPortfolio()->getOrders();
    for (Order* order : orders) {
        cout << "  └─ " << order->getOrderTypeString() << " order for " 
             << order->getStock()->getTicker() 
             << " (" << order->getStock()->getCompanyName() << "): " 
             << order->getQuantity() << " shares @ $" << order->getPrice() << endl;
    }
    
    // Simulate order execution
    cout << "\n\n5. Simulating Order Execution:" << endl;
    if (googleOrder->canExecute()) {
        cout << "Google order can be executed!" << endl;
        googleOrder->executeOrder(2750.0, 50);
        cout << "Order executed. New status: " << googleOrder->getOrderStatusString() << endl;
        
        // Create position from filled order
        if (googleOrder->isCompletelyFilled()) {
            Position* newPosition = googleOrder->createPositionFromOrder();
            if (newPosition != nullptr) {
                mainPortfolio->addPosition(newPosition);
                cout << "New position created from filled order!" << endl;
            }
        }
    }
    
    // Display updated portfolio
    cout << "\nUpdated Portfolio:" << endl;
    mainPortfolio->printPositions();
    
    cout << "\n=== Demo Complete ===" << endl;
    cout << "Architecture Summary:" << endl;
    cout << "Layer 1 (Data): Stocks class handles stock data and calculations" << endl;
    cout << "Layer 2 (Business): Position and Order classes manage trading logic" << endl;
    cout << "Layer 3 (Interface): Portfolio and User classes handle user interactions" << endl;
    cout << "Each layer connects to the layers below it, creating a clean hierarchy!" << endl;
    
    // Clean up (the User destructor will handle most cleanup)
    delete trader; // This will also delete the portfolio and its contained objects
    delete appleStock;
    delete googleStock;
    
    return 0;
}
