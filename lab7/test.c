#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "reversi.h"

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

void addPieces(char board[][26]) {
  char input[4] = {0};
  while (strcmp(fgets(input, 4, stdin), "!!!") != 0) {
    board[input[1]-'a'][input[2]-'a'] = input[0];
  }
}

bool positionInBounds(int n, int row, int col) {
  return (row >= 0 && row < n && col >= 0 && col < n);
}

char getOppositeColour(char colour) {
  if (colour == 'W') {
    return 'B';
  } else if (colour == 'B') {
    return 'W';
  }

  return 'U';
}

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

void printValid(char board[][26], int n, char colour) {
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (hasLegalDirection(board, n, row, col, colour)) {
        printf("%c%c\n",'a' + row, 'a' + col);
      }
    }
  }
}

bool isValidInput(char input[]) {
  if (!(input[0] == 'W' || input[0] == 'B')) {
    return false;
  } else if (input[1] < 'a' || input[1] > 'z' || input[2] < 'a' || input[2] > 'z') {
    return false;
  }

  return true;
}

// doesn't check for legality
void placeAndFlip(char board[][26], int n, int row, int col, char colour) {
  board[row][col] = colour;
  // check each direction (doesn't stop if one direction works)
  for (int deltaRow = -1; deltaRow <= 1; deltaRow++) {
    for (int deltaCol = -1; deltaCol <= 1; deltaCol++) {
      if (checkLegalInDirection(board, n, row, col, colour, deltaRow, deltaCol) && !(deltaRow == 0 && deltaCol == 0)) {
        char opposite = getOppositeColour(colour);
        int currRow = row + deltaRow, currCol = col + deltaCol;
        while (board[currRow][currCol] == opposite) {
          board[currRow][currCol] = colour;
          currRow += deltaRow;
          currCol += deltaCol;
        }
      }
    }
  }
}

void makeMove(char board[][26], int n) {
  char move[4] = {0};
  
  getchar();
  fgets(move, 4, stdin);

  if (isValidInput(move) && hasLegalDirection(board, n, move[1] - 'a', move[2] - 'a', move[0])) {
    printf("Valid move.\n"); 
    placeAndFlip(board, n, move[1] - 'a', move[2] - 'a', move[0]);
  } else {
    printf("Invalid move.\n");
  }
  

}
int main(void) {
  char board[26][26] = {0};
  int size;

  // size always even 
  printf("Enter the board dimension: ");
  scanf("%d", &size);

  initializeBoard(board, size);
  printBoard(board, size);

  printf("Enter board configuration:\n");
  addPieces(board);
  printBoard(board, size);

  printf("Available moves for W:\n");
  printValid(board, size, 'W');

  printf("Available moves for B:\n");
  printValid(board, size, 'B');

  printf("Enter a move:\n");
  makeMove(board, size);

  printBoard(board, size);

  return 0;
}