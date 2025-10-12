#include "Graphics.h"

int main() {
    InitWindow(900, 600, "User Login + Dashboard");
    SetTargetFPS(60);

    Graphics g;

    // Start with login screen
    g.loginScreen();

    CloseWindow();
    return 0;
}
