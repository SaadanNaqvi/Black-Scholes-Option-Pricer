#include "Graphics.h"

int main() {
    InitWindow(900, 600, "User Signup → Login → Dashboard");
    SetTargetFPS(60);

    Graphics g;

    // Start at sign-up first
    g.signupScreen();

    CloseWindow();
    return 0;
}