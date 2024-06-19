#include "Board.h"

Board::Board() {
    board = new Piece * [8];
    for (int i = 0; i < 8; ++i) {
        board[i] = new Piece[8];
        for (int j = 0; j < 8; ++j) {
            board[i][j] = Piece(PieceType::EMPTY, PieceColor::NONE);
        }
    }
    initializeBoard();
}

Board::~Board() {
    for (int i = 0; i < 8; ++i) {
        delete[] board[i];
    }
    delete[] board;
}

Piece Board::getPiece(int x, int y) const {
    return board[x][y];
}

bool Board::movePiece(int fromX, int fromY, int toX, int toY) {
    if (isMoveValid(fromX, fromY, toX, toY)) {
        board[toX][toY] = board[fromX][fromY];
        board[fromX][fromY] = Piece(PieceType::EMPTY, PieceColor::NONE);
        return true;
    }
    return false;
}

void Board::initializeBoard() {
    // Initialize pawns
    for (int i = 0; i < 8; ++i) {
        board[6][i] = Piece(PieceType::PAWN, PieceColor::BLACK);
        board[1][i] = Piece(PieceType::PAWN, PieceColor::WHITE);
    }

    // Initialize other pieces
    board[7][0] = board[7][7] = Piece(PieceType::ROOK, PieceColor::BLACK);
    board[0][0] = board[0][7] = Piece(PieceType::ROOK, PieceColor::WHITE);

    board[7][1] = board[7][6] = Piece(PieceType::KNIGHT, PieceColor::BLACK);
    board[0][1] = board[0][6] = Piece(PieceType::KNIGHT, PieceColor::WHITE);
    board[7][2] = board[7][5] = Piece(PieceType::BISHOP, PieceColor::BLACK);
    board[0][2] = board[0][5] = Piece(PieceType::BISHOP, PieceColor::WHITE);

    board[7][3] = Piece(PieceType::QUEEN, PieceColor::BLACK);
    board[0][3] = Piece(PieceType::QUEEN, PieceColor::WHITE);
    board[7][4] = Piece(PieceType::KING, PieceColor::BLACK);
    board[0][4] = Piece(PieceType::KING, PieceColor::WHITE);
}

bool Board::isMoveValid (int fromX, int fromY, int toX, int toY) const {
    // Пример: проверка дали ходът е в рамките на дъската
    if (fromX < 0 || fromX >= 8 || fromY < 0 || fromY >= 8 ||
        toX < 0 || toX >= 8 || toY < 0 || toY >= 8) {
        return false;
    }
    // Още правила за валидация могат да бъдат добавени тук
    return true;
}

bool Board::isPathClear(int fromX, int fromY, int toX, int toY) const {
    int dx = toX - fromX;
    int dy = toY - fromY;
    int steps = std::max(abs(dx), abs(dy));
    int stepX = dx == 0 ? 0 : dx / abs(dx);
    int stepY = dy == 0 ? 0 : dy / abs(dy);

    for (int i = 1; i < steps; ++i) {
        int x = fromX + i * stepX;
        int y = fromY + i * stepY;
        if (board[x][y].getType() != PieceType::EMPTY) {
            return false;
        }
    }
    return true;
}

bool Board::isCheck(PieceColor color) const {
    // Find the king's position
    int kingX = -1, kingY = -1;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece piece = board[i][j];
            if (piece.getType() == PieceType::KING && piece.getColor() == color) {
                kingX = i;
                kingY = j;
                break;
            }
        }
        if (kingX != -1) break;
    }

    // Check all opponent pieces to see if any can move to the king's position
    PieceColor opponentColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece piece = board[i][j];
            if (piece.getColor() == opponentColor) {
                if (isMoveValid(i, j, kingX, kingY)) {
                    return true;
                }
            }
        }
    }

    return false;
}
