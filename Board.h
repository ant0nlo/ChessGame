#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <vector>

class Board {
public:
    Board();
    ~Board();

    Piece getPiece(int x, int y) const;
    bool movePiece(int fromX, int fromY, int toX, int toY);
    bool isCheck(PieceColor color) const;
    bool isCheckmate(PieceColor color) const;

private:
    Piece** board;
    std::vector<std::pair<int, int>> enPassantSquares;
    bool canCastle[2][2]; // [color][side], 0 for kingside, 1 for queenside

    void initializeBoard();
    bool isMoveValid(int fromX, int fromY, int toX, int toY) const;
    bool isPathClear(int fromX, int fromY, int toX, int toY) const;
    bool isCastlingMove(int fromX, int fromY, int toX, int toY) const;
    bool isEnPassantMove(int fromX, int fromY, int toX, int toY) const;
    void performCastling(int fromX, int fromY, int toX, int toY);
    void performEnPassant(int fromX, int fromY, int toX, int toY);
    void updateCastlingRights(int fromX, int fromY, int toX, int toY);
    void updateEnPassant(int fromX, int fromY, int toX, int toY);
    bool isSquareUnderAttack(int x, int y, PieceColor attackingColor) const;
};

#endif // BOARD_H*/