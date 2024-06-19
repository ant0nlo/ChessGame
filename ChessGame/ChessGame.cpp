#include "ChessGame.h"

void ChessGame::play() {
    // Основен игрови цикъл с валидация на входа
    while (true) {
        printBoard();
        std::string from, to;
        std::cout << "Enter the source and destination coordinates (e.g., a2 a4): ";
        std::cin >> from >> to;
        if (from.length() != 2 || to.length() != 2) {
            std::cout << "Invalid input. Please enter coordinates in the format 'a2 a4'.\n";
            continue;
        }
        int fromX = 8 - (from[1] - '0');
        int fromY = from[0] - 'a';
        int toX = 8 - (to[1] - '0');
        int toY = to[0] - 'a';

        if (!board.movePiece(fromX, fromY, toX, toY)) {
            std::cout << "Invalid move. Try again.\n";
        }
    }
}

const Board& ChessGame::getBoard() const {
    return board;
}
Board& ChessGame::getBoard() {  // Добавете тази функция
    return board;
}

void ChessGame::printBoard() {
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            Piece piece = board.getPiece(i, j);
            char symbol = '.';
            if (piece.getType() != PieceType::EMPTY) {
                symbol = pieceSymbol(piece.getType(), piece.getColor());
            }
            std::cout << symbol << ' ';
        }
        std::cout << std::endl;
    }
}

char ChessGame::pieceSymbol(PieceType type, PieceColor color) {
    char symbol;
    switch (type) {
    case PieceType::KING:
        symbol = 'K';
        break;
    case PieceType::QUEEN:
        symbol = 'Q';
        break;
    case PieceType::ROOK:
        symbol = 'R';
        break;
    case PieceType::BISHOP:
        symbol = 'B';
        break;
    case PieceType::KNIGHT:
        symbol = 'N';
        break;
    case PieceType::PAWN:
        symbol = 'P';
        break;
    default:
        symbol = '.';
        break;
    }
    return color == PieceColor::WHITE ? symbol : std::tolower(symbol);
}
