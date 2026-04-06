#include "board.h"
#include <raylib.h>
#include <random>
#include <ctime>

Color boardColor = {90, 90, 90, 255};
Color hiddenTileColor = {140, 140, 140, 255};
Color revealedTileColor = {90, 90, 90, 255};
Color mineColor = {255, 55, 55, 255};

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
            if (tileGrid[x][y].currentState == TileState::hidden) {
                if (tileGrid[x][y].isMine)
                    DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, mineColor);
                else
                    DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, hiddenTileColor);
            }
            if (tileGrid[x][y].currentState == TileState::revealed) {
                if (tileGrid[x][y].isMine)
                    DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, mineColor);
                else
                    DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, revealedTileColor);
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

void Board::revealTile (short mouseX, short mouseY) {
    
}