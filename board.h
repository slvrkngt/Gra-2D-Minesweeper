#ifndef BOARD_H
#define BOARD_H
#include "tile.h"

enum class GameState {play, won, lost};

class Board {
public:
    Tile tileGrid [9][9];
    GameState currentGameState;
    short boardSize;
    short originX;
    short originY;
    short tileSize;
    Board ();
    void drawBoard(); // funkcja rysująca planszę
    void placeMines(); // funkcja umieszczająca bomby
    void triggerEvent(short, short); // odsłonięcie pola
};

#endif