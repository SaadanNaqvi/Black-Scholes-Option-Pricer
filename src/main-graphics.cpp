#include "Graphics.h"

int main() {
    InitWindow(900, 600, "Portfolio Dashboard");
    SetTargetFPS(60);

    Graphics g;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        g.dashboard();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
