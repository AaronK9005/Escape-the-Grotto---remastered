#ifndef _WIN32
    #error Compilation failed: Currently this game only support Windows.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>

#include "game.h"
#include "game_state.h"
#include "game_view.h"
#include "input_handle.h"
#include "util/ansi_wrappers.h"

#include "game_states/test_state.h"
// #include "game_states/chunk_loader_state.h"

void inform_invalid_gamestate(int ret);

int main()
{
    printf("loading game...\n");

    srand(time(NULL));

    int _return = 0;
    game_t game = { 0 };
    game_view_t view = { 0 };
    input_handle_t input = { INPUT_NONE };

    game_init(&game, "test", &test_gs);
    view_init(&view);

    ansi_hide_cursor();
    ansi_clear_screen();

    while (!game.should_close && game.state)
    {
        input_poll(&input);

        if (game.state->handle_input)
        {
            game.state->handle_input(&input, &game, &view);
        }
        else
        {
            _return = 1;
            break;
        }

        if (game.state->update)
        {
            game.state->update(&game);
        }
        else
        {
            _return = 2;
            break;
        }


        if (game.state->render)
        {
            game.state->render(&game, &view);
        }
        else
        {
            _return = 3;
            break;
        }

    }
    
    view_free(&view);
    game_free(&game);

    ansi_clear_screen();
    ansi_reset();
    ansi_show_cursor();

    if (_return == 0) {
        printf("game ended\n");
        return 0;
    }
    else
    {
        inform_invalid_gamestate(_return);
    }

    return _return;
}

void inform_invalid_gamestate(int ret) {
    ansi_clear_screen();
    const char* func_name = NULL;
    switch (ret)
    {
    case 1: func_name = "handle_input"; break;
    case 2: func_name = "update"; break;
    case 3: func_name = "render"; break;
    }
    printf("game.state has invalid '%s'\n", func_name);
}