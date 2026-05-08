#pragma once

#include "position.h"

/**
 * @brief Single 4-way direction encoded as a bit flag.
 *
 * Directions are ordered counter-clockwise starting at up.
 * These values are intended for bitwise composition in direction masks.
 */
typedef enum dir4 {
    DIR_NONE  = 0,
    DIR_UP    = 1u << 0,
    DIR_LEFT  = 1u << 2,
    DIR_DOWN  = 1u << 4,
    DIR_RIGHT = 1u << 6,

    DIR4_FIRST= DIR_UP,
    DIR4_LAST = DIR_RIGHT
} dir4;

/**
 * @brief Returns nonzero if `d` is a valid 4-way direction flag.
 */
static inline int dir4_valid(dir4 d) {
    return d == DIR_UP || d == DIR_LEFT || d == DIR_DOWN || d == DIR_RIGHT;
}

/**
 * @brief Single 8-way direction encoded as a bit flag.
 *
 * Main directions use even bit positions and diagonals use odd bit positions.
 * Directions are ordered counter-clockwise starting at up.
 */
typedef enum dir8 {
    // DIR_NONE         = 0,
    // DIR_UP           = 1u << 0,
    DIR_TOP_LEFT     = 1u << 1,
    // DIR_LEFT         = 1u << 2,
    DIR_BOTTOM_LEFT  = 1u << 3,
    // DIR_DOWN         = 1u << 4,
    DIR_BOTTOM_RIGHT = 1u << 5,
    // DIR_RIGHT        = 1u << 6,
    DIR_TOP_RIGHT    = 1u << 7,

    DIR_TL = DIR_TOP_LEFT,
    DIR_TR = DIR_TOP_RIGHT,
    DIR_BL = DIR_BOTTOM_LEFT,
    DIR_BR = DIR_BOTTOM_RIGHT,

    DIR8_FIRST= DIR_UP,
    DIR8_LAST = DIR_BR
} dir8;

/**
 * @brief Returns nonzero if `d` is a valid 8-way direction flag.
 */
static inline int dir8_valid(dir8 d) {
    return d != 0 && (d & (d - 1)) != 0; // is power of two
}

/**
 * @brief Returns the opposite 8-way direction.
 */
static inline dir8 opposite_d8(dir8 d) {
    switch (d) {
    case DIR_UP:           return DIR_DOWN;
    case DIR_TOP_LEFT:     return DIR_BOTTOM_RIGHT;
    case DIR_LEFT:         return DIR_RIGHT;
    case DIR_BOTTOM_LEFT:  return DIR_TOP_RIGHT;
    case DIR_DOWN:         return DIR_UP;
    case DIR_BOTTOM_RIGHT: return DIR_TOP_LEFT;
    case DIR_RIGHT:        return DIR_LEFT;
    case DIR_TOP_RIGHT:    return DIR_BOTTOM_LEFT;
    default:               return DIR_NONE;
    }
}

/**
 * @brief Normalized 2D direction vector.
 *
 * @param x Direction on the x-axis: -1, 0, or 1.
 * @param y Direction on the y-axis: -1, 0, or 1.
 */
typedef position_t dir_t;

/**
 * @brief Returns nonzero if `d` is a non-zero direction vector.
 */
static inline int is_dir(dir_t d) {
    return d.x != 0 || d.y != 0;
}

/**
 * @brief Converts an 8-way direction flag to a vector.
 */
static inline dir_t get_dir(dir8 d) {
    switch (d) {
    case DIR_UP:           return (dir_t){  0, -1 };
    case DIR_TOP_LEFT:     return (dir_t){ -1, -1 };
    case DIR_LEFT:         return (dir_t){ -1,  0 };
    case DIR_BOTTOM_LEFT:  return (dir_t){ -1,  1 };
    case DIR_DOWN:         return (dir_t){  0,  1 };
    case DIR_BOTTOM_RIGHT: return (dir_t){  1,  1 };
    case DIR_RIGHT:        return (dir_t){  1,  0 };
    case DIR_TOP_RIGHT:    return (dir_t){  1, -1 };
    default:               return (dir_t){  0,  0 };
    }
}

/**
 * @brief Returns the opposite vector of @p d.
 */
static inline dir_t opposite_dir(dir_t d) {
    return (dir_t){ -d.x, -d.y };
}

/**
 * @brief A set of zero or more 8-way directions.
 */
typedef unsigned char dir_set_t;
