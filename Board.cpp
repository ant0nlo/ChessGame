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
    canCastle[0][0] = canCastle[0][1] = canCastle[1][0] = canCastle[1][1] = true;

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
        Piece movingPiece = board[fromX][fromY];
        
        if (isCastlingMove(fromX, fromY, toX, toY)) {
            performCastling(fromX, fromY, toX, toY);
        } else if (isEnPassantMove(fromX, fromY, toX, toY)) {
            performEnPassant(fromX, fromY, toX, toY);
        } else {
            board[toX][toY] = movingPiece;
            board[fromX][fromY] = Piece(PieceType::EMPTY, PieceColor::NONE);
        }

        updateCastlingRights(fromX, fromY, toX, toY);
        updateEnPassant(fromX, fromY, toX, toY);
        
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

bool Board::isMoveValid(int fromX, int fromY, int toX, int toY) const {
    // Проверка дали ходът е в рамките на дъската
    if (fromX < 0 || fromX >= 8 || fromY < 0 || fromY >= 8 ||
        toX < 0 || toX >= 8 || toY < 0 || toY >= 8) {
        return false;
    }

    Piece piece = board[fromX][fromY];
    Piece targetPiece = board[toX][toY];

    // Проверка дали фигурата не се опитва да вземе фигура от същия цвят
    if (targetPiece.getColor() == piece.getColor() && targetPiece.getType() != PieceType::EMPTY) {
        return false;
    }

    if (isCastlingMove(fromX, fromY, toX, toY) || isEnPassantMove(fromX, fromY, toX, toY)) {
        return true;
    }

    int dx = toX - fromX;
    int dy = toY - fromY;

    switch (piece.getType()) {
        case PieceType::PAWN:
            // Пешка
            if (piece.getColor() == PieceColor::WHITE) {
                if (fromX == 1 && dx == 2 && dy == 0 && targetPiece.getType() == PieceType::EMPTY) {
                    return isPathClear(fromX, fromY, toX, toY);
                }
                if (dx == 1 && dy == 0 && targetPiece.getType() == PieceType::EMPTY) {
                    return true;
                }
                if (dx == 1 && abs(dy) == 1 && targetPiece.getType() != PieceType::EMPTY) {
                    return true;
                }
            } else { // BLACK
                if (fromX == 6 && dx == -2 && dy == 0 && targetPiece.getType() == PieceType::EMPTY) {
                    return isPathClear(fromX, fromY, toX, toY);
                }
                if (dx == -1 && dy == 0 && targetPiece.getType() == PieceType::EMPTY) {
                    return true;
                }
                if (dx == -1 && abs(dy) == 1 && targetPiece.getType() != PieceType::EMPTY) {
                    return true;
                }
            }
            return false;

        case PieceType::ROOK:
            // Топ
            if ((dx == 0 || dy == 0) && isPathClear(fromX, fromY, toX, toY)) {
                return true;
            }
            return false;

        case PieceType::KNIGHT:
            // Кон
            if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) {
                return true;
            }
            return false;

        case PieceType::BISHOP:
            // Офицер
            if (abs(dx) == abs(dy) && isPathClear(fromX, fromY, toX, toY)) {
                return true;
            }
            return false;

        case PieceType::QUEEN:
            // Царица
            if ((dx == 0 || dy == 0 || abs(dx) == abs(dy)) && isPathClear(fromX, fromY, toX, toY)) {
                return true;
            }
            return false;

        case PieceType::KING:
            // Цар
            if (abs(dx) <= 1 && abs(dy) <= 1) {
                return true;
            }
            return false;

        default:
            return false;
    }
    
}

bool Board::isCastlingMove(int fromX, int fromY, int toX, int toY) const {
    Piece king = board[fromX][fromY];
    if (king.getType() != PieceType::KING) return false;

    int colorIndex = (king.getColor() == PieceColor::WHITE) ? 0 : 1;
    int row = (king.getColor() == PieceColor::WHITE) ? 0 : 7;

    // Проверка за кингсайд рокада
    if (toY == fromY + 2 && canCastle[colorIndex][0] &&
        board[row][5].getType() == PieceType::EMPTY &&
        board[row][6].getType() == PieceType::EMPTY &&
        !isSquareUnderAttack(row, 4, king.getColor()) &&
        !isSquareUnderAttack(row, 5, king.getColor()) &&
        !isSquareUnderAttack(row, 6, king.getColor())) {
        return true;
    }

    // Проверка за куинсайд рокада
    if (toY == fromY - 2 && canCastle[colorIndex][1] &&
        board[row][3].getType() == PieceType::EMPTY &&
        board[row][2].getType() == PieceType::EMPTY &&
        board[row][1].getType() == PieceType::EMPTY &&
        !isSquareUnderAttack(row, 4, king.getColor()) &&
        !isSquareUnderAttack(row, 3, king.getColor()) &&
        !isSquareUnderAttack(row, 2, king.getColor())) {
        return true;
    }

    return false;
}

bool Board::isEnPassantMove(int fromX, int fromY, int toX, int toY) const {
    Piece pawn = board[fromX][fromY];
    if (pawn.getType() != PieceType::PAWN) return false;

    int direction = (pawn.getColor() == PieceColor::WHITE) ? 1 : -1;
    
    if (fromX + direction == toX && abs(toY - fromY) == 1) {
        for (const auto& square : enPassantSquares) {
            if (square.first == toX && square.second == toY) {
                return true;
            }
        }
    }

    return false;
}

void Board::performCastling(int fromX, int fromY, int toX, int toY) {
    // Преместване на царя
    board[toX][toY] = board[fromX][fromY];
    board[fromX][fromY] = Piece(PieceType::EMPTY, PieceColor::NONE);

    // Преместване на топа
    if (toY > fromY) { // Кингсайд рокада
        board[toX][toY-1] = board[toX][7];
        board[toX][7] = Piece(PieceType::EMPTY, PieceColor::NONE);
    } else { // Куинсайд рокада
        board[toX][toY+1] = board[toX][0];
        board[toX][0] = Piece(PieceType::EMPTY, PieceColor::NONE);
    }
}

void Board::performEnPassant(int fromX, int fromY, int toX, int toY) {
    // Преместване на пешката
    board[toX][toY] = board[fromX][fromY];
    board[fromX][fromY] = Piece(PieceType::EMPTY, PieceColor::NONE);

    // Премахване на взетата пешка
    board[fromX][toY] = Piece(PieceType::EMPTY, PieceColor::NONE);
}

void Board::updateCastlingRights(int fromX, int fromY, int toX, int toY) {
    Piece movingPiece = board[toX][toY];
    
    if (movingPiece.getType() == PieceType::KING) {
        int colorIndex = (movingPiece.getColor() == PieceColor::WHITE) ? 0 : 1;
        canCastle[colorIndex][0] = canCastle[colorIndex][1] = false;
    } else if (movingPiece.getType() == PieceType::ROOK) {
        int colorIndex = (movingPiece.getColor() == PieceColor::WHITE) ? 0 : 1;
        int row = (movingPiece.getColor() == PieceColor::WHITE) ? 0 : 7;
        if (fromY == 0) canCastle[colorIndex][1] = false; // Куинсайд топ
        if (fromY == 7) canCastle[colorIndex][0] = false; // Кингсайд топ
    }
}

void Board::updateEnPassant(int fromX, int fromY, int toX, int toY) {
    enPassantSquares.clear();
    
    Piece movingPiece = board[toX][toY];
    if (movingPiece.getType() == PieceType::PAWN && abs(toX - fromX) == 2) {
        int enPassantX = (fromX + toX) / 2;
        enPassantSquares.push_back({enPassantX, toY});
    }
}

bool Board::isSquareUnderAttack(int x, int y, PieceColor attackingColor) const {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].getColor() == attackingColor && isMoveValid(i, j, x, y)) {
                return true;
            }
        }
    }
    return false;
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
    // Намиране на позицията на царя
    int kingX = -1, kingY = -1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j].getType() == PieceType::KING && board[i][j].getColor() == color) {
                kingX = i;
                kingY = j;
                break;
            }
        }
        if (kingX != -1) break;
    }

    PieceColor opponentColor = (color == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    return isSquareUnderAttack(kingX, kingY, opponentColor);
}

bool Board::isCheckmate(PieceColor color) const {
    if (!isCheck(color)) return false;

    // Проверка за всички възможни ходове
    for (int fromX = 0; fromX < 8; fromX++) {
        for (int fromY = 0; fromY < 8; fromY++) {
            if (board[fromX][fromY].getColor() == color) {
                for (int toX = 0; toX < 8; toX++) {
                    for (int toY = 0; toY < 8; toY++) {
                        if (isMoveValid(fromX, fromY, toX, toY)) {
                            // Симулиране на хода
                            Board tempBoard = *this;
                            tempBoard.movePiece(fromX, fromY, toX, toY);
                            
                            // Ако този ход премахва шаха, значи не е мат
                            if (!tempBoard.isCheck(color)) {
                                return false;
                            }
                        }
                    }
                }
            }
        }
    }

    return true;
}


