#include <stddef.h>

#include "game.h"

void game_init(game_t* game) {
    if (!game) return;

    game->state = NULL;
    game->should_close = false;
}

void game_free(game_t* game) {
    if (!game) return;
}
