#pragma once

//? will I use this

#include "floor_structure.h"

typedef int bit_field32;

typedef struct world_t {
    floor_t current_floor;
    bit_field32 visited_floors;
    int number_of_floors;
    floor_t** floor_storage;
} world_t;
