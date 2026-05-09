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

int test_hi(input_handle_t* input, game_t* game, game_view_t* view) {
    switch (input->_char)
    {
    case INPUT_QUIT_PROGRAM: 
        game->should_close = true;
        break;
    case INPUT_MOVE_UP:
        floor_shift_chunks(&game->current_floor, DIR_UP);
        cp_offset_y--;
        view->should_render = true;
        break;
    case INPUT_MOVE_LEFT:
        floor_shift_chunks(&game->current_floor, DIR_LEFT);
        cp_offset_x--;
        view->should_render = true;
        break;
    case INPUT_MOVE_DOWN:
        floor_shift_chunks(&game->current_floor, DIR_DOWN);
        cp_offset_y++;
        view->should_render = true;
        break;
    case INPUT_MOVE_RIGHT:
        floor_shift_chunks(&game->current_floor, DIR_RIGHT);
        cp_offset_x++;
        view->should_render = true;
        break;
    case INPUT_SAVE_GAME:
        floor_save(&game->current_floor);
        ansi_save_pos();
        ansi_mvprintf(60, 2, "game saved");
        ansi_use_pos();
        break;
    }
    return RET_OK;
}

int test_r(game_t* game, game_view_t* view) {
    if (!view->should_render) return RET_OK;

    view->should_render = false;
    ansi_clear_screen();
    for (int i = 0; i < MAP_SIZE / 2; i++) {
        for (int j = 0; j < MAP_SIZE / 2; j++) {
            putchar(
                tile_char[
                    floor_get_tile(
                        &game->current_floor,
                        pos_new(
                            MAP_SIZE * cp_offset_x + j,
                            MAP_SIZE * cp_offset_y + i
                        )
                    )
                ]
            );
        }
        putchar('\n');
    }
    ansi_mvprintf(50, 1, "chunk: (%d, %d)       \n", cp_offset_x, cp_offset_y);

    return RET_OK;
}

game_state_t test_gs = { test_hi, noop_update, test_r };
