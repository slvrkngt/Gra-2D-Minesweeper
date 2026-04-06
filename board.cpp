#include "board.h"
#include <raylib.h>
#include <random>
#include <ctime>

Color boardColor = {90, 90, 90, 255};
Color hiddenTileColor = {140, 140, 140, 255};
Color revealedTileColor = {100, 100, 100, 255};
Color mineColor = {255, 55, 55, 255};

bool showMines = false; // widoczność bomb po odpaleniu okna gry - do testowania

Board::Board () {
    boardSize = 540;
    tileSize = boardSize/9 - 2; // -2 bo chcę zrobić margines
    currentGameState = GameState::play;
}

void Board::drawBoard () {
    short originX = 10;
    short originY = 10;
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
}

void Board::placeMines() {
    srand( time( NULL ) );
    short minesToPlace = 10;
    while (minesToPlace) {
        short randX =( std::rand() % 9 ) + 0;
        short randY =( std::rand() % 9 ) + 0;
        if(tileGrid[randX][randY].isMine == false){
            tileGrid[randX][randY].isMine = true;
            --minesToPlace;
        }
    }
}

void Board::triggerEvent (short mouseX, short mouseY) {
    mouseX -= originX;
    mouseY -= originY;
    short targetTileX = (short)(mouseX / (tileSize+2));
    short targetTileY = (short)(mouseY / (tileSize+2));
    tileGrid[targetTileX][targetTileY].currentState = TileState::revealed;
}