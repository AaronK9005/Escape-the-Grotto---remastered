#include "game_states/test_state.h"
#include "input_handle.h"
#include "game.h"
#include "game_view.h"
#include "world/floor.h"
#include "direction.h"
#include "util/ansi_wrappers.h"

int putchar(int);

static int cp_offset_x = 0;
static int cp_offset_y = 0;

static position_t player = { 0, 0 };
static int render_dist = 10;
static int saved_promt_counter = 0;

int test_i(game_t* game) {
    bool found = false;
    for (int y = 5; y < MAP_SIZE - 5 && !found; y++) {
        for (int x = 5; x < MAP_SIZE - 5; x++) {
            position_t new_pos = pos_new(x, y);
            if (floor_tile_walkable(&game->current_floor, new_pos)) {
                player = new_pos;
                found = true;
                break;
            }
        }
    }

    return RET_OK;
}

int test_hi(input_handle_t* input, game_t* game, game_view_t* view) {
    switch (input->_char)
    {
    case INPUT_QUIT_PROGRAM: 
        game->should_close = true;
        break;
    case INPUT_MOVE_UP:
    {
        position_t new_player_pos = pos_sum(player, get_dir(DIR_UP));
        if (floor_tile_walkable(&game->current_floor, new_player_pos)) {
            view->should_render = true;
            if (!pos_same(glob_to_chunk(player), glob_to_chunk(new_player_pos))) {
                floor_shift_chunks(&game->current_floor, DIR_UP);
                cp_offset_y--;
            }
            player.y--;
        }
    }
        break;
    case INPUT_MOVE_LEFT:
    {
        position_t new_player_pos = pos_sum(player, get_dir(DIR_LEFT));
        if (floor_tile_walkable(&game->current_floor, new_player_pos)) {
            view->should_render = true;
            if (!pos_same(glob_to_chunk(player), glob_to_chunk(new_player_pos))) {
                floor_shift_chunks(&game->current_floor, DIR_LEFT);
                cp_offset_x--;
            }
            player.x--;
        }
    }
        break;
    case INPUT_MOVE_DOWN:
    {
        position_t new_player_pos = pos_sum(player, get_dir(DIR_DOWN));
        if (floor_tile_walkable(&game->current_floor, new_player_pos)) {
            view->should_render = true;
            if (!pos_same(glob_to_chunk(player), glob_to_chunk(new_player_pos))) {
                floor_shift_chunks(&game->current_floor, DIR_DOWN);
                cp_offset_y++;
            }
            player.y++;
        }
    }
        break;
    case INPUT_MOVE_RIGHT:
    {
        position_t new_player_pos = pos_sum(player, get_dir(DIR_RIGHT));
        if (floor_tile_walkable(&game->current_floor, new_player_pos)) {
            view->should_render = true;
            if (!pos_same(glob_to_chunk(player), glob_to_chunk(new_player_pos))) {
                floor_shift_chunks(&game->current_floor, DIR_RIGHT);
                cp_offset_x++;
            }
            player.x++;
        }
    }
        break;
    case INPUT_SAVE_GAME:
        floor_save(&game->current_floor);
        ansi_mvprintf(60, 3, "saved");
        break;
    }

    return RET_OK;
}

int test_r(game_t* game, game_view_t* view) {
    if (!view->should_render) return RET_OK;

    view->should_render = false;
    ansi_goto_home();
    for (int i = -render_dist; i <= render_dist; i++) {
        for (int j = -render_dist; j <= render_dist; j++) {
            putchar(
                tile_char[
                    floor_get_tile(
                        &game->current_floor,
                        pos_sum(player, pos_new(j, i))
                    )
                ]
            );
        }
        putchar('\n');
    }
    ansi_mvputchar(render_dist + 1, render_dist + 1, '@');
    ansi_mvprintf(60, 1, "chunk: (%d, %d)       \n", cp_offset_x, cp_offset_y);
    ansi_mvprintf(60, 2, "player.pos: (%d, %d)      \n", player.x, player.y);

    return RET_OK;
}

game_state_t test_gs = { 
    .init = test_i,
    .handle_input = test_hi,
    .update = noop_update,
    .render = test_r,
    .finish = noop_finish
};
