#include "board.h"
#include <raylib.h>
#include <random>
#include <ctime>
#include <iostream>

Color boardColor = {90, 90, 90, 255};
Color hiddenTileColor = {140, 140, 140, 255};
Color revealedTileColor = {100, 100, 100, 255};
Color mineColor = {255, 55, 55, 255};
Color lostMessageBoxColor = {255, 55, 55, 100};
Color flagColor = {255, 55, 55, 255};

bool showMines = true; // widoczność bomb po odpaleniu okna gry - do testowania

Board::Board () {
    boardSize = 540;
    tileSize = boardSize/9 - 2; // -2 bo chcę zrobić margines
    currentGameState = GameState::setup;
}

Board::~Board () {
}

Texture2D Board::flagTexture;
short Board::revealedTiles;

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
            
            if (tileGrid[x][y].currentState == TileState::hidden) { // tile ukryty
                DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, hiddenTileColor);
                if (showMines) {
                    if (tileGrid[x][y].isMine)
                        DrawCircle(mineOriginX, mineOriginY, tileSize/10, BLACK);
                    if (tileGrid[x][y].neighborCount > 0) {
                        std::string nbCount = std::to_string(tileGrid[x][y].neighborCount);
                        DrawText(nbCount.c_str(), tileOriginX+5, tileOriginY+5, tileSize/2, BLACK);
                    }
                }
                    
                
            }
            if (tileGrid[x][y].currentState == TileState::revealed) { // tile odsłonięty
                DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, revealedTileColor);
                if (tileGrid[x][y].isMine)
                    DrawCircle(mineOriginX, mineOriginY, tileSize/3, mineColor);
                if (tileGrid[x][y].neighborCount > 0) {
                    std::string nbCount = std::to_string(tileGrid[x][y].neighborCount);
                    DrawText(nbCount.c_str(), tileOriginX+15, tileOriginY+5, tileSize-10, BLACK);
                }
            }
            if (tileGrid[x][y].currentState == TileState::flagged) { // tile oflagowany
                DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, hiddenTileColor);
                DrawTextureEx(flagTexture, {(float)tileOriginX, (float)tileOriginY}, 0, 3.5, flagColor);

                if (showMines) {
                    if (tileGrid[x][y].isMine)
                        DrawCircle(mineOriginX, mineOriginY, tileSize/10, BLACK);
                    if (tileGrid[x][y].neighborCount > 0) {
                        std::string nbCount = std::to_string(tileGrid[x][y].neighborCount);
                        DrawText(nbCount.c_str(), tileOriginX+5, tileOriginY+5, tileSize/2, BLACK);
                    }
                }
            }
        }
    }

    if(currentGameState == GameState::lost) {
        youLost();
    }
}

void Board::placeMines(short tilePosX, short tilePosY) { // ustawienie bomb, nie mogą być ustawione na polu od którego rozpoczynamy grę
    for(short x=0; x<9; ++x) { // reset bomb
        for(short y=0; y<9; ++y) { 
            tileGrid[x][y].isMine = false;
        }
    }
    srand( time( NULL ) );
    short minesToPlace = 10;
    while (minesToPlace) {
        short randX =( std::rand() % 9 ) + 0;
        short randY =( std::rand() % 9 ) + 0;
        bool placementCondition = !(
            (randX == tilePosX && randY == tilePosY+1)
            || (randX == tilePosX && randY == tilePosY-1)
            || (randX == tilePosX+1 && randY == tilePosY)
            || (randX == tilePosX+1 && randY == tilePosY+1)
            || (randX == tilePosX+1 && randY == tilePosY-1)
            || (randX == tilePosX-1 && randY == tilePosY)
            || (randX == tilePosX-1 && randY == tilePosY+1)
            || (randX == tilePosX-1 && randY == tilePosY-1)
        );
        if(tileGrid[randX][randY].isMine == false && (randX != tilePosX && randY != tilePosY) && placementCondition){
            tileGrid[randX][randY].isMine = true;
            --minesToPlace;
        }
    }
}

void Board::countNeighbours() { // liczenie sąsiadów - bomb
    for(short x=0; x<9; ++x) {
        for(short y=0; y<9; ++y) { 
            tileGrid[x][y].neighborCount = 0;

            if (x>0 && tileGrid[x-1][y].isMine) { // prawa
                ++tileGrid[x][y].neighborCount;
            }

            if (x<8 && tileGrid[x+1][y].isMine) { // lewa
                ++tileGrid[x][y].neighborCount;
            }

            if (y>0 && tileGrid[x][y-1].isMine) { // góra
                ++tileGrid[x][y].neighborCount;
            }

            if (y<8 && tileGrid[x][y+1].isMine) { // dół
                ++tileGrid[x][y].neighborCount;
            }

            if (x>0 && y>0 && tileGrid[x-1][y-1].isMine) { // prawa-góra
                ++tileGrid[x][y].neighborCount;
            }

            if (x>0 && y<8 && tileGrid[x-1][y+1].isMine) { // prawa-dół
                ++tileGrid[x][y].neighborCount;
            }

            if (x<8 && y>0 && tileGrid[x+1][y-1].isMine) { // lewa-góra
                ++tileGrid[x][y].neighborCount;
            }

            if (x<8 && y<8 && tileGrid[x+1][y+1].isMine) { // lewa-dół
                ++tileGrid[x][y].neighborCount;
            }

            if (tileGrid[x][y].isMine) {
               tileGrid[x][y].neighborCount = -1; // jeżeli pole jest bombą, neighborCount = -1
            }

            // std::cout << tileGrid[x][y].neighborCount << std::endl;
        }
    }

}

// void Board::revealNeighbouring(short tileX, short tileY) {
//     bool stopCondition = false;

//     for(short x=tileX+1; x<9; ++x) {
//         if (!stopCondition)
//             tileGrid[x][tileY].currentState = TileState::revealed;
//         if (tileGrid[x][tileY].neighborCount!=0) 
//             stopCondition=true;
//     }

//     stopCondition = false;

//     for(short x=tileX-1; x>0; --x) {
//         if (!stopCondition)
//             tileGrid[x][tileY].currentState = TileState::revealed;
//         if (tileGrid[x][tileY].neighborCount!=0) 
//             stopCondition=true;
//     }
// }

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
           countNeighbours();
            currentGameState = GameState::play;
        }
    
        tileGrid[targetTileX][targetTileY].reveal();
        // revealNeighbouring(targetTileX, targetTileY);
        
        
        if(currentGameState == GameState::play) { // co się dzieje w trakcie gry
            if(tileGrid[targetTileX][targetTileY].isMine) {
                currentGameState = GameState::lost;
            }
        }
    }
    
}

void Board::placeFlag (short mouseX, short mouseY) { // stawianie flagi
    // jeżeli kliknięto w obrębie planszy
    bool boardClicked = mouseX >= originX && mouseX <= originX + boardSize && mouseY >= originY && mouseY <= originY + boardSize;
    if (boardClicked) {
        mouseX -= originX;
        mouseY -= originY;
        // obliczenie na jakim Tile'u kliknięto
        short targetTileX = (short)(mouseX / (tileSize+2)); 
        short targetTileY = (short)(mouseY / (tileSize+2));

        if (tileGrid[targetTileX][targetTileY].currentState == TileState::hidden)
            tileGrid[targetTileX][targetTileY].currentState = TileState::flagged;
        else if (tileGrid[targetTileX][targetTileY].currentState == TileState::flagged)
            tileGrid[targetTileX][targetTileY].currentState = TileState::hidden;
    }
}

void Board::youLost() {
    short messageBoxPosX = boardSize/5;
    short messageBoxPosY = boardSize/5;
    DrawRectangle(messageBoxPosX, messageBoxPosY, 150, 40, lostMessageBoxColor);
    DrawText("You Lost!", messageBoxPosX+5, messageBoxPosY+5, 30, WHITE);
}


