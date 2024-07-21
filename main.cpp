#include "ChessGame.h"
#include <SDL_image.h>
#include <SDL.h>
#include <iostream>
#include <map>

//g++ -o main main.cpp Board.cpp ChessGame.cpp -Iinclude/SDL2 -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
const int BOARD_SIZE = 8;
const int CELL_SIZE = SCREEN_WIDTH / BOARD_SIZE;

SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren) {
    SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr) {
        std::cerr << "LoadTexture error: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    return texture;
}

void renderPiece(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y) {
    SDL_Rect dst;
    dst.x = x * CELL_SIZE;
    dst.y = y * CELL_SIZE;
    dst.w = CELL_SIZE;
    dst.h = CELL_SIZE;
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
}

std::string getPieceTexturePath(PieceType type, PieceColor color) {
    std::string color_prefix = color == PieceColor::WHITE ? "white_" : "black_";
    switch (type) {
        case PieceType::PAWN:   return "images/" + color_prefix + "pawn.png";
        case PieceType::ROOK:   return "images/" + color_prefix + "rook.png";
        case PieceType::KNIGHT: return "images/" + color_prefix + "knight.png";
        case PieceType::BISHOP: return "images/" + color_prefix + "bishop.png";
        case PieceType::QUEEN:  return "images/" + color_prefix + "queen.png";
        case PieceType::KING:   return "images/" + color_prefix + "king.png";
        default:                return "";
    }
}

void renderBoard(SDL_Renderer* renderer, const Board& board, std::map<std::pair<PieceType, PieceColor>, SDL_Texture*>& textures) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            SDL_Rect cell;
            cell.x = j * CELL_SIZE;
            cell.y = i * CELL_SIZE;
            cell.w = CELL_SIZE;
            cell.h = CELL_SIZE;

            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // White
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black
            }
            SDL_RenderFillRect(renderer, &cell);

            // Render piece if there is one
            Piece piece = board.getPiece(i, j);
            if (piece.getType() != PieceType::EMPTY) {
                std::pair<PieceType, PieceColor> key = std::make_pair(piece.getType(), piece.getColor());
                renderPiece(renderer, textures[key], j, i);
            }
        }
    }
}

void renderCurrentPlayer(SDL_Renderer* renderer, PieceColor currentPlayer) {
    SDL_Rect indicator;
    indicator.x = 10;
    indicator.y = SCREEN_HEIGHT - 30;
    indicator.w = 20;
    indicator.h = 20;

    if (currentPlayer == PieceColor::WHITE) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // Бял цвят
    } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Черен цвят
    }
    SDL_RenderFillRect(renderer, &indicator);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Chess Game", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        SDL_DestroyWindow(window);
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Load textures for chess pieces
    std::map<std::pair<PieceType, PieceColor>, SDL_Texture*> pieceTextures;
    for (PieceType type : {PieceType::PAWN, PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, PieceType::QUEEN, PieceType::KING}) {
        for (PieceColor color : {PieceColor::WHITE, PieceColor::BLACK}) {
            std::pair<PieceType, PieceColor> key = std::make_pair(type, color);
            pieceTextures[key] = loadTexture(getPieceTexturePath(type, color), renderer);
            if (pieceTextures[key] == nullptr) {
                std::cerr << "Failed to load texture for " << getPieceTexturePath(type, color) << std::endl;
            }
        }
    }

    ChessGame game;
    bool quit = false;
    SDL_Event e;

    int selectedX = -1, selectedY = -1; // Координати на избраната фигура

   while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                int boardX = x / CELL_SIZE;
                int boardY = y / CELL_SIZE;

                if (selectedX == -1 && selectedY == -1) {
                    // Избиране на фигура
                    Piece selectedPiece = game.getBoard().getPiece(boardY, boardX);
                    if (selectedPiece.getType() != PieceType::EMPTY && selectedPiece.getColor() == game.getCurrentPlayer()) {
                        selectedX = boardX;
                        selectedY = boardY;
                    }
                } else {
                    // Преместване на фигурата
                    if (game.makeMove(selectedY, selectedX, boardY, boardX)) {
                        game.switchPlayer();
                        selectedX = -1;
                        selectedY = -1;
                    } else {
                        std::cout << "Invalid move!" << std::endl;
                        selectedX = -1;
                        selectedY = -1;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        renderBoard(renderer, game.getBoard(), pieceTextures);

        renderCurrentPlayer(renderer, game.getCurrentPlayer());


        SDL_RenderPresent(renderer);

    }

    for (auto& pt : pieceTextures) {
        SDL_DestroyTexture(pt.second);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
