#pragma once

#include <stdbool.h>

typedef struct game_state_t game_state_t;

/**
 * @brief Central struct containing all game data
 */
typedef struct game_t
{
    game_state_t* state;
    bool should_close;
} game_t;

void game_init(game_t* game);
void game_free(game_t* game);