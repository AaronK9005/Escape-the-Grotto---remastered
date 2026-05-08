#include "world/tiles.h"

const char tile_char[TILE_COUNT] = {
    [TILE_FLOOR] =          ' ',
    [TILE_WALL] =           '#',
    [TILE_DOOR_OPEN] =      'O',
    [TILE_DOOR_CLOSED] =    '/'
};

const int tile_flags[TILE_COUNT] = {
    [TILE_FLOOR] = FLAG_WALKABLE,
    [TILE_WALL] = FLAG_OPAQUE,
    [TILE_DOOR_OPEN] = FLAG_WALKABLE,
    [TILE_DOOR_CLOSED] = FLAG_OPAQUE
};