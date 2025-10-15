#include "Graphics.h"
#include "Graph.h"
#include "Line.h"
#include "Bar.h"
#include "Button.h"
#include "Dropdown.h"
#include "Text.h"
#include "InputElement.h"
#include "User.h"
#include <memory>
#include <fstream>
using namespace std;

#define BG_COLOR  CLITERAL(Color){ 15, 25, 50, 255 }     // deep navy blue
#define PANEL_COLOR CLITERAL(Color){ 25, 40, 80, 255 }   // darker blue for boxes
#define TEXT_COLOR  CLITERAL(Color){ 230, 230, 240, 255 } // off-white text
#define ACCENT_COLOR CLITERAL(Color){ 50, 150, 255, 255 } // blue accent for highlights

void Graphics::dashboard() {
    // Load stock tickers from assets/stocksData
    vector<string> tickers = {"AAPL", "TSLA", "MSFT", "GOOG"};
    ifstream file("assets/stocksData");
    if (file.is_open()) {
        string ticker;
        while (file >> ticker) tickers.push_back(ticker);
    }

    // UI Elements
    Dropdown tickerSelect({100, 650, 200, 30}, tickers);
    Dropdown callPut({350, 650, 150, 30}, {"Call", "Put"});
    Dropdown optionStyle({650, 650, 180, 30}, {"American", "European"});

    Text startDate({100, 720, 180, 30}, "Start Date (YYYY-MM-DD)");
    Text endDate({360, 720, 180, 30}, "End Date (YYYY-MM-DD)");
    Text strike({650, 720, 120, 30}, "Strike Price");

    Button executeBtn({100, 550, 150, 40}, "Execute");
    Button stopBtn({280, 550, 150, 40}, "Stop");

    bool running = true;
    float simTime = 0.0f;

    while (running && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("Option Simulator Dashboard", 250, 50, 28, TEXT_COLOR);
        DrawText("Select parameters:", 100, 600, 20, ACCENT_COLOR);

        if (!isSimulating) {
            // Update only when not simulating
            tickerSelect.update();
            callPut.update();
            optionStyle.update();
            startDate.update();
            endDate.update();
            strike.update();

            if (executeBtn.isClicked()) {
                isSimulating = true;
                simTime = 0;
            }
        } else {
            // Simulation logic
            simulation(GetFrameTime());
            simTime += GetFrameTime();
            DrawText(TextFormat("Simulating... %.1fs", simTime), 500, 550, 18, GREEN);
            if (stopBtn.isClicked()) isSimulating = false;
        }

        // Draw UI
        tickerSelect.draw();
        callPut.draw();
        optionStyle.draw();
        startDate.draw();
        endDate.draw();
        strike.draw();

        if (!isSimulating)
            executeBtn.draw();
        else
            stopBtn.draw();

        EndDrawing();
    }
}

void Graphics::signupScreen() {
    Text username({300, 125, 250, 30}, "Enter username");
    Text password({300, 175, 250, 30}, "Enter password");
    Text firstName({300, 225, 250, 30}, "First name");
    Text lastName({300, 275, 250, 30}, "Last name");

    Button signupBtn({360, 400, 150, 40}, "Sign Up");
    Button loginRedirect({380, 450, 100, 30}, "Login →");

    bool accountCreated = false;
    bool done = false;

    while (!done && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("Create Account", 310, 80, 30, TEXT_COLOR);

        username.update(); password.update(); firstName.update(); lastName.update();
        username.draw();  password.draw();  firstName.draw();  lastName.draw();

        signupBtn.draw();
        loginRedirect.draw();

        // -------- Handle signup --------
        if (signupBtn.isClicked()) {
            if (signupUser(username.getContent(), firstName.getContent(),
                           lastName.getContent(), password.getContent())) {
                accountCreated = true;
            }
        }

        // Show success or error feedback
        if (accountCreated) {
            DrawText("Account created successfully!", 300, 340, 18, GREEN);
            DrawText("Click 'Login' to continue.", 320, 365, 18, LIGHTGRAY);
        }

        // -------- Redirect to login --------
        if (loginRedirect.isClicked()) {
            loginScreen();
            return;
        }

        EndDrawing();
    }
}

void Graphics::loginScreen() {
    Text username({300, 180, 250, 30}, "Username");
    Text password({300, 230, 250, 30}, "Password");
    Button loginBtn({360, 300, 150, 40}, "Login");
    Button signupRedirect({360, 360, 150, 30}, "← Sign Up");

    bool done = false;
    bool loginError = false;

    while (!done && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BG_COLOR);

        DrawText("User Login", 370, 100, 30, TEXT_COLOR);

        username.update(); password.update();
        username.draw(); password.draw();
        loginBtn.draw(); signupRedirect.draw();

        if (loginBtn.isClicked()) {
            if (loginUser(username.getContent(), password.getContent())) {
                done = true; // successful login
            } else {
                loginError = true;
            }
        }

        if (loginError) {
            DrawText("Invalid username or password", 300, 270, 18, RED);
        }

        if (signupRedirect.isClicked()) {
            signupScreen();
            return;
        }

        EndDrawing();
    }

    if (currentUser) dashboard();
}

bool Graphics::signupUser(string username, string firstName, string lastName, string password) {
    if (users.find(username) != users.end()) {
        cout << "Username already exists!" << endl;
        return false;
    }

    User* newUser = new User(username, username, firstName, lastName);
    users[username] = newUser;
    cout << "Account created for " << username << endl;
    currentUser = newUser;
    return true;
}

bool Graphics::loginUser(string username, string password) {
    if (users.find(username) == users.end()) {
        cout << "User not found." << endl;
        return false;
    }
    currentUser = users[username];
    currentUser->updateLastLogin();
    cout << "Logged in as " << username << endl;
    return true;
}


User* Graphics::getCurrentUser() {
    return currentUser;
}