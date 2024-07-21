#ifndef CHESSGAME_H
#define CHESSGAME_H

#include "Board.h"

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
