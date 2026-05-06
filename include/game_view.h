#pragma once

typedef struct game_view_t
{
    int placeholder;
} game_view_t;

void view_init(game_view_t* view);
void view_free(game_view_t* view);