#pragma once

/**
 * @brief Input enum type
 */
typedef enum {
    INPUT_NONE = -1,
    INPUT_QUIT_PROGRAM = 'q',

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