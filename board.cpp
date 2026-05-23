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
Color wonMessageBoxColor = {30, 180, 30, 100};
Color flagColor = {255, 55, 55, 255};

bool showMines = false; // widoczność bomb po odpaleniu okna gry - do testowania

Board::Board () {
    originX = 10;
    originY = 10;
    boardSize = 540;
    tileSize = boardSize/9 - 2; // -2 bo chcę zrobić margines
    currentGameState = GameState::setup;

    gameTimer = 0.0f;
    fullSeconds = 0;

    resetButtonOriginX = originX + tileSize *4 + 20;
    resetButtonOriginY = originY + boardSize + 20;
    resetButtonSize = tileSize +40;

}

Board::~Board () {
}

Texture2D Board::flagTexture;
Texture2D Board::questionMarkTexture; 
Texture2D Board::smileyFaceTexture;
Texture2D Board::winFaceTexture;
Texture2D Board::loseFaceTexture;
Texture2D Board::shockedFaceTexture;
Texture2D Board::currentFaceTexture;
short Board::revealedTiles;
short Board::flaggedTiles;

void Board::drawBoard () {

    if (shockedTimer > 0.0f) {
        shockedTimer -= GetFrameTime(); 
        
        if (shockedTimer <= 0.0f && currentGameState == GameState::play) {
            Board::currentFaceTexture = Board::smileyFaceTexture;
        }
    }

    if (currentGameState == GameState::play) {
        gameTimer += GetFrameTime();
        fullSeconds = (short)gameTimer; 
    }

    
    DrawRectangle(originX, originY, boardSize, boardSize, boardColor);

    // plansza gry
    for(short x=0; x<9; ++x) {
        for(short y=0; y<9; ++y) {
            short tileOriginX = originX + (boardSize/9) * x + 1;
            short tileOriginY = originY + (boardSize/9) * y + 1;
            short mineOriginX = originX + (boardSize/9) * x + 1 + tileSize/2;
            short mineOriginY = originY + (boardSize/9) * y + 1 + tileSize/2;
            
            // =============UKRYTE============
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
                    
            // =============ODKRYTE============    
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

            // =============OFLAGOWANE============
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

            // =============ZNAK ZAPYTANIA============
            if (tileGrid[x][y].currentState == TileState::questionMark) { // tile oflagowany
                DrawRectangle(tileOriginX, tileOriginY, tileSize, tileSize, hiddenTileColor);
                DrawTextureEx(questionMarkTexture, {(float)tileOriginX, (float)tileOriginY}, 0, 3.5, WHITE);

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

    // mine counter
    std::string mineCounterText = "Mines left: " + std::to_string(10-Board::flaggedTiles); 
    DrawRectangle(originX, originY+boardSize+10, tileSize*4, tileSize/2+20, revealedTileColor);
    DrawText(mineCounterText.c_str(), originX +10, originY+boardSize+20, tileSize/2, BLACK);

    //timer
    std::string timerText = "Time: " + std::to_string(fullSeconds);
    DrawRectangle(originX, originY+boardSize+20 + tileSize/2+20, tileSize*4, tileSize/2+20, revealedTileColor);
    DrawText(timerText.c_str(), originX + 10, originY+boardSize+30 + tileSize/2+20, tileSize / 2, BLACK);

    // reset uśmiechnięta buźka
    DrawRectangle(resetButtonOriginX, resetButtonOriginY, resetButtonSize, resetButtonSize, revealedTileColor);
    DrawTextureEx(currentFaceTexture, {(float)resetButtonOriginX, (float)resetButtonOriginY}, 0, 4, WHITE);


    if(currentGameState == GameState::lost) {
        youLost();
    }
}

void Board::drawDetails () {

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



void Board::triggerEvent (short mouseX, short mouseY) { // główna funkcja odpowiadająca za wydarzenia w grze

    bool resetClicked = mouseX >= resetButtonOriginX && mouseX <= resetButtonOriginX + resetButtonSize && 
                        mouseY >= resetButtonOriginY && mouseY <= resetButtonOriginY + resetButtonSize;

    if (resetClicked) {
        resetGame();
        return; 
    }

    // jeżeli kliknięto w obrębie planszy
    bool boardClicked = mouseX >= originX && mouseX <= originX + boardSize && mouseY >= originY && mouseY <= originY + boardSize;
    if (boardClicked) {

        if (currentGameState == GameState::setup || currentGameState == GameState::play) {
            Board::currentFaceTexture = Board::shockedFaceTexture;
            shockedTimer = 0.2f; // Ustawiamy stoper na pół sekundy
        }

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
    
        // tileGrid[targetTileX][targetTileY].reveal();
        revealNeighbouring(targetTileX, targetTileY);
        
        
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

        tileGrid[targetTileX][targetTileY].flag();
    }
}

void Board::youLost() {
    short messageBoxPosX = boardSize/5;
    short messageBoxPosY = boardSize/5;
    DrawRectangle(messageBoxPosX, messageBoxPosY, 150, 40, lostMessageBoxColor);
    DrawText("You Lost!", messageBoxPosX+5, messageBoxPosY+5, 30, WHITE);
    Board::currentFaceTexture = Board::loseFaceTexture;
}

void Board::checkIfWin() {
    if(Board::revealedTiles == 71 && currentGameState != GameState::lost) {
        currentGameState = GameState::won;
        short messageBoxPosX = boardSize/5;
        short messageBoxPosY = boardSize/5;
        DrawRectangle(messageBoxPosX, messageBoxPosY, 150, 40, wonMessageBoxColor);
        DrawText("You Won!", messageBoxPosX+5, messageBoxPosY+5, 30, WHITE);
        Board::currentFaceTexture = Board::winFaceTexture;
    }
}

void Board::revealNeighbouring(short tileX, short tileY) {
    if (tileX < 0 || tileX >= 9 || tileY < 0 || tileY >= 9) {
        return;
    }

    Tile& currentTile = tileGrid[tileX][tileY];

    if (currentTile.currentState != TileState::hidden && currentTile.currentState != TileState::questionMark) {
        return;
    }

    currentTile.reveal();

    if (currentTile.neighborCount > 0) {
        return;
    }

    for (short dx = -1; dx <= 1; ++dx) {
        for (short dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            revealNeighbouring(tileX + dx, tileY + dy);
        }
    }
}

void Board::resetGame() {
    currentGameState = GameState::setup;
    gameTimer = 0.0f;
    fullSeconds = 0;
    Board::revealedTiles = 0;
    Board::flaggedTiles = 0;
    Board::currentFaceTexture = Board::smileyFaceTexture;

    for (short x = 0; x < 9; ++x) {
        for (short y = 0; y < 9; ++y) {
            tileGrid[x][y].isMine = false;
            tileGrid[x][y].neighborCount = 0;
            tileGrid[x][y].currentState = TileState::hidden;
        }
    }
}


