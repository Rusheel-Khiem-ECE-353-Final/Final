/*
 * tetris.h
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#ifndef TETRIS_H_
#define TETRIS_H_

#include <stdbool.h>
#include <stdlib.h>

#define GRID_WIDTH          10
#define GRID_HEIGHT         20
#define SPAWN_X             4
#define SPAWN_Y             18
#define UPDATE_FREQUENCY    10

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
    float x_offset;
    float y_offset;
    bool axis;
    PieceType type;
}typedef Block;

struct Piece
{
    float x;
    float y;
    char rotation;
    void (*rotate)(int);
    Block *blocks;
    PieceType type;
}typedef Piece;

struct GameData
{
    Block **board;
    Piece *current;
    Piece *next;
    Piece *held;
    bool started;
    bool over;
    bool held_swapped;
    int fall_speed;
    float fall_amount;
}typedef GameData;

extern GameData *game;

void init_board(void);
void init_game(void);
void delete_board(void);
void delete_game(void);

void rotate_current_right(void);
void rotate_current_left(void);
void move_current_right(void);
void move_current_left(void);
void swap_held(void);
void swap_next(void);
void spawn_current(void);

void generate_next(void);
void generate_block_base(Piece*);
void generate_line(Piece*);
void generate_square(Piece*);
void generate_L(Piece*);
void generate_J(Piece*);
void generate_S(Piece*);
void generate_Z(Piece*);
void generate_T(Piece*);

void rotate_line(int);
void rotate_square(int);
void rotate_L(int);
void rotate_J(int);
void rotate_S(int);
void rotate_Z(int);
void rotate_T(int);

void clear_lines(void);
void check_over(void);
void land_current(void);
void hard_drop(void);
void enable_fast_fall(void);
void disable_fast_fall(void);

void run_cycle(void);

#endif /* TETRIS_H_ */
