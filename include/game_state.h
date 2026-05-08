#pragma once

typedef struct game_t game_t;
typedef struct input_handle_t input_handle_t;
typedef struct game_view_t game_view_t;

typedef struct game_state_t
{
    int (*handle_input)(input_handle_t* input, game_t* game, game_view_t* view);
    int (*update)(game_t* game);
    int (*render)(game_t* game, game_view_t* view);
} game_state_t;

// return value for int(*)(_) to signal success
#define RET_OK 0

// return value for int(*)(_) to signal failure
#define RET_FAIL 1

static int noop_handle_input(input_handle_t* input, game_t* game, game_view_t* view) { return RET_OK; }
static int noop_update(game_t* game) { return RET_OK; }
static int noop_render(game_t* game, game_view_t* view) { return RET_OK; }
