#ifndef BOARD_H
#define BOARD_H
#include "tile.h"

enum class GameState {setup, play, won, lost};

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
    void placeMines(short, short); // funkcja umieszczająca bomby
    void countNeighbours(); // funkcja licząca bomby sąsiadujące z polem
    void triggerEvent(short, short); // odsłonięcie pola
    void youLost();
};

#endif