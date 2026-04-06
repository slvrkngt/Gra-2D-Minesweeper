#ifndef BOARD_H
#define BOARD_H
#include "tile.h"

class Board {
public:
    Tile tileGrid [9][9];
    short boardSize;
    short tileSize;
    Board ();
};

#endif