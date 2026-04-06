#include "board.h"
#include <raylib.h>
#include <random>
#include <ctime>

Color boardColor = {90, 90, 90, 255};
Color hiddenTileColor = {140, 140, 140, 255};
Color revealedTileColor = {100, 100, 100, 255};
Color mineColor = {255, 55, 55, 255};
Color lostMessageBoxColor = {255, 55, 55, 100};

bool showMines = false; // widoczność bomb po odpaleniu okna gry - do testowania

Board::Board () {
    boardSize = 540;
    tileSize = boardSize/9 - 2; // -2 bo chcę zrobić margines
    currentGameState = GameState::setup;
}

void Board::drawBoard () {
    originX = 10;
    originY = 10;
    DrawRectangle(originX, originY, boardSize, boardSize, boardColor);
    for(short x=0; x<9; ++x) {
        for(short y=0; y<9; ++y) {
            short tileOriginX = originX + (boardSize/9) * x + 1;
            short tileOriginY = originY + (boardSize/9) * y + 1;
            short mineOriginX = originX + (boardSize/9) * x + 1 + tileSize/2;
            short mineOriginY = originY + (boardSize/9) * y + 1 + tileSize/2;
            
            if (tileGrid[x][y].currentState == TileState::hidden) {
                DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, hiddenTileColor);
                if (tileGrid[x][y].isMine && showMines)
                    DrawCircle(mineOriginX, mineOriginY, tileSize/10, BLACK);
            }
            if (tileGrid[x][y].currentState == TileState::revealed) {
                DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, revealedTileColor);
                if (tileGrid[x][y].isMine)
                    DrawCircle(mineOriginX, mineOriginY, tileSize/3, mineColor);
            }
        }
    }

    if(currentGameState == GameState::lost) {
        youLost();
    }
}

void Board::placeMines(short tilePosX, short tilePosY) { // ustawienie bomb, nie mogą być ustawione na polu od którego rozpoczynamy grę
    srand( time( NULL ) );
    short minesToPlace = 10;
    while (minesToPlace) {
        short randX =( std::rand() % 9 ) + 0;
        short randY =( std::rand() % 9 ) + 0;
        if(tileGrid[randX][randY].isMine == false && randX != tilePosX && randY != tilePosY){
            tileGrid[randX][randY].isMine = true;
            --minesToPlace;
        }
    }
}

void Board::triggerEvent (short mouseX, short mouseY) { // główna funkcja odpowiadająca za wydarzenia w grze

    // jeżeli kliknięto w obrębie planszy
    bool boardClicked = mouseX >= originX && mouseX <= originX + boardSize && mouseY >= originY && mouseY <= originY + boardSize;
    if (boardClicked) {
        mouseX -= originX;
        mouseY -= originY;
        // obliczenie na jakim Tile'u kliknięto
        short targetTileX = (short)(mouseX / (tileSize+2)); 
        short targetTileY = (short)(mouseY / (tileSize+2));

        if(currentGameState == GameState::setup) { // co się dzieje przed rozpoczęciem gry
           placeMines(targetTileX, targetTileY);
            currentGameState = GameState::play;
        }
    
        tileGrid[targetTileX][targetTileY].currentState = TileState::revealed;
        
        if(currentGameState == GameState::play) { // co się dzieje w trakcie gry
            if(tileGrid[targetTileX][targetTileY].isMine) {
                currentGameState = GameState::lost;
            }
        }
    }
    
}

void Board::youLost() {
    short messageBoxPosX = boardSize/5;
    short messageBoxPosY = boardSize/5;
    DrawRectangle(messageBoxPosX, messageBoxPosY, 150, 40, lostMessageBoxColor);
    DrawText("You Lost!", messageBoxPosX+5, messageBoxPosY+5, 30, WHITE);
}