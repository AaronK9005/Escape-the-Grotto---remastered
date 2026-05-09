#pragma once

#include <stdbool.h>

#include "world/floor_structure.h"

typedef struct game_state_t game_state_t;
typedef struct world_t world_t;
typedef int bit_field32;

#define G_NAME_LEN 64

/**
 * @brief Central struct containing all game data
 */
typedef struct game_t
{
    char name[G_NAME_LEN];
    game_state_t* state;
    bool should_close;

    struct { // world
        floor_t current_floor;
        bit_field32 visited_floors;
        int number_of_floors;
        floor_t** floor_storage;
    };
    
} game_t;

#define RET_OK 0
#define ERR_OOM 1
#define ERR_RENAME 2

int game_init(game_t* game, const char* name, game_state_t* g_state);
void game_free(game_t* game);

void game_save(game_t* game);
void game_load(game_t* game, const char* name);

int game_rename(game_t* game, const char* name);