#pragma once
#include <stdint.h>

// uint8_t is used to save memory, as each tile only needs 4 bytes to store its state (revealed, flagged, mine, number)
typedef struct
{
    uint8_t revealed;
    uint8_t flagged;

    uint8_t mine;
    uint8_t number;

} tile;

typedef struct
{
    float mine_probability;

    uint8_t frontier;
    uint8_t constrained;

} tile_info;

// define dimensions of the game board
#define WIDTH 9
#define HEIGHT 9

typedef struct
{
    tile board[HEIGHT][WIDTH];
    tile_info local_board_info[HEIGHT][WIDTH];

    uint16_t total_mines;
    uint16_t flagged_mines;

    // 1 if true, 0 if false
    uint8_t loose;
    uint8_t win;

} shared_memory_t;

extern shared_memory_t shared_memory;