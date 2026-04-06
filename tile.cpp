#include "tile.h"

Tile::Tile () {
    neighborCount = 0;
    isMine = false;
    currentState = TileState::hidden;
}