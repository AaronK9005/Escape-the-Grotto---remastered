#include "game_view.h"

void view_init(game_view_t* view) {
    if (!view) return;

    view->should_render = false;
}

void view_free(game_view_t* view) {
    if (!view) return;
}