#include <raylib.h>
#include "board.h"
#include <iostream>

Color bgColor = {40, 40, 40, 255};

int main() {
    InitWindow(560, 560, "Minesweeper"); // Stworzenie okna gry (width, height, title)
    SetTargetFPS(60); // frames per second
    Board board = Board(); // stworzenie planszy

    // Game Loop
    while (WindowShouldClose()==false) {

        if(IsMouseButtonPressed(0)) {
            board.triggerEvent(GetMouseX(), GetMouseY());
            // std::cout << GetMouseX() << ", " << GetMouseY() << std::endl;
        }

        BeginDrawing();
        ClearBackground(bgColor);
        board.drawBoard();
        
        EndDrawing();
    }

    CloseWindow(); // Zamknięcie okna gry
    return 0;
}