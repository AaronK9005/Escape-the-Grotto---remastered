#pragma once

/**
 * @brief Input enum type
 */
typedef enum {
    INPUT_NONE = -1,
    INPUT_QUIT_PROGRAM = 'q',
    INPUT_SAVE_GAME = 'S', // ctrl + S == 19

    INPUT_MOVE_UP = 'w',
    INPUT_MOVE_LEFT = 'a',
    INPUT_MOVE_DOWN = 's',
    INPUT_MOVE_RIGHT = 'd',

    INPUT_ARROW_UP = 1,
    INPUT_ARROW_LEFT,
    INPUT_ARROW_DOWN,
    INPUT_ARROW_RIGHT,

    INPUT_LEFT_CLICK = 'k',
    INPUT_RIGHT_CLICK = 'l'
} input_t;

#define CASE_INPUT_MOVES case INPUT_MOVE_UP: case INPUT_MOVE_LEFT: case INPUT_MOVE_DOWN: case INPUT_MOVE_RIGHT
#define CASE_INPUT_ARROWS case INPUT_ARROW_UP: case INPUT_ARROW_LEFT: case INPUT_ARROW_DOWN: case INPUT_ARROW_RIGHT

/**
 * @brief Wrapper for input handling
 * @param _char current input
 */
typedef struct input_handle_t
{
    input_t _char;
} input_handle_t;

/**
 * @brief Gets one input or INPUT_NONE if none awailable
 * @param ih input_handle_t to fill
 * @note NULL-safe
 */
void input_poll(input_handle_t* ih);