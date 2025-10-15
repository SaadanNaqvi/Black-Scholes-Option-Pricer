#include "Graphics.h"

int main() {
    InitWindow(1280, 720, "Black Scholes Option Simulator");
    SetTargetFPS(60);

    Graphics g;

    // Start at sign-up first
    g.signupScreen();

    CloseWindow();
    return 0;
}