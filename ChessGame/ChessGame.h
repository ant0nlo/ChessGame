#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "Board.h"
#include <iostream>
#include <string>
//g++ -o main main.cpp Board.cpp ChessGame.cpp -Iinclude/SDL2 -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
class ChessGame {
public:
    void play();
    const Board& getBoard() const;
    Board& getBoard();  // Добавете тази линия


private:
    Board board;
    void printBoard();
    char pieceSymbol(PieceType type, PieceColor color);
};

#endif // CHESSGAME_H
