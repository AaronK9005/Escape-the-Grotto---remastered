#pragma once

typedef unsigned char tile_id_t;

#define FLAG_WALKABLE       (1 << 0)
#define FLAG_OPAQUE         (1 << 1)

enum {
    #define T(NAME) TILE_##NAME,
    #include "tiles.def"
    #undef T

    TILE_COUNT
};

/**
 * @brief Global array of symbol for each tile
 */
extern const char tile_char[TILE_COUNT];

/**
 * @brief Global array of flags for each tile
 */
extern const int tile_flags[TILE_COUNT];