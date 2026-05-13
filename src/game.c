#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
    #include <direct.h>
#endif

#include "game.h"
#include "game_state.h"
#include "world/floor.h"

#define N_FLOORS 5

static inline void init_name(game_t* game, const char* name) {
    if (!name) snprintf(game->name, sizeof(game->name), "%d", time(NULL));
    else snprintf(game->name, sizeof(game->name), "%s", name);
}

static void init_folder(const char* name) {
    if (!name) return;

    char buffer[G_NAME_LEN + 32];
    snprintf(buffer, sizeof(buffer), "saved/%s", name);

#ifdef _WIN32
    _mkdir(buffer);
#endif
}

static int init_world(game_t* g) {
    const int n_floors = N_FLOORS;

    floor_t** f_storage = malloc(n_floors * sizeof(floor_t*));
    if (!f_storage) return ERR_OOM;

    for (int i = 0; i < n_floors; i++) {
        floor_t* new_f = floor_create(g->name, i);
        if (!new_f) {
            free(f_storage);
            return ERR_OOM;
        }
        f_storage[i] = new_f;

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "saved/%s/floor_%d", g->name, i);

#ifdef _WIN32
        _mkdir(buffer);
#endif
        
    }

    g->floor_storage = f_storage;
    g->number_of_floors = n_floors;
    g->current_floor = *f_storage[n_floors - 1];
    g->visited_floors |= (1 << (n_floors - 1));

    return RET_OK;
}

int game_init(game_t* game, const char* name, game_state_t* g_state) {
    if (!game || !g_state) return 1;

    memset(game, 0, sizeof(game_t));

    game->state = g_state;

    init_name(game, name);
    init_folder(game->name);
    init_world(game);
    
    g_state->init(game); // must be last, init() methods need valid game (mainly floor)

    return 0;
}

void game_free(game_t* game) {
    if (!game) return;

    if (game->floor_storage) {
        for (int i = 0; i < game->number_of_floors; i++) {
            if (game->floor_storage[i]) {
                floor_destroy(game->floor_storage[i]);
                game->floor_storage[i] = NULL;
            }
        }
        free(game->floor_storage);
        game->floor_storage = NULL;
    }
}

int game_change_state(game_t* game, game_state_t* state) {
    if (!game || !state) return 1;

    if (game->state->finish(game) != RET_OK) return 2;
    if (state->init(game) != RET_OK) return 3;

    game->state = state;

    return RET_OK;
}

void game_save(game_t* game) {
    if (!game) return;



    // FILE* f = fopen("")
}

void game_load(game_t* game, const char* name) {
    if (!game || !name) return;
}

int game_rename(game_t* game, const char* name) {
    if (!game || !name) return ERR_RENAME;

    char old_folder[G_NAME_LEN + 32];
    char new_folder[G_NAME_LEN + 32];

    snprintf(old_folder, sizeof(old_folder), "saved/%s", game->name);
    snprintf(new_folder, sizeof(new_folder), "saved/%s", name);

    if (rename(old_folder, new_folder) != 0) {
        perror("rename");
        return ERR_RENAME;
    }

    init_name(game, name);

    for (int i = 0; i < game->number_of_floors; i++) {
        floor_rename(game->floor_storage[i], game->name);
    }

    return RET_OK;
}