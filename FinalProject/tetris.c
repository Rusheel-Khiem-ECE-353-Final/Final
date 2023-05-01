/*
 * tetris.c
 *
 *  Created on: Apr 27, 2023
 *     Authors: Rusheel Dasari and Khiem Vu
 */

#include "tetris.h"

void init_board()
{
    int row, col;

    board = malloc(sizeof(Block*) * GRID_HEIGHT);
    if (board == NULL)
    {
        exit(1);
    }

    for (row = 0; row < GRID_HEIGHT; row++)
    {
        *(board + row) = malloc(sizeof(Block) * GRID_WIDTH);
        if ((*(board + row)) == NULL)
        {
            exit(1);
        }
    }

    for (row = 0; row < GRID_HEIGHT; row++)
    {
        for (col = 0; col < GRID_WIDTH; col++)
        {
            board[row][col].empty = true;
            board[row][col].linked = false;
            board[row][col].x = col;
            board[row][col].y = row;
            board[row][col].x_offset = 0;
            board[row][col].y_offset = 0;
            board[row][col].axis = false;
            board[row][col].type = EMPTY;
        }
    }
}

void init_game()
{
    init_board();
    current = malloc(sizeof(Piece));
    next = malloc(sizeof(Piece));
    held = malloc(sizeof(Piece));

    if ((current == NULL) || (next == NULL) || (held == NULL))
    {
        exit(1);
    }

    current->blocks = malloc(sizeof(Block) * 4);
    next->blocks = malloc(sizeof(Block) * 4);
    held->blocks = malloc(sizeof(Block) * 4);

    if ((current->blocks == NULL) || (next->blocks == NULL)
            || (held->blocks == NULL))
    {
        exit(1);
    }

    current->x = current->y = -1;
    next->x = next->y = -1;
    held->x = held->y = -1;
    current->rotation = next->rotation = held->rotation = 0;
    current->rotate = next->rotate = held->rotate = NULL;
    current->type = next->type = held->type = EMPTY;
    started = true;
    over = false;
    held_swapped = false;
}

void delete_board()
{
    int row;
    for (row = 0; row < GRID_HEIGHT; row++)
    {
        free(*(board + row));
        (*(board + row)) = NULL;
    }

    free(board);
    board = NULL;
}

void delete_game()
{
    delete_board();

    free(current->blocks);
    current->blocks = NULL;

    free(next->blocks);
    next->blocks = NULL;

    free(held->blocks);
    held->blocks = NULL;

    free(current);
    current = NULL;

    free(next);
    next = NULL;

    free(held);
    held = NULL;
}

void rotate_current_right()
{
    if (current != NULL && current->rotate != NULL)
    {
        current->rotate(1);
    }
}

void rotate_current_left()
{
    if (current != NULL && current->rotate != NULL)
    {
        current->rotate(-1);
    }
}

void move_current_right()
{
    if (current == NULL)
    {
        return;
    }

    int block_index;
    bool moveable = true;
    for (block_index = 0; block_index < 4; block_index++)
    {
        Block block = current->blocks[block_index];
        int x = (int) (block.x_offset + current->x);
        int y = (int) (block.y_offset + current->y);

        if (((x + 1) > 9) || (board[y][x + 1].empty == false))
        {
            moveable = false;
            break;
        }
    }

    if (moveable)
    {
        current->x++;
    }
}

void move_current_left()
{
    if (current == NULL)
    {
        return;
    }

    int block_index;
    bool moveable = true;
    for (block_index = 0; block_index < 4; block_index++)
    {
        Block block = current->blocks[block_index];
        int x = (int) (block.x_offset + current->x);
        int y = (int) (block.y_offset + current->y);

        if (((x - 1) < 0) || (board[y][x - 1].empty == false))
        {
            moveable = false;
            break;
        }
    }

    if (moveable)
    {
        current->x--;
    }
}

void swap_held()
{
    if (held_swapped == false)
    {
        if (held->type == EMPTY)
        {
            Piece *temp = held;
            held = current;
            current = temp;
            swap_next();
        }
        else
        {
            Piece *temp = held;
            held = current;
            current = temp;
            spawn_current();
        }
        held_swapped = true;
    }
}

void swap_next()
{
    if (next->type == EMPTY)
    {
        generate_next();
    }

    Piece *temp = next;
    next = current;
    next->type = EMPTY;
    next->x = next->y = -1;
    current = temp;
    spawn_current();
    generate_next();
}

void spawn_current()
{
    switch (current->type)
    {
    case LINE:
        current->x = SPAWN_X + 0.5;
        current->y = SPAWN_Y + 0.5;
        break;
    case SQUARE:
        current->x = SPAWN_X + 0.5;
        current->y = SPAWN_Y + 0.5;
        break;
    case L:
        current->x = SPAWN_X;
        current->y = SPAWN_Y;
        break;
    case J:
        current->x = SPAWN_X;
        current->y = SPAWN_Y;
        break;
    case S:
        current->x = SPAWN_X;
        current->y = SPAWN_Y;
        break;
    case Z:
        current->x = SPAWN_X;
        current->y = SPAWN_Y;
        break;
    case T:
        current->x = SPAWN_X;
        current->y = SPAWN_Y;
        break;
    default:
        current->x = current->y = -1;
        break;
    }
}

void generate_next()
{
    PieceType type = (PieceType) (rand() % 7 + 1);
    next->type = type;
    next->x = -1;
    next->y = -1;
    next->rotation = 0;

    switch (next->type)
    {
    case LINE:
        generate_line(next);
        next->rotate = rotate_line;
        break;
    case SQUARE:
        generate_square(next);
        next->rotate = rotate_square;
        break;
    case J:
        generate_J(next);
        next->rotate = rotate_J;
        break;
    case L:
        generate_L(next);
        next->rotate = rotate_L;
        break;
    case S:
        generate_S(next);
        next->rotate = rotate_S;
        break;
    case Z:
        generate_Z(next);
        next->rotate = rotate_Z;
        break;
    case T:
        generate_T(next);
        next->rotate = rotate_T;
        break;
    default:
        next->rotate = NULL;
        break;
    }
}

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
        Block block = current->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x * -1;
            current->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x;
            current->blocks[i].x_offset = y * -1;
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
            base_x = current->x + cases[d][current->rotation][case_num][0];
            base_y = current->y + cases[d][current->rotation][case_num][1];
        }
        else
        {
            base_x = current->x;
            base_y = current->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = current->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || (board[y][x].empty == false))
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
        base_x = current->x + cases[d][current->rotation][case_num][0];
        base_y = current->y + cases[d][current->rotation][case_num][1];
    }
    else
    {
        base_x = current->x;
        base_y = current->y;
    }

    if (success == true)
    {
        current->x += base_x;
        current->y += base_y;
        current->rotation = (current->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            current->blocks[i].x_offset = origins[i][0];
            current->blocks[i].y_offset = origins[i][1];
        }
    }
}

void rotate_square(int dir)
{
    return;
}

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
        Block block = current->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x * -1;
            current->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x;
            current->blocks[i].x_offset = y * -1;
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
            base_x = current->x + cases[d][current->rotation][case_num][0];
            base_y = current->y + cases[d][current->rotation][case_num][1];
        }
        else
        {
            base_x = current->x;
            base_y = current->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = current->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || (board[y][x].empty == false))
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
        base_x = current->x + cases[d][current->rotation][case_num][0];
        base_y = current->y + cases[d][current->rotation][case_num][1];
    }
    else
    {
        base_x = current->x;
        base_y = current->y;
    }

    if (success == true)
    {
        current->x += base_x;
        current->y += base_y;
        current->rotation = (current->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            current->blocks[i].x_offset = origins[i][0];
            current->blocks[i].y_offset = origins[i][1];
        }
    }
}
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
        Block block = current->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x * -1;
            current->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x;
            current->blocks[i].x_offset = y * -1;
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
            base_x = current->x + cases[d][current->rotation][case_num][0];
            base_y = current->y + cases[d][current->rotation][case_num][1];
        }
        else
        {
            base_x = current->x;
            base_y = current->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = current->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || (board[y][x].empty == false))
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
        base_x = current->x + cases[d][current->rotation][case_num][0];
        base_y = current->y + cases[d][current->rotation][case_num][1];
    }
    else
    {
        base_x = current->x;
        base_y = current->y;
    }

    if (success == true)
    {
        current->x += base_x;
        current->y += base_y;
        current->rotation = (current->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            current->blocks[i].x_offset = origins[i][0];
            current->blocks[i].y_offset = origins[i][1];
        }
    }
}
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
        Block block = current->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x * -1;
            current->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x;
            current->blocks[i].x_offset = y * -1;
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
            base_x = current->x + cases[d][current->rotation][case_num][0];
            base_y = current->y + cases[d][current->rotation][case_num][1];
        }
        else
        {
            base_x = current->x;
            base_y = current->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = current->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || (board[y][x].empty == false))
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
        base_x = current->x + cases[d][current->rotation][case_num][0];
        base_y = current->y + cases[d][current->rotation][case_num][1];
    }
    else
    {
        base_x = current->x;
        base_y = current->y;
    }

    if (success == true)
    {
        current->x += base_x;
        current->y += base_y;
        current->rotation = (current->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            current->blocks[i].x_offset = origins[i][0];
            current->blocks[i].y_offset = origins[i][1];
        }
    }
}
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
        Block block = current->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x * -1;
            current->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x;
            current->blocks[i].x_offset = y * -1;
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
            base_x = current->x + cases[d][current->rotation][case_num][0];
            base_y = current->y + cases[d][current->rotation][case_num][1];
        }
        else
        {
            base_x = current->x;
            base_y = current->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = current->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || (board[y][x].empty == false))
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
        base_x = current->x + cases[d][current->rotation][case_num][0];
        base_y = current->y + cases[d][current->rotation][case_num][1];
    }
    else
    {
        base_x = current->x;
        base_y = current->y;
    }

    if (success == true)
    {
        current->x += base_x;
        current->y += base_y;
        current->rotation = (current->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            current->blocks[i].x_offset = origins[i][0];
            current->blocks[i].y_offset = origins[i][1];
        }
    }
}
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
        Block block = current->blocks[i];
        origins[i][0] = block.x_offset;
        origins[i][1] = block.y_offset;
    }

    if (dir == 1)
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x * -1;
            current->blocks[i].x_offset = y;
        }
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            float x = current->blocks[i].x_offset;
            float y = current->blocks[i].y_offset;
            current->blocks[i].y_offset = x;
            current->blocks[i].x_offset = y * -1;
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
            base_x = current->x + cases[d][current->rotation][case_num][0];
            base_y = current->y + cases[d][current->rotation][case_num][1];
        }
        else
        {
            base_x = current->x;
            base_y = current->y;
        }

        success = true;
        for (i = 0; i < 4; i++)
        {
            Block block = current->blocks[i];
            int x = (int) (block.x_offset + base_x);
            int y = (int) (block.y_offset + base_y);
            if (((x < 0) || (x > 9) || (y < 0) || (y > 19))
                    || (board[y][x].empty == false))
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
        base_x = current->x + cases[d][current->rotation][case_num][0];
        base_y = current->y + cases[d][current->rotation][case_num][1];
    }
    else
    {
        base_x = current->x;
        base_y = current->y;
    }

    if (success == true)
    {
        current->x += base_x;
        current->y += base_y;
        current->rotation = (current->rotation + dir + 4) % 4;
    }
    else
    {
        for (i = 0; i < 4; i++)
        {
            current->blocks[i].x_offset = origins[i][0];
            current->blocks[i].y_offset = origins[i][1];
        }
    }
}

void clear_lines(void)
{
    int row = 0;
    while (row < GRID_HEIGHT)
    {
        bool full = true;
        int column;
        for (column = 0; column < GRID_WIDTH; column++)
        {
            Block block = board[row][column];
            if (block.empty == true)
            {
                row++;
                full = false;
                break;
            }
        }
        if (full)
        {
            Block *temp = board[row];
            int i;
            for (i = 0; i < GRID_WIDTH; i++)
            {
                board[row][i].empty = true;
            }

            for (i = row + 1; i < GRID_HEIGHT; i++)
            {
                board[i - 1] = board[i];
            }

            board[GRID_HEIGHT - 1] = temp;
        }
    }
}

void check_over(void)
{
    int row;
    for (row = SPAWN_Y; row < GRID_HEIGHT; row++)
    {
        int col;
        for (col = 0; col < GRID_WIDTH; col++)
        {
            if (board[row][col].empty == false)
            {
                over = true;
                break;
            }
        }
    }
}

void land_current()
{
    int block_index;
    for (block_index = 0; block_index < 4; block_index++)
    {
        Block block = current->blocks[block_index];
        int x = (int) (block.x_offset + current->x);
        int y = (int) (block.y_offset + current->y);

        board[y][x].empty = false;
        board[y][x].linked = false;
        board[y][x].type = block.type;
    }
    check_over();
    swap_next();
    held_swapped = false;
    clear_lines();
}

void hard_drop()
{
    if ((started == false) || (over == true))
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
            Block block = current->blocks[block_index];
            int x = (int) (block.x_offset + current->x);
            int y = (int) (block.y_offset + current->y);

            if (((y - 1) < 0) || (board[y - 1][x].empty == false))
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
            current->y--;
        }

    }
}

void enable_fast_fall()
{
    fall_speed *= 2;
}

void disable_fast_fall()
{
    fall_speed /= 2;
}

void run_cycle(void)
{
    if ((started == false) || (over == true))
    {
        return;
    }

    fall_amount += (1 / ((float) UPDATE_FREQUENCY)) * (float) fall_speed;
    if (fall_amount < 1)
    {
        return;
    }
    fall_amount = 0;

    int block_index;
    bool moveable = true;
    for (block_index = 0; block_index < 4; block_index++)
    {
        Block block = current->blocks[block_index];
        int x = (int) (block.x_offset + current->x);
        int y = (int) (block.y_offset + current->y);

        if (((y - 1) < 0) || (board[y - 1][x].empty == false))
        {
            land_current();
            check_over();
            moveable = false;
            break;
        }
    }

    if (moveable)
    {
        current->y--;
    }
}
