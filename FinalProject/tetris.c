/*
 * tetris.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "tetris.h"

void generate_next() {
    PieceType type = (PieceType)(rand() % 7 + 1));
    next->type = type;
    next->x = -1;
    next->y = -1;
    next->right_max = -1;
    next->left_max = -1;
    next->bottom_max = -1;

    switch (next->type) {
    case LINE:
        next->blocks;
        next->rotate = rotate_Line_piece_right;
        break;
    case SQUARE:
        next->blocks;
        next->rotate = rotate_Square_piece_right;
        break;
    case J:
        next->blocks;
        next->rotate = rotate_J_piece_right;
        break;
    case L:
        next->blocks;
        next->rotate = rotate_L_piece_right;
        break;
    case S:
        next->blocks;
        next->rotate = rotate_S_piece_right;
        break;
    case Z:
        next->blocks;
        next->rotate = rotate_Z_piece_right;
        break;
    case T:
        next->blocks;
        next->rotate = rotate_T_piece_right;
        break;
    default:
        next->blocks;
        next->rotate = NULL;
        break;
    }
}

void init_board() {
    int row, col;

    board = malloc(sizeof(Block*) * GRID_HEIGHT);
    if (board == NULL) {
        exit(1);
    }

    for(row = 0; row < GRID_HEIGHT; row++) {
        *(board + row) = malloc(sizeof(Block) * GRID_WIDTH);
        if ((*(board + row)) == NULL) {
            exit(1);
        }
    }

    for(row = 0; row < GRID_HEIGHT; row++) {
        for(col = 0; col < GRID_WIDTH; col++) {
            board[row][col]->empty = true;
            board[row][col]->linked = false;
            board[row][col]->x = col;
            board[row][col]->y = row;
            board[row][col]->x_offset = 0;
            board[row][col]->y_offset = 0;
            board[row][col]->axis = false;
            board[row][col]->type = EMPTY;
        }
    }
}

void init_game() {
    init_board();
    current = malloc(sizeof(Piece));
    next = malloc(sizeof(Piece));
    held = malloc(sizeof(Piece));

    if(current == NULL || next == NULL || held == NULL) {
        exit(1);
    }
}
