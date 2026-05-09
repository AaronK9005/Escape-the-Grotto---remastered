#pragma once

#include "chunk.h"

#define RENDER_DISTANCE 1
#define CHUNKS_GRID_SIZE (2 * RENDER_DISTANCE + 1)
#define MAX_CHUNK_IDX (2 * RENDER_DISTANCE)

/**
 * @brief Implementation of floor_t
 */
typedef struct floor_t
{
    int level; // how deep a floor is
    const char* game_name;
    chunk_t* chunk[CHUNKS_GRID_SIZE][CHUNKS_GRID_SIZE];

    chunk_t chunk_storage[CHUNKS_GRID_SIZE * CHUNKS_GRID_SIZE];
    chunk_t joker_chunk; // one extra chunk if requested one is out of standard chunk_storage
} floor_t;
