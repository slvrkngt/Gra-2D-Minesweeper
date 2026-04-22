#include <raylib.h>
#include "board.h"
#include <iostream>

Color bgColor = {40, 40, 40, 255};

int main() {
    InitWindow(560, 660, "Minesweeper"); // Stworzenie okna gry (width, height, title)
    SetTargetFPS(20); // frames per second
    Board board = Board(); // stworzenie planszy
    Board::flagTexture = LoadTexture("flagTexture.png"); 
    Board::questionMarkTexture = LoadTexture("questionMarkTexture.png"); 
    Board::revealedTiles = 0;

    // Game Loop
    while (WindowShouldClose()==false) {
        // if (board.currentGameState==GameState::setup)
        //     std::cout << "SETUP" << std::endl;
        // else if (board.currentGameState==GameState::play)
        //     std::cout << "PLAY" << std::endl;
        // else if (board.currentGameState==GameState::lost)
        //     std::cout << "LOST" << std::endl;

        if(IsMouseButtonPressed(0)) {
            board.triggerEvent(GetMouseX(), GetMouseY());
            //std::cout << GetMouseX() << ", " << GetMouseY() << std::endl;
        }

        if(IsMouseButtonPressed(1)) {
            board.placeFlag(GetMouseX(), GetMouseY());
        }
        

        BeginDrawing();
        ClearBackground(bgColor);
        board.drawBoard();
        board.checkIfWin();
        
        EndDrawing();
    }

    UnloadTexture(Board::flagTexture);
    UnloadTexture(Board::questionMarkTexture);
    CloseWindow(); // Zamknięcie okna gry
    return 0;
}