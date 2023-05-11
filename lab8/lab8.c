//
// Author: Kevin Chen 1007638790
//

// issues: buffer overflow for string with memory allocation

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
// #include "reversi.h"

// to do: change back to 26

// Sets up board with B and W at centre, and U elsewhere up to size
void initializeBoard(char board[][26], int n) {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            board[row][col] = 'U';
        }
    }

    board[n / 2 - 1][n / 2 - 1] = 'W';
    board[n / 2 - 1][n / 2] = 'B';
    board[n / 2][n / 2 - 1] = 'B';
    board[n / 2][n / 2] = 'W';
}

void printBoard(char board[][26], int n) {
  printf("  ");
  for (char i = 'a'; i < 'a' + n; i++) {
    printf("%c", i);
  }
  printf("\n");

  for (int row = 0; row < n; row++) {
    printf("%c ", 'a' + row);
    for (int col = 0; col < n; col++) {
      printf("%c", board[row][col]);
    }
    printf("\n");
  }
}

// user enters sequence of pieces. Does not flip. End with !!!
void addPieces(char board[][26]) {
  char input[4] = {0};
  while (strcmp(fgets(input, 4, stdin), "!!!") != 0) {
    board[input[1]-'a'][input[2]-'a'] = input[0];
  }
}

// check if position is within size
bool positionInBounds(int n, int row, int col) {
  return (row >= 0 && row < n && col >= 0 && col < n);
}

// get opposite colour
char getOppositeColour(char colour) {
  if (colour == 'W') {
    return 'B';
  } else if (colour == 'B') {
    return 'W';
  }

  return 'U';
}

// check if a move at a certain location in a certain direction is valid
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  char opponent = getOppositeColour(colour);

  int currRow = row + deltaRow, currCol = col + deltaCol;
  if (!positionInBounds(n, currRow, currCol) || board[currRow][currCol] != opponent) return false;

  do {
    currRow += deltaRow;
    currCol += deltaCol;
  } while (positionInBounds(n, currRow, currCol) && board[currRow][currCol] == opponent);

  if (!positionInBounds(n, currRow, currCol)) {
    return false;
  }
  
  return (board[currRow][currCol] == colour);
}

// checks all directions at a given position for a given colour, if there exists a valid move
// also checks that this place is unoccupied
bool hasLegalDirection(char board[][26], int n, int row, int col, char colour) {
  if (board[row][col] != 'U') return false;

  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (!(deltaRow == 0 && deltaCol == 0)) {
        if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol)) {
          return true;
        }
      }
    }
  }

  return false;
}

// returns whether a colour has a valid move
bool hasValid(const int n, char board[][26], char colour) {
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (hasLegalDirection(board, n, row, col, colour)) {
        return true;
      }
    }
  }
  return false;
}


// places a piece on a board and flips all corresponding pieces
// doesn't check for legality
// returns total pieces flipped
int placeAndFlip(char board[][26], int n, int row, int col, char colour) {
  board[row][col] = colour;
  int flipped = 0;
  // check each direction (doesn't stop if one direction works)
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol) && !(deltaRow == 0 && deltaCol == 0)) {
        char opposite = getOppositeColour(colour);
        int currRow = row + deltaRow, currCol = col + deltaCol;
        while (board[currRow][currCol] == opposite) {
          board[currRow][currCol] = colour;
          flipped++;
          currRow += deltaRow;
          currCol += deltaCol;
        }
      }
    }
  }

  return flipped;
}


// prompts user for input and makes that move. If this player has no valid move, return 0. if valid move made return true. if invalid move entered return -1
int userMove(char board[][26], int n, char colour) {
  if (!hasValid(n, board, colour)) {
    return 0;
  }

  char move[3] = {0};
  getchar();
  printf("Enter move for colour %c (RowCol): ", colour);

  fgets(move, 3, stdin);
  // printf("move: '%s'\n", move);

  if (hasLegalDirection(board, n, move[0] - 'a', move[1] - 'a', colour)) {
    // printf("Valid move.\n"); 
    placeAndFlip(board, n, move[0] - 'a', move[1] - 'a', colour);
    return 1;
  }

  // printf("Invalid move.\n");
  return -1;  
}

// get score for given position and colour. If occupied, automatic 0
int getScore(char board[][26], int n, int row, int col, char colour) {
  if (board[row][col] != 'U') return 0;
  // copy board
  char testBoard[26][26];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      testBoard[i][j] = board[i][j];
    }
  }

  // edges +1, corners + 2, next to corners -2

  int score = placeAndFlip(testBoard, n, row, col, colour);
  // printf("for row %d, col %d, score is %d\n", row, col, score);
  return score;
}

int makeMove(const char board[][26], int n, char turn, int *row, int *col);
// make computer move. returns true if successful, false if no move possible
int compMove(char board[][26], int n, char colour) {
  // int maxScore = 0;
  // int maxRow, maxCol;

  // for (int row = 0; row < n; row++) {
  //   for (int col = 0; col < n; col++) {
  //     int currScore = getScore(board, n, row, col, colour);
  //     if (currScore > maxScore) {
  //       maxScore = currScore;
  //       maxRow = row;
  //       maxCol = col;
  //     }
  //   }
  // }

  // if (maxScore != 0) {
  //   placeAndFlip(board, n, maxRow, maxCol, colour);
  //   printf("Computer places %c at %c%c.\n", colour, maxRow + 'a', maxCol + 'a');
  //   return 1;
  // }

  // return 0;

  if (!hasValid(n, board, colour)) return 0;

  int row, col;
  makeMove(board, n, colour, &row, &col);
  placeAndFlip(board, n, row, col, colour);
  printf("Computer places %c at %c%c.\n", colour, row + 'a', col + 'a');
  return 1;

}

// makes move, calculates if computer or user goes. If this player has no valid move, return 0. if valid move made return true. if invalid move entered return -1
int makeMovePrescribed(char board[][26], int n, char compColour, char colour) {
  // printf("%c and %c", compColour, colour);
  if (compColour == colour) {
    return compMove(board, n, colour);
  } 

  return userMove(board, n, colour);
}

// get winner. W B or 0
char getWinner(char board[][26], int n) {
  int blackCount = 0; int whiteCount = 0;
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'B') {
        blackCount++;
      } else if (board[row][col] == 'W') {
        whiteCount++;
      }
    }
  }

  if (blackCount > whiteCount) {
    return 'B';
  } else if (blackCount < whiteCount) {
    return 'W';
  }

  return 0;
}


// lab 8

// returns when neither player has valid move
bool gameOver(int n, char board[][26]) {
  return (!hasValid(n, board, 'W') && !hasValid(n, board, 'B'));
}

// returns how "good" the position is for white. greater is better
int points(int n, char board[][26]) {
  if (gameOver(n, board)) {
    char winner = getWinner(board, n);
    if (winner == 'W') return INT_MAX;
    else if (winner == 'B') return INT_MIN;
    return 0;
  }

  int whitePieces = 0, blackPieces = 0;
  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 26; j++) {
      if (board[i][j] == 'B') blackPieces++;
      else if (board[i][j] == 'W') whitePieces++;
    }
  }
/// add corner pieces bonus
  return (whitePieces - blackPieces);
}




void copyArray(char board[][26], char dest[][26]) {
  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 26; j++) {
      dest[i][j] = board[i][j];
    }
  }  
}


// white is maximizing player
// puts the move in row, col
int miniMax(int n, char board[][26], char player, int depth, int *row, int *col) {
  if (depth == 0 || gameOver(n, board)) {
    return points(n, board);
  } 

  if (player == 'W') {
    int curMax = INT_MIN;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (hasLegalDirection(board, n, i, j, player)) { // loops through all valid moves
          char currboard[26][26];
          copyArray(board, currboard);
          placeAndFlip(currboard, n, i, j, 'W');
      
          int childScore = miniMax(n, board, 'B', depth - 1, row, col);
          if (childScore > curMax) {
            curMax = childScore;
            
            *row = i;
            *col = j;
          }
        }
      }
    }

    return curMax;
  } else {
    int curMin = INT_MAX;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (hasLegalDirection(board, n, i, j, player)) { // loops through all valid moves

          char currboard[n][n];
          copyArray(board, currboard);
          placeAndFlip(currboard, n, i, j, 'B');
          int childScore = miniMax(n, board, 'W', depth - 1, row, col);
          if (childScore < curMin) {
            curMin = childScore;

            *row = i;
            *col = j;
          }
        }
      }
    }

    return curMin;
  }
}

int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
  miniMax(n, board, turn, 5, row, col);
  return 0;
}


int main(void) {
  printf("jo");
  char board[26][26] = {0};
  int size;
  char compColour, currentColour = 'B';

  // size always even 
  printf("Enter the board dimension: ");
  scanf("%d", &size);
  initializeBoard(board, size);

  // why?
  getchar();
  printf("Computer plays (B/W) : ");
  scanf("%c", &compColour);


  // addPieces(board);
  printBoard(board, size);


  char currentNoMoves = 0;
  bool invalidMoveEnd = false;

  while (true) {
    int moveCode = makeMovePrescribed(board, size, compColour, currentColour);

    if (moveCode == -1) { // invalid entered
      printf("Invalid move.\n");
      invalidMoveEnd = true;
      break;
    } else if (moveCode == 0) { // no valid move.
      if (currentNoMoves == 0) {
        printf("%c player has no valid move.\n", currentColour);
        currentNoMoves = currentColour;
        currentColour = getOppositeColour(currentColour);
      } else if (currentNoMoves != currentColour) { // both players have no moves
        break;
      }
    } else if (moveCode == 1) { // valid move
      printBoard(board, size);
      if (currentNoMoves == 0) {
        currentColour = getOppositeColour(currentColour);
      } else if (currentNoMoves == currentColour) { // currentNoMoves now has a move
        currentNoMoves = 0;
        currentColour = getOppositeColour(currentColour);
      }
    }
  }

  char winner;

  if (invalidMoveEnd) {
    winner = getOppositeColour(currentColour);
  } else {
    winner = getWinner(board, size);
  }

  if (winner != 0) {
    printf("%c player wins.", winner);
  } else {
    printf("Draw!");
  }

  return 0;
}

