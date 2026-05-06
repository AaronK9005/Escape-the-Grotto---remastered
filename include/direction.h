#pragma once

/**
 * @brief Int id for 4-way direction
 */
typedef enum direction_4 {
    DIR_NONE = 0,
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN,
    DIR_RIGHT
} dir4;

static inline int dir4_valid(dir4 d) {
    return d > DIR_NONE && d <= DIR_RIGHT;
}

/**
 * @brief Int id for 8-way direction
 */
typedef enum direction_8 {
    DIR_NONE = 0,
    DIR_UP,
    DIR_LEFT,
    DIR_DOWN,
    DIR_RIGHT,
    DIR_TOP_LEFT = DIR_RIGHT + 1,
    DIR_TOP_RIGHT,
    DIR_BOTTOM_LEFT,
    DIR_BOTTOM_RIGHT,

    DIR_TL = DIR_TOP_LEFT,
    DIR_TR = DIR_TOP_RIGHT,
    DIR_BL = DIR_BOTTOM_LEFT,
    DIR_BR = DIR_BOTTOM_RIGHT
} dir8;

static inline int dir8_valid(dir8 d) {
    return d > DIR_NONE && d <= DIR_BOTTOM_RIGHT;
}

static inline dir8 opposite_d8(dir8 d8) {
    switch (d8)
    {
    case DIR_UP: return DIR_DOWN;
    case DIR_LEFT: return DIR_RIGHT;
    case DIR_DOWN: return DIR_UP;
    case DIR_RIGHT: return DIR_LEFT;
    case DIR_TL: return DIR_BR;
    case DIR_TR: return DIR_BL;
    case DIR_BL: return DIR_TR;
    case DIR_BR: return DIR_TL;
    }
    return DIR_NONE;
}

/**
 * @brief Normilized 2d vector for representing direction
 * @param dx Direction in x-axis (-1, 0, 1)
 * @param dy Direction in y-axis (-1, 0, 1)
 */
typedef struct dir_t {
    int dx, dy;
} dir_t;

static inline dir_t get_dir(dir8 d8) {
    switch (d8)
    {
    case DIR_UP: return (dir_t) { 0, -1 };
    case DIR_LEFT: return (dir_t) { -1, 0 };
    case DIR_DOWN: return (dir_t) { 0, 1 };
    case DIR_RIGHT: return (dir_t) { 1, 0 };
    case DIR_TL: return (dir_t) { -1, -1 };
    case DIR_TR: return (dir_t) { 1, -1 };
    case DIR_BL: return (dir_t) { -1, 1 };
    case DIR_BR: return (dir_t) { 1, 1 };
    }
    return (dir_t) { 0, 0 };
}

static inline dir_t opposite_dir(dir_t dr) {
    return (dir_t) { -dr.dx, -dr.dy };
}