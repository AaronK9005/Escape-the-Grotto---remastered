#pragma once

#include <limits.h>

/**
 * @brief Main type for GLOBAL 2d position
 * 
 * Supports aritmetic operations for both numbers and positions
 * 
 * @note for chunk position use `chunk_position_t`
 * @note for position inside a chunk use `local_position_t`
 * @note invalid state via `pos_none()`
 * @warning limits same as `int`
 */
typedef struct position_t
{
    int x, y;
} position_t;

#define POS_INVALID_MEMBER INT_MIN

/**
 * @brief Creates new `position_t`
 */
static inline position_t pos_new(int x, int y) {
    return (position_t){ x, y };
}

/**
 * @brief Creates new invalid `position_t`
 */
static inline position_t pos_none() {
    return (position_t){ POS_INVALID_MEMBER, POS_INVALID_MEMBER };
}

/**
 * @brief Checks if given positions are equal
 */
static inline int pos_same(position_t p1, position_t p2) {
    return p1.x == p2.x && p1.y == p2.y;
}

/**
 * @brief Checks if given position is valid
 */
static inline int pos_valid(position_t p) {
    return p.x != POS_INVALID_MEMBER || p.y != POS_INVALID_MEMBER;
}

/**
 * @brief Adds number to both members
 * @param p valid `position_t`
 * @param n number to add
 * @return new `position_t`
 */
static inline position_t pos_add(position_t p, int n) {
    return (position_t){p.x + n, p.y + n };
}

/**
 * @brief Subtracts number from both members
 */
static inline position_t pos_sub(position_t p, int n) {
    return (position_t){ p.x - n, p.y - n };
}

/**
 * @brief Divides both members by number
 * @note Division by 0 return invalid position
 */
static inline position_t pos_div(position_t p, int n) {
    if (n == 0) return pos_none();
    else return (position_t){ p.x / n, p.y / n };
}

/**
 * @brief Mulitiplies both members by number
 */
static inline position_t pos_mul(position_t p, int n) {
    return (position_t){ p.x * n, p.y * n };
}

/**
 * @brief Applyies modulo on both members
 */
static inline position_t pos_mod(position_t p, int n) {
    return (position_t){ p.x % n, p.y % n };
}

/**
 * @brief Adds two postitions
 */
static inline position_t pos_sum(position_t p1, position_t p2) {
    return (position_t){ p1.x + p2.x, p1.y + p2.y };
}

/**
 * @brief Subtracts two positions
 */
static inline position_t pos_dif(position_t p1, position_t p2) {
    return (position_t){ p1.x - p2.x, p1.y - p2.y };
}

/**
 * @brief Divides two positions
 */
static inline position_t pos_div_pos(position_t p1, position_t p2) {
    return (position_t)
    {
        p2.x != 0 ? p1.x / p2.x : POS_INVALID_MEMBER,
        p2.y != 0 ? p1.y / p2.y : POS_INVALID_MEMBER
    };
}

/**
 * @brief Multiplies two positions
 */
static inline position_t pos_prod(position_t p1, position_t p2) {
    return (position_t){ p1.x * p2.x, p1.y * p2.y };
}

/**
 * @brief Macro that unpack members of position. Can be used for func needing two `int` instead of `position_t`
 */
#define POS_UNPACK(pos) (pos).x, (pos).y