#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "raylib.h"
#include <string>
#include <vector>
#include <memory>
#include "User.h"
#include <unordered_map>

class Graphics {
    private:
        unordered_map<string, User*> users;
        User* currentUser = nullptr;
    public:
        virtual void draw() {}
        virtual void update() {}

        void signupScreen();
        void loginScreen();

        bool signupUser(string username, string firstName, string lastName, string password);
        bool loginUser(string username, string password);

        // Dashboard that assembles everythifong visually
        void dashboard();

        User* getCurrentUser();
};

#endif
