#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #include <direct.h>
#endif

#include "game.h"
#include "world/floor.h"

#define N_FLOORS 5

static void init_folder(const char* name) {
    if (!name) return;

    char buffer[256];
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
        floor_t* new_f = floor_create(i);
        if (!new_f) {
            free(f_storage);
            return ERR_OOM;
        }
        f_storage[i] = new_f;

        char buffer[32];
        snprintf(buffer, sizeof(buffer), "saved/floor_%d", i);

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

int game_init(game_t* game) {
    if (!game) return 1;

    memset(game, 0, sizeof(game_t));

    init_folder(game->name);

    return init_world(game);
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
