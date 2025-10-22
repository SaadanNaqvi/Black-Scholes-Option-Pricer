#include "Graphics.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>
#include <numbers>
#include <unordered_map>

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

void Graphics::dashboard() {
  if (!IsWindowReady()) InitWindow(1280, 720, "Option Simulator Dashboard");
  SetTargetFPS(60);

  // ---------------------- STOCK TICKERS ----------------------
  vector<string> tickers = {"AAPL", "AMD",  "AMZN", "ATVI", "BABA", "BAC",
                            "CRM",  "CSCO", "DIS",  "EA",   "F",    "GOOG",
                            "INTC", "JPM",  "KO",   "MCD",  "META", "MSFT",
                            "MTCH", "NFLX", "NVDA", "PFE",  "PYPL", "T",
                            "TSLA", "TTD",  "WMT",  "XOM",  "YELP", "ZG"};
  ifstream file("assets/stocksData");
  if (file.is_open()) {
    string ticker;
    while (file >> ticker) tickers.push_back(ticker);
  }

  // ---------------------- UI ELEMENTS ------------------------
  Dropdown tickerSelect({100, 90, 160, 35}, tickers);
  Dropdown callPut({280, 90, 100, 35}, {"Call", "Put"});
  Dropdown optionStyle({400, 90, 150, 35}, {"European"});
  Text strike({570, 90, 100, 35}, "Strike");
  DatePicker endDatePicker({690, 90, 150, 35}, {});

  Button executeBtn({860, 90, 110, 35}, "Execute");
  Button stopBtn({860, 140, 110, 35}, "Stop");
  Button checkBtn({990, 90, 110, 35}, "Check");
  Button saveBtn({1110, 90, 110, 35}, "Save");
  Button startContractBtn({990, 140, 110, 35}, "Start Contract");
  Button buyBtn({770, 120, 100, 40}, "Buy");
  Button ignoreBtn({880, 120, 100, 40}, "Ignore");

  // ---------------------- PANELS ----------------------
  Rectangle priceGraphRect = {100, 220, 900, 330};
  Rectangle pnlGraphRect = {100, 570, 700, 120};
  Rectangle greeksPanelRect = {830, 570, 380, 120};

  unique_ptr<Line> lineGraph;
  unique_ptr<Bar> cashGraph;
  unique_ptr<Bar> pnlGraph;

  bool running = true;
  float simTime = 0.0f;
  static vector<string> dates;
  static vector<float> prices;

  // Greeks
  double delta = 0, gamma = 0, vega = 0, theta = 0, optionPrice = 0;
  bool showBSStats = false;

  // Contract
  bool contractActive = false, waitingForDecision = false,
       contractBought = false;
  string contractStartDate = "", contractEndDate = "";

  // Portfolio
  User* user = getCurrentUser();
  if (user) {
    user->CSVRead();  // restore saved portfolio
    cout << "Loaded portfolio for " << user->getUserName() << " | Cash: $"
         << user->getPrimaryPortfolio()->getCashBalance() << endl;
  }
  unordered_map<string, double> holdings;

  // Save message fade
  float saveMsgTimer = 0.0f;
  bool showSaveMsg = false;

  // =============================================================
  //                       MAIN LOOP
  // =============================================================
  while (running && !WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BG_COLOR);

    DrawText("Black-Scholes Backtester Dashboard", 380, 20, 34, TEXT_COLOR);
    DrawRectangleRounded({80, 75, 1120, 120}, 0.05f, 0, PANEL_COLOR);
    DrawText("Controls", 90, 55, 22, ACCENT_COLOR);

    // ---------------------- UI ----------------------
    tickerSelect.update();
    callPut.update();
    optionStyle.update();
    strike.update();
    endDatePicker.update();
    string selectedTicker = tickerSelect.getSelectedOption();

    if (contractActive) {
      DrawRectangleRec(tickerSelect.getBounds(), Fade(GRAY, 0.6f));
      DrawText("Locked", tickerSelect.getBounds().x + 50,
               tickerSelect.getBounds().y + 10, 16, LIGHTGRAY);
    } else
      tickerSelect.drawBase();
    callPut.drawBase();
    optionStyle.drawBase();
    strike.draw();
    endDatePicker.drawBase();

    if (!isSimulating) {
      executeBtn.draw();
      checkBtn.draw();
    } else
      stopBtn.draw();
    saveBtn.draw();
    if (!contractActive && !isSimulating) startContractBtn.draw();

    // ---------------------- LOAD DATA ----------------------
    if (!selectedTicker.empty() && selectedTicker != pausedTicker) {
      pausedTicker = selectedTicker;
      dates.clear();
      prices.clear();
      try {
        Stocks stock(selectedTicker);
        auto data = stock.priceHistory.getAllData();
        for (auto& [d, v] : data) {
          dates.push_back(d);
          prices.push_back((v.size() >= 4) ? v[3] : v[0]);
        }
        endDatePicker.setAvailableDates(dates);
      } catch (...) {
      }
    }

    // ---------------------- CHECK BUTTON (Black-Scholes)
    // ----------------------
    if (checkBtn.isClicked()) {
      try {
        if (selectedTicker.empty())
          throw runtime_error("Select a stock ticker first.");
        if (dates.empty() || prices.empty())
          throw runtime_error("No data loaded for this stock.");
        if (endDatePicker.getSelectedDate().empty())
          throw runtime_error("Select expiry date first.");
        if (strike.getContent().empty())
          throw runtime_error("Enter strike price.");

        string startDate =
            (lineGraph) ? lineGraph->getCurrentDate() : dates.front();
        string endDate = endDatePicker.getSelectedDate();
        if (endDate <= startDate)
          throw runtime_error("Expiry must be after start.");

        double strikePrice = stod(strike.getContent());
        string optType = callPut.getSelectedOption();
        string style = optionStyle.getSelectedOption();

        Stocks stock(selectedTicker);
        auto& px = stock.priceHistory;
        BSVarCalc calc;
        double S = calc.spotPrice(px, startDate);
        double T = calc.timeToMaturity(startDate, endDate);
        double sigma = calc.historicalVol(px, px.getDates().front(), startDate);
        double q = 0.01, r = 0.02;

        MarketParams mp{S, strikePrice, T, r, q, sigma};
        EuropeanCall call(mp);
        EuropeanPut put(mp);

        if (style == "European") {
          if (optType == "Call") {
            optionPrice = call.price();
            delta = exp(-q * T) * N(call.D1());
            gamma = exp(-q * T) * NDash(call.D1()) / (S * sigma * sqrt(T));
            vega = S * exp(-q * T) * NDash(call.D1()) * sqrt(T) / 100;
            theta =
                (-S * exp(-q * T) * NDash(call.D1()) * sigma / (2 * sqrt(T)) -
                 r * strikePrice * exp(-r * T) * N(call.D2()) +
                 q * S * exp(-q * T) * N(call.D1())) /
                365;
          } else {
            optionPrice = put.price();
            delta = exp(-q * T) * (N(call.D1()) - 1);
            gamma = exp(-q * T) * NDash(put.D1()) / (S * sigma * sqrt(T));
            vega = S * exp(-q * T) * NDash(put.D1()) * sqrt(T) / 100;
            theta =
                (-S * exp(-q * T) * NDash(put.D1()) * sigma / (2 * sqrt(T)) +
                 r * strikePrice * exp(-r * T) * N(-put.D2()) -
                 q * S * exp(-q * T) * N(-put.D1())) /
                365;
          }
        }
        showBSStats = true;
      } catch (exception& e) {
        DrawText(e.what(), 100, 155, 20, RED);
      }
    }

    // ---------------------- START CONTRACT ----------------------
    if (!contractActive && startContractBtn.isClicked()) {
      string expiry = endDatePicker.getSelectedDate();
      string currentDate = (lineGraph) ? lineGraph->getCurrentDate()
                                       : (dates.empty() ? "" : dates.front());
      if (dates.empty())
        DrawText("Load a stock first.", 100, 155, 20, RED);
      else if (strike.getContent().empty())
        DrawText("Enter strike price!", 100, 155, 20, RED);
      else if (expiry.empty())
        DrawText("Select expiry date!", 100, 155, 20, RED);
      else if (!currentDate.empty() && expiry <= currentDate)
        DrawText("Expiry date must be after current simulation date!", 100, 155,
                 20, RED);
      else {
        contractActive = true;
        contractStartDate = currentDate;
        contractEndDate = expiry;
        waitingForDecision = false;
        contractBought = false;
        cout << "✅ Contract started: " << contractStartDate << " → "
             << contractEndDate << endl;
      }
    }

    // ---------------------- SAVE BUTTON ----------------------
    if (saveBtn.isClicked() && user) {
      user->CSVWrite();
      saveMsgTimer = 2.0f;
      showSaveMsg = true;
      cout << "✅ User data saved for " << user->getUserName() << endl;
    }
    if (showSaveMsg && saveMsgTimer > 0) {
      DrawText("✅ Saved user stats!", 920, 120, 20,
               Fade(GREEN, saveMsgTimer / 2.0f));
      saveMsgTimer -= GetFrameTime();
      if (saveMsgTimer <= 0) showSaveMsg = false;
    }

    // ---------------------- SIMULATION ----------------------
    if (!isSimulating && executeBtn.isClicked()) {
      try {
        if (selectedTicker.empty())
          throw runtime_error("Select a stock ticker first.");
        if (dates.empty() || prices.empty())
          throw runtime_error("No data loaded.");
        int startIndex = max(pausedIndex, (int)(prices.size() * 0.25f));
        lineGraph = make_unique<Line>(priceGraphRect, dates, prices);
        lineGraph->startAnimationFrom(startIndex);
        isSimulating = true;
        simTime = 0;
      } catch (exception& e) {
        DrawText(e.what(), 100, 155, 20, RED);
      }
    }

    if (isSimulating && stopBtn.isClicked()) {
      isSimulating = false;
      if (lineGraph) {
        lineGraph->stopAnimation();
        pausedIndex = lineGraph->getCurrentIndex();
        contractStartDate = lineGraph->getPausedDate();
      }
    }

    if (isSimulating && lineGraph) {
      simTime += GetFrameTime();
      lineGraph->simulation(GetFrameTime());
      string curDate = lineGraph->getCurrentDate();
      if (contractActive && curDate == contractEndDate) {
        isSimulating = false;
        contractActive = false;
        waitingForDecision = true;
        cout << "Contract expired on " << curDate << endl;
      }
    }

    // ---------------------- DRAW GRAPHS ----------------------
    DrawRectangleRec(priceGraphRect, PANEL_COLOR);
    DrawText("Stock Price Chart", priceGraphRect.x, priceGraphRect.y - 25, 22,
             ACCENT_COLOR);
    if (lineGraph) lineGraph->draw();

    // ---------------------- STATIC CASH & PNL GRAPHS ----------------------
    double cash = user->getPrimaryPortfolio()->getCashBalance();
    double holdingsValue = 0.0;
    for (auto& [ticker, qty] : user->getPrimaryPortfolio()->getStocks()) {
      if (ticker == selectedTicker && lineGraph)
        holdingsValue += qty * lineGraph->getCurrentPrice();
      else if (!prices.empty())
        holdingsValue += qty * prices.back();
    }
    double totalValue = cash + holdingsValue;
    double pnl = totalValue - user->getPrimaryPortfolio()->getInitialValue();

    DrawRectangleRec(pnlGraphRect, PANEL_COLOR);
    DrawText("Portfolio Overview", pnlGraphRect.x, pnlGraphRect.y - 25, 22,
             ACCENT_COLOR);

    Rectangle cashRect = {pnlGraphRect.x + 50, pnlGraphRect.y + 20, 200, 80};
    Rectangle pnlRect = {pnlGraphRect.x + 320, pnlGraphRect.y + 20, 200, 80};

    cashGraph = make_unique<Bar>(cashRect, vector<float>{(float)cash});
    pnlGraph = make_unique<Bar>(pnlRect, vector<float>{(float)pnl});

    DrawText("Cash Balance", cashRect.x, cashRect.y - 22, 18, ACCENT_COLOR);
    DrawText(TextFormat("$%.2f", cash), cashRect.x + 60,
             cashRect.y + cashRect.height + 5, 18, LIME);

    DrawText("Total P&L", pnlRect.x, pnlRect.y - 22, 18, ACCENT_COLOR);
    Color pnlColor = (pnl >= 0) ? GREEN : RED;
    DrawText(TextFormat("%s$%.2f", (pnl >= 0) ? "+" : "-", fabs(pnl)),
             pnlRect.x + 60, pnlRect.y + pnlRect.height + 5, 18, pnlColor);

    cashGraph->draw();
    pnlGraph->draw();

    // Neutral percentage labels
    float cashBarTop = cashRect.y + 10;
    float pnlBarTop = pnlRect.y + 10;
    DrawText(TextFormat("%.1f%% Portfolio", (cash / totalValue) * 100.0),
             cashRect.x + 30, cashBarTop - 10, 16, LIGHTGRAY);
    DrawText(TextFormat("%.1f%% Change", (pnl / totalValue) * 100.0),
             pnlRect.x + 40, pnlBarTop - 10, 16, LIGHTGRAY);

    // ---------------------- CONTRACT POPUP ----------------------
    if (waitingForDecision) {
      Rectangle popup = {GetScreenWidth() / 2.0f - 180,
                         GetScreenHeight() / 2.0f - 75, 360, 150};
      DrawRectangleRounded(popup, 0.1f, 0, DARKGRAY);
      DrawText("Contract Expired", popup.x + 80, popup.y + 20, 22, YELLOW);
      DrawText("Execute trade?", popup.x + 100, popup.y + 55, 18, WHITE);
      buyBtn.setPosition({popup.x + 40, popup.y + 100});
      ignoreBtn.setPosition({popup.x + 200, popup.y + 100});
      buyBtn.setLabel((callPut.getSelectedOption() == "Put") ? "Sell" : "Buy");
      buyBtn.draw();
      ignoreBtn.draw();

      if (buyBtn.isClicked() && user) {
        waitingForDecision = false;
        contractBought = true;
        float cost = prices[pausedIndex];
        string ticker = selectedTicker;
        int quantity = 100;
        double totalCost = cost * quantity;
        string optType = callPut.getSelectedOption();
        Portfolio* pf = user->getPrimaryPortfolio();
        double currentCash = pf->getCashBalance();

        if (optType == "Call" && user->canBuy(totalCost)) {
          pf->setCashBalance(currentCash - totalCost);
          user->addStock(ticker, quantity);
          cout << "Bought " << quantity << " " << ticker << " @" << cost
               << endl;
        } else if (optType == "Put" && user->canSell(ticker, quantity)) {
          pf->setCashBalance(currentCash + totalCost);
          user->addStock(ticker, -quantity);
          cout << "Sold " << quantity << " " << ticker << " @" << cost << endl;
        }
      }
      if (ignoreBtn.isClicked()) {
        waitingForDecision = false;
        contractBought = false;
      }
    }

    // ---------------------- GREEKS PANEL ----------------------
    DrawRectangleRec(greeksPanelRect, PANEL_COLOR);
    DrawText("Option Greeks & Stats", greeksPanelRect.x + 10,
             greeksPanelRect.y - 25, 22, ACCENT_COLOR);
    if (showBSStats) {
      DrawText(TextFormat("Price: %.2f", optionPrice), greeksPanelRect.x + 15,
               greeksPanelRect.y + 0, 18, GREEN);
      DrawText(TextFormat("Delta: %.4f", delta), greeksPanelRect.x + 15,
               greeksPanelRect.y + 25, 18, TEXT_COLOR);
      DrawText(TextFormat("Gamma: %.4f", gamma), greeksPanelRect.x + 15,
               greeksPanelRect.y + 50, 18, TEXT_COLOR);
      DrawText(TextFormat("Vega: %.4f", vega), greeksPanelRect.x + 15,
               greeksPanelRect.y + 75, 18, TEXT_COLOR);
      DrawText(TextFormat("Theta: %.4f", theta), greeksPanelRect.x + 15,
               greeksPanelRect.y + 100, 18, TEXT_COLOR);
    }

    // ---------------------- LAYER FIX ----------------------
    if (tickerSelect.getIsOpen()) {
      callPut.close();
      optionStyle.close();
      endDatePicker.close();
    } else if (callPut.getIsOpen()) {
      tickerSelect.close();
      optionStyle.close();
      endDatePicker.close();
    } else if (optionStyle.getIsOpen()) {
      tickerSelect.close();
      callPut.close();
      endDatePicker.close();
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
