#include "tile.h"
#include "board.h"
#include <iostream>

Tile::Tile () {
    neighborCount = 0;
    isMine = false;
    currentState = TileState::hidden;
}

void Tile::reveal () {
    if(currentState != TileState::revealed)
    {
        currentState = TileState::revealed;
        if(!isMine)
            Board::revealedTiles += 1;
            // std::cout << Board::revealedTiles << std::endl;
        
    }
    
}

void Tile::flag () {
    if(currentState == TileState::hidden)
    {
        currentState = TileState::flagged;
    }
    else if (currentState == TileState::flagged)
    {
        currentState = TileState::questionMark;
    }
    else if (currentState == TileState::questionMark)
    {
        currentState = TileState::hidden;
    }
}