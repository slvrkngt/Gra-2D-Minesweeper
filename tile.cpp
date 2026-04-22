#include "tile.h"
#include "board.h"

Tile::Tile () {
    neighborCount = 0;
    isMine = false;
    currentState = TileState::hidden;
}

void Tile::reveal () {
    if(currentState != TileState::revealed)
    {
        currentState = TileState::revealed;
        Board::revealedTiles += 1;
    }
    
}