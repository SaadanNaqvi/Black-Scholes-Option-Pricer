#include "Graphics.h"
#include "Graph.h"
#include "Line.h"
#include "Bar.h"
#include "Button.h"
#include "Dropdown.h"
#include "Text.h"
#include "InputElement.h"
#include "User.h"
#include "Theme.h"
#include "Stocks.h"
#include <memory>
#include <fstream>
#include <algorithm>
#include "DatePicker.h"
#include "blackScholes.h"
#include "CSVToBSVals.h"
#include <cmath>
#include <numbers>
using namespace std;

// ==== Theme Colors ====
#define BG_COLOR       CLITERAL(Color){ 10, 18, 40, 255 }
#define PANEL_COLOR    CLITERAL(Color){ 20, 32, 65, 255 }
#define TEXT_COLOR     CLITERAL(Color){ 235, 235, 245, 255 }
#define ACCENT_COLOR   CLITERAL(Color){ 50, 150, 255, 255 }
#define GRID_COLOR     CLITERAL(Color){ 60, 70, 110, 255 }

void Graphics::dashboard() {
    if (!IsWindowReady()) InitWindow(1280, 720, "Option Simulator Dashboard");
    SetTargetFPS(60);

    // === Load available tickers ===
    vector<string> tickers = {"AAPL", "AMD", "AMZN", "ATVI", "BABA", "BAC", "CRM", "CSCO", "DIS", "EA", "F",
                              "GOOG", "INTC", "JPM", "KO", "MCD", "META", "MSFT", "MTCH", "NFLX", "NVDA",
                              "PFE", "PYPL", "T", "TSLA", "TTD", "WMT", "XOM", "YELP", "ZG"};
    ifstream file("assets/stocksData");
    if (file.is_open()) {
        string ticker;
        while (file >> ticker) tickers.push_back(ticker);
    }

    // === UI Inputs ===
    Dropdown tickerSelect({90, 70, 180, 35}, tickers);
    Dropdown callPut({290, 70, 120, 35}, {"Call", "Put"});
    Dropdown optionStyle({430, 70, 180, 35}, {"European","American"});
    Text strike({630, 70, 120, 35}, "Strike");
    DatePicker startDatePicker({770, 70, 150, 35}, {});
    DatePicker endDatePicker({940, 70, 150, 35}, {});

    // === Action Buttons ===
    Button executeBtn({1110, 70, 120, 40}, "Execute");
    Button stopBtn({1110, 120, 120, 40}, "Stop⏸");
    Button saveBtn({980, 120, 120, 40}, "Save");
    Button checkBtn({880, 120, 120, 40}, "Check");

    // === Graph setup ===
    Rectangle priceGraphRect = {90, 160, 900, 380};
    Rectangle pnlGraphRect   = {90, 570, 760, 120};
    Rectangle greeksPanelRect = {870, 570, 320, 120};

    unique_ptr<Line> lineGraph;
    unique_ptr<Bar> pnlGraph;

    bool running = true;
    float simTime = 0.0f;

    static vector<string> dates;
    static vector<float> prices;
    double delta = 0.0, gamma = 0.0, vega = 0.0, theta = 0.0, optionPrice = 0.0;
    bool showBSStats = false;


    while (running && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG_COLOR);
        DrawText("Option Simulator Dashboard", 420, 15, 34, TEXT_COLOR);

        // ==== Control panel ====
        DrawRectangleRounded({70, 60, 1140, 110}, 0.05f, 0, PANEL_COLOR);

        // Update UI
        tickerSelect.update();
        callPut.update();
        optionStyle.update();
        strike.update();
        startDatePicker.update();
        endDatePicker.update();

        // Draw UI
        tickerSelect.drawBase();
        callPut.drawBase();
        optionStyle.drawBase();
        strike.draw();
        startDatePicker.drawBase();
        endDatePicker.drawBase();
        string selectedTicker = tickerSelect.getSelectedOption();
        if (!isSimulating){
            executeBtn.draw();
            checkBtn.draw();
        }
        else stopBtn.draw();
        saveBtn.draw();
        if (checkBtn.isClicked()) {
            try {
                if (selectedTicker.empty())
                    throw runtime_error("Select a stock ticker first.");
                if (dates.empty() || prices.empty())
                    throw runtime_error("No data loaded for this stock.");

                string startDate = startDatePicker.getSelectedDate();
                string endDate   = endDatePicker.getSelectedDate();
                if (startDate.empty() || endDate.empty())
                    throw runtime_error("Please select start and expiry dates.");

                double strikePrice = stod(strike.getContent());
                string optionType  = callPut.getSelectedOption();
                string style       = optionStyle.getSelectedOption();

                Stocks stock(selectedTicker);
                auto& px = stock.priceHistory;   // This is your CSVData instance

                BSVarCalc calc;

                double S = calc.spotPrice(px, startDate);

                double T = calc.timeToMaturity(startDate, endDate);

                std::vector<std::string> allDates = px.getDates();
                if (allDates.empty())
                    throw runtime_error("No dates found for this stock!");


                std::string firstDate = allDates.front();

                double sigma = calc.historicalVol(px, allDates.front(), startDate);
                {
                    vector<string> volDates = calc.datesUpTo(px, startDate, (int)allDates.size());
                    if (volDates.front() != firstDate)
                        volDates.insert(volDates.begin(), firstDate);
                }


                double q = 0.01;   // assume 1% dividend yield
                double r = 0.02;   // assume 2% risk-free rate


                MarketParams mp{S, strikePrice, T, r, q, sigma};
                EuropeanCall callOption(mp);
                EuropeanPut putOption(mp);
                if (style == "European") {
                    if (optionType == "Call") {
                        optionPrice = callOption.price();
                        delta = std::exp(-q * T) * N(callOption.D1());
                        gamma = std::exp(-q * T) * NDash(callOption.D1()) / (S * sigma * std::sqrt(T));
                        vega  = S * std::exp(-q * T) * NDash(callOption.D1()) * std::sqrt(T) / 100;
                        theta = (-S * std::exp(-q * T) * NDash(callOption.D1()) * sigma / (2 * std::sqrt(T))
                                - r * strikePrice * std::exp(-r * T) * N(callOption.D2())
                                + q * S * std::exp(-q * T) * N(callOption.D1())) / 365;
                    } 
                    else {
                        optionPrice = putOption.price();
                        delta = std::exp(-q * T) * (N(callOption.D1()) - 1);
                        gamma = std::exp(-q * T) * NDash(putOption.D1()) / (S * sigma * std::sqrt(T));
                        vega  = S * std::exp(-q * T) * NDash(putOption.D1()) * std::sqrt(T) / 100;
                        theta = (-S * std::exp(-q * T) * NDash(putOption.D1()) * sigma / (2 * std::sqrt(T))
                                + r * strikePrice * std::exp(-r * T) * N(-putOption.D2())
                                - q * S * std::exp(-q * T) * N(-putOption.D1())) / 365;
                    }
                }

                showBSStats = true;

                // === auto-load graph starting from start date ===
                auto it = std::find(dates.begin(), dates.end(), startDate);
                int startIndex = (it != dates.end()) ? std::distance(dates.begin(), it) : 0;
                lineGraph = make_unique<Line>(priceGraphRect, dates, prices);
                pnlGraph  = make_unique<Bar>(pnlGraphRect, vector<float>(prices.begin(), prices.begin() + startIndex));
                lineGraph->startAnimationFrom(startIndex);

                cout << "Computed Black-Scholes for " << selectedTicker << " from " << startDate << endl;
            }
            catch (exception &e) {
                DrawText(e.what(), 100, 155, 20, RED);
                cerr << "error" << e.what() << endl;
            }
        }



        if (saveBtn.isClicked() && getCurrentUser() != nullptr) {
            try {
                getCurrentUser()->CSVWrite();
                DrawText("Account saved successfully!", 950, 200, 18, GREEN);
                cout << "User data saved to CSV for " << getCurrentUser()->getUserName() << endl;
            } catch (exception &e) {
                DrawText("Failed to save account!", 950, 200, 18, RED);
                cerr << "Save error: " << e.what() << endl;
            }
        }

        

        
        if (!selectedTicker.empty() && selectedTicker != pausedTicker) {
            pausedTicker = selectedTicker;
            pausedIndex = 0;
            dates.clear();
            prices.clear();

            try {
                Stocks stock(selectedTicker);
                auto data = stock.priceHistory.getAllData();
                if (!data.empty()) {
                    for (auto [date, vals] : data) {
                        dates.push_back(date);
                        if (vals.size() >= 4)
                            prices.push_back(vals[3]); // close
                        else if (!vals.empty())
                            prices.push_back(vals[0]);
                    }
                    startDatePicker.setAvailableDates(dates);
                    endDatePicker.setAvailableDates(dates);
                }
            } catch (...) {
                // silently ignore if stock data missing
            }
        }

        // === Simulation trigger ===
        if (!isSimulating && executeBtn.isClicked()) {
            try {
                if (selectedTicker.empty())
                    throw runtime_error("Select a stock ticker first.");
                if (dates.empty() || prices.empty())
                    throw runtime_error("No data loaded for this stock.");

                // Keep simulation start logic the same
                int startIndex = max(pausedIndex, (int)(prices.size() * 0.25f));
                lineGraph = make_unique<Line>(priceGraphRect, dates, prices);
                pnlGraph  = make_unique<Bar>(pnlGraphRect, vector<float>(prices.begin(), prices.begin() + startIndex));

                lineGraph->startAnimationFrom(startIndex);
                isSimulating = true;
                simTime = 0;
            }
            catch (exception &e) {
                DrawText(e.what(), 100, 155, 20, RED);
            }
        }

        // ==== Stop simulation ====
        if (isSimulating && stopBtn.isClicked()) {
            isSimulating = false;
            tickerSelect.close();
            callPut.close();
            optionStyle.close();
            startDatePicker.close();
            endDatePicker.close();

            if (lineGraph) {
                lineGraph->stopAnimation();
                pausedIndex = lineGraph->getCurrentIndex();
                DrawText(TextFormat("Paused on: %s", lineGraph->getPausedDate().c_str()),
                         980, 165, 18, YELLOW);
            }
        }

        // === Simulation active ===
        if (isSimulating) {
            simTime += GetFrameTime();
            if (lineGraph) lineGraph->simulation(GetFrameTime());
            if (pnlGraph) pnlGraph->simulation(GetFrameTime());
            DrawText(TextFormat("Simulating: %.1fs", simTime), 980, 140, 20, GREEN);
        }

        // === Graph panels ===
        DrawRectangleRec(priceGraphRect, PANEL_COLOR);
        DrawText("Stock Price Chart", priceGraphRect.x, priceGraphRect.y - 25, 22, ACCENT_COLOR);

        DrawRectangleRec(pnlGraphRect, PANEL_COLOR);
        DrawText("Portfolio P&L", pnlGraphRect.x, pnlGraphRect.y - 25, 22, ACCENT_COLOR);

        DrawRectangleRec(greeksPanelRect, PANEL_COLOR);
        DrawText("Option Greeks & Stats", greeksPanelRect.x + 10, greeksPanelRect.y - 25, 22, ACCENT_COLOR);

        if (lineGraph) lineGraph->draw();
        if (pnlGraph) pnlGraph->draw();

        // Placeholder Greek stats
        if (showBSStats) {
            DrawText(TextFormat("Price: %.2f", optionPrice), greeksPanelRect.x + 15, greeksPanelRect.y + 0, 18, GREEN);
            DrawText(TextFormat("Delta: %.4f", delta), greeksPanelRect.x + 15, greeksPanelRect.y + 25, 18, TEXT_COLOR);
            DrawText(TextFormat("Gamma: %.4f", gamma), greeksPanelRect.x + 15, greeksPanelRect.y + 50, 18, TEXT_COLOR);
            DrawText(TextFormat("Vega: %.4f", vega),  greeksPanelRect.x + 15, greeksPanelRect.y + 75, 18, TEXT_COLOR);
            DrawText(TextFormat("Theta: %.4f", theta), greeksPanelRect.x + 15, greeksPanelRect.y + 100, 18, TEXT_COLOR);
        }
        else {
            DrawText("Press 'Check' to view Black-Scholes stats", greeksPanelRect.x + 15, greeksPanelRect.y + 60, 18, LIGHTGRAY);
        }



        // === Manage layering (z-order) ===
        if (tickerSelect.getIsOpen()) { callPut.close(); optionStyle.close(); startDatePicker.close(); endDatePicker.close(); }
        else if (callPut.getIsOpen()) { tickerSelect.close(); optionStyle.close(); startDatePicker.close(); endDatePicker.close(); }
        else if (optionStyle.getIsOpen()) { tickerSelect.close(); callPut.close(); startDatePicker.close(); endDatePicker.close(); }

        // Draw expanded menus last
        tickerSelect.drawExpanded();
        callPut.drawExpanded();
        optionStyle.drawExpanded();
        startDatePicker.drawExpanded();
        endDatePicker.drawExpanded();

        EndDrawing();
    }
}



void Graphics::signupScreen() {
    if (!IsWindowReady()) InitWindow(1280, 720, "Sign Up");
    SetTargetFPS(60);

    Text username({515, 200, 250, 35}, "Enter username");
    Text password({515, 250, 250, 35}, "Enter password");
    Text firstName({515, 300, 250, 35}, "First name");
    Text lastName({515, 350, 250, 35}, "Last name");

    Button signupBtn({550, 420, 180, 45}, "Sign Up");
    Button loginRedirect({560, 480, 160, 35}, "Login →");

    bool accountCreated = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("Create Account", 470, 120, 36, TEXT_COLOR);

        username.update(); password.update(); firstName.update(); lastName.update();
        username.draw();  password.draw();  firstName.draw();  lastName.draw();

        signupBtn.draw();
        loginRedirect.draw();

        if (signupBtn.isClicked()) {
            if (signupUser(username.getContent(), firstName.getContent(),
                           lastName.getContent(), password.getContent())) {
                accountCreated = true;
            }
        }

        if (accountCreated) {
            DrawText("Account created successfully!", 460, 530, 20, GREEN);
            DrawText("Click 'Login' to continue.", 480, 555, 18, LIGHTGRAY);
        }

        if (loginRedirect.isClicked()) {
            loginScreen();
            return;
        }

        EndDrawing();
    }
}


void Graphics::loginScreen() {
    if (!IsWindowReady()) InitWindow(1280, 720, "Login");
    SetTargetFPS(60);

    Text username({515, 260, 250, 35}, "Username");
    Text password({515, 310, 250, 35}, "Password");
    Button loginBtn({550, 380, 180, 45}, "Login");
    Button signupRedirect({560, 440, 160, 35}, "← Sign Up");

    bool loginError = false;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("User Login", 520, 160, 36, TEXT_COLOR);

        username.update(); password.update();
        username.draw(); password.draw();
        loginBtn.draw(); signupRedirect.draw();

        if (loginBtn.isClicked()) {
            if (loginUser(username.getContent(), password.getContent())) {
                dashboard(); // move to dashboard after login
                return;
            } else {
                loginError = true;
            }
        }

        if (loginError)
            DrawText("Invalid username or password", 460, 490, 18, RED);

        if (signupRedirect.isClicked()) {
            signupScreen();
            return;
        }

        EndDrawing();
    }
}



bool Graphics::signupUser(string username, string firstName, string lastName, string password) {
    // Check for existing user
    if (users.find(username) != users.end()) {
        cout << "Username already exists!" << endl;
        return false;
    }

    // Create a new user with password
    User* newUser = new User(username, username, firstName, lastName, password);
    users[username] = newUser;

    newUser->activateAccount();  // Set active
    newUser->updateLastLogin();  // Set login time
    newUser->CSVWrite();         // Optionally save to file (persistent storage)

    cout << "Account created for " << username << endl;
    currentUser = newUser;
    return true;
}


bool Graphics::loginUser(string username, string password) {
    if (users.find(username) == users.end()) {
        cout << "User not found." << endl;
        return false;
    }

    User* user = users[username];
    user->CSVRead(); // refresh from file in case of updates

    if (password != user->getPassword()) {
        cout << "Invalid password." << endl;
        return false;
    }

    currentUser = user;
    user->updateLastLogin();
    cout << "Logged in as " << username << endl;
    return true;
}


void Graphics::loadAllUsersFromDisk() {
    string folder = "userData";
    for (auto entry : filesystem::directory_iterator(folder)) {
        if (entry.path().extension() == ".csv") {
            ifstream file(entry.path());
            if (!file.is_open()) continue;

            string line;
            if (getline(file, line)) {
                stringstream ss(line);
                vector<string> parts;
                string token;
                while (getline(ss, token, ',')) parts.push_back(token);

                if (parts.size() >= 4) {
                    string uname = parts[0];
                    string fname = parts[1];
                    string lname = parts[2];
                    string pass  = parts[3];

                    User* loadedUser = new User(uname, uname, fname, lname, pass);
                    loadedUser->CSVRead(); // populate extra fields
                    users[uname] = loadedUser;
                }
            }
            file.close();
        }
    }
    cout << "Loaded " << users.size() << " user(s) from disk." << endl;
}


User* Graphics::getCurrentUser() {
    return currentUser;
}
