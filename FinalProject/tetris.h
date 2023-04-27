/*
 * tetris.h
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

/*
 * struct Block {
 int rel_x;
 int rel_y;
 bool axis;
 } typedef Block;

 struct Piece {
 int x;
 int y;
 int left_max;
 int right_max;
 int bottom_max;
 Block blocks[4];
 } typedef Piece;

 struct LineBlock {
 int rel_x;
 int rel_y;
 } typedef LineBlock;

 struct SquareBlock {
 int rel_x;
 int rel_y;
 } typedef SquareBlock;

 struct LinePiece {
 int x;
 int y;
 char axis;
 int left_max;
 int right_max;
 int bottom_max;
 LineBlock blocks[4];
 } typedef LinePiece;

 struct SquarePiece {
 int x;
 int y;
 char axis;
 int left_max;
 int right_max;
 int bottom_max;
 SquareBlock blocks[4];
 } typedef SquarePiece;
 *
 *
 *
 *
 * */

#ifndef TETRIS_H_
#define TETRIS_H_

#include <stdbool.h>
#include <stdlib.h>

#define GRID_WIDTH      10
#define GRID_HEIGHT     24

enum PieceType
{
    EMPTY, LINE, SQUARE, J, L, S, Z, T
}typedef PieceType;

struct Block
{
    bool empty;
    bool linked;
    int x;
    int y;
    int x_offset;
    int y_offset;
    bool axis;
    PieceType type;
}typedef Block;

Block **board;

struct Piece
{
    int x;
    int y;
    int left_max;
    int right_max;
    int bottom_max;
    char rotation;
    Block blocks[4];
    PieceType type;
}typedef Piece;

Piece *current;
Piece *next;
Piece *held;

void generate_next(void);
void init_board(void);
void init_game(void);
void rotate_current_right(void);
void rotate_current_left(void);
void rotate_current_flip(void);
void swap_held(void);
void land_current(void);
void clear_lines(void);

#endif /* TETRIS_H_ */
