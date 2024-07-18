#include "ChessGame.h"

ChessGame::ChessGame() : currentPlayer(PieceColor::WHITE) {};

void ChessGame::play() {
    while (true) {
        printBoard();
        std::string from, to;
        std::cout << (currentPlayer == PieceColor::WHITE ? "White" : "Black") << "'s turn. Enter move (e.g., a2 a4): ";
        std::cin >> from >> to;
        if (from.length() != 2 || to.length() != 2) {
            std::cout << "Invalid input. Please enter coordinates in the format 'a2 a4'.\n";
            continue;
        }
        int fromX = 8 - (from[1] - '0');
        int fromY = from[0] - 'a';
        int toX = 8 - (to[1] - '0');
        int toY = to[0] - 'a';

        if (makeMove(fromX, fromY, toX, toY)) {
            switchPlayer();
        } else {
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

PieceColor ChessGame::getCurrentPlayer() const {
    return currentPlayer;
}

void ChessGame::switchPlayer() {
    currentPlayer = (currentPlayer == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
}
/*
bool ChessGame::makeMove(int fromX, int fromY, int toX, int toY) {
    Piece piece = board.getPiece(fromX, fromY);
    if (piece.getColor() != currentPlayer) {
        std::cout << "It's not your turn!\n";
        return false;
    }
    return board.movePiece(fromX, fromY, toX, toY);
}
*/
/*
bool ChessGame::makeMove(int fromX, int fromY, int toX, int toY) {
    Piece piece = board.getPiece(fromX, fromY);
    if (piece.getColor() != currentPlayer) {
        std::cout << "It's not your turn!\n";
        return false;
    }
    
    if (board.movePiece(fromX, fromY, toX, toY)) {
        // Check if the move puts the current player in check
        if (board.isCheck(currentPlayer)) {
            std::cout << "Check!" << std::endl;
            if(!board.isCheck(currentPlayer))
            {
               board.movePiece(toX, toY, fromX, fromY);
            }
            
            return false;
        }
        return true;
    }
    
    return false;
}
*/

bool ChessGame::makeMove(int fromX, int fromY, int toX, int toY) {
    Piece piece = board.getPiece(fromX, fromY);
    if (piece.getColor() != currentPlayer) {
        std::cout << "It's not your turn!\n";
        return false;
    }
    

    // Make a temporary move
    if (board.movePiece(fromX, fromY, toX, toY)) {
    // Check if the move puts or leaves the current player in check
    if (board.isCheck(currentPlayer)) {
        // Undo the move
        board.movePiece(toX, toY, fromX, fromY);
        std::cout << "Invalid move: You cannot put or leave yourself in check. Try again.\n";
        return false;
    }

    // Check for check on opponent
    PieceColor opponentColor = (currentPlayer == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
    if (board.isCheck(opponentColor)) {
        std::cout << "Check!\n";
    }

    return true;
}

    return false;
}
/*
bool ChessGame::makeMove(int fromX, int fromY, int toX, int toY) {
    if (board.movePiece(fromX, fromY, toX, toY)) {
        // Проверка за шах
        PieceColor opponentColor = (currentPlayer == PieceColor::WHITE) ? PieceColor::BLACK : PieceColor::WHITE;
        if (board.isCheck(opponentColor)) {
            std::cout << "Check!" << std::endl;
            if (board.isCheckmate(opponentColor)) {
                std::cout << "Checkmate! " << (currentPlayer == PieceColor::WHITE ? "White" : "Black") << " wins!" << std::endl;
                // Тук можете да добавите логика за край на играта
            }
        }
        switchPlayer();
        return true;
    }
    return false;
}
*/