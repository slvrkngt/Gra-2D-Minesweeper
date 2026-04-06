#ifndef BOARD_H
#define BOARD_H
#include "tile.h"

enum class GameState {play, won, lost};

class Board {
public:
    Tile tileGrid [9][9];
    GameState currentGameState;
    short boardSize;
    short tileSize;
    Board ();
    void drawBoard(); // funkcja rysująca planszę
    void placeMines(); // funkcja umieszczająca bomby
    void revealTile(short, short);
};

#endif