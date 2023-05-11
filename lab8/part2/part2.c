//
// Author: Kevin Chen 1007638790
//

// issues: buffer overflow for string with memory allocation

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
// #include "reversi.h"

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
bool hasValid(int n, char board[][26], char colour) {
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

int min(int a, int b) {
  if (a >= b) return b;
  return a;
}

int max(int a, int b) {
  if (a >= b) return a;
  return b;
}

void addTo(char colour, char value, int *white, int *black) {
  if (colour == 'B') *black += value;
  else if (colour == 'W') *white += value;
}
// returns how "good" the position is for white. greater is better
int points(int n, char board[][26]) {
  if (gameOver(n, board)) {
    char winner = getWinner(board, n);
    if (winner == 'W') return INT_MAX / 2;
    else if (winner == 'B') return INT_MIN / 2;
    return 0;
  }

  int whitePieces = 0, blackPieces = 0;
  for (int i = 0; i < n; i++) {
    // adding edge pieces
    addTo(board[i][0], 1, &whitePieces, &blackPieces);
    addTo(board[i][n-1], 1, &whitePieces, &blackPieces);

    addTo(board[0][i], 1, &whitePieces, &blackPieces);
    addTo(board[n-1][i], 1, &whitePieces, &blackPieces);  

    // subtract next-to-edge pieces
    addTo(board[i][1], -1, &whitePieces, &blackPieces);
    addTo(board[i][n-2], -1, &whitePieces, &blackPieces);

    addTo(board[1][i], -1, &whitePieces, &blackPieces);
    addTo(board[n-2][i], -1, &whitePieces, &blackPieces);  

    for (int j = 0; j < n; j++) {
      if (board[i][j] == 'B') blackPieces++;
      else if (board[i][j] == 'W') whitePieces++;
    }
  }
  
  // corners top up to 5
  addTo(board[0][0], 2, &whitePieces, &blackPieces);
  addTo(board[n-1][n-1], 2, &whitePieces, &blackPieces);

  addTo(board[n][0], 2, &whitePieces, &blackPieces);
  addTo(board[0][n-1], 2, &whitePieces, &blackPieces); 

  return (whitePieces - blackPieces);
}


void copyConstArray(const char board[][26], char dest[][26]) {
  for (int i = 0; i < 26; i++) {
    for (int j = 0; j < 26; j++) {
      dest[i][j] = board[i][j];
    }
  }  
}

void copyArray(char board[][26], char dest[][26], int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      dest[i][j] = board[i][j];
    }
  }  
}


// white is maximizing player
// puts the move in row, col
int miniMax(int n, char board[][26], char player, int depth, int alpha, int beta, int *row, int *col) {
  if (depth == 0 || !hasValid(n, board, player)) {
    return points(n, board);
  }

  if (player == 'W') {
    int curMax = INT_MIN;
    int maxRow = -1, maxCol = -1;

    bool alphaGreater = false;
    for (int i = 0; i < n && !alphaGreater; i++) {
      for (int j = 0; j < n && !alphaGreater; j++) {
        if (hasLegalDirection(board, n, i, j, player)) { // loops through all valid moves
          char currBoard[26][26];
          copyArray(board, currBoard, n);
          placeAndFlip(currBoard, n, i, j, 'W');
      
          int childScore = miniMax(n, currBoard, 'B', depth - 1, alpha, beta, row, col);
          if (childScore > curMax) {
            curMax = childScore;
            maxRow = i;
            maxCol = j;
          }
          // prune
          alpha = max(alpha, childScore);
          if (beta <= alpha) alphaGreater = true;
        }
      }
    }

    // printf("depth %d for %c is %d %d\n", depth, 'W', maxRow, maxCol);
    *row = maxRow;
    *col = maxCol;
    return curMax;
  } else {
    int curMin = INT_MAX;
    int minRow = -1, minCol = -1;

    bool betaLess = false;

    for (int i = 0; i < n && !betaLess; i++) {
      for (int j = 0; j < n && !betaLess; j++) {
        if (hasLegalDirection(board, n, i, j, player)) { // loops through all valid moves

          char currBoard[26][26];
          copyArray(board, currBoard, n);
          placeAndFlip(currBoard, n, i, j, 'B');
          int childScore = miniMax(n, currBoard, 'W', depth - 1, alpha, beta, row, col);
          if (childScore < curMin) {
            minRow = i;
            minCol = j;
            curMin = childScore;
          }

          beta = min(beta, childScore);
          if (beta <= alpha) betaLess = true;
        }
      }
    }

    // printf("depth %d for %c is %d %d\n", depth, 'B', minRow, minCol);
    *row = minRow;
    *col = minCol;
    return curMin;
  }
}


int makeMove(const char board[][26], int n, char turn, int *row, int *col) {
  char tempBoard[26][26];
  copyConstArray(board, tempBoard);
  miniMax(n, tempBoard, turn, 7, INT_MIN, INT_MAX, row, col);
  return 0;
}

// int main() {
//   const char board[26][26] = {
//     {'U', 'U', 'U', 'U'},
//     {'U', 'W', 'B', 'U'},
//     {'U', 'B', 'W', 'U'},
//     {'U', 'U', 'U', 'U'}
//   };

//   int row, col;

//   makeMove(board, 5, 'B', &row, &col);
//   printf("make move at %c %c", row+'a', col+'a');
// }



