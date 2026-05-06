#pragma once

#include "../position.h"

/**
 * @brief Side length of map_t
 * @file map.h
 */
#define MAP_SIZE 64

typedef char tile_id_t;

/**
 * @brief A 2d tile_id array for representing map grid in chunk
 */

typedef tile_id_t map_t[MAP_SIZE][MAP_SIZE];

/**
 * @brief Position type for location inside chunks
 * @param x ∈ <0, MAP_SIZE - 1>
 * @param y ∈ <0, MAP_SIZE - 1>
 */
typedef position_t loc_pos_t;

/// @brief Maximal value for loc_pos_t's member
#define MAX_LOC (MAP_SIZE - 1)

/**
 * @brief Transforms global position into local position
 */
static inline loc_pos_t glob_to_loc(position_t pos) {
    position_t moded = pos_mod(pos, MAP_SIZE);
    
    return {
        moded.x > 0 ? moded.x : moded.x + MAP_SIZE,
        moded.y > 0 ? moded.y : moded.y + MAP_SIZE
    };
}
