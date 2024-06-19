#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"

class Board {
public:
    Board();
    ~Board();

    Piece getPiece(int x, int y) const;
    bool movePiece(int fromX, int fromY, int toX, int toY);

private:
    Piece** board;
    void initializeBoard();
    bool isMoveValid(int fromX, int fromY, int toX, int toY) const;
    bool isPathClear(int fromX, int fromY, int toX, int toY) const;
    bool isCheck(PieceColor color) const;
};

#endif // BOARD_H
