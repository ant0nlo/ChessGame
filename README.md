
# Chess Game Project

## Overview
This project is a chess game implemented in C++ with a graphical user interface using the SDL library. It includes all the basic functionalities of a chess game, including piece movements, board visualization, and move validation.

## Features
- **Graphical User Interface**: Utilizes the SDL library to render the chessboard and pieces.
- **Interactive Gameplay**: Allows users to click and move pieces on the board.
- **Basic Move Validation**: Validates moves based on chess rules.
- **Command-line Interface**: Includes a simple text-based interface for game input and display.

## Files and Structure
The project consists of several files that encapsulate different aspects of the game:

### main.cpp
The entry point of the program. It initializes the SDL library, sets up the game window and renderer, loads textures, and handles the main game loop.

### ChessGame.h and ChessGame.cpp
Defines the `ChessGame` class which controls the flow of the game, including displaying the board and handling user input.

### Board.h and Board.cpp
Defines the `Board` class which manages the state of the chessboard, including piece positions and move validations.

### Piece.h
Defines the `Piece` class which represents individual chess pieces, their types, and colors.

## Class Descriptions

### Piece Class
Located in `Piece.h`, this class encapsulates the attributes and methods related to a chess piece.
- **Attributes**:
  - `PieceType type`: The type of the piece (KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY).
  - `PieceColor color`: The color of the piece (WHITE, BLACK, NONE).
- **Methods**:
  - `PieceType getType() const`: Returns the type of the piece.
  - `PieceColor getColor() const`: Returns the color of the piece.

### Board Class
Located in `Board.h` and `Board.cpp`, this class handles the chessboard and its state.
- **Attributes**:
  - `Piece** board`: A 2D array representing the chessboard.
- **Methods**:
  - `Board()`: Constructor, initializes the board.
  - `~Board()`: Destructor, deallocates memory.
  - `Piece getPiece(int x, int y) const`: Returns the piece at the given coordinates.
  - `bool movePiece(int fromX, int fromY, int toX, int toY)`: Moves a piece from one position to another if the move is valid.
  - `void initializeBoard()`: Sets up the initial board configuration.
  - `bool isMoveValid(int fromX, int fromY, int toX, int toY) const`: Validates if a move is within the rules.
  - `bool isPathClear(int fromX, int fromY, int toX, int toY) const`: Checks if the path between two positions is clear.
  - `bool isCheck(PieceColor color) const`: Checks if the specified color is in check.

### ChessGame Class
Located in `ChessGame.h` and `ChessGame.cpp`, this class manages the overall game flow.
- **Attributes**:
  - `Board board`: An instance of the `Board` class.
- **Methods**:
  - `void play()`: Main game loop handling user input and moves.
  - `const Board& getBoard() const`: Returns a constant reference to the board.
  - `Board& getBoard()`: Returns a reference to the board.
  - `void printBoard()`: Prints the current board state to the console.
  - `char pieceSymbol(PieceType type, PieceColor color)`: Returns the character symbol for a piece based on its type and color.

## Compilation and Execution

### Dependencies
- **SDL2**: Simple DirectMedia Layer library for handling graphics and input.
- **SDL2_image**: SDL library extension for loading images.

### Compilation
To compile the project, you can use the following command:
```sh
g++ -o main main.cpp Board.cpp ChessGame.cpp -Iinclude/SDL2 -Llib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
```

### Execution
After compiling, you can run the executable:
```sh
./main
```

## How to Play
1. **Start the Game**: Run the compiled executable.
2. **Move Pieces**: Click on a piece to select it, then click on the destination cell to move it.
3. **Invalid Moves**: If a move is invalid, an error message will be displayed, and the piece will not move.

## Future Improvements
- **Add AI**: Implement an AI to play against the computer.
- **Game Save/Load**: Add functionality to save and load game states.
- **Enhanced Validation**: Improve move validation to include all chess rules (e.g., castling, en passant).

## License
This project is licensed under the MIT License.

## Contact
For any questions or suggestions, please contact [Your Name] at [your.email@example.com].
