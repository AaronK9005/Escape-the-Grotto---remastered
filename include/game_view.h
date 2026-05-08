#pragma once

#include <stdbool.h>

typedef struct game_view_t
{
    bool should_render;
} game_view_t;

void view_init(game_view_t* view);
void view_free(game_view_t* view);