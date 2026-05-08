#pragma once

#include "../position.h"
#include "tiles.h"

/**
 * @brief Side length of map_t
 * @file map.h
 */
#define MAP_SIZE 64

/**
 * @brief A 2d tile_id array for representing map grid in chunk
 */
typedef tile_id_t map_t[MAP_SIZE][MAP_SIZE];

/**
 * @brief Position type for location inside chunks
 * @param x ∈ <0, MAP_SIZE - 1>
 * @param y ∈ <0, MAP_SIZE - 1>
 * @note get max value via MAX_LOC
 */
typedef position_t local_pos_t;

/// @brief Maximal value for local_pos_t's member
#define MAX_LOC (MAP_SIZE - 1)

/**
 * @brief Transforms global position into local position
 */
static inline local_pos_t glob_to_loc(position_t pos) {
    return (local_pos_t){
        ((pos.x % MAP_SIZE) + MAP_SIZE) % MAP_SIZE,
        ((pos.y % MAP_SIZE) + MAP_SIZE) % MAP_SIZE
    };
}