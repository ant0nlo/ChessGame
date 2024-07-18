#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "Board.h"

//g++ -o main main.cpp Board.cpp ChessGame.cpp -Iinclude/SDL2 -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

class ChessGame {
public:
    ChessGame();
    void play();
    const Board& getBoard() const;
    Board& getBoard();
    bool makeMove(int fromX, int fromY, int toX, int toY);
    PieceColor getCurrentPlayer() const;
    void switchPlayer();


private:
    Board board;
    PieceColor currentPlayer;
    void printBoard();
    char pieceSymbol(PieceType type, PieceColor color);
};

#endif // CHESSGAME_H
