/*
 * tetris.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "tetris.h"
#include "our_tasks.h"

GameData *game;

/******************************************************************************
 * Initialize the Tetris game board by allocating all required memory and
 * setting all of the initial values of the blocks in the board
 ******************************************************************************/
void init_board()
{
    int row, col;

    (game->board) = malloc(sizeof(Block*) * GRID_HEIGHT);
    if ((game->board) == NULL)
    {
        exit(1);
    }

    for (row = 0; row < GRID_HEIGHT; row++)
    {
        *((game->board) + row) = malloc(sizeof(Block) * GRID_WIDTH);
        if ((*((game->board) + row)) == NULL)
        {
            exit(1);
        }
    }

    // Set all the blocks in the board to be empty at their location
    for (row = 0; row < GRID_HEIGHT; row++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
            (game->board)[row][col].empty = true;
            (game->board)[row][col].linked = false;
            (game->board)[row][col].x = col;
            (game->board)[row][col].y = row;
            (game->board)[row][col].x_offset = 0;
            (game->board)[row][col].y_offset = 0;
            (game->board)[row][col].axis = false;
            (game->board)[row][col].type = EMPTY;
        }
    }
}

/******************************************************************************
 * Initializes the game by allocating all required memory for the game and any
 * global pieces or values needed for the game to function
 ******************************************************************************/
void init_game()
{
    game = malloc(sizeof(GameData));
    if (game == NULL)
    {
        exit(1);
    }

    game->started = false;
    game->over = false;
    game->held_swapped = false;
    game->fall_speed = 10;
    game->fall_amount = 0;

    init_board();
    (game->current) = malloc(sizeof(Piece));
    (game->next) = malloc(sizeof(Piece));
    (game->held) = malloc(sizeof(Piece));

    if (((game->current) == NULL) || ((game->next) == NULL)
            || ((game->held) == NULL))
    {
        exit(1);
    }

    (game->current)->blocks = malloc(sizeof(Block) * 4);
    (game->next)->blocks = malloc(sizeof(Block) * 4);
    (game->held)->blocks = malloc(sizeof(Block) * 4);

    if (((game->current)->blocks == NULL) || ((game->next)->blocks == NULL)
            || ((game->held)->blocks == NULL))
    {
        exit(1);
    }

    (game->current)->x = (game->current)->y = -1;
    (game->next)->x = (game->next)->y = -1;
    (game->held)->x = (game->held)->y = -1;
    (game->current)->rotation = (game->next)->rotation =
            (game->held)->rotation = 0;
    (game->current)->rotate = (game->next)->rotate = (game->held)->rotate =
            NULL;
    (game->current)->type = (game->next)->type = (game->held)->type = EMPTY;
    (game->started) = true;
    (game->over) = false;
    (game->held_swapped) = false;
}

/******************************************************************************
 * Frees all memory used for the board
 ******************************************************************************/
void delete_board()
{
    int row;
    for (row = 0; row < GRID_HEIGHT; row++)
    {
        free(*((game->board) + row));
        (*((game->board) + row)) = NULL;
    }

    free((game->board));
    (game->board) = NULL;
}

/******************************************************************************
 * Frees all memory used for the game
 ******************************************************************************/
void delete_game()
{
    delete_board();

    free((game->current)->blocks);
    (game->current)->blocks = NULL;

    free((game->next)->blocks);
    (game->next)->blocks = NULL;

    free((game->held)->blocks);
    (game->held)->blocks = NULL;

    free((game->current));
    (game->current) = NULL;

    free((game->next));
    (game->next) = NULL;

    free((game->held));
    (game->held) = NULL;

    free(game);
    game = NULL;
}

/******************************************************************************
 * If possible, rotates the current piece clockwise
 ******************************************************************************/
void rotate_current_right()
{
    if ((game->current) != NULL && (game->current)->rotate != NULL)
    {
        (game->current)->rotate(1);
    }
}

/******************************************************************************
 * If possible, rotates the current piece counterclockwise
 ******************************************************************************/
void rotate_current_left()
{
    if ((game->current) != NULL && (game->current)->rotate != NULL)
    {
        (game->current)->rotate(-1);
    }
}

/******************************************************************************
 * If possible, moves the current piece one position to the right
 ******************************************************************************/
void move_current_right()
{
    if ((game->current) == NULL)
    {
        return;
    }

    int block_index;
    bool moveable = true;
    for (block_index = 0; block_index < 4; block_index++)
    {
        Block block = (game->current)->blocks[block_index];
        int x = (int) (block.x_offset + (game->current)->x);
        int y = (int) (block.y_offset + (game->current)->y);

        if (((x + 1) > 9) || ((game->board)[y][x + 1].empty == false))
        {
            moveable = false;
            break;
        }
    }

    if (moveable)
    {
        (game->current)->x++;
    }
}

/******************************************************************************
 * If possible, moves the current piece one position to the left
 ******************************************************************************/
void move_current_left()
{
    if ((game->current) == NULL)
    {
        return;
    }

    int block_index;
    bool moveable = true;
    for (block_index = 0; block_index < 4; block_index++)
    {
        Block block = (game->current)->blocks[block_index];
        int x = (int) (block.x_offset + (game->current)->x);
        int y = (int) (block.y_offset + (game->current)->y);

        if (((x - 1) < 0) || ((game->board)[y][x - 1].empty == false))
        {
            moveable = false;
            break;
        }
    }

    if (moveable)
    {
        (game->current)->x--;
    }
}

/******************************************************************************
 * If allowed, swaps the currently held piece with the current piece
 ******************************************************************************/
void swap_held()
{
    if ((game->held_swapped) == false)
    {
        if ((game->held)->type == EMPTY)
        {
            Piece *temp = (game->held);
            (game->held) = (game->current);
            (game->current) = temp;
            swap_next();
        }
        else
        {
            Piece *temp = (game->held);
            (game->held) = (game->current);
            (game->current) = temp;
            spawn_current();
        }
        (game->held_swapped) = true;
    }
}

/******************************************************************************
 * Swaps the current piece with the next piece and generates a new next piece
 ******************************************************************************/
void swap_next()
{
    if ((game->next)->type == EMPTY)
    {
        generate_next();
    }

    Piece *temp = (game->next);
    (game->next) = (game->current);
    (game->next)->type = EMPTY;
    (game->next)->x = (game->next)->y = -1;
    (game->current) = temp;
    spawn_current();
    generate_next();
}

/******************************************************************************
 * Spawns the current piece at the top of the board
 ******************************************************************************/
void spawn_current()
{
    switch ((game->current)->type)
    {
    case LINE:
        (game->current)->x = SPAWN_X + 0.5;
        (game->current)->y = SPAWN_Y + 0.5;
        break;
    case SQUARE:
        (game->current)->x = SPAWN_X + 0.5;
        (game->current)->y = SPAWN_Y + 0.5;
        break;
    case L:
        (game->current)->x = SPAWN_X;
        (game->current)->y = SPAWN_Y;
        break;
    case J:
        (game->current)->x = SPAWN_X;
        (game->current)->y = SPAWN_Y;
        break;
    case S:
        (game->current)->x = SPAWN_X;
        (game->current)->y = SPAWN_Y;
        break;
    case Z:
        (game->current)->x = SPAWN_X;
        (game->current)->y = SPAWN_Y;
        break;
    case T:
        (game->current)->x = SPAWN_X;
        (game->current)->y = SPAWN_Y;
        break;
    default:
        (game->current)->x = (game->current)->y = -1;
        break;
    }
}

/******************************************************************************
 * Randomly generates a new next piece
 ******************************************************************************/
void generate_next()
{
    PieceType type = (PieceType) (rand() % 7 + 1);
    (game->next)->type = type;
    (game->next)->x = -1;
    (game->next)->y = -1;
    (game->next)->rotation = 0;

    switch ((game->next)->type)
    {
    case LINE:
        generate_line((game->next));
        (game->next)->rotate = rotate_line;
        break;
    case SQUARE:
        generate_square((game->next));
        (game->next)->rotate = rotate_square;
        break;
    case J:
        generate_J((game->next));
        (game->next)->rotate = rotate_J;
        break;
    case L:
        generate_L((game->next));
        (game->next)->rotate = rotate_L;
        break;
    case S:
        generate_S((game->next));
        (game->next)->rotate = rotate_S;
        break;
    case Z:
        generate_Z((game->next));
        (game->next)->rotate = rotate_Z;
        break;
    case T:
        generate_T((game->next));
        (game->next)->rotate = rotate_T;
        break;
    default:
        (game->next)->rotate = NULL;
        break;
    }
}

/******************************************************************************
 * The base method to generate a new piece
 ******************************************************************************/
void generate_block_base(Piece *piece)
{
    piece->blocks[0].axis = piece->blocks[1].axis = piece->blocks[2].axis =
            piece->blocks[3].axis = false;
    piece->blocks[0].empty = piece->blocks[1].empty = piece->blocks[2].empty =
            piece->blocks[3].empty = false;
    piece->blocks[0].linked = piece->blocks[1].linked =
            piece->blocks[2].linked = piece->blocks[3].linked = true;
    piece->blocks[0].type = piece->blocks[1].type = piece->blocks[2].type =
            piece->blocks[3].type = piece->type;
    piece->blocks[0].x = piece->blocks[1].x = piece->blocks[2].x =
            piece->blocks[3].x = 0;
    piece->blocks[0].y = piece->blocks[1].y = piece->blocks[2].y =
            piece->blocks[3].y = 0;
}

/******************************************************************************
 * Specifies the generation of a line piece
 ******************************************************************************/
void generate_line(Piece *piece)
{
    piece->rotate = rotate_line;
    generate_block_base(piece);

    piece->blocks[0].x_offset = -1.5;
    piece->blocks[0].y_offset = 0.5;

    piece->blocks[1].x_offset = -0.5;
    piece->blocks[1].y_offset = 0.5;

    piece->blocks[2].x_offset = 0.5;
    piece->blocks[2].y_offset = 0.5;

    piece->blocks[3].x_offset = 1.5;
    piece->blocks[3].y_offset = 0.5;
}

/******************************************************************************
 * Specifies the generation of a square piece
 ******************************************************************************/
void generate_square(Piece *piece)
{
    piece->rotate = rotate_square;
    generate_block_base(piece);

    piece->blocks[0].x_offset = -0.5;
    piece->blocks[0].y_offset = -0.5;

    piece->blocks[1].x_offset = -0.5;
    piece->blocks[1].y_offset = 0.5;

    piece->blocks[2].x_offset = 0.5;
    piece->blocks[2].y_offset = -0.5;

    piece->blocks[3].x_offset = 0.5;
    piece->blocks[3].y_offset = 0.5;
}

/******************************************************************************
 * Specifies the generation of an L piece
 ******************************************************************************/
void generate_L(Piece *piece)
{
    piece->rotate = rotate_L;
    generate_block_base(piece);

    piece->blocks[0].x_offset = -1;
    piece->blocks[0].y_offset = 0;

    piece->blocks[1].x_offset = 0;
    piece->blocks[1].y_offset = 0;
    piece->blocks[1].axis = true;

    piece->blocks[2].x_offset = 1;
    piece->blocks[2].y_offset = 0;

    piece->blocks[3].x_offset = 1;
    piece->blocks[3].y_offset = 1;
}

/******************************************************************************
 * Specifies the generation of a J piece
 ******************************************************************************/
void generate_J(Piece *piece)
{
    piece->rotate = rotate_J;
    generate_block_base(piece);

    piece->blocks[0].x_offset = -1;
    piece->blocks[0].y_offset = 1;

    piece->blocks[1].x_offset = -1;
    piece->blocks[1].y_offset = 0;

    piece->blocks[2].x_offset = 0;
    piece->blocks[2].y_offset = 0;
    piece->blocks[2].axis = true;

    piece->blocks[3].x_offset = 1;
    piece->blocks[3].y_offset = 0;
}

/******************************************************************************
 * Specifies the generation of an S piece
 ******************************************************************************/
void generate_S(Piece *piece)
{
    piece->rotate = rotate_S;
    generate_block_base(piece);

    piece->blocks[0].x_offset = -1;
    piece->blocks[0].y_offset = 0;

    piece->blocks[1].x_offset = 0;
    piece->blocks[1].y_offset = 0;
    piece->blocks[1].axis = true;

    piece->blocks[2].x_offset = 0;
    piece->blocks[2].y_offset = 1;

    piece->blocks[3].x_offset = 1;
    piece->blocks[3].y_offset = 1;
}

/******************************************************************************
 * Specifies the generation of a Z piece
 ******************************************************************************/
void generate_Z(Piece *piece)
{
    piece->rotate = rotate_Z;
    generate_block_base(piece);

    piece->blocks[0].x_offset = -1;
    piece->blocks[0].y_offset = 1;

    piece->blocks[1].x_offset = -1;
    piece->blocks[1].y_offset = 0;

    piece->blocks[2].x_offset = 0;
    piece->blocks[2].y_offset = 0;
    piece->blocks[2].axis = true;

    piece->blocks[3].x_offset = 1;
    piece->blocks[3].y_offset = 0;
}

/******************************************************************************
 * Specifies the generation of a T piece
 ******************************************************************************/
void generate_T(Piece *piece)
{
    piece->rotate = rotate_T;
    generate_block_base(piece);

    piece->blocks[0].x_offset = -1;
    piece->blocks[0].y_offset = 0;

    piece->blocks[1].x_offset = 0;
    piece->blocks[1].y_offset = 0;
    piece->blocks[1].axis = true;

    piece->blocks[2].x_offset = 0;
    piece->blocks[2].y_offset = 1;

    piece->blocks[3].x_offset = 1;
    piece->blocks[3].y_offset = 0;
}

/******************************************************************************
 * Specifies the rotation of a line piece based on the given direction
 ******************************************************************************/
void rotate_line(int dir)
{
    float origins[4][2] = { { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 },
                            { 0.0, 0.0 } };
    int d = (dir == 1) ? 1 : 0;
    int cases[2][4][4][2] =
            { { { { -1, 0 }, { 2, 0 }, { -1, 2 }, { 2, -1 } }, { { 2, 0 },
                                                                 { -1, 0 },
                                                                 { 2, 1 },
                                                                 { -1, -2 } },
                { { 1, 0 }, { -2, 0 }, { 1, -2 }, { -2, 1 } },
                { { -2, 0 }, { 1, 0 }, { -2, -1 }, { 1, 2 } } },
              { { { -2, 0 }, { 1, 0 }, { -2, -1 }, { 1, 2 } }, { { -1, 0 },
                                                                 { 2, 0 },
                                                                 { -1, 2 },
                                                                 { 2, -1 } },
                { { 2, 0 }, { -1, 0 }, { 2, 1 }, { -1, -2 } }, { { 1, 0 },
                                                                 { -2, 0 },
                                                                 { 1, -2 },
                                                                 { 2, -1 } } } };

    int i;
    for (i = 0; i < 4; i++)
    {
        Block block = (game->current)->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x * -1;
            (game->current)->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x;
            (game->current)->blocks[i].x_offset = y * -1;
        }
    }

    int case_num = -1;
    bool success = false;
    float base_x;
    float base_y;

    while ((success == false) && (case_num < 3))
    {
        if (case_num >= 0)
        {
            base_x = (game->current)->x
                    + cases[d][(game->current)->rotation][case_num][0];
            base_y = (game->current)->y
                    + cases[d][(game->current)->rotation][case_num][1];
        }
        else
        {
            base_x = (game->current)->x;
            base_y = (game->current)->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = (game->current)->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || ((game->board)[y][x].empty == false))
            {
                success = false;
            }
        }

        if (success == false)
        {
            break;
        }

        case_num++;
    }

    if (case_num >= 0)
    {
        base_x = (game->current)->x
                + cases[d][(game->current)->rotation][case_num][0];
        base_y = (game->current)->y
                + cases[d][(game->current)->rotation][case_num][1];
    }
    else
    {
        base_x = (game->current)->x;
        base_y = (game->current)->y;
    }

    if (success == true)
    {
        (game->current)->x += base_x;
        (game->current)->y += base_y;
        (game->current)->rotation = ((game->current)->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            (game->current)->blocks[i].x_offset = origins[i][0];
            (game->current)->blocks[i].y_offset = origins[i][1];
        }
    }
}

/******************************************************************************
 * Specifies no rotation for the square piece
 ******************************************************************************/
void rotate_square(int dir)
{
    return;
}

/******************************************************************************
 * Specifies the rotation of an L piece based on the given direction
 ******************************************************************************/
void rotate_L(int dir)
{
    float origins[4][2] = { { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 },
                            { 0.0, 0.0 } };
    int d = (dir == 1) ? 1 : 0;
    int cases[2][4][4][2] = {
            { { { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { -1, 0 },
                                                             { -1, 1 },
                                                             { 0, -2 },
                                                             { -1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } },
            { { { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { 1, 0 }, { 1, 1 },
                                                             { 0, -2 },
                                                             { 1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } } };

    int i;
    for (i = 0; i < 4; i++)
    {
        Block block = (game->current)->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x * -1;
            (game->current)->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x;
            (game->current)->blocks[i].x_offset = y * -1;
        }
    }

    int case_num = -1;
    bool success = false;
    float base_x;
    float base_y;

    while ((success == false) && (case_num < 3))
    {
        if (case_num >= 0)
        {
            base_x = (game->current)->x
                    + cases[d][(game->current)->rotation][case_num][0];
            base_y = (game->current)->y
                    + cases[d][(game->current)->rotation][case_num][1];
        }
        else
        {
            base_x = (game->current)->x;
            base_y = (game->current)->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = (game->current)->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || ((game->board)[y][x].empty == false))
            {
                success = false;
            }
        }

        if (success == false)
        {
            break;
        }

        case_num++;
    }

    if (case_num >= 0)
    {
        base_x = (game->current)->x
                + cases[d][(game->current)->rotation][case_num][0];
        base_y = (game->current)->y
                + cases[d][(game->current)->rotation][case_num][1];
    }
    else
    {
        base_x = (game->current)->x;
        base_y = (game->current)->y;
    }

    if (success == true)
    {
        (game->current)->x += base_x;
        (game->current)->y += base_y;
        (game->current)->rotation = ((game->current)->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            (game->current)->blocks[i].x_offset = origins[i][0];
            (game->current)->blocks[i].y_offset = origins[i][1];
        }
    }
}

/******************************************************************************
 * Specifies the rotation of a J piece based on the given direction
 ******************************************************************************/
void rotate_J(int dir)
{
    float origins[4][2] = { { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 },
                            { 0.0, 0.0 } };
    int d = (dir == 1) ? 1 : 0;
    int cases[2][4][4][2] = {
            { { { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { -1, 0 },
                                                             { -1, 1 },
                                                             { 0, -2 },
                                                             { -1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } },
            { { { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { 1, 0 }, { 1, 1 },
                                                             { 0, -2 },
                                                             { 1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } } };

    int i;
    for (i = 0; i < 4; i++)
    {
        Block block = (game->current)->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x * -1;
            (game->current)->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x;
            (game->current)->blocks[i].x_offset = y * -1;
        }
    }

    int case_num = -1;
    bool success = false;
    float base_x;
    float base_y;

    while ((success == false) && (case_num < 3))
    {
        if (case_num >= 0)
        {
            base_x = (game->current)->x
                    + cases[d][(game->current)->rotation][case_num][0];
            base_y = (game->current)->y
                    + cases[d][(game->current)->rotation][case_num][1];
        }
        else
        {
            base_x = (game->current)->x;
            base_y = (game->current)->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = (game->current)->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || ((game->board)[y][x].empty == false))
            {
                success = false;
            }
        }

        if (success == false)
        {
            break;
        }

        case_num++;
    }

    if (case_num >= 0)
    {
        base_x = (game->current)->x
                + cases[d][(game->current)->rotation][case_num][0];
        base_y = (game->current)->y
                + cases[d][(game->current)->rotation][case_num][1];
    }
    else
    {
        base_x = (game->current)->x;
        base_y = (game->current)->y;
    }

    if (success == true)
    {
        (game->current)->x += base_x;
        (game->current)->y += base_y;
        (game->current)->rotation = ((game->current)->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            (game->current)->blocks[i].x_offset = origins[i][0];
            (game->current)->blocks[i].y_offset = origins[i][1];
        }
    }
}

/******************************************************************************
 * Specifies the rotation of an S piece based on the given direction
 ******************************************************************************/
void rotate_S(int dir)
{
    float origins[4][2] = { { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 },
                            { 0.0, 0.0 } };
    int d = (dir == 1) ? 1 : 0;
    int cases[2][4][4][2] = {
            { { { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { -1, 0 },
                                                             { -1, 1 },
                                                             { 0, -2 },
                                                             { -1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } },
            { { { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { 1, 0 }, { 1, 1 },
                                                             { 0, -2 },
                                                             { 1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } } };

    int i;
    for (i = 0; i < 4; i++)
    {
        Block block = (game->current)->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x * -1;
            (game->current)->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x;
            (game->current)->blocks[i].x_offset = y * -1;
        }
    }

    int case_num = -1;
    bool success = false;
    float base_x;
    float base_y;

    while ((success == false) && (case_num < 3))
    {
        if (case_num >= 0)
        {
            base_x = (game->current)->x
                    + cases[d][(game->current)->rotation][case_num][0];
            base_y = (game->current)->y
                    + cases[d][(game->current)->rotation][case_num][1];
        }
        else
        {
            base_x = (game->current)->x;
            base_y = (game->current)->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = (game->current)->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || ((game->board)[y][x].empty == false))
            {
                success = false;
            }
        }

        if (success == false)
        {
            break;
        }

        case_num++;
    }

    if (case_num >= 0)
    {
        base_x = (game->current)->x
                + cases[d][(game->current)->rotation][case_num][0];
        base_y = (game->current)->y
                + cases[d][(game->current)->rotation][case_num][1];
    }
    else
    {
        base_x = (game->current)->x;
        base_y = (game->current)->y;
    }

    if (success == true)
    {
        (game->current)->x += base_x;
        (game->current)->y += base_y;
        (game->current)->rotation = ((game->current)->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            (game->current)->blocks[i].x_offset = origins[i][0];
            (game->current)->blocks[i].y_offset = origins[i][1];
        }
    }
}

/******************************************************************************
 * Specifies the rotation of a Z piece based on the given direction
 ******************************************************************************/
void rotate_Z(int dir)
{
    float origins[4][2] = { { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 },
                            { 0.0, 0.0 } };
    int d = (dir == 1) ? 1 : 0;
    int cases[2][4][4][2] = {
            { { { 1, 0 }, { 1, 1 }, { 0, -2 }, { 1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { -1, 0 },
                                                             { -1, 1 },
                                                             { 0, -2 },
                                                             { -1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } },
            { { { -1, 0 }, { -1, 1 }, { 0, -2 }, { -1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { 1, 0 }, { 1, 1 },
                                                             { 0, -2 },
                                                             { 1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } } };

    int i;
    for (i = 0; i < 4; i++)
    {
        Block block = (game->current)->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x * -1;
            (game->current)->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x;
            (game->current)->blocks[i].x_offset = y * -1;
        }
    }

    int case_num = -1;
    bool success = false;
    float base_x;
    float base_y;

    while ((success == false) && (case_num < 3))
    {
        if (case_num >= 0)
        {
            base_x = (game->current)->x
                    + cases[d][(game->current)->rotation][case_num][0];
            base_y = (game->current)->y
                    + cases[d][(game->current)->rotation][case_num][1];
        }
        else
        {
            base_x = (game->current)->x;
            base_y = (game->current)->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = (game->current)->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || ((game->board)[y][x].empty == false))
            {
                success = false;
            }
        }

        if (success == false)
        {
            break;
        }

        case_num++;
    }

    if (case_num >= 0)
    {
        base_x = (game->current)->x
                + cases[d][(game->current)->rotation][case_num][0];
        base_y = (game->current)->y
                + cases[d][(game->current)->rotation][case_num][1];
    }
    else
    {
        base_x = (game->current)->x;
        base_y = (game->current)->y;
    }

    if (success == true)
    {
        (game->current)->x += base_x;
        (game->current)->y += base_y;
        (game->current)->rotation = ((game->current)->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            (game->current)->blocks[i].x_offset = origins[i][0];
            (game->current)->blocks[i].y_offset = origins[i][1];
        }
    }
}

/******************************************************************************
 * Specifies the rotation of a T piece based on the given direction
 ******************************************************************************/
void rotate_T(int dir)
{
    float origins[4][2] = { { 0.0, 0.0 }, { 0.0, 0.0 }, { 0.0, 0.0 },
                            { 0.0, 0.0 } };
    int d = (dir == 1) ? 1 : 0;
    int cases[2][4][4][2] = {
            { { { 1, 0 }, { 1, 1 }, { 0, 0 }, { 1, -2 } },
              { { 1, 0 }, { 1, -1 }, { 0, 2 }, { 1, 2 } }, { { -1, 0 },
                                                             { 0, 0 },
                                                             { 0, -2 },
                                                             { -1, -2 } },
              { { -1, 0 }, { -1, -1 }, { 0, 2 }, { -1, 2 } } },
            { { { -1, 0 }, { -1, 1 }, { 0, 0 }, { -1, -2 } }, { { 1, 0 },
                                                                { 1, -1 },
                                                                { 0, 2 },
                                                                { 1, 2 } },
              { { 1, 0 }, { 0, 0 }, { 0, -2 }, { 1, -2 } }, { { -1, 0 }, { -1,
                                                                           -1 },
                                                              { 0, 2 },
                                                              { -1, 2 } } } };

    int i;
    for (i = 0; i < 4; i++)
    {
        Block block = (game->current)->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x * -1;
            (game->current)->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = (game->current)->blocks[i].x_offset;
            float y = (game->current)->blocks[i].y_offset;
            (game->current)->blocks[i].y_offset = x;
            (game->current)->blocks[i].x_offset = y * -1;
        }
    }

    int case_num = -1;
    bool success = false;
    float base_x;
    float base_y;

    while ((success == false) && (case_num < 3))
    {
        if (case_num >= 0)
        {
            base_x = (game->current)->x
                    + cases[d][(game->current)->rotation][case_num][0];
            base_y = (game->current)->y
                    + cases[d][(game->current)->rotation][case_num][1];
        }
        else
        {
            base_x = (game->current)->x;
            base_y = (game->current)->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = (game->current)->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || ((game->board)[y][x].empty == false))
            {
                success = false;
            }
        }

        if (success == false)
        {
            break;
        }

        case_num++;
    }

    if (case_num >= 0)
    {
        base_x = (game->current)->x
                + cases[d][(game->current)->rotation][case_num][0];
        base_y = (game->current)->y
                + cases[d][(game->current)->rotation][case_num][1];
    }
    else
    {
        base_x = (game->current)->x;
        base_y = (game->current)->y;
    }

    if (success == true)
    {
        (game->current)->x += base_x;
        (game->current)->y += base_y;
        (game->current)->rotation = ((game->current)->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            (game->current)->blocks[i].x_offset = origins[i][0];
            (game->current)->blocks[i].y_offset = origins[i][1];
        }
    }
}

/******************************************************************************
 * Clears any full lines on the board
 ******************************************************************************/
void clear_lines()
{
    int row = 0;
    while (row < GRID_HEIGHT)
    {
        bool full = true;
        int column;
        for (column = 0; column < GRID_WIDTH; column++)
        {
            Block block = (game->board)[row][column];
            if (block.empty == true)
            {
                row++;
                full = false;
                break;
            }
        }
        if (full)
        {
            Block *temp = (game->board)[row];
            int i;
            for (i = 0; i < GRID_WIDTH; i++)
            {
                (game->board)[row][i].empty = true;
            }

            for (i = row + 1; i < GRID_HEIGHT; i++)
            {
                (game->board)[i - 1] = (game->board)[i];
            }

            (game->board)[GRID_HEIGHT - 1] = temp;
        }
    }
}

/******************************************************************************
 * Checks for game over by looking for non-empty blocks in the spawn area
 ******************************************************************************/
void check_over()
{
    int row;
    for (row = SPAWN_Y; row < GRID_HEIGHT; row++)
    {
        int col;
        for (col = 0; col < GRID_WIDTH; col++)
        {
            if ((game->board)[row][col].empty == false)
            {
                (game->over) = true;
                break;
            }
        }
    }
}

/******************************************************************************
 * Lands the current piece and fills the board accordingly
 ******************************************************************************/
void land_current()
{
    int block_index;
    for (block_index = 0; block_index < 4; block_index++)
    {
        Block block = (game->current)->blocks[block_index];
        int x = (int) (block.x_offset + (game->current)->x);
        int y = (int) (block.y_offset + (game->current)->y);

        (game->board)[y][x].empty = false;
        (game->board)[y][x].linked = false;
        (game->board)[y][x].type = block.type;
    }
    check_over();
    swap_next();
    (game->held_swapped) = false;
    clear_lines();
}

/******************************************************************************
 * Instantly drops the current piece based on it's current location
 ******************************************************************************/
void hard_drop()
{
    if (((game->started) == false) || ((game->over) == true))
    {
        return;
    }

    bool droppable = true;
    while (droppable)
    {
        int block_index;
        bool moveable = true;
        for (block_index = 0; block_index < 4; block_index++)
        {
            Block block = (game->current)->blocks[block_index];
            int x = (int) (block.x_offset + (game->current)->x);
            int y = (int) (block.y_offset + (game->current)->y);

            if (((y - 1) < 0) || ((game->board)[y - 1][x].empty == false))
            {
                land_current();
                check_over();
                droppable = false;
                moveable = false;
                break;
            }
        }

        if (moveable)
        {
            (game->current)->y--;
        }

    }
}

/******************************************************************************
 * Doubles the fall speed of the piece
 ******************************************************************************/
void enable_fast_fall()
{
    (game->fall_speed) *= 2;
}

/******************************************************************************
 * Halves the fall speed of the piece
 ******************************************************************************/
void disable_fast_fall()
{
    (game->fall_speed) /= 2;
}

/******************************************************************************
 * Moves the current piece down if possible based on the update frequency
 ******************************************************************************/
void run_cycle()
{
    if (((game->started) == false) || ((game->over) == true))
    {
        return;
    }

    (game->fall_amount) += (1 / ((float) UPDATE_FREQUENCY))
            * (float) (game->fall_speed);
    if ((game->fall_amount) < 1)
    {
        return;
    }
    (game->fall_amount)--;

    int block_index;
    bool moveable = true;
    for (block_index = 0; block_index < 4; block_index++)
    {
        Block block = (game->current)->blocks[block_index];
        int x = (int) (block.x_offset + (game->current)->x);
        int y = (int) (block.y_offset + (game->current)->y);

        if (((y - 1) < 0) || ((game->board)[y - 1][x].empty == false))
        {
            land_current();
            check_over();
            moveable = false;
            break;
        }
    }

    if (moveable)
    {
        (game->current)->y--;
    }
}

void task_cycle_game(void *pvParameters)
{
    while(1)
    {
        run_cycle();
        xQueueSendToBack(Queue_Game, &game, portMAX_DELAY);
    }
}
