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
        Block block1;
        Block block2;
        Block block3;
        Block block4;
        next->blocks;
        break;
    case SQUARE:
        Block block1;
        Block block2;
        Block block3;
        Block block4;
        next->blocks;
        break;
    case J:
        Block block1;
        Block block2;
        Block block3;
        Block block4;
        next->blocks;
        break;
    case L:
        Block block1;
        Block block2;
        Block block3;
        Block block4;
        next->blocks;
        break;
    case S:
        Block block1;
        Block block2;
        Block block3;
        Block block4;
        next->blocks;
        break;
    case Z:
        Block block1;
        Block block2;
        Block block3;
        Block block4;
        next->blocks;
        break;
    case T:
        Block block1;
        Block block2;
        Block block3;
        Block block4;
        next->blocks;
        break;
    default:
        Block block1;
        Block block2;
        Block block3;
        Block block4;
        next->blocks;
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
    current = malloc(sizeof(Block));
    next = malloc(sizeof(Block));
    held = malloc(sizeof(Block));

    if(current == NULL || next == NULL || held == NULL) {
        exit(1);
    }
}
