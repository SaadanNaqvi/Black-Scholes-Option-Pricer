#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Theme.h"
#include "User.h"
#include "raylib.h"

class Graphics {
 protected:
  unordered_map<string, User*> users;
  User* currentUser = nullptr;
  bool isSimulating = false;

 private:
  int pausedIndex = 0;
  string pausedTicker = "";

 public:
  virtual void draw() {};
  virtual void update() {};
  virtual void simulation(float dt) {};

  void signupScreen();
  void loginScreen();

  bool signupUser(string username, string firstName, string lastName,
                  string password);
  bool loginUser(string username, string password);
  void loadAllUsersFromDisk();

  // Dashboard that assembles everythifong visually
  void dashboard();

  User* getCurrentUser();
};

#endif
