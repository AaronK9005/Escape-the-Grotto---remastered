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

int main()
{
    printf("loading game...\n");

    srand(time(NULL));

    game_t game = { 0 };
    game_view_t view = { 0 };
    input_handle_t input = { 0 };

    game_init(&game);
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
        else break;

        if (game.state->update)
        {
            game.state->update(&game);
        }
        else break;

        if (game.state->render)
        {
            game.state->render(&game, &view);
        }
        else break;
    }
    
    view_free(&view);
    game_free(&game);

    printf("game ended\n");

    return 0;
}