#ifndef PIECE_H
#define PIECE_H

#include <iostream>

enum class PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY };
enum class PieceColor { WHITE, BLACK, NONE };

class Piece {
public:
    Piece(PieceType type = PieceType::EMPTY, PieceColor color = PieceColor::NONE)
        : type(type), color(color) {}
    PieceType getType() const { return type; }
    PieceColor getColor() const { return color; }

private:
    PieceType type;
    PieceColor color;
};

#endif // PIECE_H
