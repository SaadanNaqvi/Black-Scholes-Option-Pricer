#include "Graphics.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>
#include <numbers>

#include "Bar.h"
#include "Button.h"
#include "CSVToBSVals.h"
#include "DatePicker.h"
#include "Dropdown.h"
#include "Graph.h"
#include "InputElement.h"
#include "Line.h"
#include "Stocks.h"
#include "Text.h"
#include "Theme.h"
#include "User.h"
#include "blackScholes.h"

using namespace std;

// =============================================================
//                    DASHBOARD FUNCTION
// =============================================================
void Graphics::dashboard() {
    if (!IsWindowReady()) InitWindow(1280, 720, "Option Simulator Dashboard");
    SetTargetFPS(60);

    // ---------------------- STOCK TICKERS ----------------------
    vector<string> tickers = {
        "AAPL", "AMD", "AMZN", "ATVI", "BABA", "BAC", "CRM", "CSCO", "DIS",
        "EA",   "F",   "GOOG", "INTC", "JPM",  "KO",  "MCD", "META", "MSFT",
        "MTCH", "NFLX","NVDA", "PFE",  "PYPL", "T",   "TSLA","TTD", "WMT",
        "XOM",  "YELP","ZG"
    };
    ifstream file("assets/stocksData");
    if (file.is_open()) {
        string ticker;
        while (file >> ticker) tickers.push_back(ticker);
    }

    // ---------------------- UI ELEMENTS ------------------------
    Dropdown tickerSelect({100, 90, 160, 35}, tickers);
    Dropdown callPut({280, 90, 100, 35}, {"Call", "Put"});
    Dropdown optionStyle({400, 90, 150, 35}, {"European", "American"});
    Text strike({570, 90, 100, 35}, "Strike");
    DatePicker endDatePicker({690, 90, 150, 35}, {});

    // ---------------------- BUTTONS ----------------------------
    Button executeBtn({860, 90, 110, 35}, "Execute");
    Button stopBtn({860, 140, 110, 35}, "Stop");
    Button checkBtn({990, 90, 110, 35}, "Check");
    Button saveBtn({1110, 90, 110, 35}, "Save");
    Button startContractBtn({990, 140, 110, 35}, "Start Contract");
    Button buyBtn({770, 120, 100, 40}, "Buy");
    Button ignoreBtn({880, 120, 100, 40}, "Ignore");


    // ---------------------- GRAPH PANELS -----------------------
    Rectangle priceGraphRect  = {100, 220, 900, 330};
    Rectangle pnlGraphRect    = {100, 570, 700, 120};
    Rectangle greeksPanelRect = {830, 570, 380, 120};

    unique_ptr<Line> lineGraph;
    unique_ptr<Bar> pnlGraph;

    // ---------------------- STATE VARIABLES --------------------
    bool running = true;
    float simTime = 0.0f;

    static vector<string> dates;
    static vector<float> prices;

    double delta = 0.0, gamma = 0.0, vega = 0.0, theta = 0.0, optionPrice = 0.0;
    bool showBSStats = false;

    // Contract-related
    bool contractActive = false;
    bool waitingForDecision = false;
    [[maybe_unused]] bool contractBought = false;
    string contractStartDate = "";
    string contractEndDate = "";

    // =============================================================
    //                       MAIN RENDER LOOP
    // =============================================================
    while (running && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("Black Scholes Backtester Dashboard", 420, 20, 34, TEXT_COLOR);

        // ---------------------- CONTROL PANEL --------------------
        DrawRectangleRounded({80, 75, 1120, 120}, 0.05f, 0, PANEL_COLOR);
        DrawText("Controls", 90, 55, 22, ACCENT_COLOR);

        // Update UI elements
        tickerSelect.update();
        callPut.update();
        optionStyle.update();
        strike.update();
        endDatePicker.update();

        string selectedTicker = tickerSelect.getSelectedOption();

        // ---------------------- UI DRAWING -----------------------
        if (contractActive) {
            // Grey out and lock ticker select
            DrawRectangleRec(tickerSelect.getBounds(), Fade(GRAY, 0.7f));
            DrawText("Locked", tickerSelect.getBounds().x + 50, tickerSelect.getBounds().y + 10, 16, LIGHTGRAY);
        } else {
            tickerSelect.drawBase();
        }

        callPut.drawBase();
        optionStyle.drawBase();
        strike.draw();
        endDatePicker.drawBase();

        // Buttons
        if (!isSimulating) {
            executeBtn.draw();
            checkBtn.draw();
        } else {
            stopBtn.draw();
        }
        saveBtn.draw();

        if (!contractActive && !isSimulating && !contractStartDate.empty())
            startContractBtn.draw();

        // =============================================================
        //                 BLACK-SCHOLES CHECK BUTTON
        // =============================================================
        if (checkBtn.isClicked()) {
            try {
                if (selectedTicker.empty())
                    throw runtime_error("Select a stock ticker first.");
                if (dates.empty() || prices.empty())
                    throw runtime_error("No data loaded for this stock.");
                if (contractStartDate.empty())
                    throw runtime_error("Pause the simulation first to set start date.");

                string startDate = contractStartDate;
                string endDate = endDatePicker.getSelectedDate();
                if (endDate.empty())
                    throw runtime_error("Please select expiry date first.");

                // 🔒 Prevent contract before current simulation date
                if (endDate <= startDate)
                    throw runtime_error("Expiry date must be after current simulation date.");

                double strikePrice = stod(strike.getContent());
                string optionType = callPut.getSelectedOption();
                string style = optionStyle.getSelectedOption();

                Stocks stock(selectedTicker);
                auto& px = stock.priceHistory;
                BSVarCalc calc;

                double S = calc.spotPrice(px, startDate);
                double T = calc.timeToMaturity(startDate, endDate);

                vector<string> allDates = px.getDates();
                if (allDates.empty())
                    throw runtime_error("No dates found for this stock!");

                double sigma = calc.historicalVol(px, allDates.front(), startDate);
                double q = 0.01, r = 0.02;

                MarketParams mp{S, strikePrice, T, r, q, sigma};
                EuropeanCall callOption(mp);
                EuropeanPut putOption(mp);

                if (style == "European") {
                    if (optionType == "Call") {
                        optionPrice = callOption.price();
                        delta = exp(-q * T) * N(callOption.D1());
                        gamma = exp(-q * T) * NDash(callOption.D1()) / (S * sigma * sqrt(T));
                        vega  = S * exp(-q * T) * NDash(callOption.D1()) * sqrt(T) / 100;
                        theta = (-S * exp(-q * T) * NDash(callOption.D1()) * sigma / (2 * sqrt(T))
                                 - r * strikePrice * exp(-r * T) * N(callOption.D2())
                                 + q * S * exp(-q * T) * N(callOption.D1())) / 365;
                    } else {
                        optionPrice = putOption.price();
                        delta = exp(-q * T) * (N(callOption.D1()) - 1);
                        gamma = exp(-q * T) * NDash(putOption.D1()) / (S * sigma * sqrt(T));
                        vega  = S * exp(-q * T) * NDash(putOption.D1()) * sqrt(T) / 100;
                        theta = (-S * exp(-q * T) * NDash(putOption.D1()) * sigma / (2 * sqrt(T))
                                 + r * strikePrice * exp(-r * T) * N(-putOption.D2())
                                 - q * S * exp(-q * T) * N(-putOption.D1())) / 365;
                    }
                }

                showBSStats = true;
                cout << "Computed Black-Scholes from " << startDate << " to " << endDate << endl;
            } catch (exception &e) {
                DrawText(e.what(), 100, 155, 20, RED);
            }
        }

        // =============================================================
        //                     CONTRACT LOGIC
        // =============================================================
        if (!contractActive && startContractBtn.isClicked()) {
            if (contractStartDate.empty()) {
                DrawText("Pause simulation first to set start date!", 100, 155, 20, RED);
            } else if (endDatePicker.getSelectedDate().empty()) {
                DrawText("Please select an expiry date first!", 100, 155, 20, RED);
            } else if (endDatePicker.getSelectedDate() <= contractStartDate) {
                DrawText("Expiry date must be after current simulation date!", 100, 155, 20, RED);
            } else {
                contractActive = true;
                contractEndDate = endDatePicker.getSelectedDate();
                waitingForDecision = false;
                contractBought = false;
                cout << "Started contract: " << contractStartDate << " → " << contractEndDate << endl;
            }
        }

        // =============================================================
        //                       TICKER DATA LOAD
        // =============================================================
        if (contractActive) {
            DrawText("⚠️ Contract active — cannot switch stock.", 100, 155, 20, ORANGE);
        }
        else if (!selectedTicker.empty() && selectedTicker != pausedTicker) {
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
                        prices.push_back((vals.size() >= 4) ? vals[3] : vals[0]);
                    }
                    endDatePicker.setAvailableDates(dates);
                }
            } catch (...) {}
        }

        // =============================================================
        //                    SIMULATION EXECUTION
        // =============================================================
        if (!isSimulating && executeBtn.isClicked()) {
            try {
                if (selectedTicker.empty())
                    throw runtime_error("Select a stock ticker first.");
                if (dates.empty() || prices.empty())
                    throw runtime_error("No data loaded for this stock.");

                int startIndex = max(pausedIndex, (int)(prices.size() * 0.25f));
                lineGraph = make_unique<Line>(priceGraphRect, dates, prices);
                pnlGraph  = make_unique<Bar>(
                    pnlGraphRect,
                    vector<float>(prices.begin(), prices.begin() + startIndex)
                );

                lineGraph->startAnimationFrom(startIndex);
                isSimulating = true;
                simTime = 0;
            } catch (exception &e) {
                DrawText(e.what(), 100, 155, 20, RED);
            }
        }

        // =============================================================
        //                        STOP SIMULATION
        // =============================================================
        if (isSimulating && stopBtn.isClicked()) {
            isSimulating = false;
            if (lineGraph) {
                lineGraph->stopAnimation();
                pausedIndex = lineGraph->getCurrentIndex();
                contractStartDate = lineGraph->getPausedDate();
                DrawText(TextFormat("Paused on: %s", contractStartDate.c_str()), 980, 165, 18, YELLOW);
            }
        }

        // =============================================================
        //                     ACTIVE SIMULATION
        // =============================================================
        if (isSimulating) {
            simTime += GetFrameTime();
            if (lineGraph) lineGraph->simulation(GetFrameTime());
            if (pnlGraph)  pnlGraph->simulation(GetFrameTime());

            DrawText(TextFormat("⏱ Simulating: %.1fs", simTime), 980, 140, 20, GREEN);

            if (contractActive && !contractEndDate.empty()) {
                string currentDate = lineGraph->getCurrentDate();
                if (currentDate == contractEndDate) {
                    isSimulating = false;
                    contractActive = false;
                    waitingForDecision = true;
                    lineGraph->stopAnimation();
                    pausedIndex = lineGraph->getCurrentIndex();
                    cout << "Contract expired at " << currentDate << endl;
                }
            }
        }

        // =============================================================
        //                     DRAW GRAPH PANELS
        // =============================================================
        DrawRectangleRec(priceGraphRect, PANEL_COLOR);
        DrawText("Stock Price Chart", priceGraphRect.x, priceGraphRect.y - 25, 22, ACCENT_COLOR);

        DrawRectangleRec(pnlGraphRect, PANEL_COLOR);
        DrawText("Portfolio P&L", pnlGraphRect.x, pnlGraphRect.y - 25, 22, ACCENT_COLOR);

        DrawRectangleRec(greeksPanelRect, PANEL_COLOR);
        DrawText("Option Greeks & Stats", greeksPanelRect.x + 10, greeksPanelRect.y - 25, 22, ACCENT_COLOR);

        if (lineGraph) lineGraph->draw();
        if (pnlGraph) pnlGraph->draw();

        // =============================================================
        //                   BLACK-SCHOLES GREEKS
        // =============================================================
        if (showBSStats) {
            DrawText(TextFormat("Price: %.2f", optionPrice), greeksPanelRect.x + 15, greeksPanelRect.y + 0, 18, GREEN);
            DrawText(TextFormat("Delta: %.4f", delta), greeksPanelRect.x + 15, greeksPanelRect.y + 25, 18, TEXT_COLOR);
            DrawText(TextFormat("Gamma: %.4f", gamma), greeksPanelRect.x + 15, greeksPanelRect.y + 50, 18, TEXT_COLOR);
            DrawText(TextFormat("Vega: %.4f", vega), greeksPanelRect.x + 15, greeksPanelRect.y + 75, 18, TEXT_COLOR);
            DrawText(TextFormat("Theta: %.4f", theta), greeksPanelRect.x + 15, greeksPanelRect.y + 100, 18, TEXT_COLOR);
        } else {
            DrawText("Press 'Check ⚙️' to view Black-Scholes stats",
                     greeksPanelRect.x + 15, greeksPanelRect.y + 60, 18, LIGHTGRAY);
        }

        // =============================================================
        //                     CONTRACT POPUP
        // =============================================================
        if (waitingForDecision) {
            Rectangle popup = {
              static_cast<float>(GetScreenWidth()/2 - 180),
              static_cast<float>(GetScreenHeight()/2 - 75),
              360.0f, 150.0f
          };

            DrawRectangleRounded(popup, 0.1f, 0, DARKGRAY);
            DrawText("Contract expired", popup.x + 60, popup.y + 20, 22, YELLOW);
            DrawText("Would you like to buy?", popup.x + 60, popup.y + 55, 18, WHITE);

            buyBtn.setPosition({popup.x + 40, popup.y + 100});
            ignoreBtn.setPosition({popup.x + 200, popup.y + 100});
            buyBtn.draw();
            ignoreBtn.draw();

            if (buyBtn.isClicked()) {
                waitingForDecision = false;
                contractBought = true;
                contractActive = false;
                contractStartDate.clear();
                contractEndDate.clear();
                cout << "User chose to BUY contract." << endl;
            }
            if (ignoreBtn.isClicked()) {
                waitingForDecision = false;
                contractBought = false;
                contractActive = false;
                contractStartDate.clear();
                contractEndDate.clear();
                cout << "User chose to IGNORE contract." << endl;
            }
        }

        // =============================================================
        //                          LAYERING
        // =============================================================
        if (tickerSelect.getIsOpen()) {
            callPut.close(); optionStyle.close(); endDatePicker.close();
        } else if (callPut.getIsOpen()) {
            tickerSelect.close(); optionStyle.close(); endDatePicker.close();
        } else if (optionStyle.getIsOpen()) {
            tickerSelect.close(); callPut.close(); endDatePicker.close();
        }

        tickerSelect.drawExpanded();
        callPut.drawExpanded();
        optionStyle.drawExpanded();
        endDatePicker.drawExpanded();

        EndDrawing();
    }
}




// has inputs of *
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

    username.update();
    password.update();
    firstName.update();
    lastName.update();
    username.draw();
    password.draw();
    firstName.draw();
    lastName.draw();

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

    username.update();
    password.update();
    username.draw();
    password.draw();
    loginBtn.draw();
    signupRedirect.draw();

    if (loginBtn.isClicked()) {
      if (loginUser(username.getContent(), password.getContent())) {
        dashboard();  // move to dashboard after login
        return;
      } else {
        loginError = true;
      }
    }

    if (loginError) DrawText("Invalid username or password", 460, 490, 18, RED);

    if (signupRedirect.isClicked()) {
      signupScreen();
      return;
    }

    EndDrawing();
  }
}

bool Graphics::signupUser(string username, string firstName, string lastName,
                          string password) {
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
  user->CSVRead();  // refresh from file in case of updates

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
          string pass = parts[3];

          User* loadedUser = new User(uname, uname, fname, lname, pass);
          loadedUser->CSVRead();  // populate extra fields
          users[uname] = loadedUser;
        }
      }
      file.close();
    }
  }
  cout << "Loaded " << users.size() << " user(s) from disk." << endl;
}

User* Graphics::getCurrentUser() { return currentUser; }
