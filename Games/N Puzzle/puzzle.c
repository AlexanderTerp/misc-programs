// Author:          Alexander M. Terp
// Date created:    December 28, 2016
// Purpose:         Allows the user to play the n-puzzle game. This particular
//                  program is for the 4x4 15-puzzle variant, however, this can
//                  easily be altered by changing NUM_ROWS and NUM_COLS. For
//                  information, visit www.wikipedia.org/wiki/15_puzzle.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <windows.h>

#define NUM_ROWS 4
#define NUM_COLS 4
#define NUM_PIECES (NUM_ROWS * NUM_COLS - 1)

// ASCII value for the space character.
#define SPACE 32

void print_board(int board[NUM_ROWS][NUM_COLS]);
void generate_board(int board[NUM_ROWS][NUM_COLS], int unused_pieces[]);
int is_valid_move(int board[NUM_ROWS][NUM_COLS], int piece_row, 
    int piece_col, int space_row, int space_col);
int move_piece(int piece, int board[NUM_ROWS][NUM_COLS]);
void generate_pieces(int unused_pieces[NUM_PIECES]);

int main(void) {
    // Create the numbered pieces for the puzzle.
    int unused_pieces[NUM_PIECES];
    generate_pieces(unused_pieces);

    // Randomly generate an initially jumbled game board.
    int board[NUM_ROWS][NUM_COLS];
    generate_board(board, unused_pieces);

    int piece_to_move;

    system("cls");
    while (1) {
        // Print current board.
        printf("\n");
        print_board(board);

        // Prompt for a move.
        printf("\n> ");
        scanf("%d", &piece_to_move);

        // Evaluate move.
        system("cls");
        if ( !move_piece(piece_to_move, board) ) {
            printf("Invalid move.");
        }
    }

    return 0;
}

void generate_board(int board[NUM_ROWS][NUM_COLS], int unused_pieces[]) {
    /* Randomly fills the board with the numbers in unused_pieces. Leaves one
    blank space as per the rules of the game. */

    // Seed random number generator.
    srand(time(NULL));

    int row, col, piece;
    // Iterate through every "position" in board.
    for (row = 0; row < NUM_ROWS; row++) {
        for (col = 0; col < NUM_COLS; col++) {
            if (row == NUM_ROWS-1 && col == NUM_COLS-1) {
                // Must be finished filling board, leave this one as a blank.
                break;
            }

            // Pick a random piece to insert onto the board next.
            piece = rand() % NUM_PIECES;

            // If the piece has been picked before, choose the next one.
            while (unused_pieces[piece] == 0) {
                piece = (piece + 1) % NUM_PIECES;
            }

            // Add the piece. Replace it in unused_pieces with a 0 to signify
            // that it is now used.
            board[row][col] = unused_pieces[piece];
            unused_pieces[piece] = 0;
        }
    }

    // Make the blank spot on the board a 0 which will act as a sentinal in
    // other functions.
    board[NUM_ROWS-1][NUM_COLS-1] = 0;
}

void print_board(int board[NUM_ROWS][NUM_COLS]) {
    /* Prints out the board in a formatted fashion to stdout. */

    int row, col;
    for (row = 0; row < NUM_ROWS; row++) {
        for (col = 0; col < NUM_COLS; col++) {
            if (board[row][col] == 0) {
                // Must be the blank space. Print a blank space.
                printf(" %2c ", SPACE);
            }
            else {
                // Must be a number. Print that number.
                printf(" %2d ", board[row][col]);
            }
        }
        printf("\n");
    }
}

int is_valid_move(int board[NUM_ROWS][NUM_COLS], int piece_row, 
    int piece_col, int space_row, int space_col) {
    /* Given the coordinates for the proposed piece to be moved and the space,
    returns whether the move is valid or not (1 or 0 respectively). */
    
    return ( (piece_row == space_row && abs(piece_col - space_col) == 1) || 
             (piece_col == space_col && abs(piece_row - space_row) == 1) );
}

int move_piece(int piece, int board[NUM_ROWS][NUM_COLS]) {
    /* Accepts a proposed move in the form of an int referring to the int on
    the piece. Checks if the move is valid and then carries it out. Returns 1
    if the move was successful, otherwise returns 0. */

    // Find the coordinates of both the piece and blank space.
    int row, col, piece_row, piece_col, space_row, space_col, piece_found = 0, 
        space_found = 0;
    for (row = 0; row < NUM_ROWS; row++) {
        for (col = 0; col < NUM_COLS; col++) {
            if (board[row][col] == piece) {
                piece_found = 1;
                piece_row = row;
                piece_col = col;
            }

            if (board[row][col] == 0) {
                space_found = 1;
                space_row = row;
                space_col = col;
            }

            if (piece_found && space_found) {
                break;
            }
        }
        if (piece_found && space_found) {
            break;
        }
    }

    // Checks if the move is valid. If so, moves the proposed piece and returns 
    // 1 to indicate success. Otherwise, returns 0 to indicate move failure.
    if ( is_valid_move(board, piece_row, piece_col, space_row, space_col) ) {
        board[space_row][space_col] = piece;
        board[piece_row][piece_col] = 0;
        return 1;
    } else {
        return 0;
    }
}

void generate_pieces(int unused_pieces[NUM_PIECES]) {
    /* Fills an array of ints with increasing numbers. Starts at 1 and goes to
    NUM_PIECES. Will represent the pieces in the puzzle. */

    int i;
    for (i = 1; i <= NUM_PIECES; i++) {
        unused_pieces[i-1] = i;
    }
}