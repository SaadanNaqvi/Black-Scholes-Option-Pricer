#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include <string>
#include <vector>
#include <memory>
#include "User.h"
#include <unordered_map>
#include "Theme.h"

class Graphics {
    protected:
        unordered_map<string, User*> users;
        User* currentUser = nullptr;
        bool isSimulating = false;
    public:
        virtual void draw() {}
        virtual void update() {}
        virtual void simulation(float dt) {}

        void signupScreen();
        void loginScreen();

        bool signupUser(string username, string firstName, string lastName, string password);
        bool loginUser(string username, string password);

        // Dashboard that assembles everythifong visually
        void dashboard();
        

        User* getCurrentUser();
};

#endif
