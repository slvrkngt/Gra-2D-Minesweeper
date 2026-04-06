#include <raylib.h>
#include "board.h"

int main() {
    InitWindow(800, 800, "Minesweeper"); // Stworzenie okna gry (width, height, title)
    SetTargetFPS(60); // frames per second

    // Game Loop
    while (WindowShouldClose()==false) {


        BeginDrawing();
        EndDrawing();
    }

    CloseWindow(); // Zamknięcie okna gry
    return 0;
}