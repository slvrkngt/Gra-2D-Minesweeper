#ifndef BOARD_H
#define BOARD_H
#include "tile.h"
#include <raylib.h>

enum class GameState {setup, play, won, lost};

class Board {
public:
    Tile tileGrid [9][9];
    GameState currentGameState;
    short boardSize;
    short originX;
    short originY;
    short tileSize;
    static short revealedTiles;
    static short flaggedTiles;
    static Texture2D flagTexture; 
    static Texture2D questionMarkTexture; 
    Board ();
    ~Board ();
    void drawBoard(); // funkcja rysująca planszę
    void drawDetails(); // funkcja rysująca szczegóły
    void placeMines(short, short); // funkcja umieszczająca bomby
    void countNeighbours(); // funkcja licząca bomby sąsiadujące z polem
    void revealNeighbouring(short, short); // funkcja odsłaniająca sąsiadujące puste pola
    void triggerEvent(short, short); // odsłonięcie pola
    void placeFlag(short, short);
    void youLost();
    void checkIfWin();

};

#endif