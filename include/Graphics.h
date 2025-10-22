#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Theme.h"
#include "User.h"
#include "raylib.h"

 // The Graphics class is the central controller for the visual components of the Option Simulator app. It manages different screens (login, signup,
  // and main dashboard) and coordinates user interactions with their stored portfolio data.
 
 // This class also manages simulation states, handles user authentication, and draws UI panels, graphs, and inputs using Raylib.
 
 // Responsibilities:
 // - Render login/signup screens
 // - Manage dashboard and portfolio visualization
 // - Control simulation and option contracts
 // - Read/write user data to disk (CSV)

class Graphics {
 protected:
  unordered_map<string, User*> users;
  User* currentUser = nullptr; // pointer to the logged in user
  bool isSimulating = false;

 private:
  int pausedIndex = 0; // index of the day that it was paused on
  string pausedTicker = ""; // Store the ticker when paused

 public:
  virtual void draw() {};
  virtual void update() {};
  virtual void simulation(float dt) {}; // base simulation function

  // login and signup screens
  void signupScreen();
  void loginScreen();

  bool signupUser(string username, string firstName, string lastName, string password);
  bool loginUser(string username, string password);
  void loadAllUsersFromDisk();

  // Dashboard that assembles everythifong visually
  void dashboard();

  User* getCurrentUser();
};

#endif
