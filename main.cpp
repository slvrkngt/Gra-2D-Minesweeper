#include <raylib.h>
#include "board.h"

Color bgColor = {40, 40, 40, 255};

int main() {
    InitWindow(560, 560, "Minesweeper"); // Stworzenie okna gry (width, height, title)
    SetTargetFPS(60); // frames per second

    // Game Loop
    while (WindowShouldClose()==false) {

        Board board = Board();

        BeginDrawing();
        ClearBackground(bgColor);
        board.drawBoard();
        
        EndDrawing();
    }

    CloseWindow(); // Zamknięcie okna gry
    return 0;
}