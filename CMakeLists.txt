cmake_minimum_required(VERSION 4.2.1)
project(EscapeTheGrotto VERSION 1.1 LANGUAGES C)

add_executable(game 
    src/main.c
    src/input_handle.c
    src/ansi_wrappers.c
    src/game.c
    src/game_view.c
    src/chunk.c
    src/chunk_generate.c
    src/tiles.c
    src/floor.c
    src/test_state.c
)

target_include_directories(game PRIVATE include)