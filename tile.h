#ifndef TILE_H
#define TILE_H

enum class TileState {hidden, revealed, flagged, questionMark};

class Tile {
public:
    short neighborCount;
    bool isMine;
    TileState currentState;
    Tile ();
    void reveal();
    void flag();
};

#endif